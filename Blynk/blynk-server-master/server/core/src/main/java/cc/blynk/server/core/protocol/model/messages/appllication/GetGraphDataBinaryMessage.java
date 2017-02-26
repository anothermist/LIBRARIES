package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.BinaryMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class GetGraphDataBinaryMessage extends BinaryMessage {

    public GetGraphDataBinaryMessage(int messageId, byte[] data) {
        super(messageId, GET_GRAPH_DATA_RESPONSE, data);
    }

    @Override
    public String toString() {
        return "GetGraphDataBinaryMessage{" + super.toString() + "}";
    }
}
