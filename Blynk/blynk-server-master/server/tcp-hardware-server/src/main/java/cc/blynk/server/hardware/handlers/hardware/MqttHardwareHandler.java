package cc.blynk.server.hardware.handlers.hardware;

import cc.blynk.server.Holder;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import cc.blynk.server.hardware.handlers.hardware.logic.*;
import cc.blynk.server.hardware.handlers.hardware.mqtt.logic.MqttHardwareLogic;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.mqtt.MqttMessage;
import io.netty.handler.codec.mqtt.MqttMessageFactory;
import io.netty.handler.codec.mqtt.MqttMessageType;
import io.netty.handler.codec.mqtt.MqttPublishMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 29.07.15.
 */
public class MqttHardwareHandler extends BaseSimpleChannelInboundHandler<MqttMessage> {

    public final HardwareStateHolder state;
    private final MqttHardwareLogic hardware;
    private final MailLogic email;
    //private final BridgeLogic bridge;
    private final PushLogic push;
    private final TwitLogic tweet;
    private final SmsLogic smsLogic;
    private final SetWidgetPropertyLogic propertyLogic;
    private final HardwareSyncLogic sync;
    private final BlynkInternalLogic info;
    private final GlobalStats stats;

    public MqttHardwareHandler(Holder holder, HardwareStateHolder stateHolder) {
        super(holder.limits, stateHolder);
        this.hardware = new MqttHardwareLogic(holder.sessionDao, holder.reportingDao);

        final long defaultNotificationQuotaLimit = holder.props.getLongProperty("notifications.frequency.user.quota.limit") * 1000;
        this.email = new MailLogic(holder.blockingIOProcessor, holder.mailWrapper, defaultNotificationQuotaLimit);
        this.push = new PushLogic(holder.gcmWrapper, defaultNotificationQuotaLimit);
        this.tweet = new TwitLogic(holder.blockingIOProcessor, holder.twitterWrapper, defaultNotificationQuotaLimit);
        this.smsLogic = new SmsLogic(holder.smsWrapper, defaultNotificationQuotaLimit);
        this.propertyLogic = new SetWidgetPropertyLogic(holder.sessionDao);
        this.sync = new HardwareSyncLogic();
        this.info = new BlynkInternalLogic(holder.props.getIntProperty("hard.socket.idle.timeout", 0));

        this.state = stateHolder;
        this.stats = holder.stats;
    }

    @Override
    public void messageReceived(ChannelHandlerContext ctx, MqttMessage msg) {
        this.stats.incrementHardStat();
        MqttMessageType messageType = msg.fixedHeader().messageType();

        switch (messageType) {
            case PUBLISH :
                MqttPublishMessage publishMessage = (MqttPublishMessage) msg;
                String topic = publishMessage.variableHeader().topicName();

                switch (topic.toLowerCase()) {
                    case "hardware" :
                        hardware.messageReceived(ctx, state, publishMessage);
                        break;
                }

                break;

            case PINGREQ :
                ctx.writeAndFlush(MqttMessageFactory.newMessage(msg.fixedHeader(), msg.variableHeader(), null), ctx.voidPromise());
                break;

            case DISCONNECT :
                log.trace("Got disconnect. Closing...");
                ctx.close();
                break;
        }

    }

}
