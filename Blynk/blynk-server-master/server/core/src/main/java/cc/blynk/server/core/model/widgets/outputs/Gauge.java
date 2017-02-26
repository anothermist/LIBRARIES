package cc.blynk.server.core.model.widgets.outputs;

import cc.blynk.server.core.model.widgets.OnePinReadingWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Gauge extends OnePinReadingWidget {

    private String valueFormatting;

    @Override
    public String getModeType() {
        return "in";
    }

    @Override
    public int getPrice() {
        return 300;
    }
}
