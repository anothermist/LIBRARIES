package cc.blynk.server.core.protocol.model.messages;

import java.nio.charset.StandardCharsets;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public abstract class StringMessage extends MessageBase {

    public String body;

    public StringMessage(int messageId, short command, int length, String body) {
        super(messageId, command, length);
        this.body = body;
    }

    @Override
    public byte[] getBytes() {
        return body.getBytes(StandardCharsets.UTF_8);
    }

    @Override
    public String toString() {
        return super.toString() + ", body='" + body + "'";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        if (!super.equals(o)) return false;

        StringMessage that = (StringMessage) o;

        return !(body != null ? !body.equals(that.body) : that.body != null);

    }

    @Override
    public int hashCode() {
        int result = super.hashCode();
        result = 31 * result + (body != null ? body.hashCode() : 0);
        return result;
    }
}
