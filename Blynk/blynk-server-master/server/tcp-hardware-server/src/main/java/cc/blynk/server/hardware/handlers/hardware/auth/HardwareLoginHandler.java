package cc.blynk.server.hardware.handlers.hardware.auth;

import cc.blynk.server.Holder;
import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.TokenValue;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.protocol.handlers.DefaultExceptionHandler;
import cc.blynk.server.core.protocol.model.messages.appllication.LoginMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.handlers.DefaultReregisterHandler;
import cc.blynk.server.handlers.common.HardwareNotLoggedHandler;
import cc.blynk.server.hardware.handlers.hardware.HardwareHandler;
import cc.blynk.server.redis.RedisClient;
import cc.blynk.utils.IPUtils;
import cc.blynk.utils.StringUtils;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.CONNECT_REDIRECT;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE_CONNECTED;
import static cc.blynk.server.core.protocol.enums.Response.INVALID_TOKEN;
import static cc.blynk.utils.BlynkByteBufUtil.makeASCIIStringMessage;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;

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
public class HardwareLoginHandler extends SimpleChannelInboundHandler<LoginMessage> implements DefaultReregisterHandler, DefaultExceptionHandler {

    private static final Logger log = LogManager.getLogger(DefaultExceptionHandler.class);

    private static final int HARDWARE_PIN_MODE_MSG_ID = 1;

    private final Holder holder;
    private final RedisClient redisClient;
    private final BlockingIOProcessor blockingIOProcessor;
    private final String listenPort;

    public HardwareLoginHandler(Holder holder, int listenPort) {
        this.holder = holder;
        this.redisClient = holder.redisClient;
        this.blockingIOProcessor = holder.blockingIOProcessor;
        this.listenPort = String.valueOf(listenPort);
    }

    private static void completeLogin(Channel channel, Session session, User user, DashBoard dash, int deviceId, int msgId) {
        log.debug("completeLogin. {}", channel);

        session.addHardChannel(channel);
        channel.write(ok(msgId));

        final String body = dash.buildPMMessage(deviceId);
        if (dash.isActive && body.length() > 2) {
            channel.write(makeASCIIStringMessage(HARDWARE, HARDWARE_PIN_MODE_MSG_ID, body));
        }

        channel.flush();

        session.sendToApps(HARDWARE_CONNECTED, msgId, dash.id, deviceId);
        Device device = dash.getDeviceById(deviceId);
        if (device != null) {
            log.trace("Connected device id {}, dash id {}", deviceId, dash.id);
            device.connected();
            device.lastLoggedIP = IPUtils.getIp(channel);
        }

        log.info("{} hardware joined.", user.name);
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, LoginMessage message) throws Exception {
        final String token = message.body.trim();
        final TokenValue tokenValue = holder.tokenManager.getUserByToken(token);

        //no user on current server, trying to find server that user belongs to.
        if (tokenValue == null) {
            //checkUserOnOtherServer(ctx, token, message.id);
            log.debug("HardwareLogic token is invalid. Token '{}', '{}'", token, ctx.channel().remoteAddress());
            ctx.writeAndFlush(makeResponse(message.id, INVALID_TOKEN), ctx.voidPromise());
            return;
        }

        final User user = tokenValue.user;
        final int dashId = tokenValue.dashId;
        final int deviceId = tokenValue.deviceId;

        DashBoard dash = user.profile.getDashById(dashId);
        if (dash == null) {
            log.warn("User : {} requested token {} for non-existing {} dash id.", user.name, token, dashId);
            ctx.writeAndFlush(makeResponse(message.id, INVALID_TOKEN), ctx.voidPromise());
            return;
        }

        ctx.pipeline().remove(this);
        ctx.pipeline().remove(HardwareNotLoggedHandler.class);
        HardwareStateHolder hardwareStateHolder = new HardwareStateHolder(dashId, deviceId, user, token);
        ctx.pipeline().addLast("HHArdwareHandler", new HardwareHandler(holder, hardwareStateHolder));

        Session session = holder.sessionDao.getOrCreateSessionByUser(hardwareStateHolder.userKey, ctx.channel().eventLoop());

        if (session.initialEventLoop != ctx.channel().eventLoop()) {
            log.debug("Re registering hard channel. {}", ctx.channel());
            reRegisterChannel(ctx, session, channelFuture -> completeLogin(channelFuture.channel(), session, user, dash, deviceId, message.id));
        } else {
            completeLogin(ctx.channel(), session, user, dash, deviceId, message.id);
        }
    }

    private void checkUserOnOtherServer(ChannelHandlerContext ctx, String token, int msgId) {
        blockingIOProcessor.execute(() -> {
            String server = redisClient.getServerByToken(token);
            // no server found, that's means token is wrong.
            if (server == null || server.equals(holder.currentIp)) {
                log.debug("HardwareLogic token is invalid. Token '{}', '{}'", token, ctx.channel().remoteAddress());
                ctx.writeAndFlush(makeResponse(msgId, INVALID_TOKEN), ctx.voidPromise());
            } else {
                log.info("Redirecting token '{}', '{}' to {}", token, ctx.channel().remoteAddress(), server);
                ctx.writeAndFlush(makeASCIIStringMessage(CONNECT_REDIRECT, msgId, server + StringUtils.BODY_SEPARATOR + listenPort), ctx.voidPromise());
            }
        });
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        handleGeneralException(ctx, cause);
    }

}
