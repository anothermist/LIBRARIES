package cc.blynk.server.application.handlers.main.auth;

import cc.blynk.server.Holder;
import cc.blynk.server.application.handlers.main.AppHandler;
import cc.blynk.server.application.handlers.sharing.auth.AppShareLoginHandler;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.FacebookTokenResponse;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.handlers.DefaultExceptionHandler;
import cc.blynk.server.core.protocol.model.messages.appllication.LoginMessage;
import cc.blynk.server.handlers.DefaultReregisterHandler;
import cc.blynk.server.handlers.common.UserNotLoggedHandler;
import cc.blynk.utils.IPUtils;
import cc.blynk.utils.JsonParser;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.SimpleChannelInboundHandler;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.asynchttpclient.AsyncCompletionHandler;
import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.Response;

import java.util.NoSuchElementException;

import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.server.core.protocol.enums.Response.USER_NOT_AUTHENTICATED;
import static cc.blynk.server.core.protocol.enums.Response.USER_NOT_REGISTERED;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;


/**
 * Handler responsible for managing apps login messages.
 * Initializes netty channel with a state tied with user.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
@ChannelHandler.Sharable
public class AppLoginHandler extends SimpleChannelInboundHandler<LoginMessage> implements DefaultReregisterHandler, DefaultExceptionHandler {

    private static final String URL = "https://graph.facebook.com/me?fields=email&access_token=";
    private static final Logger log = LogManager.getLogger(AppLoginHandler.class);

    private final Holder holder;
    private final DefaultAsyncHttpClient asyncHttpClient;

    public AppLoginHandler(Holder holder) {
        this.holder = holder;
        this.asyncHttpClient = holder.asyncHttpClient;
    }

    private static void cleanPipeline(ChannelPipeline pipeline) {
        pipeline.remove(AppLoginHandler.class);
        pipeline.remove(UserNotLoggedHandler.class);
        pipeline.remove(RegisterHandler.class);
        pipeline.remove(AppShareLoginHandler.class);
        pipeline.remove(GetServerHandler.class);
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, LoginMessage message) throws Exception {
        //warn: split may be optimized
        String[] messageParts = message.body.split("\0");

        if (messageParts.length < 2) {
            log.error("Wrong income message format.");
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND), ctx.voidPromise());
            return;
        }

        final String username = messageParts[0].toLowerCase();
        final OsType osType = messageParts.length > 3 ? OsType.parse(messageParts[2]) : OsType.OTHER;
        final String version = messageParts.length > 3 ? messageParts[3] : null;

        if (messageParts.length == 5) {
            if (AppName.FACEBOOK.equals(messageParts[4])) {
                facebookLogin(ctx, message.id, username, messageParts[1], osType, version);
            } else {
                final String appName = messageParts[4];
                blynkLogin(ctx, message.id, username, messageParts[1], osType, version, appName);
            }
        } else {
            //todo this is for back compatibility
            blynkLogin(ctx, message.id, username, messageParts[1], osType, version, AppName.BLYNK);
        }
    }

    private void facebookLogin(ChannelHandlerContext ctx, int messageId, String username, String token, OsType osType, String version) {
        asyncHttpClient.prepareGet(URL + token)
                .execute(new AsyncCompletionHandler<Response>() {
                    @Override
                    public Response onCompleted(Response response) throws Exception {
                        if (response.getStatusCode() != 200) {
                            log.warn("Error getting facebook token {} for user {}. Reason : {}", token, username, response.getResponseBody());
                            ctx.writeAndFlush(makeResponse(messageId, NOT_ALLOWED), ctx.voidPromise());
                            return response;
                        }

                        try {
                            String responseBody = response.getResponseBody();
                            FacebookTokenResponse facebookTokenResponse = JsonParser.parseFacebookTokenResponse(responseBody);
                            if (username.equalsIgnoreCase(facebookTokenResponse.email)) {
                                User user = holder.userDao.getByName(username, AppName.BLYNK);
                                if (user == null) {
                                    user = holder.userDao.addFacebookUser(username, AppName.BLYNK);
                                }

                                login(ctx, messageId, user, osType, version);
                            }
                        } catch (Exception e) {
                            log.error("Error during facebook response parsing for user {}. Reason : {}", username, response.getResponseBody());
                            ctx.writeAndFlush(makeResponse(messageId, NOT_ALLOWED), ctx.voidPromise());
                        }

                        return response;
                    }

                    @Override
                    public void onThrowable(Throwable t) {
                        log.error("Error performing facebook request. Token {} for user {}. Reason : {}", token, username, t.getMessage());
                        ctx.writeAndFlush(makeResponse(messageId, NOT_ALLOWED), ctx.voidPromise());
                    }
                });
    }

    private void blynkLogin(ChannelHandlerContext ctx, int msgId, String username, String pass, OsType osType, String version, String appName) {
        User user = holder.userDao.getByName(username, appName);

        if (user == null) {
            log.warn("User '{}' not registered. {}", username, ctx.channel().remoteAddress());
            ctx.writeAndFlush(makeResponse(msgId, USER_NOT_REGISTERED), ctx.voidPromise());
            return;
        }

        if (user.pass == null) {
            log.warn("Facebook user '{}' tries to login with pass. {}", username, ctx.channel().remoteAddress());
            ctx.writeAndFlush(makeResponse(msgId, USER_NOT_AUTHENTICATED), ctx.voidPromise());
            return;
        }

        if (!user.pass.equals(pass)) {
            log.warn("User '{}' credentials are wrong. {}", username, ctx.channel().remoteAddress());
            ctx.writeAndFlush(makeResponse(msgId, USER_NOT_AUTHENTICATED), ctx.voidPromise());
            return;
        }

        login(ctx, msgId, user, osType, version);
    }

    private void login(ChannelHandlerContext ctx, int messageId, User user, OsType osType, String version) {
        AppStateHolder appStateHolder = new AppStateHolder(user, osType, version);

        try {
            cleanPipeline(ctx.pipeline());
        } catch (NoSuchElementException e) {
            //this case possible when few login commands come at same time to different threads
            //just do nothing and ignore.
            //https://github.com/blynkkk/blynk-server/issues/224
            return;
        }

        ctx.pipeline().addLast("AAppHandler", new AppHandler(holder, appStateHolder));

        Session session = holder.sessionDao.getOrCreateSessionByUser(appStateHolder.userKey, ctx.channel().eventLoop());
        user.lastLoggedIP = IPUtils.getIp(ctx.channel());
        user.lastLoggedAt = System.currentTimeMillis();

        if (session.initialEventLoop != ctx.channel().eventLoop()) {
            log.debug("Re registering app channel. {}", ctx.channel());
            reRegisterChannel(ctx, session, channelFuture -> completeLogin(channelFuture.channel(), session, user, messageId));
        } else {
            completeLogin(ctx.channel(), session, user, messageId);
        }
    }

    private void completeLogin(Channel channel, Session session, User user, int msgId) {
        session.addAppChannel(channel);
        channel.writeAndFlush(ok(msgId), channel.voidPromise());
        for (DashBoard dashBoard : user.profile.dashBoards) {
            if (dashBoard.isAppConnectedOn && dashBoard.isActive) {
                log.trace("{}-{}. Sendeind App Connected event to hardware.", user.name, user.appName);
                session.sendMessageToHardware(dashBoard.id, Command.BLYNK_INTERNAL, 7777, "acon");
            }
        }
        log.info("{} {}-app joined.", user.name, user.appName);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        handleGeneralException(ctx, cause);
    }

}
