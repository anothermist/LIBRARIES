package cc.blynk.server.core.model.widgets.others.eventor;

import cc.blynk.server.core.model.widgets.others.rtc.StringToZoneId;
import cc.blynk.server.core.model.widgets.others.rtc.ZoneIdToString;
import cc.blynk.utils.DateTimeUtils;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;

import java.time.ZoneId;
import java.util.Arrays;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 30.01.17.
 */
public class TimerTime {

    public static final int[] ALL_DAYS = new int[] {1, 2, 3, 4, 5, 6, 7};

    public int id;

    public int[] days;

    public int time = -1;

    @JsonSerialize(using = ZoneIdToString.class)
    @JsonDeserialize(using = StringToZoneId.class, as = ZoneId.class)
    public ZoneId tzName;

    public TimerTime() {
    }

    //this is special constructor for Timer back compatibility.
    //todo remove in future versions.
    public TimerTime(int time) {
        this.time = time;
        this.days = ALL_DAYS;
        this.tzName = DateTimeUtils.UTC;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof TimerTime)) return false;

        TimerTime timerTime = (TimerTime) o;

        if (id != timerTime.id) return false;
        if (time != timerTime.time) return false;
        if (!Arrays.equals(days, timerTime.days)) return false;
        return !(tzName != null ? !tzName.equals(timerTime.tzName) : timerTime.tzName != null);

    }

    @Override
    public int hashCode() {
        int result = id;
        result = 31 * result + (days != null ? Arrays.hashCode(days) : 0);
        result = 31 * result + time;
        result = 31 * result + (tzName != null ? tzName.hashCode() : 0);
        return result;
    }
}
