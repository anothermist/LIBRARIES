package cc.blynk.server.core.stats.model;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.01.17.
 */
public class CommandStat {

    public int response;
    public int redeem;
    public int hardwareConnected;
    public int register;
    public int login;
    public int loadProfile;
    public int appSync;
    public int sharing;
    public int getToken;
    public int ping;
    public int activate;
    public int deactivate;
    public int refreshToken;
    public int getGraphData;
    public int exportGraphData;
    public int setWidgetProperty;
    public int bridge;
    public int hardware;
    public int getSharedDash;
    public int getShareToken;
    public int refreshShareToken;
    public int shareLogin;
    public int createProject;
    public int updateProject;
    public int deleteProject;
    public int hardwareSync;
    public int internal;

    public int sms;
    public int tweet;
    public int email;
    public int push;
    public int addPushToken;

    public int createWidget;
    public int updateWidget;
    public int deleteWidget;

    public int createDevice;
    public int updateDevice;
    public int deleteDevice;
    public int getDevices;

    public int createTag;
    public int updateTag;
    public int deleteTag;
    public int getTags;

    public int addEnergy;
    public int getEnergy;

    public int getServer;
    public int connectRedirect;

    public int webSockets;

    public int eventor;
    public int webhooks;

    public int appTotal;
    public int hardTotal;

    public void assign(short field, int val) {
        switch (field) {
            case RESPONSE :
                this.response = val;
                break;
            case REDEEM :
                this.redeem = val;
                break;
            case HARDWARE_CONNECTED :
                this.hardwareConnected = val;
                break;
            case REGISTER :
                this.register = val;
                break;
            case LOGIN :
                this.login = val;
                break;
            case LOAD_PROFILE_GZIPPED :
                this.loadProfile = val;
                break;
            case APP_SYNC :
                this.appSync = val;
                break;
            case SHARING :
                this.sharing = val;
                break;
            case GET_TOKEN :
                this.getToken = val;
                break;
            case PING :
                this.ping = val;
                break;
            case SMS :
                this.sms = val;
                break;
            case ACTIVATE_DASHBOARD :
                this.activate = val;
                break;
            case DEACTIVATE_DASHBOARD :
                this.deactivate = val;
                break;
            case REFRESH_TOKEN :
                this.refreshToken = val;
                break;
            case GET_GRAPH_DATA :
                this.getGraphData = val;
                break;
            case EXPORT_GRAPH_DATA :
                this.exportGraphData = val;
                break;
            case SET_WIDGET_PROPERTY :
                this.setWidgetProperty = val;
                break;
            case BRIDGE :
                this.bridge = val;
                break;
            case HARDWARE :
                this.hardware = val;
                break;
            case GET_SHARED_DASH :
                this.getSharedDash = val;
                break;
            case GET_SHARE_TOKEN :
                this.getShareToken = val;
                break;
            case REFRESH_SHARE_TOKEN :
                this.refreshShareToken = val;
                break;
            case SHARE_LOGIN :
                this.shareLogin = val;
                break;
            case CREATE_DASH :
                this.createProject = val;
                break;
            case UPDATE_DASH :
                this.updateProject = val;
                break;
            case DELETE_DASH :
                this.deleteProject = val;
                break;
            case HARDWARE_SYNC :
                this.hardwareSync = val;
                break;
            case BLYNK_INTERNAL :
                this.internal = val;
                break;
            case ADD_PUSH_TOKEN :
                this.addPushToken = val;
                break;
            case TWEET :
                this.tweet = val;
                break;
            case EMAIL :
                this.email = val;
                break;
            case PUSH_NOTIFICATION :
                this.push = val;
                break;
            case CREATE_WIDGET :
                this.createWidget = val;
                break;
            case UPDATE_WIDGET :
                this.updateWidget = val;
                break;
            case DELETE_WIDGET :
                this.deleteWidget = val;
                break;
            case CREATE_DEVICE :
                this.createDevice = val;
                break;
            case UPDATE_DEVICE :
                this.updateDevice = val;
                break;
            case DELETE_DEVICE :
                this.deleteDevice = val;
                break;
            case GET_DEVICES :
                this.getDevices = val;
                break;
            case CREATE_TAG :
                this.createTag = val;
                break;
            case UPDATE_TAG :
                this.updateTag = val;
                break;
            case DELETE_TAG :
                this.deleteTag = val;
                break;
            case GET_TAGS :
                this.getTags = val;
                break;
            case ADD_ENERGY :
                this.addEnergy = val;
                break;
            case GET_ENERGY :
                this.getEnergy = val;
                break;
            case GET_SERVER :
                this.getServer = val;
                break;
            case CONNECT_REDIRECT :
                this.connectRedirect = val;
                break;
            case WEB_SOCKETS :
                this.webSockets = val;
                break;
            case EVENTOR :
                this.eventor = val;
                break;
            case WEB_HOOKS :
                this.webhooks = val;
                break;
        }
    }

}
