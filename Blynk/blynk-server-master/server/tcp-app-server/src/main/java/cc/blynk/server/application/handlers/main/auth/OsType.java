package cc.blynk.server.application.handlers.main.auth;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 16.03.16.
 */
public enum OsType {

    ANDROID,
    IOS,
    //3d party clients
    OTHER;

    public static OsType parse(String type) {
        switch (type.toLowerCase()) {
            case "ios" :
                return IOS;
            case "android" :
                return ANDROID;
            default:
                return OTHER;
        }
    }

}
