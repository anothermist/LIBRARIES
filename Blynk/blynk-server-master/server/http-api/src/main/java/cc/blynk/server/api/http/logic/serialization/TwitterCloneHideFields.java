package cc.blynk.server.api.http.logic.serialization;

import com.fasterxml.jackson.annotation.JsonIgnore;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 08.07.16.
 */
public abstract class TwitterCloneHideFields {

    @JsonIgnore
    String token;

    @JsonIgnore
    String secret;

    @JsonIgnore
    String username;

}
