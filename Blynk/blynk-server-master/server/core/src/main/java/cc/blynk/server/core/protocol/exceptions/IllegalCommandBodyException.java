package cc.blynk.server.core.protocol.exceptions;

import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/3/2015.
 */
public class IllegalCommandBodyException extends BaseServerException {

    public IllegalCommandBodyException(String message) {
        super(message, Response.ILLEGAL_COMMAND_BODY);
    }

}
