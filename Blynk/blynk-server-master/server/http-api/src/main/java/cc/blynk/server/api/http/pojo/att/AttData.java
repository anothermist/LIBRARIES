package cc.blynk.server.api.http.pojo.att;

import java.util.Date;
import java.util.Map;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 29.07.16.
 */
public class AttData {

    public String trigger;

    public Date timestamp;

    public String event;

    public AttDevice device;

    public Map<String, AttValue> values;

}
