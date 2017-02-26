package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.CREATE_DEVICE;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class CreateDevice extends StringMessage {

    public CreateDevice(int messageId, String body) {
        super(messageId, CREATE_DEVICE, body.length(), body);
    }

    @Override
    public String toString() {
        return "CreateDevice{" + super.toString() + "}";
    }
}
