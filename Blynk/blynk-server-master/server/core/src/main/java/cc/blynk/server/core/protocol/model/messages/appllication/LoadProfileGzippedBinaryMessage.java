package cc.blynk.server.core.protocol.model.messages.appllication;

import cc.blynk.server.core.protocol.model.messages.BinaryMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class LoadProfileGzippedBinaryMessage extends BinaryMessage {

    public LoadProfileGzippedBinaryMessage(int messageId, byte[] data) {
        super(messageId, LOAD_PROFILE_GZIPPED, data);
    }

    @Override
    public String toString() {
        return "LoadProfileGzippedBinaryMessage{" + super.toString() + "}";
    }
}
