package cc.blynk.server.notifications.mail;

import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClientConfig;
import org.junit.Ignore;
import org.junit.Test;

import java.io.InputStream;
import java.util.Properties;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 06.04.15.
 */
public class MailWrapperTest {

    private DefaultAsyncHttpClient httpclient = new DefaultAsyncHttpClient(
            new DefaultAsyncHttpClientConfig.Builder()
    .setUserAgent(null)
    .setKeepAlive(false)
    .build()
    );

    @Test
    @Ignore
    public void sendMail() throws Exception {
        Properties properties = new Properties();
        try (InputStream classPath = MailWrapperTest.class.getResourceAsStream("/mail.properties")) {
            if (classPath != null) {
                properties.load(classPath);
            }
        }

        String to = "";
        MailWrapper mailWrapper = new MailWrapper(properties);
        mailWrapper.sendText(to, "Hello", "Body!");
    }

    @Test
    @Ignore
    public void sendMailWithAttachments() throws Exception {
        Properties properties = new Properties();
        try (InputStream classPath = MailWrapperTest.class.getResourceAsStream("/mail.properties")) {
            if (classPath != null) {
                properties.load(classPath);
            }
        }

        String to = "doom369@gmail.com";
        MailWrapper mailWrapper = new MailWrapper(properties);

        mailWrapper.sendText(to, "Hello", "Body!");
    }

    @Test
    @Ignore
    public void sendMailWithHttpProvider() throws Exception {
        Properties properties = new Properties();
        try (InputStream classPath = MailWrapperTest.class.getResourceAsStream("/mail.properties")) {
            if (classPath != null) {
                properties.load(classPath);
            }
        }

        String to = "";

        MailWrapper mailWrapper = new MailWrapper(properties);

        mailWrapper.sendText(to, "Hello", "Happy Blynking!\n" +
                "-\n" +
                "Getting Started Guide -> http://www.blynk.cc/getting-started\n" +
                "Documentation -> http://docs.blynk.cc/\n" +
                "Latest Blynk library -> https://github.com/blynkkk/blynk-library/releases/download/v0.3.9/Blynk_v0.3.9.zip\n" +
                "Latest Blynk server -> https://github.com/blynkkk/blynk-server/releases/download/v0.18.1/server-0.18.1.jar\n" +
                "-\n" +
                "http://www.blynk.cc\n" +
                "twitter.com/blynk_app\n" +
                "www.facebook.com/blynkapp");
    }

}
