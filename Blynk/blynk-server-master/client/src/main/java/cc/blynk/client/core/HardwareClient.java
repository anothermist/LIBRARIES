package cc.blynk.client.core;

import cc.blynk.client.handlers.ClientReplayingMessageDecoder;
import cc.blynk.server.core.protocol.handlers.encoders.MessageEncoder;
import cc.blynk.server.core.protocol.model.messages.common.PingMessage;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;

import java.util.Random;
import java.util.concurrent.TimeUnit;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.03.15.
 */
public class HardwareClient extends BaseClient {

    public HardwareClient(String host, int port) {
        super(host, port, new Random());
        log.info("Creating hardware client. Host : {}, port : {}", host, port);
        //pinging for hardware client to avoid closing from server side for inactivity
        nioEventLoopGroup.scheduleAtFixedRate(() -> send(new PingMessage(777)), 12, 12, TimeUnit.SECONDS);
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return new ChannelInitializer<SocketChannel> () {
            @Override
            public void initChannel(SocketChannel ch) throws Exception {
                ChannelPipeline pipeline = ch.pipeline();
                pipeline.addLast(new ClientReplayingMessageDecoder());
                pipeline.addLast(new MessageEncoder(new GlobalStats()));
            }
        };
    }
}
