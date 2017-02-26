package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.DELETE_TAG;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class DeleteTag extends StringMessage {

    public DeleteTag(int messageId, String body) {
        super(messageId, DELETE_TAG, body.length(), body);
    }

    @Override
    public String toString() {
        return "DeleteTag{" + super.toString() + "}";
    }
}
