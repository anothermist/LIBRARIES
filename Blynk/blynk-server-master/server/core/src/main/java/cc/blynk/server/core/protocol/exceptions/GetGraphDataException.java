package cc.blynk.server.core.protocol.exceptions;

import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/3/2015.
 */
public class GetGraphDataException extends BaseServerException {

    public GetGraphDataException() {
        super("Server exception!", Response.GET_GRAPH_DATA);
    }

}
