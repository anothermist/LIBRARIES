package cc.blynk.server.core.protocol.model.messages.hardware;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.APP_CONNECTED;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class AppConnectedMessage extends StringMessage {

    public AppConnectedMessage(int messageId) {
        super(messageId, APP_CONNECTED, 0, "");
    }

    @Override
    public String toString() {
        return "ConnectRedirectMessage{" + super.toString() + "}";
    }
}
