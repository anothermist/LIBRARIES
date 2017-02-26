package cc.blynk.server.application.handlers.main.logic.reporting;

import cc.blynk.server.core.model.enums.GraphType;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.reporting.GraphPinRequest;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.10.15.
 */
public class GraphPinRequestData extends GraphPinRequest {

    public GraphPinRequestData(int dashId, int deviceId, String[] messageParts, final int pinIndex, int valuesPerPin) {
        try {
            this.dashId = dashId;
            this.deviceId = deviceId;
            pinType = PinType.getPinType(messageParts[pinIndex * valuesPerPin].charAt(0));
            pin = Byte.parseByte(messageParts[pinIndex * valuesPerPin + 1]);
            count = Integer.parseInt(messageParts[pinIndex * valuesPerPin + 2]);
            type = GraphType.getPeriodByType(messageParts[pinIndex * valuesPerPin + 3].charAt(0));
        } catch (NumberFormatException e) {
            throw new IllegalCommandException("HardwareLogic command body incorrect.");
        }
    }
}
