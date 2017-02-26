package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.DELETE_DEVICE;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class DeleteDevice extends StringMessage {

    public DeleteDevice(int messageId, String body) {
        super(messageId, DELETE_DEVICE, body.length(), body);
    }

    @Override
    public String toString() {
        return "DeleteDevice{" + super.toString() + "}";
    }
}
