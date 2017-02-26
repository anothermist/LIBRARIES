package cc.blynk.server.core.protocol.exceptions;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/3/2015.
 */
public class BaseServerException extends RuntimeException {

    public final int msgId;
    public final int errorCode;

    /**
     * Should be used for handlers that are not processed within BaseSimpleChannelInboundHandler
     */
    public BaseServerException(String message, int msgId, int errorCode) {
        super(message);
        this.msgId = msgId;
        this.errorCode = errorCode;
    }

    public BaseServerException(String message, int errorCode) {
        super(message);
        this.msgId = -1;
        this.errorCode = errorCode;
    }
}
