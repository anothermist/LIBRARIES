package cc.blynk.server.application.handlers.main.logic.sharing;

import cc.blynk.server.core.dao.SharedTokenValue;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.exceptions.InvalidTokenException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.JsonParser;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class GetSharedDashLogic {

    private final TokenManager tokenManager;

    public GetSharedDashLogic(TokenManager tokenManager) {
        this.tokenManager = tokenManager;
    }

    public void messageReceived(ChannelHandlerContext ctx, StringMessage message) {
        String token = message.body;

        SharedTokenValue tokenValue = tokenManager.getUserBySharedToken(token);

        if (tokenValue == null) {
            throw new InvalidTokenException("Illegal sharing token. No user with those shared token.", message.id);
        }

        User userThatShared = tokenValue.user;

        DashBoard dashBoard = userThatShared.profile.getDashByIdOrThrow(tokenValue.dashId);

        if (ctx.channel().isWritable()) {
            ctx.writeAndFlush(produce(message.id, message.command, JsonParser.toJsonSharedDashboard(dashBoard)), ctx.voidPromise());
        }
    }

}