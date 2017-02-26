package cc.blynk.server.workers.timer;

import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.widgets.others.eventor.TimerTime;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 27.12.16.
 */
public class TimerKey {

    public final UserKey userKey;

    public final int dashId;

    public final int deviceId;

    public final long widgetId;

    public final int additionalId;

    public final TimerTime time;

    public TimerKey(UserKey userKey, int dashId, int deviceId, long widgetId, int additionalId, TimerTime time) {
        this.userKey = userKey;
        this.dashId = dashId;
        this.deviceId = deviceId;
        this.widgetId = widgetId;
        this.additionalId = additionalId;
        this.time = time;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof TimerKey)) return false;

        TimerKey timerKey = (TimerKey) o;

        if (dashId != timerKey.dashId) return false;
        if (deviceId != timerKey.deviceId) return false;
        if (widgetId != timerKey.widgetId) return false;
        if (additionalId != timerKey.additionalId) return false;
        if (userKey != null ? !userKey.equals(timerKey.userKey) : timerKey.userKey != null) return false;
        return !(time != null ? !time.equals(timerKey.time) : timerKey.time != null);
    }

    @Override
    public int hashCode() {
        int result = userKey != null ? userKey.hashCode() : 0;
        result = 31 * result + dashId;
        result = 31 * result + deviceId;
        result = 31 * result + (int) (widgetId ^ (widgetId >>> 32));
        result = 31 * result + additionalId;
        result = 31 * result + (time != null ? time.hashCode() : 0);
        return result;
    }
}
