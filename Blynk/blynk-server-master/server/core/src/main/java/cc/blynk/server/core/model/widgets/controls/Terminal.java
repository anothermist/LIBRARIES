package cc.blynk.server.core.model.widgets.controls;

import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.OnePinWidget;
import cc.blynk.utils.ParseUtil;
import cc.blynk.utils.structure.LimitedArrayDeque;
import io.netty.channel.Channel;

import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.StringUtils.prependDashIdAndDeviceId;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Terminal extends OnePinWidget {

    private static final int POOL_SIZE = ParseUtil.parseInt(System.getProperty("terminal.strings.pool.size", "25"));
    private transient final LimitedArrayDeque<String> lastCommands = new LimitedArrayDeque<>(POOL_SIZE);

    public boolean autoScrollOn;

    public boolean terminalInputOn;

    public boolean textLightOn;

    @Override
    public boolean updateIfSame(int deviceId, byte pin, PinType type, String value) {
        if (isSame(deviceId, pin, type)) {
            //todo remove this line after migration.
            this.value = null;
            this.lastCommands.add(value);
            return true;
        }
        return false;
    }

    @Override
    public void sendAppSync(Channel appChannel, int dashId, int targetId) {
        if (pin == Pin.NO_PIN || pinType == null || lastCommands.size() == 0) {
            return;
        }
        if (targetId == ANY_TARGET || this.deviceId == targetId) {
            for (String storedValue : lastCommands) {
                String body = prependDashIdAndDeviceId(dashId, deviceId, makeHardwareBody(pinType, pin, storedValue));
                appChannel.write(makeUTF8StringMessage(APP_SYNC, SYNC_DEFAULT_MESSAGE_ID, body));
            }
        }
    }

    @Override
    public String makeHardwareBody() {
        if (isNotValid() || lastCommands.size() == 0) {
            return null;
        }
        return isPWMSupported() ?
                makeHardwareBody(PinType.ANALOG, pin, lastCommands.getLast()) :
                makeHardwareBody(pinType, pin, lastCommands.getLast());
    }

    @Override
    public String getModeType() {
        return "in";
    }

    @Override
    public int getPrice() {
        return 200;
    }
}
