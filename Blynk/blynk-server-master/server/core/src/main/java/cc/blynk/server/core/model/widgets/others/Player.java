package cc.blynk.server.core.model.widgets.others;

import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.OnePinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 28.03.16.
 */
public class Player extends OnePinWidget {

    public volatile boolean isOnPlay;

    @Override
    public boolean updateIfSame(int deviceId, byte pin, PinType type, String value) {
        if (isSame(deviceId, pin, type)) {
            this.value = value;
            switch (value) {
                case "play" :
                    isOnPlay = true;
                    break;
                case "stop" :
                    isOnPlay = false;
                    break;
            }
            return true;
        }
        return false;
    }

    @Override
    public String getModeType() {
        return "out";
    }

    @Override
    public int getPrice() {
        return 400;
    }

    @Override
    public void setProperty(String property, String propertyValue) {
        switch (property) {
            case "isOnPlay" :
                this.isOnPlay = Boolean.parseBoolean(propertyValue);
                break;
            default:
                super.setProperty(property, propertyValue);
                break;
        }
    }
}
