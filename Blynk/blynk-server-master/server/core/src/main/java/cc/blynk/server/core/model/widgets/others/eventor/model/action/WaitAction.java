package cc.blynk.server.core.model.widgets.others.eventor.model.action;

import io.netty.channel.ChannelHandlerContext;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.08.16.
 */
public class WaitAction extends BaseAction {

    public int seconds;

    public long lastActivation;

    public BaseAction delayedAction;

    public WaitAction() {
    }

    public WaitAction(int seconds, BaseAction delayedAction) {
        this.seconds = seconds;
        this.delayedAction = delayedAction;
    }

    public void execute(ChannelHandlerContext ctx, String triggerValue) {
        /*
        long now = System.currentTimeMillis();

        if (delayedAction != null && lastActivation + seconds * 1000 < now) {
            lastActivation = now;

            ctx.executor().schedule(() -> { delayedAction.execute(ctx, triggerValue);}, seconds, TimeUnit.SECONDS);
        }
        */
    }

    @Override
    public boolean isValid() {
        return true;
    }
}
