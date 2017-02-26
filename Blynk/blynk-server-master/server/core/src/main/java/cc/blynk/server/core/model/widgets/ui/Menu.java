package cc.blynk.server.core.model.widgets.ui;

import cc.blynk.server.core.model.widgets.OnePinWidget;
import cc.blynk.utils.StringUtils;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 28.03.16.
 */
public class Menu extends OnePinWidget {

    public volatile String[] labels;

    public String hint;

    @Override
    public String getModeType() {
        return "out";
    }

    @Override
    public int getPrice() {
        return 400;
    }

    @Override
    public void setProperty(String property, String propertyValue) {
        switch (property) {
            case "labels" :
                this.labels = propertyValue.split(StringUtils.BODY_SEPARATOR_STRING);
                break;
            default:
                super.setProperty(property, propertyValue);
                break;
        }
    }
}
