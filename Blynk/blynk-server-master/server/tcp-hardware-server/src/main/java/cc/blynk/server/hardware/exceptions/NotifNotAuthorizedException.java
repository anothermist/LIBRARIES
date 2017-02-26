package cc.blynk.server.hardware.exceptions;

import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.exceptions.BaseServerException;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/3/2015.
 */
public class NotifNotAuthorizedException extends BaseServerException {

    public NotifNotAuthorizedException(String message) {
        super(message, Response.NOTIFICATION_NOT_AUTHORIZED);
    }

}
