package cc.blynk.server.application.handlers.main.logic.dashboard.widget;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Timer;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.model.widgets.ui.Tabs;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.workers.timer.TimerWorker;
import cc.blynk.utils.ArrayUtil;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.ArrayList;

import static cc.blynk.utils.BlynkByteBufUtil.ok;
import static cc.blynk.utils.StringUtils.split2;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.02.16.
 */
public class DeleteWidgetLogic {

    private static final Logger log = LogManager.getLogger(DeleteWidgetLogic.class);

    private final TimerWorker timerWorker;

    public DeleteWidgetLogic(TimerWorker timerWorker) {
        this.timerWorker = timerWorker;
    }

    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        String[] split = split2(message.body);

        if (split.length < 2) {
            throw new IllegalCommandException("Wrong income message format.");
        }

        int dashId = ParseUtil.parseInt(split[0]) ;
        long widgetId = ParseUtil.parseLong(split[1]);

        final User user = state.user;
        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);

        log.debug("Removing widget with id {}.", widgetId);

        int existingWidgetIndex = dash.getWidgetIndexById(widgetId);
        Widget widgetToDelete = dash.widgets[existingWidgetIndex];
        if (widgetToDelete instanceof Tabs) {
            deleteTabs(timerWorker, user, state.userKey, dash, 0);
        }

        existingWidgetIndex = dash.getWidgetIndexById(widgetId);
        deleteWidget(user, dash, existingWidgetIndex);

        if (widgetToDelete instanceof Timer) {
            timerWorker.delete(state.userKey, (Timer) widgetToDelete, dashId);
        } else if (widgetToDelete instanceof Eventor) {
            timerWorker.delete(state.userKey, (Eventor) widgetToDelete, dashId);
        }

        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

    /**
     * Removes all widgets with tabId greater than lastTabIndex
     */
    public static void deleteTabs(TimerWorker timerWorker, User user, UserKey userKey, DashBoard dash, int lastTabIndex) {
        ArrayList<Widget> zeroTabWidgets = new ArrayList<>();
        int removedWidgetPrice = 0;
        for (Widget widgetToDelete : dash.widgets) {
            if (widgetToDelete.tabId > lastTabIndex) {
                removedWidgetPrice += widgetToDelete.getPrice();
                if (widgetToDelete instanceof Timer) {
                    timerWorker.delete(userKey, (Timer) widgetToDelete, dash.id);
                } else if (widgetToDelete instanceof Eventor) {
                    timerWorker.delete(userKey, (Eventor) widgetToDelete, dash.id);
                }
            } else {
                zeroTabWidgets.add(widgetToDelete);
            }
        }

        user.recycleEnergy(removedWidgetPrice);
        dash.widgets = zeroTabWidgets.toArray(new Widget[zeroTabWidgets.size()]);
        dash.updatedAt = System.currentTimeMillis();
    }

    private static void deleteWidget(User user, DashBoard dash, int existingWidgetIndex) {
        user.recycleEnergy(dash.widgets[existingWidgetIndex].getPrice());
        dash.widgets = ArrayUtil.remove(dash.widgets, existingWidgetIndex, Widget.class);
        dash.updatedAt = System.currentTimeMillis();
    }

}
