package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareConnectedMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import cc.blynk.server.hardware.HardwareServer;
import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClientConfig;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import static cc.blynk.server.core.protocol.enums.Response.DEVICE_NOT_IN_NETWORK;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static org.junit.Assert.assertNotNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 5/09/2016.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class MultiAppTest extends IntegrationBase {

    private BaseServer httpServer;
    private BaseServer appServer;
    private BaseServer hardwareServer;
    private AsyncHttpClient httpclient;
    private String httpServerUrl;
    private ClientPair clientPair;

    @Before
    public void init() throws Exception {
        httpServer = new HttpAPIServer(holder).start();
        hardwareServer = new HardwareServer(holder).start();
        appServer = new AppServer(holder).start();
        httpServerUrl = String.format("http://localhost:%s/", httpPort);

        httpclient = new DefaultAsyncHttpClient(
                new DefaultAsyncHttpClientConfig.Builder()
                        .setUserAgent("")
                        .setKeepAlive(true)
                        .build());

        if (clientPair == null) {
            clientPair = initAppAndHardPair(tcpAppPort, tcpHardPort, properties);
        }
        clientPair.hardwareClient.reset();
        clientPair.appClient.reset();
    }

    @After
    public void shutdown() {
        httpServer.close();
        appServer.close();
        hardwareServer.close();
        clientPair.stop();
    }

    @Test
    public void testCreateFewAccountWithDifferentApp() throws Exception {
        TestAppClient appClient1 = new TestAppClient("localhost", tcpAppPort, properties);
        appClient1.start();
        TestAppClient appClient2 = new TestAppClient("localhost", tcpAppPort, properties);
        appClient2.start();

        String token1 = workflowForUser(appClient1, "test@blynk.cc", "a", "testapp1");
        String token2 = workflowForUser(appClient2, "test@blynk.cc", "a", "testapp2");

        appClient1.reset();
        appClient2.reset();

        TestHardClient hardClient1 = new TestHardClient("localhost", tcpHardPort);
        hardClient1.start();
        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient1.send("login " + token1);
        verify(hardClient1.responseMock, timeout(2000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(appClient1.responseMock, timeout(2000)).channelRead(any(), eq(new HardwareConnectedMessage(1, "1-0")));
        hardClient2.send("login " + token2);
        verify(hardClient2.responseMock, timeout(2000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(appClient2.responseMock, timeout(2000)).channelRead(any(), eq(new HardwareConnectedMessage(1, "1-0")));

        hardClient1.send("hardware vw 1 100");
        verify(appClient1.responseMock, timeout(2000)).channelRead(any(), eq(new HardwareMessage(2, b("1 vw 1 100"))));
        verify(appClient2.responseMock, timeout(500).times(0)).channelRead(any(), eq(new HardwareMessage(1, b("1 vw 1 100"))));

        appClient1.reset();
        appClient2.reset();

        hardClient2.send("hardware vw 1 100");
        verify(appClient2.responseMock, timeout(2000)).channelRead(any(), eq(new HardwareMessage(2, b("1 vw 1 100"))));
        verify(appClient1.responseMock, timeout(500).times(0)).channelRead(any(), eq(new HardwareMessage(1, b("1 vw 1 100"))));

    }

    private String workflowForUser(TestAppClient appClient, String username, String pass, String appName) throws Exception{
        appClient.send("register " + username + " " + pass + " " + appName);
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        appClient.send("login " + username + " " + pass + " Android 1.10.4 " + appName);
        //we should wait until login finished. Only after that we can send commands
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        DashBoard dash = new DashBoard();
        dash.id = 1;
        dash.name = "test";
        appClient.send("createDash " + dash.toString());
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(3, OK)));
        appClient.send("activate 1");
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(4, DEVICE_NOT_IN_NETWORK)));

        appClient.reset();
        appClient.send("getToken 1");

        String token = appClient.getBody();
        assertNotNull(token);
        return token;
    }

}
