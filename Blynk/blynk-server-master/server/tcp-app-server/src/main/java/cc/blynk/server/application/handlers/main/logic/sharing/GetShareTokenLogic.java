package cc.blynk.server.application.handlers.main.logic.sharing;

import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.exceptions.NotAllowedException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.GET_SHARE_TOKEN;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class GetShareTokenLogic {

    private static final int PRIVATE_TOKEN_PRICE = 1000;

    private final TokenManager tokenManager;

    public GetShareTokenLogic(TokenManager tokenManager) {
        this.tokenManager = tokenManager;
    }

    public void messageReceived(ChannelHandlerContext ctx, User user, StringMessage message) {
        String dashBoardIdString = message.body;

        int dashId;
        try {
            dashId = Integer.parseInt(dashBoardIdString);
        } catch (NumberFormatException ex) {
            throw new NotAllowedException("Dash board id not valid. Id : " + dashBoardIdString);
        }

        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);
        String token = dash.sharedToken;

        //if token not exists. generate new one
        if (token == null) {
            token = tokenManager.refreshSharedToken(user, dash);
            user.subtractEnergy(PRIVATE_TOKEN_PRICE);
        }

        ctx.writeAndFlush(makeUTF8StringMessage(GET_SHARE_TOKEN, message.id, token), ctx.voidPromise());
    }
}
