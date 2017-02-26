package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.model.widgets.AppSyncWidget;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.DEVICE_NOT_IN_NETWORK;
import static cc.blynk.utils.AppStateHolderUtil.getAppState;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.BlynkByteBufUtil.ok;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class ActivateDashboardLogic {

    public static final int PIN_MODE_MSG_ID = 1;

    private static final Logger log = LogManager.getLogger(ActivateDashboardLogic.class);

    private final SessionDao sessionDao;

    public ActivateDashboardLogic(SessionDao sessionDao) {
        this.sessionDao = sessionDao;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        final User user = state.user;
        String dashBoardIdString = message.body;

        int dashId = ParseUtil.parseInt(dashBoardIdString);

        log.debug("Activating dash {} for user {}", dashBoardIdString, user.name);
        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);
        dash.activate();
        user.lastModifiedTs = System.currentTimeMillis();

        Session session = sessionDao.userSession.get(state.userKey);

        //todo simplify?
        if (session.isHardwareConnected(dashId)) {
            for (Device device : dash.devices) {
                if (session.sendMessageToHardware(dashId, HARDWARE, PIN_MODE_MSG_ID, dash.buildPMMessage(device.id), device.id)) {
                    log.debug("No device in session.");
                    if (ctx.channel().isWritable()) {
                        ctx.writeAndFlush(makeResponse(PIN_MODE_MSG_ID, Response.DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
                    }
                }
            }

            ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
        } else {
            log.debug("No device in session.");
            ctx.writeAndFlush(makeResponse(message.id, DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
        }

        for (Channel appChannel : session.appChannels) {
            if (appChannel != ctx.channel() && getAppState(appChannel) != null) {
                appChannel.write(makeUTF8StringMessage(message.command, message.id, message.body));
            }

            //todo remove after migration to new "AppSync" method
            for (Widget widget : dash.widgets) {
                if (widget instanceof AppSyncWidget) {
                    ((AppSyncWidget) widget).sendAppSync(appChannel, dashId);
                }
            }

            appChannel.flush();
        }
    }

}
