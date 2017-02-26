package cc.blynk.server.core.protocol.exceptions;

import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/23/2015.
 */
public class NotAllowedException extends BaseServerException {

    public NotAllowedException(String message) {
        super(message, Response.NOT_ALLOWED);
    }

}
