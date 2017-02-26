package cc.blynk.server.core.protocol.exceptions;

import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/3/2015.
 */
public class InvalidTokenException extends BaseServerException {

    public InvalidTokenException(String message, int msgId) {
        super(message, msgId, Response.INVALID_TOKEN);
    }

}
