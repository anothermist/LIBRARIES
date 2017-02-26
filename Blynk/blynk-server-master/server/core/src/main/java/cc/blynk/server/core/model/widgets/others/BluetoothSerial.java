package cc.blynk.server.core.model.widgets.others;

import cc.blynk.server.core.model.widgets.NoPinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class BluetoothSerial extends NoPinWidget {

    public String name;

    public int deviceId;

    @Override
    public int getPrice() {
        return 0;
    }
}
