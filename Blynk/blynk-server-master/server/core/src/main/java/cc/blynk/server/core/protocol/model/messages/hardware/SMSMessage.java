package cc.blynk.server.core.protocol.model.messages.hardware;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class SMSMessage extends StringMessage {

    public SMSMessage(int messageId, String body) {
        super(messageId, SMS, body.length(), body);
    }

    @Override
    public String toString() {
        return "SMSMessage{" + super.toString() + "}";
    }
}
