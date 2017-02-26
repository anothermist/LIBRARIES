package cc.blynk.server.core.protocol.exceptions;

import cc.blynk.server.core.protocol.enums.Response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/23/2015.
 */
public class EnergyLimitException extends BaseServerException {

    public EnergyLimitException(String message) {
        super(message, Response.ENERGY_LIMIT);
    }

}
