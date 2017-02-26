package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.GET_SERVER;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class GetServerMessage extends StringMessage {

    public GetServerMessage(int messageId, String body) {
        super(messageId, GET_SERVER, body.length(), body);
    }

    @Override
    public String toString() {
        return "GetServerMessage{" + super.toString() + "}";
    }
}
