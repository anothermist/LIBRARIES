package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.PinStorageKey;
import cc.blynk.server.core.model.widgets.AppSyncWidget;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;

import java.util.Map;

import static cc.blynk.server.core.model.widgets.AppSyncWidget.ANY_TARGET;
import static cc.blynk.server.core.model.widgets.AppSyncWidget.SYNC_DEFAULT_MESSAGE_ID;
import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;
import static cc.blynk.utils.BlynkByteBufUtil.ok;
import static cc.blynk.utils.StringUtils.prependDashIdAndDeviceId;
import static cc.blynk.utils.StringUtils.split2Device;

/**
 * Request state sync info for widgets.
 * Supports sync for all widgets and sync for specific target
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class AppSyncLogic {

    public static void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        String[] dashIdAndTargetIdString = split2Device(message.body);
        int dashId = ParseUtil.parseInt(dashIdAndTargetIdString[0]);
        int targetId = AppSyncWidget.ANY_TARGET;

        if (dashIdAndTargetIdString.length == 2) {
            targetId = ParseUtil.parseInt(dashIdAndTargetIdString[1]);
        }

        DashBoard dash = state.user.profile.getDashByIdOrThrow(dashId);

        sendSyncAndOk(ctx, dash, targetId, message.id);
    }

    public static void sendSyncAndOk(ChannelHandlerContext ctx, DashBoard dash, int targetId, int msgId) {
        ctx.write(ok(msgId), ctx.voidPromise());

        final Channel appChannel = ctx.channel();
        for (Widget widget : dash.widgets) {
            if (widget instanceof AppSyncWidget) {
                ((AppSyncWidget) widget).sendAppSync(appChannel, dash.id, targetId);
            }
        }

        for (Map.Entry<PinStorageKey, String> entry : dash.pinsStorage.entrySet()) {
            PinStorageKey key = entry.getKey();
            if (targetId == ANY_TARGET || targetId == key.deviceId) {
                String body = prependDashIdAndDeviceId(dash.id, key.deviceId, Pin.makeHardwareBody(key.pinType, key.pin, entry.getValue()));
                ctx.write(makeUTF8StringMessage(APP_SYNC, SYNC_DEFAULT_MESSAGE_ID, body), ctx.voidPromise());
            }
        }

        ctx.flush();
    }

}
