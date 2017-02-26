package cc.blynk.server.core.model;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 18.05.16.
 */
public class HardwareInfo {

    public String version;

    public String boardType;

    public String cpuType;

    public String connectionType;

    public String buildDate;

    public int heartbeatInterval;

    //used for json.
    public HardwareInfo() {
    }

    public HardwareInfo(String[] info) {
        for (int i = 0; i < info.length; i++) {
            if (i < info.length - 1) {
                intiField(info[i], info[++i]);
            }
        }
    }

    private void intiField(final String key, final String value) {
        switch (key) {
            case "h-beat" :
                try {
                    this.heartbeatInterval = Integer.parseInt(value);
                } catch (NumberFormatException nfe) {
                    this.heartbeatInterval = -1;
                }
                break;
            case "ver" :
                this.version = value;
                break;
            case "dev" :
                this.boardType = value;
                break;
            case "cpu" :
                this.cpuType = value;
                break;
            case "con" :
                this.connectionType = value;
                break;
            case "build" :
                this.buildDate = value;
                break;
        }
    }

}
