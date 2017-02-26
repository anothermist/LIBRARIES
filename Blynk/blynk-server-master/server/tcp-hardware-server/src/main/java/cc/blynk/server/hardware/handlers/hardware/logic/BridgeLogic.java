package cc.blynk.server.hardware.handlers.hardware.logic;

import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.utils.StringUtils;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static cc.blynk.server.core.protocol.enums.Response.DEVICE_NOT_IN_NETWORK;
import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;
import static cc.blynk.utils.StateHolderUtil.getHardState;

/**
 * Bridge handler responsible for forwarding messages between different hardware via Blynk Server.
 * SendTo device defined by Auth Token.
 *
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class BridgeLogic {

    private static final Logger log = LogManager.getLogger(BridgeLogic.class);
    private final HardwareLogic hardwareLogic;
    private final SessionDao sessionDao;
    private final Map<String, String> sendToMap;

    public BridgeLogic(SessionDao sessionDao, HardwareLogic hardwareLogic) {
        this.sessionDao = sessionDao;
        this.sendToMap = new ConcurrentHashMap<>();
        this.hardwareLogic = hardwareLogic;
    }

    private static boolean isInit(String body) {
        return body.length() > 0 && body.charAt(0) == 'i';
    }

    public void messageReceived(ChannelHandlerContext ctx, HardwareStateHolder state, StringMessage message) {
        Session session = sessionDao.userSession.get(state.userKey);
        String[] split = message.body.split(StringUtils.BODY_SEPARATOR_STRING);

        if (split.length < 3) {
            log.error("Wrong bridge body. '{}'", message.body);
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND), ctx.voidPromise());
            return;
        }

        final String bridgePin = split[0];

        if (isInit(split[1])) {
            final String token = split[2];

            sendToMap.put(bridgePin, token);

            ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
        } else {
            final String token = sendToMap.get(bridgePin);

            if (sendToMap.size() == 0 || token == null) {
                log.debug("Bridge not initialized. {}", state.user.name);
                ctx.writeAndFlush(makeResponse(message.id, NOT_ALLOWED), ctx.voidPromise());
                return;
            }

            if (session.hardwareChannels.size() > 1) {
                boolean messageWasSent = false;
                message.body = message.body.substring(message.body.indexOf(StringUtils.BODY_SEPARATOR_STRING) + 1);
                for (Channel channel : session.hardwareChannels) {
                    if (channel != ctx.channel() && channel.isWritable()) {
                        HardwareStateHolder hardwareState = getHardState(channel);
                        if (token.equals(hardwareState.token)) {
                            messageWasSent = true;
                            hardwareLogic.messageReceived(ctx, hardwareState, message);
                            channel.writeAndFlush(message, channel.voidPromise());
                        }
                    }
                }
                if (!messageWasSent) {
                    ctx.writeAndFlush(makeResponse(message.id, DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
                }
            } else {
                ctx.writeAndFlush(makeResponse(message.id, DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
            }
        }
    }
}

