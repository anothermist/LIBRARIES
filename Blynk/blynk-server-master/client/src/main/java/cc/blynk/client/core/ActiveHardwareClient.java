package cc.blynk.client.core;

import cc.blynk.client.handlers.ClientReplayingMessageDecoder;
import cc.blynk.client.handlers.hardware.HardwareEchoHandler;
import cc.blynk.server.core.protocol.handlers.encoders.MessageEncoder;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import cc.blynk.server.core.protocol.model.messages.common.PingMessage;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;

import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.03.15.
 */
public class ActiveHardwareClient extends BaseClient {

    private int buttonVal = 1;
    private int ledVal = 20;

    public ActiveHardwareClient(String host, int port) {
        super(host, port, new Random());
        log.info("Creating hardware client. Host : {}, port : {}", host, port);
        //pinging for hardware client to avoid closing from server side for inactivity
        nioEventLoopGroup.scheduleAtFixedRate(() -> send(new PingMessage(777)), 12, 12, TimeUnit.SECONDS);
    }

    private static HardwareMessage makeCommand(String body) {
        return new HardwareMessage(778, (body.replaceAll(" ", "\0")));
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return new ChannelInitializer<SocketChannel> () {
            @Override
            public void initChannel(SocketChannel ch) throws Exception {
                ChannelPipeline pipeline = ch.pipeline();
                pipeline.addLast(new ClientReplayingMessageDecoder());
                pipeline.addLast(new MessageEncoder(new GlobalStats()));
                pipeline.addLast(new HardwareEchoHandler());
            }
        };
    }

    public void start(String token) {
        super.start();

        send("login " + token);

        nioEventLoopGroup.scheduleAtFixedRate(() -> {
            send(makeCommand("vw 4 " + ThreadLocalRandom.current().nextInt(100)));
            send(makeCommand("dw 3 " + ThreadLocalRandom.current().nextInt(255)));
            send(makeCommand("dw 0 " + buttonVal));
            send(makeCommand("vw 5 " + (buttonVal == 1 ? 255 : 0)));
            send(makeCommand("vw 6 " + ledVal));
            send(makeCommand("aw 6 " + ledVal));
            send(makeCommand("vw 10 p 0 0 ledVal:" + ledVal));
            if (buttonVal == 1) {
                buttonVal = 0;
            } else {
                buttonVal = 1;
            }
            if (ledVal > 255) {
                ledVal = 0;
                send(makeCommand("vw 10 clr"));
            } else {
                ledVal += 20;
            }
        }, 1, 1, TimeUnit.SECONDS);

    }

    public void send(String line) {
        send(produceMessageBaseOnUserInput(line, 1));
    }
}
