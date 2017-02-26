package cc.blynk.server.notifications.push.android;

import cc.blynk.server.notifications.push.GCMMessage;
import cc.blynk.server.notifications.push.enums.Priority;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectWriter;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 26.06.15.
 */
public class AndroidGCMMessage implements GCMMessage {

    private static final ObjectWriter writer = new ObjectMapper()
            .setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.NONE)
            .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.ANY)
            .writerFor(AndroidGCMMessage.class);
    private final String to;
    private final Priority priority;
    private final GCMData data;

    public AndroidGCMMessage(String to, Priority priority, String message, int dashId) {
        this.to = to;
        this.priority = priority;
        this.data = new GCMData(message, dashId);
    }

    @Override
    public String getToken() {
        return to;
    }

    @Override
    public String toJson() throws JsonProcessingException {
        return writer.writeValueAsString(this);
    }

    private static class GCMData {
        private final String message;
        private final int dashId;

        public GCMData(String message, int dashId) {
            this.message = message;
            this.dashId = dashId;
        }
    }

}
