package cc.blynk.server.core.reporting;

import cc.blynk.server.core.model.enums.GraphType;
import cc.blynk.server.core.model.enums.PinType;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.10.15.
 */
public abstract class GraphPinRequest {

    public int dashId;

    public int deviceId;

    public PinType pinType;

    public byte pin;

    public int count;

    public GraphType type;

}
