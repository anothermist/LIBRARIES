package cc.blynk.server.hardware.handlers.hardware.logic;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.widgets.notifications.SMS;
import cc.blynk.server.core.processors.NotificationBase;
import cc.blynk.server.core.protocol.exceptions.NotificationBodyInvalidException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.notifications.sms.SMSWrapper;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Response.NOTIFICATION_ERROR;
import static cc.blynk.server.core.protocol.enums.Response.NOTIFICATION_NOT_AUTHORIZED;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;

/**
 * Sends tweets from hardware.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class SmsLogic extends NotificationBase {

    private static final Logger log = LogManager.getLogger(SmsLogic.class);

    private static final int MAX_SMS_BODY_SIZE = 160;

    private final SMSWrapper smsWrapper;

    public SmsLogic(SMSWrapper smsWrapper, long notificationQuotaLimit) {
        super(notificationQuotaLimit);
        this.smsWrapper = smsWrapper;
    }

    public void messageReceived(ChannelHandlerContext ctx, HardwareStateHolder state, StringMessage message) {
        if (message.body == null || message.body.isEmpty() || message.body.length() > MAX_SMS_BODY_SIZE) {
            throw new NotificationBodyInvalidException();
        }

        DashBoard dash = state.user.profile.getDashByIdOrThrow(state.dashId);
        SMS smsWidget = dash.getWidgetByType(SMS.class);

        if (smsWidget == null || !dash.isActive ||
                smsWidget.to == null || smsWidget.to.isEmpty()) {
            log.debug("User has no access phone number provided.");
            ctx.writeAndFlush(makeResponse(message.id, NOTIFICATION_NOT_AUTHORIZED), ctx.voidPromise());
            return;
        }

        checkIfNotificationQuotaLimitIsNotReached();

        log.trace("Sending sms for user {}, with message : '{}'.", state.user.name, message.body);
        sms(ctx.channel(), state.user.name, smsWidget.to, message.body, message.id);
    }

    private void sms(Channel channel, String username, String to, String body, int msgId) {
        try {
            smsWrapper.send(to, body);
            channel.writeAndFlush(ok(msgId), channel.voidPromise());
        } catch (Exception e) {
            log.error("Error sending sms for user {}. Reason : {}",  username, e.getMessage());
            channel.writeAndFlush(makeResponse(msgId, NOTIFICATION_ERROR), channel.voidPromise());
        }
    }

}
