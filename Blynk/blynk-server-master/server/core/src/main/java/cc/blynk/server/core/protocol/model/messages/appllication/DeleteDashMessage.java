package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class DeleteDashMessage extends StringMessage {

    public DeleteDashMessage(int messageId, String body) {
        super(messageId, DELETE_DASH, body.length(), body);
    }

    @Override
    public String toString() {
        return "DeleteDashMessage{" + super.toString() + "}";
    }
}
