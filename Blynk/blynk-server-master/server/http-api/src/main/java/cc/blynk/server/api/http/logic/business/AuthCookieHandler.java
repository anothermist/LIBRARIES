package cc.blynk.server.api.http.logic.business;

import cc.blynk.core.http.Response;
import cc.blynk.server.core.model.auth.User;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.handler.codec.http.FullHttpRequest;
import io.netty.handler.codec.http.HttpHeaderNames;
import io.netty.handler.codec.http.cookie.Cookie;
import io.netty.handler.codec.http.cookie.ServerCookieDecoder;
import io.netty.util.AttributeKey;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Set;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.05.16.
 */
public class AuthCookieHandler extends ChannelInboundHandlerAdapter {

    public final static AttributeKey<User> userAttributeKey = AttributeKey.valueOf("user");
    private static final Logger log = LogManager.getLogger(AuthCookieHandler.class);
    private final String authPath;
    private final SessionHolder sessionHolder;

    public AuthCookieHandler(String authPath, SessionHolder sessionHolder) {
        this.authPath = authPath;
        this.sessionHolder = sessionHolder;
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        if (msg instanceof FullHttpRequest) {
            FullHttpRequest request = (FullHttpRequest) msg;
            if (request.uri().startsWith(authPath)) {
                User user = getUser(request);

                //access to API that requires cookies and no auth cookie
                if (user != null) {
                    if (request.uri().equals("/business/logout")) {
                        ctx.channel().attr(userAttributeKey).set(null);
                    } else {
                        ctx.channel().attr(userAttributeKey).set(user);
                    }
                } else {
                    if (request.uri().endsWith("/login") || request.uri().startsWith("/static/business")) {
                    } else {
                        ctx.writeAndFlush(Response.redirect("/static/business/login.html"), ctx.voidPromise());
                        return;
                    }
                }
            }
        }

        super.channelRead(ctx, msg);
    }

    private User getUser(FullHttpRequest request) {
        String cookieString = request.headers().get(HttpHeaderNames.COOKIE);

        if (cookieString != null) {
            Set<Cookie> cookies = ServerCookieDecoder.STRICT.decode(cookieString);
            if (!cookies.isEmpty()) {
                for (Cookie cookie : cookies) {
                    if (sessionHolder.isValid(cookie)) {
                        return sessionHolder.getUser(cookie.value());
                    }
                }
            }
        }

        return null;
    }

}
