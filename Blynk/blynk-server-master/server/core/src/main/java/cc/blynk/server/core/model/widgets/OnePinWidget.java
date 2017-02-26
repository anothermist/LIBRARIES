package cc.blynk.server.core.model.widgets;

import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.ui.DeviceSelector;
import cc.blynk.utils.JsonParser;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.StringUtils.BODY_SEPARATOR;
import static cc.blynk.utils.StringUtils.prependDashIdAndDeviceId;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 02.12.15.
 */
//todo all this should be replaced with 1 Pin field.
public abstract class OnePinWidget extends Widget implements AppSyncWidget, HardwareSyncWidget {

    public int deviceId;

    public PinType pinType;

    public byte pin = -1;

    public boolean pwmMode;

    public boolean rangeMappingOn;

    public int min;

    public int max;

    public volatile String value;

    protected static String makeHardwareBody(PinType pinType, byte pin, String value) {
        return "" + pinType.pintTypeChar + 'w' + BODY_SEPARATOR + pin + BODY_SEPARATOR + value;
    }

    @Override
    public void sendAppSync(Channel appChannel, int dashId, int targetId) {
        //do not send SYNC message for widgets assigned to device selector
        //as it will be duplicated later.
        if (this.deviceId >= DeviceSelector.DEVICE_SELECTOR_STARTING_ID) {
            return;
        }
        if (targetId == ANY_TARGET || this.deviceId == targetId) {
            String hardBody = makeHardwareBody();
            if (hardBody != null) {
                String body = prependDashIdAndDeviceId(dashId, this.deviceId, hardBody);
                appChannel.write(makeUTF8StringMessage(APP_SYNC, SYNC_DEFAULT_MESSAGE_ID, body));
            }
        }
    }

    public boolean isNotValid() {
        return pin == Pin.NO_PIN || pinType == null;
    }

    public String makeHardwareBody() {
        if (isNotValid() || value == null) {
            return null;
        }
        return isPWMSupported() ? makeHardwareBody(PinType.ANALOG, pin, value) : makeHardwareBody(pinType, pin, value);
    }

    @Override
    public boolean updateIfSame(int deviceId, byte pin, PinType type, String value) {
        if (isSame(deviceId, pin, type)) {
            this.value = value;
            return true;
        }
        return false;
    }

    @Override
    public void sendHardSync(ChannelHandlerContext ctx, int msgId, int deviceId) {
        if (this.deviceId == deviceId) {
            String body = makeHardwareBody();
            if (body != null) {
                ctx.write(makeUTF8StringMessage(HARDWARE, msgId, body), ctx.voidPromise());
            }
        }
    }

    //todo cover with test
    public boolean isSame(int deviceId, byte pin, PinType type) {
        return this.deviceId == deviceId && this.pin == pin && (
                (type == this.pinType) ||
                (this.isPWMSupported() && type == PinType.ANALOG) ||
                (type == PinType.DIGITAL && this.pinType == PinType.ANALOG)
        );
    }

    @Override
    public String getValue(byte pin, PinType type) {
        return value;
    }

    @Override
    public boolean hasValue(String searchValue) {
        return searchValue.equals(value);
    }

    @Override
    public String getJsonValue() {
        if (value == null) {
            return "[]";
        }
        return JsonParser.valueToJsonAsString(value);
    }

    @Override
    public void append(StringBuilder sb, int deviceId) {
        if (this.deviceId == deviceId) {
            append(sb, pin, pinType, getModeType());
        }
    }

    public boolean isPWMSupported() {
        return false;
    }

}
