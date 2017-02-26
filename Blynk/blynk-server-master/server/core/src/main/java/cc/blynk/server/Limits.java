package cc.blynk.server;

import cc.blynk.utils.ServerProperties;

/**
 * This is helper class for holding all user limits.
 * It is created for dependency injection mostly.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.01.17.
 */
public class Limits {

    //user limits
    public final int DEVICE_LIMIT;
    public final int TAGS_LIMIT;
    public final int DASHBOARDS_LIMIT;
    public final int WIDGET_SIZE_LIMIT_BYTES;
    public final int PROFILE_SIZE_LIMIT_BYTES;
    public final long TRAFFIC_LIMIT;

    //hardware side limits
    public final long NOTIFICATION_PERIOD_LIMIT_SEC;
    public final int USER_QUOTA_LIMIT;
    public final int USER_QUOTA_LIMIT_WARN_PERIOD_MILLIS;
    public final long WEBHOOK_PERIOD_LIMITATION;
    public final int WEBHOOK_RESPONSE_SUZE_LIMIT_BYTES;
    public final int WEBHOOK_FAILURE_LIMIT;

    public Limits(ServerProperties props) {
        this.DEVICE_LIMIT = props.getIntProperty("user.devices.limit", 25);
        this.TAGS_LIMIT = props.getIntProperty("user.tags.limit", 100);
        this.DASHBOARDS_LIMIT = props.getIntProperty("user.dashboard.max.limit", 100);
        this.WIDGET_SIZE_LIMIT_BYTES = props.getIntProperty("user.widget.max.size.limit", 10) * 1024;
        this.PROFILE_SIZE_LIMIT_BYTES = props.getIntProperty("user.profile.max.size", 64) * 1024;
        this.TRAFFIC_LIMIT = props.getIntProperty("user.traffic.limit", 256) * 1024;

        this.NOTIFICATION_PERIOD_LIMIT_SEC = props.getLongProperty("notifications.frequency.user.quota.limit", 15L) * 1000L;
        this.USER_QUOTA_LIMIT = props.getIntProperty("user.message.quota.limit", 100);
        this.USER_QUOTA_LIMIT_WARN_PERIOD_MILLIS = props.getIntProperty("user.message.quota.limit.exceeded.warning.period", 60000);
        this.WEBHOOK_PERIOD_LIMITATION = isUnlimited(props.getLongProperty("webhooks.frequency.user.quota.limit", 1000), -1L);
        this.WEBHOOK_RESPONSE_SUZE_LIMIT_BYTES = props.getIntProperty("webhooks.response.size.limit", 64) * 1024;
        this.WEBHOOK_FAILURE_LIMIT = isUnlimited(props.getIntProperty("webhooks.failure.count.limit", 10), Integer.MAX_VALUE);
    }

    private static int isUnlimited(int val, int max) {
        if (val == 0) {
            return max;
        }
        return val;
    }

    private static long isUnlimited(long val, long max) {
        if (val == 0) {
            return max;
        }
        return val;
    }

}
