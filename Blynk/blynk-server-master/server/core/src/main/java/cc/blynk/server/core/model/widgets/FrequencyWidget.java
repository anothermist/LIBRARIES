package cc.blynk.server.core.model.widgets;

import io.netty.channel.Channel;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 02.12.15.
 */
public interface FrequencyWidget {

    int READING_MSG_ID = 7778;

    void writeReadingCommand(Channel channel);

    int getDeviceId();

    boolean isTicked(long now);

}
