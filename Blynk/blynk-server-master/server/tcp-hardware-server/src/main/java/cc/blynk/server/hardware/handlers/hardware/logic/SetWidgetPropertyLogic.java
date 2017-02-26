package cc.blynk.server.hardware.handlers.hardware.logic;

import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.SET_WIDGET_PROPERTY;
import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND_BODY;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;
import static cc.blynk.utils.StringUtils.split3;

/**
 * Handler that allows to change widget properties from hardware side.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class SetWidgetPropertyLogic {

    private static final Logger log = LogManager.getLogger(SetWidgetPropertyLogic.class);

    private final SessionDao sessionDao;

    public SetWidgetPropertyLogic(SessionDao sessionDao) {
        this.sessionDao = sessionDao;
    }

    public void messageReceived(ChannelHandlerContext ctx, HardwareStateHolder state, StringMessage message) {
        Session session = sessionDao.userSession.get(state.userKey);

        String[] bodyParts = split3(message.body);

        if (bodyParts.length != 3) {
            log.debug("SetWidgetProperty command body has wrong format. {}", message.body);
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND_BODY), ctx.voidPromise());
            return;
        }

        final int deviceId = state.deviceId;

        byte pin = ParseUtil.parseByte(bodyParts[0]);
        String property = bodyParts[1];
        String propertyValue = bodyParts[2];

        if (property.length() == 0 || propertyValue.length() == 0) {
            log.debug("SetWidgetProperty command body has wrong format. {}", message.body);
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND_BODY), ctx.voidPromise());
            return;
        }

        DashBoard dash = state.user.profile.getDashByIdOrThrow(state.dashId);

        if (!dash.isActive) {
            return;
        }

        //for now supporting only virtual pins
        Widget widget = dash.findWidgetByPin(deviceId, pin, PinType.VIRTUAL);

        if (widget == null) {
            log.debug("No widget for SetWidgetProperty command. {}", message.body);
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND_BODY), ctx.voidPromise());
            return;
        }

        try {
            widget.setProperty(property, propertyValue);
            dash.updatedAt = System.currentTimeMillis();
        } catch (Exception e) {
            log.debug("Error setting widget property. Reason : {}", e.getMessage());
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND_BODY), ctx.voidPromise());
            return;
        }

        session.sendToApps(SET_WIDGET_PROPERTY, message.id, dash.id, deviceId, message.body);
        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

}
