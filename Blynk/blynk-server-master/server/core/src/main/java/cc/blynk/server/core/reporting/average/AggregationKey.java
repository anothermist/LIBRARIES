package cc.blynk.server.core.reporting.average;

import cc.blynk.server.core.model.enums.GraphType;

import java.io.Serializable;
import java.util.Comparator;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 10.08.15.
 */
public final class AggregationKey implements Serializable {

    public static final Comparator<AggregationKey> AGGREGATION_KEY_COMPARATOR = (o1, o2) -> (int) (o1.ts - o2.ts);

    public final String username;
    public final int dashId;
    public final int deviceId;
    public final char pinType;
    public final byte pin;
    public final long ts;

    public AggregationKey(String username, int dashId, int deviceId, char pinType, byte pin, long ts) {
        this.username = username;
        this.dashId = dashId;
        this.deviceId = deviceId;
        this.pinType = pinType;
        this.pin = pin;
        this.ts = ts;
    }

    public long getTs(GraphType type) {
        return ts * type.period;
    }

    public boolean isOutdated(long nowTruncatedToPeriod) {
        return ts < nowTruncatedToPeriod;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        AggregationKey that = (AggregationKey) o;

        if (dashId != that.dashId) return false;
        if (deviceId != that.deviceId) return false;
        if (pinType != that.pinType) return false;
        if (pin != that.pin) return false;
        if (ts != that.ts) return false;
        return !(username != null ? !username.equals(that.username) : that.username != null);

    }

    @Override
    public int hashCode() {
        int result = username != null ? username.hashCode() : 0;
        result = 31 * result + dashId;
        result = 31 * result + deviceId;
        result = 31 * result + (int) pinType;
        result = 31 * result + (int) pin;
        result = 31 * result + (int) (ts ^ (ts >>> 32));
        return result;
    }
}
