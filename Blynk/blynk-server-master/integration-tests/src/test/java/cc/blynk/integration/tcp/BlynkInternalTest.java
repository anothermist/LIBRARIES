package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.HardwareInfo;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.BlynkInternalMessage;
import cc.blynk.server.hardware.HardwareServer;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.io.InputStream;

import static cc.blynk.server.core.protocol.enums.Response.OK;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/2/2015.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class BlynkInternalTest extends IntegrationBase {

    private BaseServer appServer;
    private BaseServer hardwareServer;
    private ClientPair clientPair;

    @Before
    public void init() throws Exception {
        this.hardwareServer = new HardwareServer(holder).start();
        this.appServer = new AppServer(holder).start();

        this.clientPair = initAppAndHardPair();
    }

    @After
    public void shutdown() {
        this.appServer.close();
        this.hardwareServer.close();
        this.clientPair.stop();
    }

    @Test
    public void testGetRTC() throws Exception {
        clientPair.appClient.send(("createWidget 1\0{\"type\":\"RTC\",\"id\":99, \"pin\":99, \"pinType\":\"VIRTUAL\", " +
                "\"x\":0,\"y\":0,\"width\":0,\"height\":0}"));

        clientPair.hardwareClient.send("internal rtc");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), any());

        String rtcResponse = clientPair.hardwareClient.getBody();
        assertNotNull(rtcResponse);

        String rtcTime = rtcResponse.split("\0")[1];

        assertNotNull(rtcTime);
        assertEquals(10, rtcTime.length());
        assertTrue(rtcTime.startsWith("14"));
    }

    @Test
    public void testHardwareLoginWithInfo() throws Exception {
        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        clientPair.appClient.send("getToken 1");
        String token2 = clientPair.appClient.getBody();
        hardClient2.send("login " + token2);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        hardClient2.send("internal " + b("ver 0.3.1 h-beat 10 buff-in 256 dev Arduino cpu ATmega328P con W5100"));

        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.reset();


        HardwareInfo hardwareInfo = new HardwareInfo();
        hardwareInfo.version = "0.3.1";
        hardwareInfo.heartbeatInterval = 10;
        hardwareInfo.boardType = "Arduino";
        hardwareInfo.cpuType = "ATmega328P";
        hardwareInfo.connectionType = "W5100";

        InputStream is = IntegrationBase.class.getResourceAsStream("/json_test/user_profile_json.txt");
        Profile expectedProfile = parseProfile(is);
        expectedProfile.dashBoards[0].hardwareInfo = hardwareInfo;

        clientPair.appClient.send("loadProfileGzipped");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), any());

        Profile profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;
        assertEquals(expectedProfile.toString(), profile.toString());


        hardClient2.stop().awaitUninterruptibly();
    }

    @Test
    public void appConnectedEvent() throws Exception {
        clientPair.appClient.send("updateDash {\"id\":1, \"name\":\"test board\", \"isAppConnectedOn\":true}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);
        appClient.start();

        appClient.send("login " + DEFAULT_TEST_USER + " 1 Android 1.13.3");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new BlynkInternalMessage(7777, "acon")));
    }

    @Test
    public void appDisconnectedEvent() throws Exception {
        clientPair.appClient.send("updateDash {\"id\":1, \"name\":\"test board\", \"isAppConnectedOn\":true}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.stop().await();

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new BlynkInternalMessage(7777, "adis")));
    }

}
