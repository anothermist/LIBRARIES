package cc.blynk.server.core.protocol.model.messages;

import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class ResponseMessage extends MessageBase {

    public ResponseMessage(int messageId, int responseCode) {
        super(messageId, Command.RESPONSE, responseCode);
    }

    @Override
    public byte[] getBytes() {
        return null;
    }

    @Override
    public String toString() {
        return "ResponseMessage{id=" + id +
                ", command=" + Command.getNameByValue(command) +
                ", responseCode=" + Response.getNameByValue(length) + "}";
    }
}
