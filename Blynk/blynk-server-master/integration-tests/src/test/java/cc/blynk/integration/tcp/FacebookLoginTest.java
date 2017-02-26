package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.widgets.MultiPinWidget;
import cc.blynk.server.core.model.widgets.OnePinWidget;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareConnectedMessage;
import cc.blynk.server.hardware.HardwareServer;
import io.netty.channel.ChannelFuture;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.List;

import static cc.blynk.server.core.protocol.enums.Response.OK;
import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 28.04.16.
 */
@RunWith(MockitoJUnitRunner.class)
@Ignore("ignored cause requires token to work properly")
public class FacebookLoginTest extends IntegrationBase {

    private BaseServer appServer;
    private BaseServer hardwareServer;
    private final String facebookAuthToken = "";

    @Before
    public void init() throws Exception {
        this.hardwareServer = new HardwareServer(holder).start();
        this.appServer = new AppServer(holder).start();
    }

    @After
    public void shutdown() {
        this.appServer.close();
        this.hardwareServer.close();
    }

    @Test
    public void testLoginWorksForNewUser() throws Exception {
        String host = "localhost";
        String username = "shartax@gmail.com";

        ClientPair clientPair = initAppAndHardPair(host, tcpAppPort, tcpHardPort, username + " 1", null, properties, 10000);

        ChannelFuture channelFuture = clientPair.appClient.stop();
        channelFuture.await();

        TestAppClient appClient = new TestAppClient(host, tcpAppPort, properties);
        appClient.start();
        appClient.send("login " + username + "\0" + facebookAuthToken + "\0" + "Android" + "\0" + "1.10.4" + "\0" + "facebook");
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        String expected = readTestUserProfile();

        appClient.reset();
        appClient.send("loadProfileGzipped");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), any());

        Profile profile = parseProfile(appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        assertEquals(expected, profile.toString());
    }

    @Test
    public void testFacebookLoginWorksForExistingUser() throws Exception {
        initFacebookAppAndHardPair("localhost", tcpAppPort, tcpHardPort, "shartax@gmail.com", facebookAuthToken);
    }

    private ClientPair initFacebookAppAndHardPair(String host, int appPort, int hardPort, String user, String facebookAuthToken) throws Exception {
        TestAppClient appClient = new TestAppClient(host, appPort, properties);
        TestHardClient hardClient = new TestHardClient(host, hardPort);

        appClient.start();
        hardClient.start();

        String userProfileString = readTestUserProfile(null);
        Profile profile = parseProfile(userProfileString);

        int expectedSyncCommandsCount = 0;
        for (Widget widget : profile.dashBoards[0].widgets) {
            if (widget instanceof OnePinWidget) {
                if (((OnePinWidget) widget).makeHardwareBody() != null) {
                    expectedSyncCommandsCount++;
                }
            } else if (widget instanceof MultiPinWidget) {
                MultiPinWidget multiPinWidget = ((MultiPinWidget) widget);
                if (multiPinWidget.pins != null) {
                    for (Pin pin : multiPinWidget.pins) {
                        if (pin.notEmpty()) {
                            expectedSyncCommandsCount++;
                        }
                    }
                }
            }
        }

        int dashId = profile.dashBoards[0].id;

        appClient.send("login " + user + "\0" + facebookAuthToken + "\0" + "Android" + "\0" + "1.10.4" + "\0" + "facebook");
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        appClient.send("addEnergy " + 10000 + "\0" + "123");
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        saveProfile(appClient, profile.dashBoards);

        appClient.send("activate " + dashId);
        appClient.send("getToken " + dashId);

        ArgumentCaptor<Object> objectArgumentCaptor = ArgumentCaptor.forClass(Object.class);
        verify(appClient.responseMock, timeout(2000).times(4 + profile.dashBoards.length + expectedSyncCommandsCount)).channelRead(any(), objectArgumentCaptor.capture());

        List<Object> arguments = objectArgumentCaptor.getAllValues();
        String token = getGetTokenMessage(arguments).body;

        hardClient.send("login " + token);
        verify(hardClient.responseMock, timeout(2000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(appClient.responseMock, timeout(2000)).channelRead(any(), eq(new HardwareConnectedMessage(1, String.valueOf(dashId))));

        appClient.reset();
        hardClient.reset();

        return new ClientPair(appClient, hardClient, token);
    }


}
