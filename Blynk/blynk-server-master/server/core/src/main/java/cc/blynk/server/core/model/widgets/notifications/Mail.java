package cc.blynk.server.core.model.widgets.notifications;

import cc.blynk.server.core.model.widgets.NoPinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Mail extends NoPinWidget {

    public String to;

    @Override
    public int getPrice() {
        return 100;
    }

}
