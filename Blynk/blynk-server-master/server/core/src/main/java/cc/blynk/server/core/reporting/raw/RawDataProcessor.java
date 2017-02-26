package cc.blynk.server.core.reporting.raw;

import cc.blynk.server.core.reporting.average.AggregationKey;
import cc.blynk.utils.NumberUtil;

import java.util.Collections;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Simply stores every record in memory that should be stored in reporting DB lately.
 * Could cause OOM at high request rate. However we don't use it very high loads.
 * So this is fine for now.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 25.01.17.
 */
public class RawDataProcessor {

    public final Map<AggregationKey, Object> rawStorage;

    public RawDataProcessor(boolean enable) {
        if (enable) {
            rawStorage = new ConcurrentHashMap<>();
        } else {
            rawStorage = Collections.emptyMap();
        }
    }

    //todo 2 millis is minimum allowed interval for data pushing.
    public void collect(String username, int dashId, int deviceId, char pinType, byte pin, long ts, String stringValue, double doubleValue) {
        final AggregationKey aggregationKey = new AggregationKey(username, dashId, deviceId, pinType, pin, ts);
        if (doubleValue == NumberUtil.NO_RESULT) {
            rawStorage.put(aggregationKey, stringValue);
        } else {
            rawStorage.put(aggregationKey, doubleValue);
        }
    }

}
