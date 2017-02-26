package cc.blynk.server.application.handlers.main.logic.sharing;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.application.handlers.sharing.auth.AppShareStateHolder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.exceptions.NotAllowedException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.REFRESH_SHARE_TOKEN;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.utils.AppStateHolderUtil.getShareState;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class RefreshShareTokenLogic {

    private final TokenManager tokenManager;
    private final SessionDao sessionDao;

    public RefreshShareTokenLogic(TokenManager tokenManager, SessionDao sessionDao) {
        this.tokenManager = tokenManager;
        this.sessionDao = sessionDao;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        String dashBoardIdString = message.body;

        int dashId;
        try {
            dashId = Integer.parseInt(dashBoardIdString);
        } catch (NumberFormatException ex) {
            throw new NotAllowedException("Dash board id not valid. Id : " + dashBoardIdString);
        }

        final User user = state.user;
        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);

        String token = tokenManager.refreshSharedToken(user, dash);

        Session session = sessionDao.userSession.get(state.userKey);
        for (Channel appChannel : session.appChannels) {
            AppShareStateHolder localState = getShareState(appChannel);
            if (localState != null && localState.dashId == dashId) {
                ChannelFuture cf = appChannel.writeAndFlush(makeResponse(message.id, NOT_ALLOWED));
                cf.addListener(channelFuture -> appChannel.close());
            }
        }

        ctx.writeAndFlush(makeUTF8StringMessage(REFRESH_SHARE_TOKEN, message.id, token), ctx.voidPromise());
    }
}
