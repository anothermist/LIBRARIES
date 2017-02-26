package cc.blynk.server.core.model.widgets.controls;

import cc.blynk.server.core.model.widgets.OnePinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class OneAxisJoystick extends OnePinWidget {

    public boolean autoReturnOn;

    public boolean horizontal;

    @Override
    public String getModeType() {
        return "out";
    }

    @Override
    public boolean isPWMSupported() {
        return pwmMode;
    }

    @Override
    public int getPrice() {
        return 200;
    }

}
