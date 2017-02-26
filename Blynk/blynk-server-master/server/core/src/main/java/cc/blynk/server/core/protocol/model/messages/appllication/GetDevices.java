package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.GET_DEVICES;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class GetDevices extends StringMessage {

    public GetDevices(int messageId, String body) {
        super(messageId, GET_DEVICES, body.length(), body);
    }

    @Override
    public String toString() {
        return "GetDevices{" + super.toString() + "}";
    }
}
