package cc.blynk.server.core.model.widgets.others.eventor.model.action.notification;

import cc.blynk.server.core.model.widgets.others.eventor.model.action.BaseAction;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.08.16.
 */
public abstract class NotificationAction extends BaseAction {

    public String message;

    @Override
    public boolean isValid() {
        return message != null && !message.isEmpty();
    }

}
