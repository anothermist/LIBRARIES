package cc.blynk.server.core.stats.model;

import static cc.blynk.server.core.protocol.enums.Command.HTTP_EMAIL;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_GET_HISTORY_DATA;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_GET_PIN_DATA;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_GET_PROJECT;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_IS_APP_CONNECTED;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_IS_HARDWARE_CONNECTED;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_NOTIFY;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_QR;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_TOTAL;
import static cc.blynk.server.core.protocol.enums.Command.HTTP_UPDATE_PIN_DATA;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.01.17.
 */
public class HttpStat {

    public int isHardwareConnected;
    public int isAppConnected;
    public int getPinData;
    public int updatePinData;
    public int email;
    public int notify;
    public int getProject;
    public int qr;
    public int getHistoryPinData;
    public int total;

    public void assign(short field, int val) {
        switch (field) {
            case HTTP_IS_HARDWARE_CONNECTED :
                this.isHardwareConnected = val;
                break;
            case HTTP_IS_APP_CONNECTED :
                this.isAppConnected = val;
                break;
            case HTTP_GET_PIN_DATA :
                this.getPinData = val;
                break;
            case HTTP_UPDATE_PIN_DATA :
                this.updatePinData = val;
                break;
            case HTTP_NOTIFY :
                this.notify = val;
                break;
            case HTTP_EMAIL :
                this.email = val;
                break;
            case HTTP_GET_PROJECT :
                this.getProject = val;
                break;
            case HTTP_QR :
                this.qr = val;
                break;
            case HTTP_GET_HISTORY_DATA:
                this.getHistoryPinData = val;
                break;
            case HTTP_TOTAL :
                this.total = val;
                break;
        }
    }
}

