package cc.blynk.utils;

import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import io.netty.channel.Channel;

/**
 * Used instead of Netty's DefaultAttributeMap as it faster and
 * doesn't involves any synchronization at all.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.09.15.
 */
public class StateHolderUtil {

    public static HardwareStateHolder getHardState(Channel channel) {
        final BaseSimpleChannelInboundHandler handler = channel.pipeline().get(BaseSimpleChannelInboundHandler.class);
        return handler == null ? null : (HardwareStateHolder) handler.state;
    }

    public static boolean isSameDash(Channel channel, int dashId) {
        final BaseSimpleChannelInboundHandler handler = channel.pipeline().get(BaseSimpleChannelInboundHandler.class);
        return ((HardwareStateHolder) handler.state).dashId == dashId;
    }

    public static boolean isSameDashAndDeviceId(Channel channel, int dashId, int deviceId) {
        final BaseSimpleChannelInboundHandler handler = channel.pipeline().get(BaseSimpleChannelInboundHandler.class);
        final HardwareStateHolder hardwareStateHolder = (HardwareStateHolder) handler.state;
        return hardwareStateHolder.dashId == dashId && hardwareStateHolder.deviceId == deviceId;
    }

}
