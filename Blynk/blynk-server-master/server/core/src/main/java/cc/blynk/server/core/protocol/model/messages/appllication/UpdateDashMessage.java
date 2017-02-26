package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.UPDATE_DASH;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class UpdateDashMessage extends StringMessage {

    public UpdateDashMessage(int messageId, String body) {
        super(messageId, UPDATE_DASH, body.length(), body);
    }

    @Override
    public String toString() {
        return "UpdateDashMessage{" + super.toString() + "}";
    }
}
