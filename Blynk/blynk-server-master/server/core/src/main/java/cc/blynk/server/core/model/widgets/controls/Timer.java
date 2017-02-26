package cc.blynk.server.core.model.widgets.controls;

import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.widgets.OnePinWidget;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.StringUtils;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Timer extends OnePinWidget {

    public int startTime = -1;

    public String startValue;

    public int stopTime = -1;

    public String stopValue;

    public boolean isValidStart() {
        return isValidTime(startTime) && isValidValue(startValue);
    }

    public boolean isValidStop() {
        return isValidTime(stopTime) && isValidValue(stopValue);
    }

    private static boolean isValidTime(int time) {
        return time > -1 && time < 86400;
    }

    private static boolean isValidValue(String value) {
        return value != null && !value.isEmpty();
    }

    @Override
    public void sendHardSync(ChannelHandlerContext ctx, int msgId, int deviceId) {
        if (value != null && this.deviceId == deviceId) {
            ctx.write(makeUTF8StringMessage(HARDWARE, msgId, value), ctx.voidPromise());
        }
    }

    @Override
    public String makeHardwareBody() {
        if (pin == Pin.NO_PIN || value == null || pinType == null) {
            return null;
        }
        return value;
    }

    @Override
    //todo refactor value of timer.
    public String getJsonValue() {
        if (value == null) {
            return "[]";
        }
        String[] values = value.split(StringUtils.BODY_SEPARATOR_STRING);
        if (values.length == 3) {
            return JsonParser.valueToJsonAsString(values[2]);
        } else {
            return JsonParser.valueToJsonAsString(value);
        }
    }

    @Override
    public String getModeType() {
        return "out";
    }

    @Override
    public int getPrice() {
        return 200;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Timer)) return false;

        Timer timer = (Timer) o;

        return id == timer.id;

    }

    @Override
    public int hashCode() {
        return (int) (id ^ (id >>> 32));
    }
}
