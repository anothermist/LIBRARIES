package cc.blynk.server.core.model.widgets.ui;

import cc.blynk.server.core.model.widgets.NoPinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.02.16.
 */
public class Tabs extends NoPinWidget {

    public Tab[] tabs;

    public boolean bubbleOn;

    public volatile int color;

    public int textColor;

    public Tabs() {
        this.tabId = -1;
    }

    @Override
    public int getPrice() {
        return 0;
    }

}
