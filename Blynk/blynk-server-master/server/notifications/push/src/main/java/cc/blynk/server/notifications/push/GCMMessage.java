package cc.blynk.server.notifications.push;

import com.fasterxml.jackson.core.JsonProcessingException;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 16.07.15.
 */
public interface GCMMessage {

    String getToken();

    String toJson() throws JsonProcessingException;

}
