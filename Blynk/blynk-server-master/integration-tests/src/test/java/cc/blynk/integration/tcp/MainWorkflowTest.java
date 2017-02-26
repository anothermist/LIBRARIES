package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.dao.ReportingDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.model.enums.GraphType;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.others.Player;
import cc.blynk.server.core.model.widgets.ui.TimeInput;
import cc.blynk.server.core.protocol.model.messages.BinaryMessage;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateDevice;
import cc.blynk.server.core.protocol.model.messages.appllication.GetTokenMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareConnectedMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.server.notifications.push.android.AndroidGCMMessage;
import cc.blynk.server.notifications.push.enums.Priority;
import cc.blynk.utils.ByteUtils;
import cc.blynk.utils.FileUtils;
import cc.blynk.utils.JsonParser;
import io.netty.channel.ChannelFuture;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.runners.MockitoJUnitRunner;

import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.ZoneId;
import java.util.List;

import static cc.blynk.server.core.protocol.enums.Command.GET_ENERGY;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.DEVICE_NOT_IN_NETWORK;
import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND;
import static cc.blynk.server.core.protocol.enums.Response.INVALID_TOKEN;
import static cc.blynk.server.core.protocol.enums.Response.NOTIFICATION_INVALID_BODY;
import static cc.blynk.server.core.protocol.enums.Response.NOTIFICATION_NOT_AUTHORIZED;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.server.core.protocol.enums.Response.NO_ACTIVE_DASHBOARD;
import static cc.blynk.server.core.protocol.enums.Response.NO_DATA;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.enums.Response.QUOTA_LIMIT;
import static cc.blynk.server.core.protocol.enums.Response.USER_ALREADY_REGISTERED;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.after;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.atLeast;
import static org.mockito.Mockito.contains;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.startsWith;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/2/2015.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class MainWorkflowTest extends IntegrationBase {

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
    public void createBasicProfile() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);

        appClient.start();

        appClient.send("register test@test.com 1");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("login test@test.com 1 Android RC13");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.send("createDash {\"id\":1, \"createdAt\":1, \"name\":\"test board\"}\"");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        appClient.send("createWidget 1\0{\"id\":1, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":1}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        appClient.reset();

        appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;

        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"test board\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":0,\"y\":0,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"pushMode\":false}],\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}]}", profile.toString());

        appClient.send("createWidget 1\0{\"id\":2, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.reset();

        appClient.send("loadProfileGzipped");
        profile = parseProfile(appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"test board\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":0,\"y\":0,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"pushMode\":false},{\"type\":\"BUTTON\",\"id\":2,\"x\":2,\"y\":2,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"Some Text 2\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":2,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"pushMode\":false}],\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}]}", profile.toString());

        appClient.send("updateWidget 1\0{\"id\":2, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"new label\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":3}\"");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.reset();

        appClient.send("loadProfileGzipped");
        profile = parseProfile(appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"test board\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":0,\"y\":0,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"pushMode\":false},{\"type\":\"BUTTON\",\"id\":2,\"x\":2,\"y\":2,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"new label\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":3,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"pushMode\":false}],\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}]}", profile.toString());

        appClient.send("deleteWidget 1\0" + "3");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, ILLEGAL_COMMAND)));

        appClient.send("deleteWidget 1\0" + "1");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        appClient.send("deleteWidget 1\0" + "2");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        appClient.reset();

        appClient.send("loadProfileGzipped");
        profile = parseProfile(appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"test board\",\"createdAt\":1,\"updatedAt\":0,\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}]}", profile.toString());
    }

    @Test
    public void testConnectAppAndHardware() throws Exception {
        // we just test that app and hardware can actually connect
    }

    @Test
    public void testRegisterWithAnotherApp() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);

        appClient.start();

        appClient.send("register test@test.com 1 MyApp");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("login test@test.com 1 Android 1.13.3 MyApp");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.send("createDash {\"id\":1, \"createdAt\":1, \"name\":\"test board\"}\"");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        appClient.send("createWidget 1\0{\"id\":1, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":1}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        appClient.reset();

        appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;

        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"test board\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":0,\"y\":0,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"pushMode\":false}],\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}]}", profile.toString());
    }

    @Test
    public void testDoubleLogin() throws Exception {
        clientPair.hardwareClient.send("login " + DEFAULT_TEST_USER + " 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, USER_ALREADY_REGISTERED)));
    }

    @Test
    public void testNoEnergyDrainForBusinessApps() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);

        appClient.start();

        appClient.send("register test@test.com 1 MyApp");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("login test@test.com 1 Android 1.13.3 MyApp");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.send("createDash {\"id\":2, \"createdAt\":1458856800001, \"name\":\"test board\"}\"");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        appClient.send("getEnergy");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(4, GET_ENERGY, "2000")));

        appClient.send("createWidget 2\0{\"id\":2, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(5, OK)));

        appClient.send("createWidget 2\0{\"id\":3, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(6, OK)));

        appClient.send("createWidget 2\0{\"id\":4, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(7, OK)));

        appClient.send("createWidget 2\0{\"id\":5, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(8, OK)));

        appClient.send("createWidget 2\0{\"id\":6, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(9, OK)));

        appClient.send("createWidget 2\0{\"id\":7, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(10, OK)));
    }

    @Test
    public void testPingCommandWorks() throws Exception {
        clientPair.appClient.send("ping");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
    }

    @Test
    public void testTerminalStorageRemembersCommands() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(16, profile.dashBoards[0].widgets.length);

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "7500")));

        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":0, \"label\":\"Some Text\", \"type\":\"TERMINAL\", \"pinType\":\"VIRTUAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.hardwareClient.send("hardware vw 17 1");
        clientPair.hardwareClient.send("hardware vw 17 2");
        clientPair.hardwareClient.send("hardware vw 17 3");
        clientPair.hardwareClient.send("hardware vw 17 4");
        clientPair.hardwareClient.send("hardware vw 17 5");
        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), eq(produce(5, HARDWARE, b("1 vw 17 5"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;
        assertEquals(17, profile.dashBoards[0].widgets.length);
        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"My Dashboard\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"pushMode\":false},{\"type\":\"SLIDER\",\"id\":2,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":2,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":3,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":3,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"0\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":4,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":4,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"244\",\"sendOnReleaseOn\":false},{\"type\":\"TIMER\",\"id\":5,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":5,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"dw\\u00005\\u00001\",\"startTime\":0,\"stopTime\":-1},{\"type\":\"LED\",\"id\":6,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":6,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":7,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":5000},{\"type\":\"DIGIT4_DISPLAY\",\"id\":30,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":30,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":100},{\"type\":\"GRAPH\",\"id\":8,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":8,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"frequency\":0,\"isBar\":false},{\"type\":\"NOTIFICATION\",\"id\":9,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"androidTokens\":{\"uid\":\"token\"},\"notifyWhenOffline\":true,\"notifyWhenOfflineIgnorePeriod\":0,\"priority\":\"normal\"},{\"type\":\"TWITTER\",\"id\":10,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"token\":\"token\",\"secret\":\"secret\"},{\"type\":\"RTC\",\"id\":11,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":9,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"LCD\",\"id\":12,\"x\":0,\"y\":0,\"color\":-1,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":0,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"89.888037459418\",\"min\":-100,\"max\":100},{\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"-58.74774244674501\",\"min\":-100,\"max\":100}],\"advancedMode\":false,\"textFormatLine1\":\"pin1 : /pin0/\",\"textFormatLine2\":\"pin2 : /pin1/\",\"textLight\":false,\"frequency\":1000},{\"type\":\"RGB\",\"id\":13,\"x\":2,\"y\":3,\"color\":616861439,\"width\":4,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255}],\"splitMode\":false,\"sendOnReleaseOn\":true},{\"type\":\"LOGGER\",\"id\":14,\"x\":0,\"y\":6,\"color\":0,\"width\":8,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"ANALOG\",\"min\":0,\"max\":255},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0}],\"period\":\"THREE_MONTHS\",\"max\":0,\"min\":0,\"autoYCoords\":false,\"showLegends\":true},{\"type\":\"LCD\",\"id\":15,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023},{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023}],\"advancedMode\":true,\"textLight\":false,\"frequency\":0},{\"type\":\"TERMINAL\",\"id\":102,\"x\":5,\"y\":0,\"color\":0,\"width\":1,\"height\":1,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":17,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"autoScrollOn\":false,\"terminalInputOn\":false,\"textLightOn\":false}],\"boardType\":\"UNO\",\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":true,\"isActive\":true}]}", profile.toString());
    }

    @Test
    public void testAddAndRemoveTabs() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(16, profile.dashBoards[0].widgets.length);

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "7500")));

        clientPair.appClient.send("createWidget 1\0{\"id\":100, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"tabs\":[{\"label\":\"tab 1\"}, {\"label\":\"tab 2\"}, {\"label\":\"tab 3\"}], \"type\":\"TABS\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("createWidget 1\0{\"id\":101, \"width\":1, \"height\":1, \"x\":15, \"y\":0, \"tabId\":1, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":18}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(5, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(6, GET_ENERGY, "7100")));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(19, profile.dashBoards[0].widgets.length);

        clientPair.appClient.send("deleteWidget 1\0" + "100");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, GET_ENERGY, "7300")));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(17, profile.dashBoards[0].widgets.length);
        assertNotNull(profile.dashBoards[0].findWidgetByPin(0, (byte) 17, PinType.DIGITAL));
    }

    @Test
    public void testAddAndUpdateTabs() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(16, profile.dashBoards[0].widgets.length);

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "7500")));

        clientPair.appClient.send("createWidget 1\0{\"id\":100, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"tabs\":[{\"label\":\"tab 1\"}, {\"label\":\"tab 2\"}, {\"label\":\"tab 3\"}], \"type\":\"TABS\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("createWidget 1\0{\"id\":101, \"width\":1, \"height\":1, \"x\":15, \"y\":0, \"tabId\":1, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":18}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":2, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(5, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(6, GET_ENERGY, "7100")));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(19, profile.dashBoards[0].widgets.length);

        clientPair.appClient.send("updateWidget 1\0{\"id\":100, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"tabs\":[{\"label\":\"tab 1\"}, {\"label\":\"tab 2\"}], \"type\":\"TABS\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, GET_ENERGY, "7300")));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(18, profile.dashBoards[0].widgets.length);
        assertNull(profile.dashBoards[0].findWidgetByPin(0, (byte) 17, PinType.DIGITAL));
        assertNotNull(profile.dashBoards[0].findWidgetByPin(0, (byte) 18, PinType.DIGITAL));
    }

    @Test
    public void testPurchaseEnergy() throws Exception {
        clientPair.appClient.send("addEnergy " + "1000" + "\0" + "5262996016779471529.4493624392154338");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, NOT_ALLOWED)));

        clientPair.appClient.send("addEnergy " + "1000" + "\0" + "A3B93EE9-BC65-499E-A660-F2A84F2AF1FC");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, NOT_ALLOWED)));

        clientPair.appClient.send("addEnergy " + "1000" + "\0" + "com.blynk.energy.280001461578468247");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, NOT_ALLOWED)));

        clientPair.appClient.send("addEnergy " + "1000");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, NOT_ALLOWED)));

        clientPair.appClient.send("addEnergy " + "1000" + "\0");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(5, NOT_ALLOWED)));

        clientPair.appClient.send("addEnergy " + "1000" + "\0" + "150000195113772");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(6, OK)));

        clientPair.appClient.send("addEnergy " + "1000" + "\0" + "1370-3990-1414-55681");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(7, OK)));
    }

    @Test
    @Ignore
    public void testProfileMetadata() throws Exception {
        clientPair.appClient.send("saveMetadata {\"lat\":123.123,\"lon\":124.124}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.reset();
        clientPair.appClient.send("getMetadata");
        String token = clientPair.appClient.getBody();
        assertNotNull(token);
        assertEquals("{\"lat\":123.123,\"lon\":124.124}", token);
    }

    @Test
    public void testApplicationPingCommandOk() throws Exception {
        clientPair.appClient.send("ping");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.reset();

        clientPair.appClient.send("ping");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
    }

    @Test
    public void testHardPingCommandOk() throws Exception {
        clientPair.hardwareClient.send("ping");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("ping");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));
    }

    @Test
    public void testDashCommands() throws Exception {
        clientPair.appClient.send("updateDash {\"id\":10, \"name\":\"test board update\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND)));

        clientPair.appClient.send("createDash {\"id\":10, \"name\":\"test board\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.send("createDash {\"id\":10, \"name\":\"test board\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, NOT_ALLOWED)));

        clientPair.appClient.send("updateDash {\"id\":10, \"name\":\"test board update\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        clientPair.hardwareClient.send("ping");

        clientPair.appClient.send("deleteDash 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(5, OK)));

        clientPair.appClient.send("deleteDash 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(6, ILLEGAL_COMMAND)));

        Profile responseProfile;
        DashBoard responseDash;

        clientPair.appClient.send("loadProfileGzipped");
        responseProfile = parseProfile(clientPair.appClient.getBody(7));
        responseProfile.dashBoards[0].updatedAt = 0;
        responseProfile.dashBoards[0].createdAt = 0;
        assertEquals("{\"dashBoards\":[{\"id\":10,\"name\":\"test board update\",\"createdAt\":0,\"updatedAt\":0,\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}]}", responseProfile.toString());

        clientPair.appClient.send("loadProfileGzipped 10");
        responseDash = JsonParser.parseDashboard(clientPair.appClient.getBody(8));
        responseDash.updatedAt = 0;
        responseDash.createdAt = 0;
        assertEquals("{\"id\":10,\"name\":\"test board update\",\"createdAt\":0,\"updatedAt\":0,\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}", responseDash.toString());

        clientPair.appClient.send("loadProfileGzipped 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(9, ILLEGAL_COMMAND)));

        clientPair.appClient.send("activate 10");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(10, DEVICE_NOT_IN_NETWORK)));

        clientPair.appClient.send("loadProfileGzipped");
        responseProfile = parseProfile(clientPair.appClient.getBody(11));
        responseProfile.dashBoards[0].updatedAt = 0;
        responseProfile.dashBoards[0].createdAt = 0;
        String expectedProfile = "{\"dashBoards\":[{\"id\":10,\"name\":\"test board update\",\"createdAt\":0,\"updatedAt\":0,\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":true}]}";
        assertEquals(expectedProfile, responseProfile.toString());

        clientPair.appClient.send("updateDash {\"id\":10,\"name\":\"test board update\",\"keepScreenOn\":false,\"isShared\":false,\"isActive\":false}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        expectedProfile = "{\"dashBoards\":[{\"id\":10,\"name\":\"test board update\",\"createdAt\":0,\"updatedAt\":0,\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":true}]}";
        clientPair.appClient.send("loadProfileGzipped");
        responseProfile = parseProfile(clientPair.appClient.getBody(13));
        responseProfile.dashBoards[0].updatedAt = 0;
        responseProfile.dashBoards[0].createdAt = 0;
        assertEquals(expectedProfile, responseProfile.toString());
    }

    @Test
    public void testHardwareChannelClosedOnDashRemoval() throws Exception {
        clientPair.appClient.send("deleteDash 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        assertTrue(clientPair.hardwareClient.isClosed());
    }

    @Test
    public void deleteDashDeletesTokensAlso() throws Exception {
        clientPair.appClient.send("createDash {\"id\":10, \"name\":\"test board\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.reset();
        clientPair.appClient.send("getToken 10");
        String token = clientPair.appClient.getBody();
        assertNotNull(token);

        clientPair.appClient.reset();
        clientPair.appClient.send("getShareToken 10");
        String sharedToken = clientPair.appClient.getBody();
        assertNotNull(sharedToken);

        clientPair.appClient.send("deleteDash 10");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        //todo on delete also close existing connections?
        TestHardClient newHardClient = new TestHardClient("localhost", tcpHardPort);
        newHardClient.start();
        newHardClient.send("login " + token);
        verify(newHardClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, INVALID_TOKEN)));

        TestAppClient newAppClient = new TestAppClient("localhost", tcpAppPort, properties);
        newAppClient.start();
        newAppClient.send("shareLogin " + "dima@mail.ua " + sharedToken + " Android 24");

        verify(newAppClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, NOT_ALLOWED)));
    }

    @Test
    public void loadGzippedProfile() throws Exception{
        String expected = readTestUserProfile();

        clientPair.appClient.send("loadProfileGzipped");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), any());

        Profile profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;
        assertEquals(expected, profile.toString());
    }

    @Test
    public void testGetGraphDataFor1Pin() throws Exception {
        String tempDir = holder.props.getProperty("data.folder");

        final Path userReportFolder = Paths.get(tempDir, "data", DEFAULT_TEST_USER);
        if (Files.notExists(userReportFolder)) {
            Files.createDirectories(userReportFolder);
        }

        Path pinReportingDataPath = Paths.get(tempDir, "data", DEFAULT_TEST_USER, ReportingDao.generateFilename(1, 0, PinType.DIGITAL.pintTypeChar, (byte) 8, GraphType.HOURLY));

        FileUtils.write(pinReportingDataPath, 1.11D, 1111111);
        FileUtils.write(pinReportingDataPath, 1.22D, 2222222);

        clientPair.appClient.send("getgraphdata 1 d 8 24 h");

        ArgumentCaptor<BinaryMessage> objectArgumentCaptor = ArgumentCaptor.forClass(BinaryMessage.class);
        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), objectArgumentCaptor.capture());
        BinaryMessage graphDataResponse = objectArgumentCaptor.getValue();

        assertNotNull(graphDataResponse);
        byte[] decompressedGraphData = ByteUtils.decompress(graphDataResponse.getBytes());
        ByteBuffer bb = ByteBuffer.wrap(decompressedGraphData);

        assertEquals(1, bb.getInt());
        assertEquals(2, bb.getInt());
        assertEquals(1.11D, bb.getDouble(), 0.1);
        assertEquals(1111111, bb.getLong());
        assertEquals(1.22D, bb.getDouble(), 0.1);
        assertEquals(2222222, bb.getLong());

    }

    @Test
    public void testDeleteGraphCommandWorks() throws Exception {
        clientPair.appClient.send("getgraphdata 1 d 8 del");

        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
    }

    @Test
    public void testSendEmail() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);
        appClient.start();
        appClient.send("login dima@mail.ua 1");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("email 1");
        verify(mailWrapper, timeout(1000)).sendText(eq(DEFAULT_TEST_USER), eq("Auth Token for My Dashboard project and device UNO"), startsWith("Auth Token : "));
    }

    @Test
    public void testSendEmailForDevice() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);
        appClient.start();
        appClient.send("login dima@mail.ua 1");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("email 1 0");
        verify(mailWrapper, timeout(1000)).sendText(eq(DEFAULT_TEST_USER), eq("Auth Token for My Dashboard project and device UNO"), startsWith("Auth Token : "));
    }

    @Test
    //todo fix this test
    public void testSendEmailForMultiDevices() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);
        appClient.start();
        appClient.send("login dima@mail.ua 1");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        clientPair.appClient.send("getDevices 1");
        String response = clientPair.appClient.getBody(2);

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);

        appClient.send("email 1");

        String expectedBody = String.format("Auth Token for device 'UNO' : %s\n" +
                "Auth Token for device 'My Device' : %s\n" +
                "\n" +
                "Happy Blynking!\n" +
                "-\n" +
                "Getting Started Guide -> http://www.blynk.cc/getting-started\n" +
                "Documentation -> http://docs.blynk.cc/\n" +
                "Latest Blynk library -> https://github.com/blynkkk/blynk-library/releases/download/v0.4.4/Blynk_Release_v0.4.4.zip\n" +
                "Latest Blynk server -> https://github.com/blynkkk/blynk-server/releases/download/v0.22.3/server-0.22.3.jar\n" +
                "-\n" +
                "http://www.blynk.cc\n" +
                "twitter.com/blynk_app\n" +
                "www.facebook.com/blynkapp\n", devices[0].token, devices[1].token);

        verify(mailWrapper, timeout(1000)).sendText(eq(DEFAULT_TEST_USER), eq("Auth Tokens for My Dashboard project and 2 devices"), eq(expectedBody));
    }

    @Test
    public void testExportDataFromHistoryGraph() throws Exception {
        clientPair.appClient.send("export 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND)));

        clientPair.appClient.send("export 1 666");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, ILLEGAL_COMMAND)));

        clientPair.appClient.send("export 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, ILLEGAL_COMMAND)));

        clientPair.appClient.send("export 1 14");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, NO_DATA)));

        //generate fake reporting data
        Path userReportDirectory = Paths.get(holder.props.getProperty("data.folder"), "data", DEFAULT_TEST_USER);
        Files.createDirectories(userReportDirectory);
        Path userReportFile = Paths.get(userReportDirectory.toString(), ReportingDao.generateFilename(1, 0, PinType.ANALOG.pintTypeChar, (byte) 7, GraphType.MINUTE));
        FileUtils.write(userReportFile, 1.1, 1L);
        FileUtils.write(userReportFile, 2.2, 2L);

        clientPair.appClient.send("export 1 14");
        verify(mailWrapper, timeout(1000)).sendHtml(eq(DEFAULT_TEST_USER), eq("History graph data for project My Dashboard"), contains("/dima@mail.ua_1_a7.csv.gz"));
    }

    @Test
    @Ignore
    public void testLargeMessageIsNotAccepted() throws Exception {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 32 * 1024 + 1; i++) {
            sb.append("a");
        }
        clientPair.hardwareClient.send("hardware vw 1 " + sb.toString());
        verify(clientPair.appClient.responseMock, after(500).never()).channelRead(any(), any());
    }

    @Test
    public void testSendUnicodeChar() throws Exception {
        clientPair.hardwareClient.send("hardware vw 1 °F");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 °F"))));
    }

    @Test
    public void testAppSendAnyHardCommandAndBack() throws Exception {
        clientPair.appClient.send("hardware 1 dw 1 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("dw 1 1"))));

        clientPair.hardwareClient.send("hardware ar 1");

        ArgumentCaptor<StringMessage> objectArgumentCaptor = ArgumentCaptor.forClass(StringMessage.class);
        verify(clientPair.appClient.responseMock, timeout(500).times(1)).channelRead(any(), objectArgumentCaptor.capture());

        List<StringMessage> arguments = objectArgumentCaptor.getAllValues();
        StringMessage hardMessage = arguments.get(0);
        assertEquals(1, hardMessage.id);
        assertEquals(HARDWARE, hardMessage.command);
        assertEquals(6, hardMessage.length);
        assertEquals(b("1 ar 1"), hardMessage.body);
    }

    @Test
    public void testAppNoActiveDashForHard() throws Exception {
        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 aw 1 1"))));

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, after(500).never()).channelRead(any(), eq(new ResponseMessage(2, NO_ACTIVE_DASHBOARD)));
    }

    @Test
    public void testHardwareSendsWrongCommand() throws Exception {
        clientPair.hardwareClient.send("hardware aw 1 ");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND)));

        clientPair.hardwareClient.send("hardware aw 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, ILLEGAL_COMMAND)));
    }

    @Test
    public void testAppChangeActiveDash() throws Exception {
        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 aw 1 1"))));

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Profile newProfile = parseProfile(readTestUserProfile("user_profile_json_3_dashes.txt"));
        clientPair.appClient.send("createDash " + newProfile.dashBoards[1]);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, after(500).never()).channelRead(any(), eq(new ResponseMessage(2, NO_ACTIVE_DASHBOARD)));

        clientPair.appClient.send("activate 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, DEVICE_NOT_IN_NETWORK)));

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, after(500).never()).channelRead(any(), eq(new ResponseMessage(3, NO_ACTIVE_DASHBOARD)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(4, HARDWARE, b("1 aw 1 1"))));
    }

    @Test
    public void testActive2AndDeactivate1() throws Exception {
        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        Profile newProfile = parseProfile(readTestUserProfile("user_profile_json_3_dashes.txt"));
        clientPair.appClient.send("createDash " + newProfile.dashBoards[1]);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.send("activate 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, DEVICE_NOT_IN_NETWORK)));

        clientPair.appClient.reset();

        clientPair.appClient.send("getToken 2");
        String token2 = clientPair.appClient.getBody();
        hardClient2.send("login " + token2);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.reset();

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 aw 1 1"))));

        hardClient2.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("2 aw 1 1"))));


        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, after(500).never()).channelRead(any(), eq(new ResponseMessage(2, NO_ACTIVE_DASHBOARD)));

        hardClient2.send("hardware aw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, HARDWARE, b("2 aw 1 1"))));
        hardClient2.stop().awaitUninterruptibly();
    }

    @Test
    public void testPushWhenHardwareOffline() throws Exception {
        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.await();

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Your UNO went offline. \"My Dashboard\" project is disconnected.", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testPushHandler() throws Exception {
        clientPair.hardwareClient.send("push Yo!");

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Yo!", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testActivateWorkflow() throws Exception {
        clientPair.appClient.send("activate 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND)));

        clientPair.appClient.send("deactivate 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, ILLEGAL_COMMAND)));

        clientPair.appClient.send("hardware 1 ar 1 1");
        //todo check no response
        verify(clientPair.appClient.responseMock, never()).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));
    }

    @Test
    public void testTweetNotWorks() throws Exception {
        reset(blockingIOProcessor);

        clientPair.hardwareClient.send("tweet");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, NOTIFICATION_INVALID_BODY)));

        clientPair.hardwareClient.send("tweet ");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, NOTIFICATION_INVALID_BODY)));

        StringBuilder a = new StringBuilder();
        for (int i = 0; i < 141; i++) {
            a.append("a");
        }

        clientPair.hardwareClient.send("tweet " + a);
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, NOTIFICATION_INVALID_BODY)));

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("tweet yo");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, NOTIFICATION_NOT_AUTHORIZED)));
    }

    @Test
    public void testSmsWorks() throws Exception {
        reset(blockingIOProcessor);

        clientPair.hardwareClient.send("sms");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, NOTIFICATION_INVALID_BODY)));

        //no sms widget
        clientPair.hardwareClient.send("sms yo");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, NOTIFICATION_NOT_AUTHORIZED)));

        //adding sms widget
        clientPair.appClient.send("createWidget 1\0{\"id\":432, \"width\":1, \"height\":1, \"to\":\"3809683423423\", \"x\":0, \"y\":0, \"type\":\"SMS\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("sms yo");
        verify(smsWrapper, timeout(500)).send(eq("3809683423423"), eq("yo"));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.hardwareClient.send("sms yo");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, QUOTA_LIMIT)));
    }

    @Test
    public void testTweetWorks() throws Exception {
        reset(blockingIOProcessor);

        clientPair.hardwareClient.send("tweet yo");
        verify(twitterWrapper, timeout(500)).send(eq("token"), eq("secret"), eq("yo"));

        clientPair.hardwareClient.send("tweet yo");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, QUOTA_LIMIT)));
    }

    @Test
    public void testEmailMininalValidation() throws Exception {
        reset(blockingIOProcessor);

        //adding email widget
        clientPair.appClient.send("createWidget 1\0{\"id\":432, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"type\":\"EMAIL\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("email to subj body");
        verify(mailWrapper, after(500).never()).sendHtml(eq("to"), eq("subj"), eq("body"));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND)));
    }

    @Test
    public void testEmailWorks() throws Exception {
        reset(blockingIOProcessor);

        //no email widget
        clientPair.hardwareClient.send("email to subj body");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, NOT_ALLOWED)));

        //adding email widget
        clientPair.appClient.send("createWidget 1\0{\"id\":432, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"type\":\"EMAIL\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("email to@to.com subj body");
        verify(mailWrapper, timeout(500)).sendHtml(eq("to@to.com"), eq("subj"), eq("body"));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.hardwareClient.send("email to@to.com subj body");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, QUOTA_LIMIT)));
    }

    @Test
    public void testEmailWorkWithEmailFromApp() throws Exception {
        reset(blockingIOProcessor);

        //adding email widget
        clientPair.appClient.send("createWidget 1\0{\"id\":432, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"to\":\"test@mail.ua\", \"type\":\"EMAIL\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("email subj body");
        verify(mailWrapper, timeout(500)).sendHtml(eq("test@mail.ua"), eq("subj"), eq("body"));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
    }

    @Test
    public void testPlayerUpdateWorksAsExpected() throws Exception {
        clientPair.appClient.send(("createWidget 1\0{\"type\":\"PLAYER\",\"id\":99, \"pin\":99, \"pinType\":\"VIRTUAL\", " +
                "\"x\":0,\"y\":0,\"width\":1,\"height\":1}"));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("hardware 1 vw 99 play");
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 play"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        Player player = (Player) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(player);
        assertTrue(player.isOnPlay);

        clientPair.appClient.send("hardware 1 vw 99 stop");
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 stop"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        player = (Player) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(player);
        assertFalse(player.isOnPlay);
    }

    @Test
    public void testTimeInputUpdateWorksAsExpected() throws Exception {
        clientPair.appClient.send(("createWidget 1\0{\"type\":\"TIME_INPUT\",\"id\":99, \"pin\":99, \"pinType\":\"VIRTUAL\", " +
                "\"x\":0,\"y\":0,\"width\":1,\"height\":1}"));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("hardware 1 vw " + b("99 82800 82860 Europe/Kiev 1"));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 82800 82860 Europe/Kiev 1"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        TimeInput timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(82800, timeInput.startAt);
        assertEquals(82860, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertArrayEquals(new int[] {1}, timeInput.days);


        clientPair.appClient.send("hardware 1 vw " + b("99 82800 82860 Europe/Kiev "));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 82800 82860 Europe/Kiev "))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(82800, timeInput.startAt);
        assertEquals(82860, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertNull(timeInput.days);

        clientPair.appClient.send("hardware 1 vw " + b("99 82800  Europe/Kiev "));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 82800  Europe/Kiev "))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(82800, timeInput.startAt);
        assertEquals(-1, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertNull(timeInput.days);

        clientPair.appClient.send("hardware 1 vw " + b("99 82800  Europe/Kiev 1,2,3,4"));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 82800  Europe/Kiev 1,2,3,4"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(82800, timeInput.startAt);
        assertEquals(-1, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertArrayEquals(new int[]{1,2,3,4}, timeInput.days);

        clientPair.appClient.send("hardware 1 vw " + b("99   Europe/Kiev 1,2,3,4"));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99   Europe/Kiev 1,2,3,4"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(-1, timeInput.startAt);
        assertEquals(-1, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertArrayEquals(new int[]{1,2,3,4}, timeInput.days);

        clientPair.appClient.send("hardware 1 vw " + b("99 82800 82800 Europe/Kiev  10800"));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 82800 82800 Europe/Kiev  10800"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(82800, timeInput.startAt);
        assertEquals(82800, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertNull(timeInput.days);

        clientPair.appClient.send("hardware 1 vw " + b("99 ss sr Europe/Kiev  10800"));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 ss sr Europe/Kiev  10800"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        timeInput = (TimeInput) profile.dashBoards[0].findWidgetByPin(0, (byte) 99, PinType.VIRTUAL);
        assertNotNull(timeInput);
        assertEquals(-2, timeInput.startAt);
        assertEquals(-3, timeInput.stopAt);
        assertEquals(ZoneId.of("Europe/Kiev"), timeInput.tzName);
        assertNull(timeInput.days);
    }

    @Test
    public void testEmailWorkWithNoEmailInApp() throws Exception {
        reset(blockingIOProcessor);

        //adding email widget
        clientPair.appClient.send("createWidget 1\0{\"id\":432, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"width\":1, \"height\":1, \"type\":\"EMAIL\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("email subj body");
        verify(mailWrapper, timeout(500)).sendHtml(eq("dima@mail.ua"), eq("subj"), eq("body"));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
    }


    @Test
    public void testWrongCommandForAggregation() throws Exception {
        clientPair.hardwareClient.send("hardware vw 10 aaaa");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 10 aaaa"))));
    }

    @Test
    public void testWrongPin() throws Exception {
        clientPair.hardwareClient.send("hardware vw x aaaa");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND)));
    }

    @Test
    public void testAppSendWAwWorks() throws Exception {
        String body = "aw 8 333";
        clientPair.hardwareClient.send("hardware " + body);

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 aw 8 333"))));
    }

    @Test
    public void testClosedConnectionWhenNotLogged() throws Exception {
        TestAppClient appClient2 = new TestAppClient("localhost", tcpAppPort, properties);
        appClient2.start();
        appClient2.send("getToken 1");
        verify(appClient2.responseMock, after(600).never()).channelRead(any(), any());
        assertTrue(appClient2.isClosed());

        appClient2.send("login dima@mail.ua 1 Android 1RC7");
        verify(appClient2.responseMock, after(200).never()).channelRead(any(), any());
    }

    @Test
    public void testSendPinModeCommandWhenHardwareGoesOnline() throws Exception {
        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.await();

        assertTrue(channelFuture.isDone());

        String body = "vw 13 1";
        clientPair.appClient.send("hardware 1 " + body);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, DEVICE_NOT_IN_NETWORK)));

        TestHardClient hardClient = new TestHardClient("localhost", tcpHardPort);
        hardClient.start();
        hardClient.send("login " + clientPair.token);
        verify(hardClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in"))));
        verify(hardClient.responseMock, times(2)).channelRead(any(), any());
        hardClient.stop().awaitUninterruptibly();
    }

    @Test
    public void testSendGeneratedPinModeCommandWhenHardwareGoesOnline() throws Exception {
        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.awaitUninterruptibly();

        assertTrue(channelFuture.isDone());

        clientPair.appClient.send("hardware 1 vw 1 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, DEVICE_NOT_IN_NETWORK)));

        TestHardClient hardClient = new TestHardClient("localhost", tcpHardPort);
        hardClient.start();
        hardClient.send("login " + clientPair.token);
        verify(hardClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        String expectedBody = "pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in";
        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b(expectedBody))));
        verify(hardClient.responseMock, times(2)).channelRead(any(), any());
        hardClient.stop().awaitUninterruptibly();
    }

    @Test
    public void testSendHardwareCommandToNotActiveDashboard() throws Exception {
        clientPair.appClient.send("createDash " + "{\"id\":2,\"name\":\"My Dashboard2\"}");
        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        clientPair.appClient.reset();

        clientPair.appClient.send("getToken 2");

        //getting token for second GetTokenMessage
        ArgumentCaptor<GetTokenMessage> objectArgumentCaptor = ArgumentCaptor.forClass(GetTokenMessage.class);
        verify(clientPair.appClient.responseMock, timeout(2000).times(1)).channelRead(any(), objectArgumentCaptor.capture());
        List<GetTokenMessage> arguments = objectArgumentCaptor.getAllValues();
        GetTokenMessage getTokenMessage = arguments.get(0);
        String token = getTokenMessage.body;

        clientPair.appClient.reset();

        //connecting separate hardware to non active dashboard
        TestHardClient nonActiveDashHardClient = new TestHardClient("localhost", tcpHardPort);
        nonActiveDashHardClient.start();
        nonActiveDashHardClient.send("login " + token);
        verify(nonActiveDashHardClient.responseMock, timeout(2000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        nonActiveDashHardClient.reset();


        //sending hardware command from hardware that has no active dashboard
        nonActiveDashHardClient.send("hardware aw 1 1");
        //verify(nonActiveDashHardClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, NO_ACTIVE_DASHBOARD)));
        verify(clientPair.appClient.responseMock, timeout(1000).times(1)).channelRead(any(), any());
        verify(clientPair.appClient.responseMock, timeout(1000).times(1)).channelRead(any(), eq(new HardwareConnectedMessage(1, "2-0")));

        clientPair.hardwareClient.send("hardware aw 1 1");
        verify(clientPair.hardwareClient.responseMock, after(1000).never()).channelRead(any(), any());
        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), eq(produce(1, HARDWARE, b("1 aw 1 1"))));
        nonActiveDashHardClient.stop().awaitUninterruptibly();
    }

    @Test
    public void testConnectAppAndHardwareAndSendCommands() throws Exception {
        for (int i = 0; i < 100; i++) {
            clientPair.appClient.send("hardware 1 aw 1 1");
        }

        verify(clientPair.hardwareClient.responseMock, timeout(500).times(100)).channelRead(any(), any());
    }

    @Test
    public void testTryReachQuotaLimit() throws Exception {
        String body = "aw 100 100";

        //within 1 second sending more messages than default limit 100.
        for (int i = 0; i < 200; i++) {
            clientPair.hardwareClient.send("hardware " + body);
            sleep(5);
        }

        ArgumentCaptor<ResponseMessage> objectArgumentCaptor = ArgumentCaptor.forClass(ResponseMessage.class);
        verify(clientPair.hardwareClient.responseMock, timeout(1000)).channelRead(any(), objectArgumentCaptor.capture());
        List<ResponseMessage> arguments = objectArgumentCaptor.getAllValues();
        ResponseMessage responseMessage = arguments.get(0);
        assertTrue(responseMessage.id > 100);

        //at least 100 iterations should be
        for (int i = 0; i < 100; i++) {
            verify(clientPair.appClient.responseMock).channelRead(any(), eq(produce(i+1, HARDWARE, b("1 " + body))));
        }

        clientPair.appClient.reset();
        clientPair.hardwareClient.reset();

        //check no more accepted
        for (int i = 0; i < 10; i++) {
            clientPair.hardwareClient.send("hardware " + body);
            sleep(9);
        }

        verify(clientPair.hardwareClient.responseMock, never()).channelRead(any(), eq(new ResponseMessage(1, QUOTA_LIMIT)));
        verify(clientPair.appClient.responseMock, never()).channelRead(any(), eq(produce(1, HARDWARE, b(body))));
    }

    @Test
    public void testCreateProjectWithDevicesGeneratesNewTokens() throws Exception {
        DashBoard dashBoard = new DashBoard();
        dashBoard.id = 2;
        dashBoard.name = "Test Dash";

        Device device = new Device();
        device.id = 1;
        device.name = "MyDevice";
        device.token = "aaa";
        dashBoard.devices = new Device[] {
                device
        };

        clientPair.appClient.send("createDash " + dashBoard.toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("getDevices 2");
        String response = clientPair.appClient.getBody(2);

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(1, devices.length);
        assertEquals(1, devices[0].id);
        assertEquals("MyDevice", devices[0].name);
        assertNotEquals("aaa", devices[0].token);
    }

    @Test
    @Ignore("hard to test this case...")
    public void testTryReachQuotaLimitAndWarningExceededLimit() throws Exception {
        String body = "1 ar 100 100";

        //within 1 second sending more messages than default limit 100.
        for (int i = 0; i < 1000 / 9; i++) {
            clientPair.appClient.send("hardware " + body, 1);
            sleep(9);
        }

        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, QUOTA_LIMIT)));
        verify(clientPair.hardwareClient.responseMock, atLeast(100)).channelRead(any(), eq(produce(1, HARDWARE, b(body))));

        clientPair.appClient.reset();
        clientPair.hardwareClient.reset();

        //waiting to avoid limit.
        sleep(1000);

        for (int i = 0; i < 100000 / 9; i++) {
            clientPair.appClient.send("hardware " + body, 1);
            sleep(9);
        }

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, QUOTA_LIMIT)));
        verify(clientPair.hardwareClient.responseMock, atLeast(100)).channelRead(any(), eq(produce(1, HARDWARE, b(body))));

    }

}
