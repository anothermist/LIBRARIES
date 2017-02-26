package cc.blynk.server.api.http.logic.business;

import cc.blynk.core.http.BaseHttpHandler;
import cc.blynk.core.http.rest.HandlerHolder;
import cc.blynk.core.http.rest.HandlerRegistry;
import cc.blynk.core.http.rest.URIDecoder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.http.FullHttpResponse;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.01.16.
 */
public class AuthHttpHandler extends BaseHttpHandler {

    public AuthHttpHandler(TokenManager tokenManager, SessionDao sessionDao, GlobalStats globalStats) {
        super(tokenManager, sessionDao, globalStats);
    }

    @Override
    public void finishHttp(ChannelHandlerContext ctx, URIDecoder uriDecoder, HandlerHolder handlerHolder, Object[] params) {
        User user = ctx.channel().attr(AuthCookieHandler.userAttributeKey).get();

        if (user == null) {
            ctx.writeAndFlush(HandlerRegistry.invoke(handlerHolder, params), ctx.voidPromise());
            return;
        }

        if (params.length > 0) {
            params[0] = user;
        }

        Session session = sessionDao.getOrCreateSessionByUser(new UserKey(user), ctx.channel().eventLoop());
        if (session.initialEventLoop != ctx.channel().eventLoop()) {
            log.debug("Re registering http channel. {}", ctx.channel());
            reRegisterChannel(ctx, session, channelFuture -> completeLogin(channelFuture.channel(), HandlerRegistry.invoke(handlerHolder, params)));
        } else {
            completeLogin(ctx.channel(), HandlerRegistry.invoke(handlerHolder, params));
        }
    }

    private void completeLogin(Channel channel, FullHttpResponse response) {
        channel.writeAndFlush(response);
        log.debug("Re registering http channel finished.");
    }
}
