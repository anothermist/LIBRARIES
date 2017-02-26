package cc.blynk.server.application.handlers.main.logic.dashboard.widget;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Timer;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.model.widgets.others.webhook.WebHook;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.protocol.exceptions.NotAllowedException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.workers.timer.TimerWorker;
import cc.blynk.utils.ArrayUtil;
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
public class CreateWidgetLogic {

    private static final Logger log = LogManager.getLogger(CreateWidgetLogic.class);

    private final int MAX_WIDGET_SIZE;
    private final TimerWorker timerWorker;

    public CreateWidgetLogic(int maxWidgetSize, TimerWorker timerWorker) {
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

        log.debug("Creating new widget {}.", widgetString);

        for (Widget widget : dash.widgets) {
            if (widget.id == newWidget.id) {
                throw new NotAllowedException("Widget with same id already exists.");
            }
        }

        if (newWidget instanceof WebHook) {
            String url = ((WebHook) newWidget).url;
            if (url != null && !url.toLowerCase().startsWith("http")) {
                throw new IllegalCommandException("Url for webhook doesn't have scheme.");
            }
        }

        user.subtractEnergy(newWidget.getPrice());
        dash.widgets = ArrayUtil.add(dash.widgets, newWidget, Widget.class);
        dash.cleanPinStorage(newWidget);
        dash.updatedAt = System.currentTimeMillis();

        user.lastModifiedTs = dash.updatedAt;

        if (newWidget instanceof Timer) {
            timerWorker.add(state.userKey, (Timer) newWidget, dashId);
        } else if (newWidget instanceof Eventor) {
            timerWorker.add(state.userKey, (Eventor) newWidget, dashId);
        }

        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

}
