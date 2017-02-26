package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.StringMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class ExportDataMessage extends StringMessage {

    public ExportDataMessage(int messageId, String body) {
        super(messageId, EXPORT_GRAPH_DATA, body.length(), body);
    }

    @Override
    public String toString() {
        return "ExportGraphData{" + super.toString() + "}";
    }
}
