package cc.blynk.server.core.model.widgets;

import cc.blynk.server.core.model.enums.PinType;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.04.16.
 */
public abstract class NoPinWidget extends Widget {

    @Override
    public boolean updateIfSame(int deviceId, byte pin, PinType type, String values) {
        return false;
    }

    @Override
    public boolean isSame(int deviceId, byte pin, PinType type) {
        return false;
    }

    @Override
    public String getJsonValue() {
        return null;
    }

    @Override
    public String getModeType() {
        return null;
    }

    @Override
    public String getValue(byte pin, PinType type) {
        return null;
    }

    @Override
    public boolean hasValue(String searchValue) {
        return false;
    }

    @Override
    public void append(StringBuilder sb, int deviceId) {
    }

}
