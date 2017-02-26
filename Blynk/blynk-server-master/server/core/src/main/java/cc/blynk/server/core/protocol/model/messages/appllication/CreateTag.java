package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.CREATE_TAG;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class CreateTag extends StringMessage {

    public CreateTag(int messageId, String body) {
        super(messageId, CREATE_TAG, body.length(), body);
    }

    @Override
    public String toString() {
        return "CreateTag{" + super.toString() + "}";
    }
}
