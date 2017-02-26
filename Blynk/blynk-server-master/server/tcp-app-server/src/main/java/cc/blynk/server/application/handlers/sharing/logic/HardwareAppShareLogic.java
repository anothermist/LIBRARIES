package cc.blynk.server.application.handlers.sharing.logic;

import cc.blynk.server.application.handlers.sharing.auth.AppShareStateHolder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.widgets.FrequencyWidget;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandBodyException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import cc.blynk.utils.StringUtils;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.server.core.protocol.enums.Response.NO_ACTIVE_DASHBOARD;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.StringUtils.split2;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class HardwareAppShareLogic {

    private static final Logger log = LogManager.getLogger(HardwareAppShareLogic.class);

    private final SessionDao sessionDao;

    public HardwareAppShareLogic(SessionDao sessionDao) {
        this.sessionDao = sessionDao;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppShareStateHolder state, StringMessage message) {
        Session session = sessionDao.userSession.get(state.userKey);

        String[] split = split2(message.body);

        String[] dashIdAndDeviceIdString = split[0].split("-");
        int dashId = ParseUtil.parseInt(dashIdAndDeviceIdString[0]);
        int deviceId = 0;

        //new logic for multi devices
        if (dashIdAndDeviceIdString.length == 2) {
            deviceId = ParseUtil.parseInt(dashIdAndDeviceIdString[1]);
        }

        DashBoard dashBoard = state.user.profile.getDashByIdOrThrow(dashId);

        if (!dashBoard.isActive) {
            log.debug("No active dashboard.");
            ctx.writeAndFlush(makeResponse(message.id, NO_ACTIVE_DASHBOARD), ctx.voidPromise());
            return;
        }

        if (!dashBoard.isShared) {
            log.debug("Dashboard is not shared. User : {}, {}", state.user.name, ctx.channel().remoteAddress());
            ctx.writeAndFlush(makeResponse(message.id, NOT_ALLOWED), ctx.voidPromise());
            return;
        }

        char operation = split[1].charAt(1);
        DashBoard dash = state.user.profile.getDashByIdOrThrow(dashId);

        switch (operation) {
            case 'w':
                dash.update(deviceId, split[1]);

                final String sharedToken = state.token;
                if (sharedToken != null) {
                    for (Channel appChannel : session.appChannels) {
                        if (appChannel != ctx.channel() && appChannel.isWritable() && Session.needSync(appChannel, sharedToken)) {
                            appChannel.writeAndFlush(makeUTF8StringMessage(APP_SYNC, message.id, message.body), appChannel.voidPromise());
                        }
                    }
                }

                if (session.sendMessageToHardware(dashId, HARDWARE, message.id, split[1], deviceId)) {
                    log.debug("No device in session.");
                    ctx.writeAndFlush(makeResponse(message.id, Response.DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
                }
                break;


            //todo fully remove this section???
            case 'r':
                Widget widget = dash.findWidgetByPin(deviceId, split[1].split(StringUtils.BODY_SEPARATOR_STRING));
                if (widget == null) {
                    throw new IllegalCommandBodyException("No frequency widget for read command.");
                }

                if (!(widget instanceof FrequencyWidget)) {
                    //corner case for 3-d parties. sometimes users need to read pin state even from non-frequency widgets
                    if (session.sendMessageToHardware(dashId, HARDWARE, message.id, split[1], deviceId)) {
                        log.debug("No device in session.");
                        ctx.writeAndFlush(makeResponse(message.id, Response.DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
                    }
                }
                break;
        }


    }

}
