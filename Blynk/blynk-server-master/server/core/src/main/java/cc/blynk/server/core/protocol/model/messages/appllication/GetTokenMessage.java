package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class GetTokenMessage extends StringMessage {

    public GetTokenMessage(int messageId, String body) {
        super(messageId, GET_TOKEN, body.length(), body);
    }

    @Override
    public String toString() {
        return "GetTokenMessage{" + super.toString() + "}";
    }
}
