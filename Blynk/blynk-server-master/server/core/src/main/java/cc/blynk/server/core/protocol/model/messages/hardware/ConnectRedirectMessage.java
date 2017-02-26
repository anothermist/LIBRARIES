package cc.blynk.server.core.protocol.model.messages.hardware;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.CONNECT_REDIRECT;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class ConnectRedirectMessage extends StringMessage {

    public ConnectRedirectMessage(int messageId, String body) {
        super(messageId, CONNECT_REDIRECT, body.length(), body);
    }

    @Override
    public String toString() {
        return "ConnectRedirectMessage{" + super.toString() + "}";
    }
}
