package cc.blynk.server.core.model;

import cc.blynk.server.core.model.enums.PinType;

import static cc.blynk.utils.StringUtils.BODY_SEPARATOR;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 03.07.15.
 */
public class Pin {

    public static final int NO_PIN = -1;

    public byte pin = NO_PIN;

    public boolean pwmMode;

    public boolean rangeMappingOn;

    public PinType pinType;

    public String value;

    public int min;

    public int max = 255;

    public String label;

    public Pin() {
    }

    public Pin(int pin, PinType pinType) {
        this.pin = (byte) pin;
        this.pinType = pinType;
    }

    public Pin(byte pin, PinType pinType) {
        this.pin = pin;
        this.pinType = pinType;
    }

    public static String makeReadingHardwareBody(char pinType, byte pin) {
        return "" + pinType + 'r' + BODY_SEPARATOR + pin;
    }

    public static String makeHardwareBody(char pinType, String pin, String value) {
        return "" + pinType + 'w'
                + BODY_SEPARATOR + pin
                + BODY_SEPARATOR + value;
    }

    public static String makeHardwareBody(PinType pinType, byte pin, String value) {
        return "" + pinType.pintTypeChar + 'w'
                + BODY_SEPARATOR + pin
                + BODY_SEPARATOR + value;
    }

    public static String makeHardwareBody(boolean pwmMode, PinType pinType, byte pin, String value) {
        return pwmMode ? makeHardwareBody(PinType.ANALOG, pin, value) : makeHardwareBody(pinType, pin, value);
    }

    public boolean isSame(byte pin, PinType type) {
        return this.pin == pin && ((this.pwmMode && type == PinType.ANALOG) || (type == this.pinType));
    }

    public String makeHardwareBody() {
        return pwmMode ? makeHardwareBody(PinType.ANALOG, pin, value) : makeHardwareBody(pinType, pin, value);
    }

    public boolean isNotValid() {
        return pin == NO_PIN || pinType == null;
    }

    public boolean notEmpty() {
        return value != null && !isNotValid();
    }
}
