package cc.blynk.server.core.protocol.model.messages.hardware;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class PushMessage extends StringMessage {

    public PushMessage(int messageId, String body) {
        super(messageId, PUSH_NOTIFICATION, body.length(), body);
    }

    @Override
    public String toString() {
        return "PushMessage{" + super.toString() + "}";
    }
}
