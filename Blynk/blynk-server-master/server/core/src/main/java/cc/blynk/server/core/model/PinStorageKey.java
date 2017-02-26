package cc.blynk.server.core.model;

import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.utils.StringUtils;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.11.16.
 */
public final class PinStorageKey {

    public final int deviceId;

    public final byte pin;

    public final PinType pinType;

    public PinStorageKey(int deviceId, PinType pinType, byte pin) {
        this.deviceId = deviceId;
        this.pinType = pinType;
        this.pin = pin;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        PinStorageKey that = (PinStorageKey) o;

        if (deviceId != that.deviceId) return false;
        if (pin != that.pin) return false;
        return pinType == that.pinType;

    }

    @Override
    public int hashCode() {
        int result = deviceId;
        result = 31 * result + (int) pin;
        result = 31 * result + (pinType != null ? pinType.hashCode() : 0);
        return result;
    }

    @Override
    public String toString() {
        return String.valueOf(deviceId) + StringUtils.DEVICE_SEPARATOR + pinType.pintTypeChar + pin;
    }
}
