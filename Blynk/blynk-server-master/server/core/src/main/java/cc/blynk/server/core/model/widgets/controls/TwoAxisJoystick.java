package cc.blynk.server.core.model.widgets.controls;

import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.widgets.HardwareSyncWidget;
import cc.blynk.server.core.model.widgets.MultiPinWidget;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;

import java.util.StringJoiner;

import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.StringUtils.BODY_SEPARATOR_STRING;
import static cc.blynk.utils.StringUtils.prependDashIdAndDeviceId;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class TwoAxisJoystick extends MultiPinWidget implements HardwareSyncWidget {

    public boolean split;

    public boolean autoReturnOn;

    public boolean portraitLocked;

    @Override
    public void sendHardSync(ChannelHandlerContext ctx, int msgId, int deviceId) {
        if (pins == null || this.deviceId != deviceId) {
            return;
        }
        if (split) {
            for (Pin pin : pins) {
                if (pin.notEmpty()) {
                    ctx.write(makeUTF8StringMessage(HARDWARE, msgId, pin.makeHardwareBody()), ctx.voidPromise());
                }
            }
        } else {
            if (pins[0].notEmpty()) {
                ctx.write(makeUTF8StringMessage(HARDWARE, msgId, pins[0].makeHardwareBody()), ctx.voidPromise());
            }
        }
    }

    @Override
    public void sendAppSync(Channel appChannel, int dashId, int targetId) {
        if (pins == null) {
            return;
        }
        if (targetId == ANY_TARGET || this.deviceId == targetId) {
            if (split) {
                for (Pin pin : pins) {
                    if (pin.notEmpty()) {
                        String body = prependDashIdAndDeviceId(dashId, deviceId, pin.makeHardwareBody());
                        appChannel.write(makeUTF8StringMessage(APP_SYNC, SYNC_DEFAULT_MESSAGE_ID, body), appChannel.voidPromise());
                    }
                }
            } else {
                if (pins[0].notEmpty()) {
                    String body = prependDashIdAndDeviceId(dashId, deviceId, pins[0].makeHardwareBody());
                    appChannel.write(makeUTF8StringMessage(APP_SYNC, SYNC_DEFAULT_MESSAGE_ID, body), appChannel.voidPromise());
                }
            }
        }
    }

    @Override
    public String getJsonValue() {
        if (pins == null) {
            return "[]";
        }

        if (isSplitMode()) {
            return super.getJsonValue();
        } else {
            StringJoiner sj = new StringJoiner(",", "[", "]");
            if (pins[0].notEmpty()) {
                for (String pinValue : pins[0].value.split(BODY_SEPARATOR_STRING)) {
                    sj.add("\"" + pinValue + "\"");
                }
            }
            return sj.toString();
        }
    }

    public boolean isSplitMode() {
        return split;
    }

    @Override
    public String getModeType() {
        return "out";
    }


    @Override
    public int getPrice() {
        return 400;
    }
}
