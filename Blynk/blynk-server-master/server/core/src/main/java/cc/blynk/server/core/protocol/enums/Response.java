package cc.blynk.server.core.protocol.enums;

import cc.blynk.utils.ReflectionUtil;

import java.util.Map;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public final class Response {

    public static final int OK = 200;
    public static final int QUOTA_LIMIT = 1;
    public static final int ILLEGAL_COMMAND = 2;
    public static final int USER_NOT_REGISTERED = 3;
    public static final int USER_ALREADY_REGISTERED = 4;
    public static final int USER_NOT_AUTHENTICATED = 5;
    public static final int NOT_ALLOWED = 6;
    public static final int DEVICE_NOT_IN_NETWORK = 7;
    public static final int NO_ACTIVE_DASHBOARD = 8;
    public static final int INVALID_TOKEN = 9;
    public static final int ILLEGAL_COMMAND_BODY = 11;
    public static final int GET_GRAPH_DATA = 12;
    public static final int NOTIFICATION_INVALID_BODY = 13;
    public static final int NOTIFICATION_NOT_AUTHORIZED = 14;
    public static final int NOTIFICATION_ERROR = 15;
    //reserved
    public static final int BLYNK_TIMEOUT = 16;
    public static final int NO_DATA = 17;
    public static final int DEVICE_WENT_OFFLINE = 18;
    public static final int SERVER_ERROR = 19;
    public static final int NOT_SUPPORTED_VERSION = 20;
    public static final int ENERGY_LIMIT = 21;

    //all this code just to make logging more user-friendly
    private final static Map<Integer, String> valuesName = ReflectionUtil.generateMapOfValueNameInteger(Response.class);

    public static String getNameByValue(int val) {
        return valuesName.get(val);
    }
    //--------------------------------------------------------
}
