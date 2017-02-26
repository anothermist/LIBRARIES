package cc.blynk.server.websocket.handlers;

import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.websocketx.BinaryWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketHandshakeException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.01.16.
 */
public class WebSocketHandler extends SimpleChannelInboundHandler<WebSocketFrame> {

    private static final Logger log = LogManager.getLogger(WebSocketHandler.class);

    private final GlobalStats globalStats;

    public WebSocketHandler(GlobalStats globalStats) {
        this.globalStats = globalStats;
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, WebSocketFrame frame) throws Exception {
        globalStats.mark(Command.WEB_SOCKETS);
        if (frame instanceof BinaryWebSocketFrame) {
            ctx.fireChannelRead(frame.retain().content());
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        if (cause instanceof WebSocketHandshakeException) {
            log.debug("Web Socket Handshake Exception.", cause);
        }
    }


}
