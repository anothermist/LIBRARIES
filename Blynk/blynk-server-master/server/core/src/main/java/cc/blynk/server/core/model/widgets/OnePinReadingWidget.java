package cc.blynk.server.core.model.widgets;

import cc.blynk.server.core.model.Pin;
import io.netty.buffer.ByteBuf;
import io.netty.channel.Channel;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 02.02.17.
 */
public abstract class OnePinReadingWidget extends OnePinWidget implements FrequencyWidget {

    private int frequency;

    private transient long lastRequestTS;

    @Override
    public boolean isTicked(long now) {
        if (frequency > 0 && now > lastRequestTS + frequency) {
            this.lastRequestTS = now;
            return true;
        }
        return false;
    }

    @Override
    public int getDeviceId() {
        return deviceId;
    }

    @Override
    public void writeReadingCommand(Channel channel) {
        if (isNotValid()) {
            return;
        }
        ByteBuf msg = makeUTF8StringMessage(HARDWARE, READING_MSG_ID, Pin.makeReadingHardwareBody(pinType.pintTypeChar, pin));
        channel.write(msg, channel.voidPromise());
    }

}
