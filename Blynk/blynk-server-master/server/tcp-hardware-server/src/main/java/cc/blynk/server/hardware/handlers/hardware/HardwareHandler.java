package cc.blynk.server.hardware.handlers.hardware;

import cc.blynk.server.Holder;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import cc.blynk.server.handlers.common.PingLogic;
import cc.blynk.server.hardware.handlers.hardware.logic.*;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 29.07.15.
 */
public class HardwareHandler extends BaseSimpleChannelInboundHandler<StringMessage> {

    public final HardwareStateHolder state;
    private final HardwareLogic hardware;
    private final MailLogic email;
    private final BridgeLogic bridge;
    private final PushLogic push;
    private final TwitLogic tweet;
    private final SmsLogic smsLogic;
    private final SetWidgetPropertyLogic propertyLogic;
    private final HardwareSyncLogic sync;
    private final BlynkInternalLogic info;
    private final GlobalStats stats;

    public HardwareHandler(Holder holder, HardwareStateHolder stateHolder) {
        super(holder.limits, stateHolder);
        this.hardware = new HardwareLogic(holder, stateHolder.user.name);
        this.bridge = new BridgeLogic(holder.sessionDao, hardware);

        this.email = new MailLogic(holder.blockingIOProcessor, holder.mailWrapper, holder.limits.NOTIFICATION_PERIOD_LIMIT_SEC);
        this.push = new PushLogic(holder.gcmWrapper, holder.limits.NOTIFICATION_PERIOD_LIMIT_SEC);
        this.tweet = new TwitLogic(holder.blockingIOProcessor, holder.twitterWrapper, holder.limits.NOTIFICATION_PERIOD_LIMIT_SEC);
        this.smsLogic = new SmsLogic(holder.smsWrapper, holder.limits.NOTIFICATION_PERIOD_LIMIT_SEC);
        this.propertyLogic = new SetWidgetPropertyLogic(holder.sessionDao);
        this.sync = new HardwareSyncLogic();
        this.info = new BlynkInternalLogic(holder.props.getIntProperty("hard.socket.idle.timeout", 0));

        this.state = stateHolder;
        this.stats = holder.stats;
    }

    @Override
    public void messageReceived(ChannelHandlerContext ctx, StringMessage msg) {
        this.stats.incrementHardStat();
        switch (msg.command) {
            case HARDWARE:
                hardware.messageReceived(ctx, state, msg);
                break;
            case PING:
                PingLogic.messageReceived(ctx, msg.id);
                break;
            case BRIDGE:
                bridge.messageReceived(ctx, state, msg);
                break;
            case EMAIL:
                email.messageReceived(ctx, state, msg);
                break;
            case PUSH_NOTIFICATION:
                push.messageReceived(ctx, state, msg);
                break;
            case TWEET:
                tweet.messageReceived(ctx, state, msg);
                break;
            case SMS:
                smsLogic.messageReceived(ctx, state, msg);
                break;
            case HARDWARE_SYNC:
                sync.messageReceived(ctx, state, msg);
                break;
            case BLYNK_INTERNAL:
                info.messageReceived(ctx, state, msg);
                break;
            case SET_WIDGET_PROPERTY:
                propertyLogic.messageReceived(ctx, state, msg);
                break;
        }
    }

}
