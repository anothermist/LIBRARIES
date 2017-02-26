package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.GET_TAGS;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class GetTags extends StringMessage {

    public GetTags(int messageId, String body) {
        super(messageId, GET_TAGS, body.length(), body);
    }

    @Override
    public String toString() {
        return "GetTags{" + super.toString() + "}";
    }
}
