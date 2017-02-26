package cc.blynk.server.notifications.mail;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 14.09.16.
 */
public interface MailClient {

    void sendText(String to, String subj, String body) throws Exception;

    void sendHtml(String to, String subj, String body) throws Exception;

}
