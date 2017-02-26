package cc.blynk.server.notifications.mail;

import java.util.Properties;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 06.04.15.
 */
public class MailWrapper {

    public static final String MAIL_PROPERTIES_FILENAME = "mail.properties";

    private final MailClient client;

    public MailWrapper(Properties mailProperties) {
        String host = mailProperties.getProperty("mail.smtp.host");
        if (host != null && host.contains("sparkpostmail")) {
            client = new SparkPostMailClient(mailProperties);
        } else {
            client = new GMailClient(mailProperties);
        }
    }

    public void sendText(String to, String subj, String body) throws Exception {
        client.sendText(to, subj, body);
    }

    public void sendHtml(String to, String subj, String body) throws Exception {
        client.sendHtml(to, subj, body);
    }

}
