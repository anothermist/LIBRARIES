package cc.blynk.client;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 * Convertor between user-friendly command and protocol command code
 */
public class CommandParser {

    public static Short parseCommand(String stringCommand) {
        switch (stringCommand.toLowerCase()) {
            case "hardware" :
                return HARDWARE;
            case "ping" :
                return PING;
            case "loadprofilegzipped" :
                return LOAD_PROFILE_GZIPPED;
            case "appsync" :
                return APP_SYNC;
            case "sharing" :
                return SHARING;
            case "gettoken" :
                return GET_TOKEN;
            case "refreshtoken" :
                return REFRESH_TOKEN;
            case "login" :
                return LOGIN;
            case "getgraphdata" :
                return GET_GRAPH_DATA;
            case "export" :
                return EXPORT_GRAPH_DATA;
            case "activate" :
                return ACTIVATE_DASHBOARD;
            case "deactivate" :
                return DEACTIVATE_DASHBOARD;
            case "register" :
                return REGISTER;
            case "setproperty" :
                return SET_WIDGET_PROPERTY;

            case "tweet" :
                return TWEET;
            case "email" :
                return EMAIL;
            case "push" :
                return PUSH_NOTIFICATION;
            case "sms" :
                return SMS;
            case "addpushtoken" :
                return ADD_PUSH_TOKEN;

            case "bridge" :
                return BRIDGE;
            case "createdash" :
                return CREATE_DASH;
            case "updatedash" :
                return UPDATE_DASH;
            case "deletedash" :
                return DELETE_DASH;
            case "createwidget" :
                return CREATE_WIDGET;
            case "updatewidget" :
                return UPDATE_WIDGET;
            case "deletewidget" :
                return DELETE_WIDGET;
            case "hardsync" :
                return HARDWARE_SYNC;
            case "internal" :
                return BLYNK_INTERNAL;

            case "createdevice" :
                return CREATE_DEVICE;
            case "updatedevice" :
                return UPDATE_DEVICE;
            case "deletedevice" :
                return DELETE_DEVICE;
            case "getdevices" :
                return GET_DEVICES;

            case "createtag" :
                return CREATE_TAG;
            case "updatetag" :
                return UPDATE_TAG;
            case "deletetag" :
                return DELETE_TAG;
            case "gettags" :
                return GET_TAGS;

            case "addenergy" :
                return ADD_ENERGY;
            case "getenergy" :
                return GET_ENERGY;

            case "getserver" :
                return GET_SERVER;

            //sharing section
            case "sharelogin" :
                return SHARE_LOGIN;
            case "getsharetoken" :
                return GET_SHARE_TOKEN;
            case "getshareddash" :
                return GET_SHARED_DASH;
            case "refreshsharetoken" :
                return REFRESH_SHARE_TOKEN;

            default:
                throw new IllegalArgumentException("Unsupported command");
        }
    }

}
