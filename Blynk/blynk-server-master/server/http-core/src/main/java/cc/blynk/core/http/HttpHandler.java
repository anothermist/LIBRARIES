package cc.blynk.core.http;

import cc.blynk.core.http.rest.HandlerHolder;
import cc.blynk.core.http.rest.HandlerRegistry;
import cc.blynk.core.http.rest.URIDecoder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.channel.ChannelHandlerContext;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 26.05.16.
 */
public class HttpHandler extends BaseHttpHandler {

    public HttpHandler(TokenManager tokenManager, SessionDao sessionDao, GlobalStats globalStats) {
        super(tokenManager, sessionDao, globalStats);
    }

    @Override
    public void finishHttp(ChannelHandlerContext ctx, URIDecoder uriDecoder, HandlerHolder handlerHolder, Object[] params) {
        ctx.writeAndFlush(HandlerRegistry.invoke(handlerHolder, params), ctx.voidPromise());
    }

}
