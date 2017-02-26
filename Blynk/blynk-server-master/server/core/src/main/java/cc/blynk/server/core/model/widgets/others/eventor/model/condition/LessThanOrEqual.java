package cc.blynk.server.core.model.widgets.others.eventor.model.condition;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.08.16.
 */
public class LessThanOrEqual extends BaseCondition {

    public double value;

    public LessThanOrEqual() {
    }

    public LessThanOrEqual(double value) {
        this.value = value;
    }

    @Override
    public boolean isValid(double in) {
        return in <= value;
    }

}
