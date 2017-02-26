package cc.blynk.utils.properties;

import cc.blynk.server.notifications.mail.MailWrapper;
import cc.blynk.utils.ServerProperties;

import java.util.Map;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 03.01.17.
 */
public class MailProperties extends ServerProperties {

    public MailProperties(Map<String, String> cmdProperties) {
        super(cmdProperties, MailWrapper.MAIL_PROPERTIES_FILENAME);
    }
}
