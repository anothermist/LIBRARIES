package cc.blynk.server.core.model.auth;

import cc.blynk.server.core.protocol.model.messages.ResponseWithBodyMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.core.stats.metrics.InstanceLoadMeter;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import cc.blynk.utils.ArrayUtil;
import io.netty.buffer.ByteBuf;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.EventLoop;
import io.netty.util.internal.ConcurrentSet;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.HashSet;
import java.util.Set;

import static cc.blynk.server.core.protocol.enums.Command.RESPONSE;
import static cc.blynk.server.core.protocol.enums.Response.DEVICE_WENT_OFFLINE;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.StateHolderUtil.getHardState;
import static cc.blynk.utils.StateHolderUtil.isSameDash;
import static cc.blynk.utils.StateHolderUtil.isSameDashAndDeviceId;
import static cc.blynk.utils.StringUtils.DEVICE_SEPARATOR;
import static cc.blynk.utils.StringUtils.prependDashIdAndDeviceId;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 * <p>
 * DefaultChannelGroup.java too complicated. so doing in simple way for now.
 */
public class Session {

    private static final Logger log = LogManager.getLogger(Session.class);

    public final EventLoop initialEventLoop;
    public final Set<Channel> appChannels = new ConcurrentSet<>();
    public final Set<Channel> hardwareChannels = new ConcurrentSet<>();

    private final ChannelFutureListener appRemover = future -> removeAppChannel(future.channel());
    private final ChannelFutureListener hardRemover = future -> removeHardChannel(future.channel());

    public Session(EventLoop initialEventLoop) {
        this.initialEventLoop = initialEventLoop;
    }

    private static int getRequestRate(Set<Channel> channels) {
        double sum = 0;
        for (Channel c : channels) {
            BaseSimpleChannelInboundHandler handler = c.pipeline().get(BaseSimpleChannelInboundHandler.class);
            if (handler != null) {
                InstanceLoadMeter loadMeter = handler.getQuotaMeter();
                sum += loadMeter.getOneMinuteRateNoTick();
            }
        }
        return (int) sum;
    }

    public static boolean needSync(Channel channel, String sharedToken) {
        BaseSimpleChannelInboundHandler appHandler = channel.pipeline().get(BaseSimpleChannelInboundHandler.class);
        return appHandler != null && appHandler.state.contains(sharedToken);
    }

    public void addAppChannel(Channel appChannel) {
        if (appChannels.add(appChannel)) {
            appChannel.closeFuture().addListener(appRemover);
        }
    }

    public void removeAppChannel(Channel appChannel) {
        if (appChannels.remove(appChannel)) {
            appChannel.closeFuture().removeListener(appRemover);
        }
    }

    public void addHardChannel(Channel hardChannel) {
        if (hardwareChannels.add(hardChannel)) {
            hardChannel.closeFuture().addListener(hardRemover);
        }
    }

    public void removeHardChannel(Channel hardChannel) {
        if (hardwareChannels.remove(hardChannel)) {
            hardChannel.closeFuture().removeListener(hardRemover);
        }
    }

    private Set<Channel> filter(int activeDashId, int[] deviceIds) {
        final Set<Channel> targetChannels = new HashSet<>();
        for (Channel channel : hardwareChannels) {
            final HardwareStateHolder hardwareState = getHardState(channel);
            if (hardwareState.dashId == activeDashId &&
                    (deviceIds.length == 0 || ArrayUtil.contains(deviceIds, hardwareState.deviceId))) {
                targetChannels.add(channel);
            }
        }
        return targetChannels;
    }

    private Set<Channel> filter(int activeDashId, int deviceId) {
        final Set<Channel> targetChannels = new HashSet<>();
        for (Channel channel : hardwareChannels) {
            if (isSameDashAndDeviceId(channel, activeDashId, deviceId)) {
                targetChannels.add(channel);
            }
        }
        return targetChannels;
    }

    public boolean sendMessageToHardware(int activeDashId, short cmd, int msgId, String body, int deviceId) {
        return hardwareChannels.size() == 0 || sendMessageToHardware(filter(activeDashId, deviceId), cmd, msgId, body);
    }

