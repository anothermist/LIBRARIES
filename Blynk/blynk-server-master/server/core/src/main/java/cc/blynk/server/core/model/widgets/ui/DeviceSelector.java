package cc.blynk.server.core.model.widgets.ui;

import cc.blynk.server.core.model.widgets.NoPinWidget;
import cc.blynk.server.core.model.widgets.Target;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 02.02.17.
 */
public class DeviceSelector extends NoPinWidget implements Target {

    public static final int DEVICE_SELECTOR_STARTING_ID = 200_000;

    //this is selected deviceId in widget
    public volatile int value = 0;

    //this is only for UI. to show only those devices in picker.
    public int[] deviceIds;

    @Override
    public int[] getDeviceIds() {
        return new int[] {value};
    }

    @Override
    public int getPrice() {
        return 1900;
    }

}
