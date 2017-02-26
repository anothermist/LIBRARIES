package cc.blynk.server.core.model.widgets.others.eventor.model.condition;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.08.16.
 */
public class Equal extends BaseCondition {

    public double value;

    public Equal() {
    }

    public Equal(double value) {
        this.value = value;
    }

    @Override
    public boolean isValid(double in) {
        return Double.compare(in, value) == 0;
    }

}
