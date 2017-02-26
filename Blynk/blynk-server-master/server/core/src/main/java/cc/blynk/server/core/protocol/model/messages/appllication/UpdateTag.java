package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.UPDATE_TAG;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class UpdateTag extends StringMessage {

    public UpdateTag(int messageId, String body) {
        super(messageId, UPDATE_TAG, body.length(), body);
    }

    @Override
    public String toString() {
        return "UpdateDevice{" + super.toString() + "}";
    }
}
