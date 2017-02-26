package cc.blynk.utils;

import cc.blynk.server.application.handlers.main.AppHandler;
import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.application.handlers.sharing.AppShareHandler;
import cc.blynk.server.application.handlers.sharing.auth.AppShareStateHolder;
import io.netty.channel.Channel;
import io.netty.channel.ChannelPipeline;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 05.01.16.
 */
public class AppStateHolderUtil {

    public static AppStateHolder getAppState(Channel channel) {
        return getAppState(channel.pipeline());
    }

    private static AppStateHolder getAppState(ChannelPipeline pipeline) {
        AppHandler handler = pipeline.get(AppHandler.class);
        if (handler == null) {
            return getShareState(pipeline);
        }
        return handler.state;
    }

    public static AppShareStateHolder getShareState(Channel channel) {
        return getShareState(channel.pipeline());
    }

    private static AppShareStateHolder getShareState(ChannelPipeline pipeline) {
        AppShareHandler handler = pipeline.get(AppShareHandler.class);
        return handler == null ? null : handler.state;
    }


}
