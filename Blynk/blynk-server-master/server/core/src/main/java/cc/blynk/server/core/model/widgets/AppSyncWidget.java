package cc.blynk.server.core.model.widgets;

import io.netty.channel.Channel;

/**
 * Interface defines if pin value of widget should be send to application on activate.
 * Usually all widgets that have pins should implement this interface otherwise widget
 * state may be outdated on application side.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.08.16.
 */
public interface AppSyncWidget {

    int SYNC_DEFAULT_MESSAGE_ID = 1111;
    int ANY_TARGET = -1;

    void sendAppSync(Channel appChannel, int dashId, int targetId);

    default void sendAppSync(Channel appChannel, int dashId) {
        sendAppSync(appChannel, dashId, ANY_TARGET);
    }

}
