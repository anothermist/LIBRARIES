package cc.blynk.server.core.model.widgets.others;

import cc.blynk.server.core.model.widgets.NoPinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Video extends NoPinWidget {

    public String url;

    @Override
    public int getPrice() {
        return 500;
    }

}
