package cc.blynk.server.core.model.widgets.others.eventor.model.action;

import cc.blynk.server.core.model.widgets.others.eventor.model.action.notification.MailAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.notification.NotifyAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.notification.TwitAction;
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
        @JsonSubTypes.Type(value = SetPinAction.class, name = "SETPIN"),
        @JsonSubTypes.Type(value = WaitAction.class, name = "WAIT"),
        @JsonSubTypes.Type(value = NotifyAction.class, name = "NOTIFY"),
        @JsonSubTypes.Type(value = MailAction.class, name = "MAIL"),
        @JsonSubTypes.Type(value = TwitAction.class, name = "TWIT"),
})
public abstract class BaseAction {

    public abstract boolean isValid();

}
