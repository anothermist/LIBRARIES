package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.utils.AppStateHolderUtil.getAppState;
import static cc.blynk.utils.BlynkByteBufUtil.ok;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class DeActivateDashboardLogic {

    private static final Logger log = LogManager.getLogger(ActivateDashboardLogic.class);

    private final SessionDao sessionDao;

    public DeActivateDashboardLogic(SessionDao sessionDao) {
        this.sessionDao = sessionDao;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        final User user = state.user;
        if (message.length > 0) {
            log.debug("DeActivating dash {} for user {}", message.body, user.name);
            int dashId = ParseUtil.parseInt(message.body);
            DashBoard dashBoard = user.profile.getDashByIdOrThrow(dashId);
            dashBoard.deactivate();
        } else {
            for (DashBoard dashBoard : user.profile.dashBoards) {
                dashBoard.deactivate();
            }
        }
        user.lastModifiedTs = System.currentTimeMillis();

        Session session = sessionDao.userSession.get(state.userKey);
        for (Channel appChannel : session.appChannels) {
            if (appChannel != ctx.channel() && getAppState(appChannel) != null) {
                appChannel.writeAndFlush(message, appChannel.voidPromise());
            }
        }

        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

}
