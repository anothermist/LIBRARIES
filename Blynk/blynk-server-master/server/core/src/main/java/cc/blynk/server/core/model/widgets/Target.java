package cc.blynk.server.core.model.widgets;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 05.01.17.
 */
public interface Target {

    int[] getDeviceIds();

    default boolean isTag() {
        return false;
    }

}
