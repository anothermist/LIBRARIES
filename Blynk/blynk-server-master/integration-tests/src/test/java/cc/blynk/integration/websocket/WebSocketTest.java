package cc.blynk.integration.websocket;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.websocket.WebSocketClient;
import cc.blynk.server.Holder;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.server.websocket.WebSocketServer;
import cc.blynk.utils.properties.GCMProperties;
import cc.blynk.utils.properties.MailProperties;
import cc.blynk.utils.properties.SmsProperties;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Collections;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.01.16.
 */
@RunWith(MockitoJUnitRunner.class)
public class WebSocketTest extends IntegrationBase {

    private static BaseServer webSocketServer;
    private static BaseServer hardwareServer;
    private static BaseServer appServer;
    private static ClientPair clientPair;
    private static Holder localHolder;

    //web socket ports
    public static int tcpWebSocketPort;

    @AfterClass
    public static void shutdown() throws Exception {
        webSocketServer.close();
        appServer.close();
        hardwareServer.close();
        clientPair.stop();
        localHolder.close();
    }

    @BeforeClass
    public static void init() throws Exception {
        properties.setProperty("data.folder", getRelativeDataFolder("/profiles"));
        localHolder = new Holder(properties,
                new MailProperties(Collections.emptyMap()),
                new SmsProperties(Collections.emptyMap()),
                new GCMProperties(Collections.emptyMap())
        );
        tcpWebSocketPort = properties.getIntProperty("tcp.websocket.port");
        webSocketServer = new WebSocketServer(localHolder).start();
        appServer = new AppServer(localHolder).start();
        hardwareServer = new HardwareServer(localHolder).start();
        clientPair = initAppAndHardPair(tcpAppPort, tcpHardPort, properties);
    }

    @Test
    public void testBasicWebSocketCommandsOk2() throws Exception{
        WebSocketClient webSocketClient = new WebSocketClient("localhost", tcpWebSocketPort, "/websockets", false);
        webSocketClient.start();
        webSocketClient.send("login 4ae3851817194e2596cf1b7103603ef8");
        verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        webSocketClient.send("ping");
        verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));
    }

    @Test
    public void testBasicWebSocketCommandsOk() throws Exception{
        WebSocketClient webSocketClient = new WebSocketClient("localhost", tcpWebSocketPort, WebSocketServer.WEBSOCKET_PATH, false);
        webSocketClient.start();
        webSocketClient.send("login 4ae3851817194e2596cf1b7103603ef8");
        verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        webSocketClient.send("ping");
        verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));
    }

    @Test
    public void testSyncBetweenWebSocketsAndAppWorks() throws Exception {
        clientPair.appClient.reset();
        clientPair.hardwareClient.reset();
        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody();

        WebSocketClient webSocketClient = new WebSocketClient("localhost", tcpWebSocketPort, WebSocketServer.WEBSOCKET_PATH, false);
        webSocketClient.start();
        webSocketClient.send("login " + token);
        verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("hardware 1 vw 4 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 4 1"))));
        verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 4 1"))));

        webSocketClient.send("hardware vw 4 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 4 2"))));

        clientPair.hardwareClient.send("hardware vw 4 3");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 4 3"))));

        clientPair.appClient.reset();
        WebSocketClient webSocketClient2 = new WebSocketClient("localhost", tcpWebSocketPort, WebSocketServer.WEBSOCKET_PATH, false);
        webSocketClient2.start();
        webSocketClient2.send("login " + token);
        verify(webSocketClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(webSocketClient2.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(1, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in"))));
        webSocketClient2.msgId = 1000;

        for (int i = 1; i <= 10; i++) {
            clientPair.appClient.send("hardware 1 vw 4 " + i);
            verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(i, HARDWARE, b("vw 4 " + i))));
            verify(webSocketClient.responseMock, timeout(500)).channelRead(any(), eq(produce(i, HARDWARE, b("vw 4 " + i))));
            verify(webSocketClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(i, HARDWARE, b("vw 4 " + i))));
            webSocketClient2.send("hardsync " + b("vr 4"));
            verify(webSocketClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(1000 + i, HARDWARE, b("vw 4 " + i))));
        }
    }

    @Test
    public void testSyncBetweenWebSocketsAndAppWorksLoop() throws Exception {
        for (int i = 0; i < 10; i++) {
            testSyncBetweenWebSocketsAndAppWorks();
        }
    }


}
