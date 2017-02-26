package cc.blynk.server.core.model.enums;

import cc.blynk.server.core.protocol.exceptions.IllegalCommandBodyException;

/**
 * User: ddumanskiy
 * Date: 10.12.13
 * Time: 10:15
 */
public enum PinType {

    DIGITAL('d'),
    VIRTUAL('v'),
    ANALOG('a');

    public final char pintTypeChar;
    public final String pinTypeString;

    PinType(char pinType) {
        this.pintTypeChar = pinType;
        this.pinTypeString = String.valueOf(pinType);
    }

    public static PinType getPinType(char pinTypeChar) {
        switch (pinTypeChar) {
            case 'a' :
            case 'A' :
                return ANALOG;
            case 'v' :
            case 'V' :
                return VIRTUAL;
            case 'd' :
            case 'D' :
                return DIGITAL;
            default:
                throw new IllegalCommandBodyException("Invalid pin type.");
        }
    }

    public static String getPinTypeString(char pinTypeChar) {
        switch (pinTypeChar) {
            case 'a' :
            case 'A' :
                return ANALOG.pinTypeString;
            case 'v' :
            case 'V' :
                return VIRTUAL.pinTypeString;
            case 'd' :
            case 'D' :
                return DIGITAL.pinTypeString;
            default:
                throw new IllegalCommandBodyException("Invalid pin type.");
        }
    }
}
