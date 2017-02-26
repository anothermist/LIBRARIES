package cc.blynk.server.core.model.widgets.others.eventor;

import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.BaseAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.BaseCondition;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.08.16.
 */
public class Rule {

    public Pin triggerPin;

    public TimerTime triggerTime;

    public BaseCondition condition;

    public BaseAction[] actions;

    public boolean isActive;

    public transient boolean isProcessed;

    public Rule() {
    }

    public Rule(Pin triggerPin, BaseCondition condition, BaseAction[] actions) {
        this.triggerPin = triggerPin;
        this.condition = condition;
        this.actions = actions;
    }

    private boolean notEmpty() {
        return triggerPin != null && condition != null && actions != null;
    }

    public boolean isReady(byte pin, PinType pinType) {
        return isActive && notEmpty() && triggerPin.isSame(pin, pinType);
    }

    public boolean isValidTimerRule() {
        return triggerTime != null && actions != null && actions.length > 0 && actions[0].isValid();
    }

    public boolean isValid(double value) {
        return condition.isValid(value);
    }

}
