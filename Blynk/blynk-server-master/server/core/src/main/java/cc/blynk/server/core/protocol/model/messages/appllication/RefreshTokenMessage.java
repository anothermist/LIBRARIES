package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class RefreshTokenMessage extends StringMessage {

    public RefreshTokenMessage(int messageId, String body) {
        super(messageId, REFRESH_TOKEN, body.length(), body);
    }

    @Override
    public String toString() {
        return "RefreshTokenMessage{" + super.toString() + "}";
    }
}
