package cc.blynk.server.core.model.widgets.others.eventor.model.condition;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.08.16.
 */
@JsonTypeInfo(
        use = JsonTypeInfo.Id.NAME,
        include = JsonTypeInfo.As.PROPERTY,
        property = "type")
@JsonSubTypes({
        @JsonSubTypes.Type(value = GreaterThan.class, name = "GT"),
        @JsonSubTypes.Type(value = GreaterThanOrEqual.class, name = "GTE"),
        @JsonSubTypes.Type(value = LessThan.class, name = "LT"),
        @JsonSubTypes.Type(value = LessThanOrEqual.class, name = "LTE"),
        @JsonSubTypes.Type(value = Equal.class, name = "EQ"),
        @JsonSubTypes.Type(value = NotEqual.class, name = "NEQ"),
        @JsonSubTypes.Type(value = Between.class, name = "BETWEEN"),
        @JsonSubTypes.Type(value = NotBetween.class, name = "NOT_BETWEEN")

})
public abstract class BaseCondition {

    public abstract boolean isValid(double in);

}
