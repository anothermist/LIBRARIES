package cc.blynk.server.core.model.device;

import cc.blynk.server.core.model.widgets.Target;
import cc.blynk.utils.ArrayUtil;
import cc.blynk.utils.JsonParser;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 16.11.16.
 */
public class Tag implements Target {

    public static final int START_TAG_ID = 100_000;

    public int id;

    public volatile String name;

    public volatile int[] deviceIds = ArrayUtil.EMPTY_INTS;

    public boolean isNotValid() {
        return name == null || name.isEmpty() || name.length() > 40 || id < START_TAG_ID || deviceIds.length > 100;
    }

    public Tag() {
    }

    public Tag(int id, String name) {
        this.id = id;
        this.name = name;
    }

    @Override
    public int[] getDeviceIds() {
        return deviceIds;
    }

    @Override
    public boolean isTag() {
        return true;
    }

    public void update(Tag tag) {
        this.name = tag.name;
        this.deviceIds = tag.deviceIds;
    }

    @Override
    public String toString() {
        return JsonParser.toJson(this);
    }
}
