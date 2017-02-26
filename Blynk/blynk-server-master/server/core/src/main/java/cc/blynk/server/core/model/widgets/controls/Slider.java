package cc.blynk.server.core.model.widgets.controls;

import cc.blynk.server.core.model.widgets.OnePinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Slider extends OnePinWidget {

    public boolean sendOnReleaseOn;

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
