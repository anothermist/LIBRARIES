package cc.blynk.server.core.model.widgets.others.webhook;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 05.09.16.
 */
public class Header {

    public String name;

    public String value;

    public Header() {
    }

    public Header(String name, String value) {
        this.name = name;
        this.value = value;
    }

    public boolean isValid() {
        return name != null && value != null;
    }
}
