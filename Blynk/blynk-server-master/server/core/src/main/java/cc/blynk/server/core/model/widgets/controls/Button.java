package cc.blynk.server.core.model.widgets.controls;

import cc.blynk.server.core.model.widgets.OnePinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Button extends OnePinWidget {

    public boolean pushMode;

    public volatile String onLabel;

    public volatile String offLabel;

    @Override
    public String getModeType() {
        return "out";
    }

    @Override
    public int getPrice() {
        return 200;
    }

    @Override
    public void setProperty(String property, String propertyValue) {
        switch (property) {
            case "onLabel" :
                this.onLabel = propertyValue;
                break;
            case "offLabel" :
                this.offLabel = propertyValue;
                break;
            default:
                super.setProperty(property, propertyValue);
                break;
        }
    }
}
