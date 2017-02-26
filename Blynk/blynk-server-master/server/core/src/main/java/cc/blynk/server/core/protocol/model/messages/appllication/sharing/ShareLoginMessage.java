package cc.blynk.server.core.protocol.model.messages.appllication.sharing;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class ShareLoginMessage extends StringMessage {

    public ShareLoginMessage(int messageId, String body) {
        super(messageId, SHARE_LOGIN, body.length(), body);
    }

    @Override
    public String toString() {
        return "ShareLoginMessage{" + super.toString() + "}";
    }
}
