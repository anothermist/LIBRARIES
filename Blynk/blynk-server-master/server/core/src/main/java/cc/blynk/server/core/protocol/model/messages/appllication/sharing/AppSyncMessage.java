package cc.blynk.server.core.protocol.model.messages.appllication.sharing;

import cc.blynk.server.core.model.widgets.AppSyncWidget;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.model.messages.StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 27.10.15.
 */
public class AppSyncMessage extends StringMessage {

    //only for tests
    public AppSyncMessage(String body) {
        super(AppSyncWidget.SYNC_DEFAULT_MESSAGE_ID, Command.APP_SYNC, body.length(), body);
    }

    public AppSyncMessage(int messageId, String body) {
        super(messageId, Command.APP_SYNC, body.length(), body);
    }

    @Override
    public String toString() {
        return "AppSyncMessage{" + super.toString() + "}";
    }
}
