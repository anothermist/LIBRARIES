package cc.blynk.server.notifications.mail;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.mail.Message;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
import java.util.Properties;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 14.09.16.
 */
public class GMailClient implements MailClient {

    private static final Logger log = LogManager.getLogger(MailWrapper.class);

    private final Session session;
    private final InternetAddress from;

    public GMailClient(Properties mailProperties) {
        final String username = mailProperties.getProperty("mail.smtp.username");
        final String password = mailProperties.getProperty("mail.smtp.password");

        log.info("Initializing gmail smtp mail transport. Username : {}. SMTP host : {}:{}",
                username, mailProperties.getProperty("mail.smtp.host"), mailProperties.getProperty("mail.smtp.port"));

        this.session = Session.getInstance(mailProperties, new javax.mail.Authenticator() {
            protected PasswordAuthentication getPasswordAuthentication() {
                return new PasswordAuthentication(username, password);
            }
        });
        try {
            this.from = new InternetAddress(username);
        } catch (AddressException e) {
            throw new RuntimeException("Error initializing MailWrapper.");
        }
    }

    @Override
    public void sendText(String to, String subj, String body) throws Exception {
        send(to, subj, body, "text/plain; charset=UTF-8");
    }

    @Override
    public void sendHtml(String to, String subj, String body) throws Exception {
        send(to, subj, body, "text/html");
    }

    private void send(String to, String subj, String body, String contentType) throws Exception {
        MimeMessage message = new MimeMessage(session);
        message.setFrom(from);
        message.setRecipients(Message.RecipientType.TO, InternetAddress.parse(to));
        message.setSubject(subj, "UTF-8");
        message.setContent(body, contentType);

        Transport.send(message);
        log.trace("Mail to {} was sent. Subj : {}, body : {}", to, subj, body);
    }

}
