package cc.blynk.server.hardware;

import cc.blynk.server.Holder;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.handlers.common.HardwareNotLoggedHandler;
import cc.blynk.server.hardware.handlers.hardware.HardwareChannelStateHandler;
import cc.blynk.server.hardware.handlers.hardware.mqtt.auth.MqttHardwareLoginHandler;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.mqtt.MqttDecoder;
import io.netty.handler.codec.mqtt.MqttEncoder;
import io.netty.handler.timeout.ReadTimeoutHandler;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class MQTTHardwareServer extends BaseServer {

    private final ChannelInitializer<SocketChannel> channelInitializer;

    public MQTTHardwareServer(Holder holder) {
        super(holder.props.getIntProperty("hardware.mqtt.port"), holder.transportTypeHolder);

        final int hardTimeoutSecs = holder.props.getIntProperty("hard.socket.idle.timeout", 0);
        final MqttHardwareLoginHandler mqttHardwareLoginHandler = new MqttHardwareLoginHandler(holder);
        final HardwareChannelStateHandler hardwareChannelStateHandler = new HardwareChannelStateHandler(holder.sessionDao, holder.gcmWrapper);

        channelInitializer = new ChannelInitializer<SocketChannel>() {
            @Override
            protected void initChannel(SocketChannel ch) throws Exception {
                ChannelPipeline pipeline = ch.pipeline();
                //non-sharable handlers
                if (hardTimeoutSecs > 0) {
                    pipeline.addLast("MqttReadTimeout", new ReadTimeoutHandler(hardTimeoutSecs));
                }
                pipeline.addLast(hardwareChannelStateHandler);
                pipeline.addLast(new MqttDecoder());
                pipeline.addLast(MqttEncoder.INSTANCE);
                pipeline.addLast(mqttHardwareLoginHandler);
                pipeline.addLast(new HardwareNotLoggedHandler());
            }
        };

        log.debug("hard.socket.idle.timeout = {}", hardTimeoutSecs);
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return channelInitializer;
    }

    @Override
    protected String getServerName() {
        return "Mqtt hardware";
    }

    @Override
    public void close() {
        System.out.println("Shutting down Mqtt hardware server...");
        super.close();
    }

}
