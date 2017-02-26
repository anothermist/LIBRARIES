package cc.blynk.server.core.model.widgets.outputs;

import cc.blynk.server.core.model.widgets.OnePinReadingWidget;
import io.netty.channel.ChannelHandlerContext;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Graph extends OnePinReadingWidget {

    public boolean isBar;

    @Override
    public void sendHardSync(ChannelHandlerContext ctx, int msgId, int deviceId) {
    }

    @Override
    public String getModeType() {
        return "in";
    }

    @Override
    public int getPrice() {
        return 400;
    }
}
