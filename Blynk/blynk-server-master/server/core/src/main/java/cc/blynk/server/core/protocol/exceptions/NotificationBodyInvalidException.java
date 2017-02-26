package cc.blynk.server.core.protocol.exceptions;

import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/3/2015.
 */
public class NotificationBodyInvalidException extends BaseServerException {

    public NotificationBodyInvalidException() {
        super("Notification message is empty or larger than limit.", Response.NOTIFICATION_INVALID_BODY);
    }

}