    public boolean sendMessageToHardware(int activeDashId, short cmd, int msgId, String body, int... deviceIds) {
        return hardwareChannels.size() == 0 || sendMessageToHardware(filter(activeDashId, deviceIds), cmd, msgId, body);
    }

    private boolean sendMessageToHardware(Set<Channel> targetChannels, short cmd, int msgId, String body) {
        final int channelsNum = targetChannels.size();
        if (channelsNum == 0) {
            return true; // -> no active hardware
        }

        send(targetChannels, channelsNum, cmd, msgId, body);

        return false; // -> there is active hardware
    }

    public boolean isHardwareConnected() {
        return hardwareChannels.size() > 0;
    }

    public boolean isHardwareConnected(int dashId, int deviceId) {
        for (Channel channel : hardwareChannels) {
            if (isSameDashAndDeviceId(channel, dashId, deviceId)) {
                return true;
            }
        }
        return false;
    }

    public boolean isHardwareConnected(int dashId) {
        for (Channel channel : hardwareChannels) {
            if (isSameDash(channel, dashId)) {
                return true;
            }
        }
        return false;
    }

    public void sendOfflineMessageToApps(int dashId) {
        if (isAppConnected()) {
            log.trace("Sending device offline message.");
            ResponseWithBodyMessage response = new ResponseWithBodyMessage(0, RESPONSE, DEVICE_WENT_OFFLINE, dashId);
            for (Channel appChannel : appChannels) {
                if (appChannel.isWritable()) {
                    appChannel.writeAndFlush(response, appChannel.voidPromise());
                }
            }
        }
    }

    public void sendToApps(short cmd, int msgId, int dashId, int deviceId) {
        final int targetsNum = appChannels.size();
        if (targetsNum == 0) {
            return;
        }

        send(appChannels, targetsNum, cmd, msgId, "" + dashId + DEVICE_SEPARATOR + deviceId);
    }

    public void sendToApps(short cmd, int msgId, int dashId, int deviceId, String body) {
        final int targetsNum = appChannels.size();
        if (targetsNum == 0) {
            return;
        }

        send(appChannels, targetsNum, cmd, msgId, prependDashIdAndDeviceId(dashId, deviceId, body));
    }

    private void send(Set<Channel> targets, int targetsNum, short cmd, int msgId, String body) {
        ByteBuf msg = makeUTF8StringMessage(cmd, msgId, body);
        if (targetsNum > 1) {
            msg.retain(targetsNum - 1).markReaderIndex();
        }

        for (Channel channel : targets) {
            if (channel.isWritable()) {
                log.trace("Sending {} to channel {}", body, channel);
                channel.writeAndFlush(msg, channel.voidPromise());
            }
            if (msg.refCnt() > 0) {
                msg.resetReaderIndex();
            }
        }
    }

    public void sendToSharedApps(Channel sendingChannel, String sharedToken, short cmd, int msgId, String body) {
        final Set<Channel> targetChannels = new HashSet<>();
        for (Channel channel : appChannels) {
            if (channel != sendingChannel && channel.isWritable() && needSync(channel, sharedToken)) {
                targetChannels.add(channel);
            }
        }

        final int channelsNum = targetChannels.size();
        if (channelsNum == 0) {
            return;
        }

        send(targetChannels, channelsNum, cmd, msgId, body);
    }

    public boolean isAppConnected() {
        return appChannels.size() > 0;
    }

    public int getAppRequestRate() {
        return getRequestRate(appChannels);
    }

    public int getHardRequestRate() {
        return getRequestRate(hardwareChannels);
    }

    public void closeHardwareChannelByDeviceId(int dashId, int deviceId) {
        for (Channel channel : hardwareChannels) {
            if (isSameDashAndDeviceId(channel, dashId, deviceId)) {
                channel.close();
            }
        }
    }

    public void closeHardwareChannelByDashId(int dashId) {
        for (Channel channel : hardwareChannels) {
            if (isSameDash(channel, dashId)) {
                channel.close();
            }
        }
    }

    public void closeAll() {
        hardwareChannels.forEach(io.netty.channel.Channel::close);
        appChannels.forEach(io.netty.channel.Channel::close);
    }

}
