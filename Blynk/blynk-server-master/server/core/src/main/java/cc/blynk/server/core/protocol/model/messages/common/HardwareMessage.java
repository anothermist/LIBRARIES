package cc.blynk.server.core.protocol.model.messages.common;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class HardwareMessage extends StringMessage {

    public HardwareMessage(int messageId, String body) {
        super(messageId, HARDWARE, body.length(), body);
    }

    @Override
    public String toString() {
        return "HardwareMessage{" + super.toString() + "}";
    }
}
