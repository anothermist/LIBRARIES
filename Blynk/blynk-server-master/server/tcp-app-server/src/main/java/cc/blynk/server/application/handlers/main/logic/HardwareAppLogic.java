package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.Holder;
import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.FrequencyWidget;
import cc.blynk.server.core.model.widgets.Target;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.ui.DeviceSelector;
import cc.blynk.server.core.processors.EventorProcessor;
import cc.blynk.server.core.processors.WebhookProcessor;
import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import cc.blynk.utils.StringUtils;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND_BODY;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.StringUtils.split2;
import static cc.blynk.utils.StringUtils.split2Device;
import static cc.blynk.utils.StringUtils.split3;

/**
 * Responsible for handling incoming hardware commands from applications and forwarding it to
 * appropriate hardware.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class HardwareAppLogic {

    private static final Logger log = LogManager.getLogger(HardwareAppLogic.class);

    private final SessionDao sessionDao;
    private final WebhookProcessor webhookProcessor;
    private final EventorProcessor eventorProcessor;

    public HardwareAppLogic(Holder holder, String username) {
        this.sessionDao = holder.sessionDao;
        this.webhookProcessor = new WebhookProcessor(holder.asyncHttpClient,
                holder.limits.WEBHOOK_PERIOD_LIMITATION,
                holder.limits.WEBHOOK_RESPONSE_SUZE_LIMIT_BYTES,
                holder.limits.WEBHOOK_FAILURE_LIMIT,
                holder.stats,
                username);
        this.eventorProcessor = holder.eventorProcessor;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        Session session = sessionDao.userSession.get(state.userKey);

        String[] split = split2(message.body);

        String[] dashIdAndTargetIdString = split2Device(split[0]);
        int dashId = ParseUtil.parseInt(dashIdAndTargetIdString[0]);
        //deviceId or tagId or device selector widget id
        int targetId = 0;

        //new logic for multi devices
        if (dashIdAndTargetIdString.length == 2) {
            targetId = ParseUtil.parseInt(dashIdAndTargetIdString[1]);
        }

        DashBoard dash = state.user.profile.getDashByIdOrThrow(dashId);

        //if no active dashboard - do nothing. this could happen only in case of app. bug
        if (!dash.isActive) {
            return;
        }

        //sending message only if widget assigned to device or tag has assigned devices
        Target target = dash.getTarget(targetId);
        if (target == null) {
            log.debug("No assigned target id for received command.");
            return;
        }

        final int[] deviceIds = target.getDeviceIds();

        if (deviceIds.length == 0) {
            log.debug("No devices assigned to target.");
            return;
        }

        final char operation = split[1].charAt(1);
        switch (operation) {
            case 'u' :
                String[] splitBody = split3(split[1]);
                final int widgetId = ParseUtil.parseInt(splitBody[1]);
                Widget deviceSelector = dash.getWidgetById(widgetId);
                if (deviceSelector instanceof DeviceSelector) {
                    final int selectedDeviceId = ParseUtil.parseInt(splitBody[2]);
                    ((DeviceSelector) deviceSelector).value = selectedDeviceId;
                    AppSyncLogic.sendSyncAndOk(ctx, dash, selectedDeviceId, message.id);
                }
                break;
            case 'w' :
                splitBody = split3(split[1]);
                final PinType pinType = PinType.getPinType(splitBody[0].charAt(0));
                final byte pin = ParseUtil.parseByte(splitBody[1]);
                final String value = splitBody[2];

                for (int deviceId : deviceIds) {
                    dash.update(deviceId, pin, pinType, value);
                }

                //additional state for tag widget itself
                if (target.isTag()) {
                    dash.update(targetId, pin, pinType, value);
                }

                //sending to shared dashes and master-master apps
                session.sendToSharedApps(ctx.channel(), dash.sharedToken, APP_SYNC, message.id, message.body);

                if (session.sendMessageToHardware(dashId, HARDWARE, message.id, split[1], deviceIds)) {
                    log.debug("No device in session.");
                    ctx.writeAndFlush(makeResponse(message.id, Response.DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
                }

                process(dash, targetId, session, pin, pinType, value);

                break;


            //todo fully remove this section???
            case 'r' :
                Widget widget = dash.findWidgetByPin(targetId, split[1].split(StringUtils.BODY_SEPARATOR_STRING));
                if (widget == null) {
                    log.debug("No widget for read command.");
                    ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND_BODY), ctx.voidPromise());
                    return;
                }
                //corner case for 3-d parties. sometimes users need to read pin state even from non-frequency widgets
                if (!(widget instanceof FrequencyWidget)) {
                    if (session.sendMessageToHardware(dashId, HARDWARE, message.id, split[1], targetId)) {
                        log.debug("No device in session.");
                        ctx.writeAndFlush(makeResponse(message.id, Response.DEVICE_NOT_IN_NETWORK), ctx.voidPromise());
                    }
                }
                break;
        }
    }

    private void process(DashBoard dash, int deviceId, Session session, byte pin, PinType pinType, String value) {
        try {
            eventorProcessor.process(session, dash, deviceId, pin, pinType, value);
            webhookProcessor.process(session, dash, deviceId, pin, pinType, value);
        } catch (Exception e) {
            log.error("Error processing eventor/webhook.", e);
        }
    }

}
