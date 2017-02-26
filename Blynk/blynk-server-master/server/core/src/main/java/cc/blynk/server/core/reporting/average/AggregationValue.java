package cc.blynk.server.core.reporting.average;

import java.io.Serializable;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 10.08.15.
 */
public class AggregationValue implements Serializable {

    private double values = 0;
    private long count = 0;

    public void update(double val) {
        values += val;
        count++;
    }

    public double calcAverage() {
        return values / count;
    }
}
