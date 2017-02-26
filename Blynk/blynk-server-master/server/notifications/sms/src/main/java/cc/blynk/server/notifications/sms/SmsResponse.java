package cc.blynk.server.notifications.sms;

import com.fasterxml.jackson.annotation.JsonProperty;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 20.03.16.
 */
public class SmsResponse {

    @JsonProperty("message-count")
    public String messageCount;

    public Message[] messages;

    public static class Message {

        public String status;

        @JsonProperty("message-id")
        public String messageId;

        @JsonProperty("error-text")
        public String error;

        @JsonProperty("message-price")
        public String price;

        @JsonProperty("remaining-balance")
        public String remainingBalance;

    }
}
