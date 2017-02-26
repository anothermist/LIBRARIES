package cc.blynk.integration.http;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Button;
import cc.blynk.server.core.protocol.model.messages.appllication.SetWidgetPropertyMessage;
import cc.blynk.server.hardware.HardwareServer;
import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClientConfig;
import org.asynchttpclient.Response;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.concurrent.Future;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class HttpAPISetPropertyAsyncClientTest extends IntegrationBase {

    private static BaseServer httpServer;
    private static AsyncHttpClient httpclient;
    private static String httpsServerUrl;

    private static BaseServer hardwareServer;
    private static BaseServer appServer;
    private static ClientPair clientPair;

    @AfterClass
    public static void shutdown() throws Exception {
        httpclient.close();
        httpServer.close();
        appServer.close();
        hardwareServer.close();
        clientPair.stop();
    }

    @BeforeClass
    public static void init() throws Exception {
        httpServer = new HttpAPIServer(staticHolder).start();
        httpsServerUrl = String.format("http://localhost:%s/", httpPort);
        httpclient = new DefaultAsyncHttpClient(
                new DefaultAsyncHttpClientConfig.Builder()
                        .setUserAgent("")
                        .setKeepAlive(false)
                        .build()
        );
        hardwareServer = new HardwareServer(staticHolder).start();
        appServer = new AppServer(staticHolder).start();

        clientPair = initAppAndHardPair(tcpAppPort, tcpHardPort, properties);
    }

    @Before
    public void reset() {
        clientPair.appClient.reset();
    }

    @Test
    public void testChangeLabelPropertyViaGet() throws Exception {
        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody();

        Future<Response> f = httpclient.prepareGet(httpsServerUrl + token + "/update/v4?label=My-New-Label").execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(111, b("1 4 label My-New-Label"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte) 4, PinType.VIRTUAL);
        assertNotNull(widget);
        assertEquals("My-New-Label", widget.label);
    }

    @Test
    public void testChangeColorPropertyViaGet() throws Exception {
        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody();

        Future<Response> f = httpclient.prepareGet(httpsServerUrl + token + "/update/v4?color=%23000000").execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(111, b("1 4 color #000000"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte) 4, PinType.VIRTUAL);
        assertNotNull(widget);
        assertEquals(255, widget.color);
    }

    @Test
    public void testChangeOnLabelPropertyViaGet() throws Exception {
        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody();
        clientPair.appClient.reset();
        clientPair.appClient.send("updateWidget 1\0{\"id\":1, \"width\":1, \"height\":1,  \"x\":1, \"y\":1, \"label\":\"Some Text\", \"type\":\"BUTTON\",         \"pinType\":\"VIRTUAL\", \"pin\":1, \"value\":\"1\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        Future<Response> f = httpclient.prepareGet(httpsServerUrl + token + "/update/v1?onLabel=newOnButtonLabel").execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(111, b("1 1 onLabel newOnButtonLabel"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        Button button = (Button) profile.dashBoards[0].findWidgetByPin(0, (byte) 1, PinType.VIRTUAL);
        assertNotNull(button);
        assertEquals("newOnButtonLabel", button.onLabel);
    }


    @Test
    public void testChangeOffLabelPropertyViaGet() throws Exception {
        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody();
        clientPair.appClient.reset();
        clientPair.appClient.send("updateWidget 1\0{\"id\":1, \"width\":1, \"height\":1, \"x\":1, \"y\":1, \"label\":\"Some Text\", \"type\":\"BUTTON\",         \"pinType\":\"VIRTUAL\", \"pin\":1, \"value\":\"1\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        Future<Response> f = httpclient.prepareGet(httpsServerUrl + token + "/update/v1?offLabel=newOffButtonLabel").execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(111, b("1 1 offLabel newOffButtonLabel"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        Button button = (Button) profile.dashBoards[0].findWidgetByPin(0, (byte) 1, PinType.VIRTUAL);
        assertNotNull(button);
        assertEquals("newOffButtonLabel", button.offLabel);
    }

}
