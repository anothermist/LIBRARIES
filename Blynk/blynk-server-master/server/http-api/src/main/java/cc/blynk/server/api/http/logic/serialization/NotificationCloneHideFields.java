package cc.blynk.server.api.http.logic.serialization;

import com.fasterxml.jackson.annotation.JsonIgnore;

import java.util.Map;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 08.07.16.
 */
public abstract class NotificationCloneHideFields {

    @JsonIgnore
    Map<String, String> androidTokens;

    @JsonIgnore
    Map<String, String> iOSTokens;

}
