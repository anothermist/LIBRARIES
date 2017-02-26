package cc.blynk.utils;

import cc.blynk.server.core.protocol.exceptions.ParseException;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 1/31/2015.
 */
public final class ParseUtil {

    public static int parseInt(String intString) {
        try {
            return Integer.parseInt(intString);
        } catch (NumberFormatException ex) {
            throw new ParseException(intString + " not a valid int number.");
        }
    }

    public static byte parseByte(String byteString) {
        try {
            return Byte.parseByte(byteString);
        } catch (NumberFormatException ex) {
            throw new ParseException(byteString + " not a valid byte number.");
        }
    }

    public static long parseLong(String longString) {
        try {
            return Long.parseLong(longString);
        } catch (NumberFormatException nfe) {
            throw new ParseException(longString + " not a valid long number.");
        }
    }

}
