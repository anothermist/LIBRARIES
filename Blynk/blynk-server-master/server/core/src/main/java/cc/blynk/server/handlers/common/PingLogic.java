package cc.blynk.server.handlers.common;

import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.utils.BlynkByteBufUtil.ok;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class PingLogic {

    public static void messageReceived(ChannelHandlerContext ctx, int messageId) {
        if (ctx.channel().isWritable()) {
            ctx.writeAndFlush(ok(messageId), ctx.voidPromise());
        }
    }

}
