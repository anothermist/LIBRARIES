package cc.blynk.server.core.model.widgets.others.eventor.model.condition;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.08.16.
 */
public class Between extends BaseCondition {

    public double left;

    public double right;

    public Between() {
    }

    public Between(double left, double right) {
        this.left = left;
        this.right = right;
    }

    @Override
    public boolean isValid(double in) {
        return (left < in) && (in < right);
    }

}
