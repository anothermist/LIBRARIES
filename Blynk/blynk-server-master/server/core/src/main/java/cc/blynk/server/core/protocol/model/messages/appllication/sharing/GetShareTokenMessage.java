package cc.blynk.server.core.protocol.model.messages.appllication.sharing;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class GetShareTokenMessage extends StringMessage {

    public GetShareTokenMessage(int messageId, String body) {
        super(messageId, GET_SHARE_TOKEN, body.length(), body);
    }

    @Override
    public String toString() {
        return "GetshareTokenMessage{" + super.toString() + "}";
    }
}
