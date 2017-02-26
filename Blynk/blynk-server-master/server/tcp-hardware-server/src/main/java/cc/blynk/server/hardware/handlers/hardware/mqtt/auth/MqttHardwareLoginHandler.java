package cc.blynk.server.hardware.handlers.hardware.mqtt.auth;

import cc.blynk.server.Holder;
import cc.blynk.server.core.dao.TokenValue;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.handlers.DefaultExceptionHandler;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.handlers.DefaultReregisterHandler;
import cc.blynk.server.handlers.common.HardwareNotLoggedHandler;
import cc.blynk.server.hardware.handlers.hardware.MqttHardwareHandler;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.mqtt.MqttConnAckMessage;
import io.netty.handler.codec.mqtt.MqttConnAckVariableHeader;
import io.netty.handler.codec.mqtt.MqttConnectMessage;
import io.netty.handler.codec.mqtt.MqttConnectReturnCode;
import io.netty.handler.codec.mqtt.MqttFixedHeader;
import io.netty.handler.codec.mqtt.MqttMessageType;
import io.netty.handler.codec.mqtt.MqttQoS;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE_CONNECTED;
import static io.netty.handler.codec.mqtt.MqttConnectReturnCode.CONNECTION_REFUSED_BAD_USER_NAME_OR_PASSWORD;

/**
 * Handler responsible for managing hardware and apps login messages.
 * Initializes netty channel with a state tied with user.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
@ChannelHandler.Sharable
public class MqttHardwareLoginHandler extends SimpleChannelInboundHandler<MqttConnectMessage> implements DefaultReregisterHandler, DefaultExceptionHandler {

    private static final Logger log = LogManager.getLogger(DefaultExceptionHandler.class);

    private static final MqttConnAckMessage ACCEPTED = createConnAckMessage(MqttConnectReturnCode.CONNECTION_ACCEPTED);

    private final Holder holder;

    public MqttHardwareLoginHandler(Holder holder) {
        this.holder = holder;
    }

    private static void completeLogin(Channel channel, Session session, User user, DashBoard dash, int deviceId, int msgId) {
        log.debug("completeLogin. {}", channel);

        session.addHardChannel(channel);
        channel.writeAndFlush(ACCEPTED);

        session.sendToApps(HARDWARE_CONNECTED, msgId, dash.id, deviceId);

        log.info("{} mqtt hardware joined.", user.name);
    }

    private static MqttConnAckMessage createConnAckMessage(MqttConnectReturnCode code) {
        MqttFixedHeader mqttFixedHeader = new MqttFixedHeader(MqttMessageType.CONNACK, false, MqttQoS.AT_MOST_ONCE, false, 2);
        MqttConnAckVariableHeader mqttConnAckVariableHeader = new MqttConnAckVariableHeader(code, true);
        return new MqttConnAckMessage(mqttFixedHeader, mqttConnAckVariableHeader);
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, MqttConnectMessage message) throws Exception {
        String username = message.payload().userName().toLowerCase();
        String token = message.payload().password();

        final TokenValue tokenValue = holder.tokenManager.getUserByToken(token);

        if (tokenValue == null || !tokenValue.user.name.equalsIgnoreCase(username)) {
            log.debug("MqttHardwareLogic token is invalid. Token '{}', '{}'", token, ctx.channel().remoteAddress());
            ctx.writeAndFlush(createConnAckMessage(CONNECTION_REFUSED_BAD_USER_NAME_OR_PASSWORD), ctx.voidPromise());
            return;
        }

        final User user = tokenValue.user;
        final int dashId = tokenValue.dashId;
        final int deviceId = tokenValue.deviceId;

        DashBoard dash = user.profile.getDashById(dashId);
        if (dash == null) {
            log.warn("User : {} requested token {} for non-existing {} dash id.", user.name, token, dashId);
            ctx.writeAndFlush(createConnAckMessage(CONNECTION_REFUSED_BAD_USER_NAME_OR_PASSWORD), ctx.voidPromise());
            return;
        }

        ctx.pipeline().remove(this);
        ctx.pipeline().remove(HardwareNotLoggedHandler.class);
        HardwareStateHolder hardwareStateHolder = new HardwareStateHolder(dashId, deviceId, user, token);
        ctx.pipeline().addLast("HHArdwareMqttHandler", new MqttHardwareHandler(holder, hardwareStateHolder));

        Session session = holder.sessionDao.getOrCreateSessionByUser(hardwareStateHolder.userKey, ctx.channel().eventLoop());

        if (session.initialEventLoop != ctx.channel().eventLoop()) {
            log.debug("Re registering hard channel. {}", ctx.channel());
            reRegisterChannel(ctx, session, channelFuture -> completeLogin(channelFuture.channel(), session, user, dash, deviceId, -1));
        } else {
            completeLogin(ctx.channel(), session, user, dash, deviceId, -1);
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        handleGeneralException(ctx, cause);
    }

}
