package cc.blynk.server.handlers.common;

import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.handlers.DefaultExceptionHandler;
import cc.blynk.server.core.protocol.model.messages.appllication.LoginMessage;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;

import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
@ChannelHandler.Sharable
public class AlreadyLoggedHandler extends SimpleChannelInboundHandler<LoginMessage> implements DefaultExceptionHandler {

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, LoginMessage msg) throws Exception {
        if (ctx.channel().isWritable()) {
            ctx.writeAndFlush(makeResponse(msg.id, Response.USER_ALREADY_REGISTERED), ctx.voidPromise());
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        handleGeneralException(ctx, cause);
    }

}
