package cc.blynk.server.websocket;

import cc.blynk.server.Holder;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.protocol.handlers.decoders.MessageDecoder;
import cc.blynk.server.core.protocol.handlers.encoders.MessageEncoder;
import cc.blynk.server.handlers.common.HardwareNotLoggedHandler;
import cc.blynk.server.hardware.handlers.hardware.HardwareChannelStateHandler;
import cc.blynk.server.hardware.handlers.hardware.auth.HardwareLoginHandler;
import cc.blynk.server.websocket.handlers.WebSocketHandler;
import cc.blynk.server.websocket.handlers.WebSocketWrapperEncoder;
import cc.blynk.utils.SslUtil;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.codec.http.websocketx.WebSocketServerProtocolHandler;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.timeout.ReadTimeoutHandler;

import static cc.blynk.server.websocket.WebSocketServer.WEBSOCKET_PATH;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13/01/2016.
 */
public class WebSocketSSLServer extends BaseServer {

    private final ChannelInitializer<SocketChannel> channelInitializer;

    public WebSocketSSLServer(Holder holder) {
        super(holder.props.getIntProperty("ssl.websocket.port"), holder.transportTypeHolder);

        final int hardTimeoutSecs = holder.props.getIntProperty("hard.socket.idle.timeout", 0);
        final HardwareLoginHandler hardwareLoginHandler = new HardwareLoginHandler(holder, port);
        final HardwareChannelStateHandler hardwareChannelStateHandler = new HardwareChannelStateHandler(holder.sessionDao, holder.gcmWrapper);
        final WebSocketWrapperEncoder webSocketWrapperEncoder = new WebSocketWrapperEncoder();

        final SslContext sslCtx = SslUtil.initSslContext(
                holder.props.getProperty("https.cert", holder.props.getProperty("server.ssl.cert")),
                holder.props.getProperty("https.key", holder.props.getProperty("server.ssl.key")),
                holder.props.getProperty("https.key.pass", holder.props.getProperty("server.ssl.key.pass")),
                    SslUtil.fetchSslProvider(holder.props)
        );

        channelInitializer = new ChannelInitializer<SocketChannel>() {
            @Override
            protected void initChannel(SocketChannel ch) throws Exception {
                final ChannelPipeline pipeline = ch.pipeline();
                if (hardTimeoutSecs > 0) {
                    pipeline.addLast("WSSReadTimeout", new ReadTimeoutHandler(hardTimeoutSecs));
                }
                pipeline.addLast("WSSContext", sslCtx.newHandler(ch.alloc()));
                pipeline.addLast("WSSHttpServerCodec", new HttpServerCodec());
                pipeline.addLast("WSSHttpObjectAggregator", new HttpObjectAggregator(65536));
                pipeline.addLast("WSSWebSocketServerProtocolHandler", new WebSocketServerProtocolHandler(WEBSOCKET_PATH, true));
                pipeline.addLast("WSSWebSocket", new WebSocketHandler(holder.stats));

                //hardware handlers
                pipeline.addLast("WSSChannelState", hardwareChannelStateHandler);
                pipeline.addLast("WSSMessageDecoder", new MessageDecoder(holder.stats));
                pipeline.addLast("WSSSocketWrapper", webSocketWrapperEncoder);
                pipeline.addLast("WSSMessageEncoder", new MessageEncoder(holder.stats));
                pipeline.addLast("WSSLogin", hardwareLoginHandler);
                pipeline.addLast("WSSNotLogged", new HardwareNotLoggedHandler());
            }
        };
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return channelInitializer;
    }

    @Override
    protected String getServerName() {
        return "Web SSL Sockets";
    }

    @Override
    public void close() {
        System.out.println("Shutting down Web SSL Sockets server...");
        super.close();
    }

}
