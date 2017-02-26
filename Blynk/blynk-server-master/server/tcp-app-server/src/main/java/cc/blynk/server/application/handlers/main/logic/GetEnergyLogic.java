package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.GET_ENERGY;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 14.03.16.
 */
public class GetEnergyLogic {

    public static void messageReceived(ChannelHandlerContext ctx, User user, StringMessage message) {
        ctx.writeAndFlush(makeUTF8StringMessage(GET_ENERGY, message.id, "" + user.energy), ctx.voidPromise());
    }

}
