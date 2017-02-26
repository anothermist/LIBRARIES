package cc.blynk.server.core.model.widgets;

import io.netty.channel.ChannelHandlerContext;

/**
 * Marker interface. Used in order to define if pin value from this widget should be sent back
 * to hardware on HARDWARE_SYNC command.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 03.12.15.
 */
public interface HardwareSyncWidget {

    void sendHardSync(ChannelHandlerContext ctx, int msgId, int deviceId);

}
