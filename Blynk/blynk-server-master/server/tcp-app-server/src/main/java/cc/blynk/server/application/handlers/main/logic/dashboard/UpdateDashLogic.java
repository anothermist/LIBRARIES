package cc.blynk.server.application.handlers.main.logic.dashboard;

import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Timer;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.protocol.exceptions.NotAllowedException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.workers.timer.TimerWorker;
import cc.blynk.utils.JsonParser;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.utils.BlynkByteBufUtil.ok;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class UpdateDashLogic {

    private static final Logger log = LogManager.getLogger(UpdateDashLogic.class);

    private final int DASH_MAX_SIZE;
    private final TimerWorker timerWorker;

    public UpdateDashLogic(TimerWorker timerWorker, int maxDashSize) {
        this.timerWorker = timerWorker;
        this.DASH_MAX_SIZE = maxDashSize;
    }

    //todo should accept only dash info and ignore widgets. should be fixed after migration
    public void messageReceived(ChannelHandlerContext ctx, AppStateHolder state, StringMessage message) {
        String dashString = message.body;

        if (dashString == null || dashString.isEmpty()) {
            throw new IllegalCommandException("Income create dash message is empty.");
        }

        if (dashString.length() > DASH_MAX_SIZE) {
            throw new NotAllowedException("User dashboard is larger then limit.");
        }

        log.debug("Trying to parse user dash : {}", dashString);
        DashBoard updatedDash = JsonParser.parseDashboard(dashString);

        if (updatedDash == null) {
            throw new IllegalCommandException("Project parsing error.");
        }

        log.debug("Saving dashboard.");

        final User user = state.user;

        DashBoard existingDash = user.profile.getDashByIdOrThrow(updatedDash.id);

        for (Widget widget : existingDash.widgets) {
            if (widget instanceof Timer) {
                timerWorker.delete(state.userKey, (Timer) widget, existingDash.id);
            } else if (widget instanceof Eventor) {
                timerWorker.delete(state.userKey, (Eventor) widget, existingDash.id);
            }
        }

        for (Widget widget : updatedDash.widgets) {
            if (widget instanceof Timer) {
                timerWorker.add(state.userKey, (Timer) widget, updatedDash.id);
            } else if (widget instanceof Eventor) {
                timerWorker.add(state.userKey, (Eventor) widget, updatedDash.id);
            }
        }

        existingDash.updateFields(updatedDash);
        user.lastModifiedTs = existingDash.updatedAt;

        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

}
