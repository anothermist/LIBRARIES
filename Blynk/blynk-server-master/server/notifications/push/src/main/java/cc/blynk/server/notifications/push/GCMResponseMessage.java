package cc.blynk.server.notifications.push;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 26.06.15.
 */
public class GCMResponseMessage {

    public int success;

    public int failure;

    public long multicast_id;

    public GCMResult[] results;

}
