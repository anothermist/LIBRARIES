package cc.blynk.integration;

import cc.blynk.server.Holder;
import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.notifications.mail.MailWrapper;
import cc.blynk.server.notifications.push.GCMWrapper;
import cc.blynk.server.notifications.sms.SMSWrapper;
import cc.blynk.server.notifications.twitter.TwitterWrapper;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.ServerProperties;
import org.apache.commons.lang.SystemUtils;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.util.EntityUtils;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.mockito.Mock;

import java.io.IOException;
import java.net.URI;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.attribute.FileAttribute;
import java.util.Collections;
import java.util.List;

import static org.mockito.Mockito.mock;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 20.01.16.
 */
public abstract class BaseTest {

    public static ServerProperties properties;

    //tcp app/hardware ports
    public static int tcpAppPort;
    public static int tcpHardPort;

    //http/s ports
    public static int httpPort;
    public static int httpsPort;

    public Holder holder;
    @Mock
    public BlockingIOProcessor blockingIOProcessor;
    @Mock
    public TwitterWrapper twitterWrapper;
    @Mock
    public MailWrapper mailWrapper;
    @Mock
    public GCMWrapper gcmWrapper;
    @Mock
    public SMSWrapper smsWrapper;


    public static Holder staticHolder;

    public static void sleep(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            //we can ignore it
        }
    }

    @BeforeClass
    public static void initProps() {
        properties = new ServerProperties(Collections.emptyMap());

        //disable native linux epoll transport for non linux envs.
        if (!SystemUtils.IS_OS_LINUX) {
            System.out.println("WARNING : DISABLING NATIVE EPOLL TRANSPORT. SYSTEM : " + SystemUtils.OS_NAME);
            properties.put("enable.native.epoll.transport", false);
        }

        tcpAppPort = properties.getIntProperty("app.ssl.port");
        tcpHardPort = properties.getIntProperty("hardware.default.port");

        httpPort = properties.getIntProperty("http.port");
        httpsPort = properties.getIntProperty("https.port");

        staticHolder = new Holder(properties, mock(TwitterWrapper.class), mock(MailWrapper.class), mock(GCMWrapper.class), mock(SMSWrapper.class));
    }

    @Before
    public void initHolderAndDataFolder() throws Exception {
        if (getDataFolder() != null) {
            properties.setProperty("data.folder", getDataFolder());
        }

        this.holder = new Holder(properties, twitterWrapper, mailWrapper, gcmWrapper, smsWrapper);
    }

    @After
    public void closeTransport() {
        this.holder.close();
    }

    @AfterClass
    public static void closeStaticTransport() {
        staticHolder.close();
    }

    public String getDataFolder() {
        try {
            return Files.createTempDirectory("blynk_test_", new FileAttribute[0]).toString();
        } catch (IOException e) {
            throw new RuntimeException("Unable create temp dir.", e);
        }
    }

    public static String getRelativeDataFolder(String path) {
        URL resource = IntegrationBase.class.getResource(path);
        URI uri = null;
        try {
            uri = resource.toURI();
        } catch (Exception e) {
            //ignoring. that's fine.
        }
        String resourcesPath = Paths.get(uri).toAbsolutePath().toString();
        System.out.println("Resource path : " + resourcesPath);
        return resourcesPath;
    }

    @SuppressWarnings("unchecked")
    public static List<String> consumeJsonPinValues(String response) {
        return JsonParser.readAny(response, List.class);
    }

    @SuppressWarnings("unchecked")
    public static List<String> consumeJsonPinValues(CloseableHttpResponse response) throws IOException {
        return JsonParser.readAny(consumeText(response), List.class);
    }

    @SuppressWarnings("unchecked")
    public static String consumeText(CloseableHttpResponse response) throws IOException {
        return EntityUtils.toString(response.getEntity());
    }

}

