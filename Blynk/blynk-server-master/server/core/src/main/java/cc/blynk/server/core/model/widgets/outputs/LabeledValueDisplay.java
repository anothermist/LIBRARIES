package cc.blynk.server.core.model.widgets.outputs;

import cc.blynk.server.core.model.widgets.OnePinReadingWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class LabeledValueDisplay extends OnePinReadingWidget {

    private TextAlignment textAlignment;

    private String valueFormatting;

    @Override
    public String getModeType() {
        return "in";
    }

    @Override
    public int getPrice() {
        return 400;
    }

}
