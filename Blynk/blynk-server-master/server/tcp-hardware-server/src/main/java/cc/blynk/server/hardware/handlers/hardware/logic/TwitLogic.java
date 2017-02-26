package cc.blynk.server.hardware.handlers.hardware.logic;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.widgets.notifications.Twitter;
import cc.blynk.server.core.processors.NotificationBase;
import cc.blynk.server.core.protocol.exceptions.NotificationBodyInvalidException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.notifications.twitter.TwitterWrapper;
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
public class TwitLogic extends NotificationBase {

    private static final Logger log = LogManager.getLogger(TwitLogic.class);

    private final BlockingIOProcessor blockingIOProcessor;
    private final TwitterWrapper twitterWrapper;

    public TwitLogic(BlockingIOProcessor blockingIOProcessor, TwitterWrapper twitterWrapper, long notificationQuotaLimit) {
        super(notificationQuotaLimit);
        this.blockingIOProcessor = blockingIOProcessor;
        this.twitterWrapper = twitterWrapper;
    }

    public void messageReceived(ChannelHandlerContext ctx, HardwareStateHolder state, StringMessage message) {
        if (Twitter.isWrongBody(message.body)) {
            throw new NotificationBodyInvalidException();
        }

        DashBoard dash = state.user.profile.getDashByIdOrThrow(state.dashId);
        Twitter twitterWidget = dash.getWidgetByType(Twitter.class);

        if (twitterWidget == null || !dash.isActive ||
                twitterWidget.token == null || twitterWidget.token.isEmpty() ||
                twitterWidget.secret == null || twitterWidget.secret.isEmpty()) {
            log.debug("User has no access token provided for twit widget.");
            ctx.writeAndFlush(makeResponse(message.id, NOTIFICATION_NOT_AUTHORIZED), ctx.voidPromise());
            return;
        }

        checkIfNotificationQuotaLimitIsNotReached();

        log.trace("Sending Twit for user {}, with message : '{}'.", state.user.name, message.body);
        twit(ctx.channel(), state.user.name, twitterWidget.token, twitterWidget.secret, message.body, message.id);
    }

    private void twit(Channel channel, String username, String token, String secret, String body, int msgId) {
        blockingIOProcessor.execute(() -> {
            try {
                twitterWrapper.send(token, secret, body);
                channel.writeAndFlush(ok(msgId), channel.voidPromise());
            } catch (Exception e) {
                logError(e.getMessage(), username);
                channel.writeAndFlush(makeResponse(msgId, NOTIFICATION_ERROR), channel.voidPromise());
            }
        });
    }

    private static void logError(String errorMessage, String username) {
        if (errorMessage != null) {
            if (errorMessage.contains("Status is a duplicate")) {
                log.warn("Duplicate twit status for user {}.", username);
            } else if (errorMessage.contains("Authentication credentials")) {
                log.warn("Tweet authentication failure for {}.", username);
            } else if (errorMessage.contains("The request is understood, but it has been refused.")) {
                log.warn("User twit account is banned by twitter. {}.", username);
            } else {
                log.error("Error sending twit for user {}. Reason : {}", username, errorMessage);
            }
        }
    }

}
