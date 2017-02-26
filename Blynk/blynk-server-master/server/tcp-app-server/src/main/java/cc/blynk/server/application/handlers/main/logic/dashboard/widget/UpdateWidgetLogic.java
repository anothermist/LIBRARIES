package cc.blynk.server.application.handlers.main.logic.dashboard.widget;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Timer;
import cc.blynk.server.core.model.widgets.notifications.Notification;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.model.widgets.ui.Tabs;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.protocol.exceptions.NotAllowedException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.workers.timer.TimerWorker;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.utils.BlynkByteBufUtil.ok;
import static cc.blynk.utils.StringUtils.split2;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.02.16.
 */
public class UpdateWidgetLogic {

    private static final Logger log = LogManager.getLogger(UpdateWidgetLogic.class);

    private final int MAX_WIDGET_SIZE;
    private final TimerWorker timerWorker;

    public UpdateWidgetLogic(int maxWidgetSize, TimerWorker timerWorker) {
        this.MAX_WIDGET_SIZE = maxWidgetSize;
        this.timerWorker = timerWorker;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        String[] split = split2(message.body);

        if (split.length < 2) {
            throw new IllegalCommandException("Wrong income message format.");
        }

        int dashId = ParseUtil.parseInt(split[0]) ;
        String widgetString = split[1];

        if (widgetString == null || widgetString.isEmpty()) {
            throw new IllegalCommandException("Income widget message is empty.");
        }

        if (widgetString.length() > MAX_WIDGET_SIZE) {
            throw new NotAllowedException("Widget is larger then limit.");
        }

        final User user = state.user;
        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);

        Widget newWidget = JsonParser.parseWidget(widgetString);

        if (newWidget.width < 1 || newWidget.height < 1) {
            throw new NotAllowedException("Widget has wrong dimensions.");
        }

        log.debug("Updating widget {}.", widgetString);

        int existingWidgetIndex = dash.getWidgetIndexById(newWidget.id);

        if (newWidget instanceof Tabs) {
            Tabs newTabs = (Tabs) newWidget;
            DeleteWidgetLogic.deleteTabs(timerWorker, user, state.userKey, dash, newTabs.tabs.length - 1);
        }

        Widget prevWidget = dash.widgets[existingWidgetIndex];

        if (prevWidget instanceof Notification && newWidget instanceof Notification) {
            Notification prevNotif = (Notification) prevWidget;
            Notification newNotif = (Notification) newWidget;
            newNotif.iOSTokens.putAll(prevNotif.iOSTokens);
            newNotif.androidTokens.putAll(prevNotif.androidTokens);
        }

        //non atomic update. changes could not be visible in saving thread.
        //ignoring for now
        dash.widgets[existingWidgetIndex] = newWidget;
        dash.cleanPinStorage(newWidget);
        dash.updatedAt = System.currentTimeMillis();
        user.lastModifiedTs = dash.updatedAt;

        if (prevWidget instanceof Timer) {
            timerWorker.delete(state.userKey, (Timer) prevWidget, dashId);
        } else if (prevWidget instanceof Eventor) {
            timerWorker.delete(state.userKey, (Eventor) prevWidget, dashId);
        }

        if (newWidget instanceof Timer) {
            timerWorker.add(state.userKey, (Timer) newWidget, dashId);
        } else if (newWidget instanceof Eventor) {
            timerWorker.add(state.userKey, (Eventor) newWidget, dashId);
        }

        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

}
