package cc.blynk.server.core.model.widgets.ui;

import cc.blynk.server.core.model.widgets.NoPinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.09.16.
 */
public class Image extends NoPinWidget {

    public String imageId;

    @Override
    public int getPrice() {
        return 200;
    }
}
