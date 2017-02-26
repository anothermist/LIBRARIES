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
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.codec.http.websocketx.WebSocketServerProtocolHandler;
import io.netty.handler.timeout.ReadTimeoutHandler;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13/01/2016.
 */
public class WebSocketServer extends BaseServer {

    private final ChannelInitializer<SocketChannel> channelInitializer;
    public static final String WEBSOCKET_PATH = "/websocket";

    public WebSocketServer(Holder holder) {
        super(holder.props.getIntProperty("tcp.websocket.port"), holder.transportTypeHolder);

        final int hardTimeoutSecs = holder.props.getIntProperty("hard.socket.idle.timeout", 0);
        final HardwareLoginHandler hardwareLoginHandler = new HardwareLoginHandler(holder, port);
        final HardwareChannelStateHandler hardwareChannelStateHandler = new HardwareChannelStateHandler(holder.sessionDao, holder.gcmWrapper);

        channelInitializer = new ChannelInitializer<SocketChannel>() {
            @Override
            protected void initChannel(SocketChannel ch) throws Exception {
                final ChannelPipeline pipeline = ch.pipeline();
                if (hardTimeoutSecs > 0) {
                    pipeline.addLast("WSReadTimeout", new ReadTimeoutHandler(hardTimeoutSecs));
                }
                pipeline.addLast("WSHttpServerCodec", new HttpServerCodec());
                pipeline.addLast("WSHttpObjectAggregator", new HttpObjectAggregator(65536));
                pipeline.addLast("WSWebSocketServerProtocolHandler", new WebSocketServerProtocolHandler(WEBSOCKET_PATH, true));
                pipeline.addLast("WSWebSocket", new WebSocketHandler(holder.stats));

                //hardware handlers
                pipeline.addLast("WSChannelState", hardwareChannelStateHandler);
                pipeline.addLast("WSMessageDecoder", new MessageDecoder(holder.stats));
                pipeline.addLast("WSSocketWrapper", new WebSocketWrapperEncoder());
                pipeline.addLast("WSMessageEncoder", new MessageEncoder(holder.stats));
                pipeline.addLast("WSLogin", hardwareLoginHandler);
                pipeline.addLast("WSNotLogged", new HardwareNotLoggedHandler());
            }
        };
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return channelInitializer;
    }

    @Override
    protected String getServerName() {
        return "Web Sockets";
    }

    @Override
    public void close() {
        System.out.println("Shutting down Web Sockets server...");
        super.close();
    }

}
