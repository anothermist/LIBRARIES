package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Button;
import cc.blynk.server.core.model.widgets.others.Player;
import cc.blynk.server.core.model.widgets.ui.Menu;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.SetWidgetPropertyMessage;
import cc.blynk.server.hardware.HardwareServer;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND_BODY;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.after;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/2/2015.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class SetPropertyTest extends IntegrationBase {

    private BaseServer appServer;
    private BaseServer hardwareServer;
    private ClientPair clientPair;


    @Before
    public void init() throws Exception {
        this.hardwareServer = new HardwareServer(holder).start();
        this.appServer = new AppServer(holder).start();
        this.clientPair = initAppAndHardPair("user_profile_json.txt");
    }

    @After
    public void shutdown() {
        this.appServer.close();
        this.hardwareServer.close();
        this.clientPair.stop();
    }

    @Test
    public void testSetWidgetProperty() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());

        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte)4, PinType.VIRTUAL);
        assertNotNull(widget);
        assertEquals("Some Text", widget.label);

        clientPair.hardwareClient.send("setProperty 4 label MyNewLabel");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(1, b("1 4 label MyNewLabel"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());

        widget = profile.dashBoards[0].findWidgetByPin(0, (byte)4, PinType.VIRTUAL);
        assertNotNull(widget);
        assertEquals("MyNewLabel", widget.label);
    }

    @Test
    public void testSetButtonProperty() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":0, \"onLabel\":\"On\", \"offLabel\":\"Off\" , \"type\":\"BUTTON\", \"pinType\":\"VIRTUAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("setProperty 17 onLabel вкл");
        clientPair.hardwareClient.send("setProperty 17 offLabel выкл");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(1, b("1 17 onLabel вкл"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(2, b("1 17 offLabel выкл"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());

        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte) 17, PinType.VIRTUAL);
        assertNotNull(widget);
        assertTrue(widget instanceof Button);
        Button button = (Button) widget;

        assertEquals("вкл", button.onLabel);
        assertEquals("выкл", button.offLabel);
    }


    @Test
    public void testSetBooleanProperty() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":0, \"label\":\"Some Text\", \"type\":\"PLAYER\", \"pinType\":\"VIRTUAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("setProperty 17 isOnPlay true");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(1, b("1 17 isOnPlay true"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());

        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte) 17, PinType.VIRTUAL);
        assertNotNull(widget);
        assertTrue(widget instanceof Player);
        Player playerWidget = (Player) widget;

        assertTrue(playerWidget.isOnPlay);
    }

    @Test
    public void testSetStringArrayWidgetPropertyForMenu() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":0, \"label\":\"Some Text\", \"type\":\"MENU\", \"pinType\":\"VIRTUAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.hardwareClient.send("setProperty 17 labels label1 label2 label3");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(1, b("1 17 labels label1 label2 label3"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());

        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte) 17, PinType.VIRTUAL);
        assertNotNull(widget);
        assertTrue(widget instanceof Menu);
        Menu menuWidget = (Menu) widget;

        assertArrayEquals(new String[] {"label1", "label2", "label3"}, menuWidget.labels);


    }

    @Test
    public void testSetWrongWidgetProperty() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;

        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"My Dashboard\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"pushMode\":false},{\"type\":\"SLIDER\",\"id\":2,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":2,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":3,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":3,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"0\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":4,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":4,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"244\",\"sendOnReleaseOn\":false},{\"type\":\"TIMER\",\"id\":5,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":5,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"dw\\u00005\\u00001\",\"startTime\":0,\"stopTime\":-1},{\"type\":\"LED\",\"id\":6,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":6,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":7,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":5000},{\"type\":\"DIGIT4_DISPLAY\",\"id\":30,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":30,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":100},{\"type\":\"GRAPH\",\"id\":8,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":8,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"frequency\":0,\"isBar\":false},{\"type\":\"NOTIFICATION\",\"id\":9,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"androidTokens\":{\"uid\":\"token\"},\"notifyWhenOffline\":true,\"notifyWhenOfflineIgnorePeriod\":0,\"priority\":\"normal\"},{\"type\":\"TWITTER\",\"id\":10,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"token\":\"token\",\"secret\":\"secret\"},{\"type\":\"RTC\",\"id\":11,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":9,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"LCD\",\"id\":12,\"x\":0,\"y\":0,\"color\":-1,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":0,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"89.888037459418\",\"min\":-100,\"max\":100},{\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"-58.74774244674501\",\"min\":-100,\"max\":100}],\"advancedMode\":false,\"textFormatLine1\":\"pin1 : /pin0/\",\"textFormatLine2\":\"pin2 : /pin1/\",\"textLight\":false,\"frequency\":1000},{\"type\":\"RGB\",\"id\":13,\"x\":2,\"y\":3,\"color\":616861439,\"width\":4,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255}],\"splitMode\":false,\"sendOnReleaseOn\":true},{\"type\":\"LOGGER\",\"id\":14,\"x\":0,\"y\":6,\"color\":0,\"width\":8,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"ANALOG\",\"min\":0,\"max\":255},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0}],\"period\":\"THREE_MONTHS\",\"max\":0,\"min\":0,\"autoYCoords\":false,\"showLegends\":true},{\"type\":\"LCD\",\"id\":15,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023},{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023}],\"advancedMode\":true,\"textLight\":false,\"frequency\":0}],\"boardType\":\"UNO\",\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":true,\"isActive\":true}]}", profile.toString());

        clientPair.hardwareClient.send("setProperty 4 YYY MyNewLabel");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND_BODY)));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;

        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"My Dashboard\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"pushMode\":false},{\"type\":\"SLIDER\",\"id\":2,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":2,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":3,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":3,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"0\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":4,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":4,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"244\",\"sendOnReleaseOn\":false},{\"type\":\"TIMER\",\"id\":5,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":5,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"dw\\u00005\\u00001\",\"startTime\":0,\"stopTime\":-1},{\"type\":\"LED\",\"id\":6,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":6,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":7,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":5000},{\"type\":\"DIGIT4_DISPLAY\",\"id\":30,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":30,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":100},{\"type\":\"GRAPH\",\"id\":8,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":8,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"frequency\":0,\"isBar\":false},{\"type\":\"NOTIFICATION\",\"id\":9,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"androidTokens\":{\"uid\":\"token\"},\"notifyWhenOffline\":true,\"notifyWhenOfflineIgnorePeriod\":0,\"priority\":\"normal\"},{\"type\":\"TWITTER\",\"id\":10,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"token\":\"token\",\"secret\":\"secret\"},{\"type\":\"RTC\",\"id\":11,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":9,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"LCD\",\"id\":12,\"x\":0,\"y\":0,\"color\":-1,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":0,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"89.888037459418\",\"min\":-100,\"max\":100},{\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"-58.74774244674501\",\"min\":-100,\"max\":100}],\"advancedMode\":false,\"textFormatLine1\":\"pin1 : /pin0/\",\"textFormatLine2\":\"pin2 : /pin1/\",\"textLight\":false,\"frequency\":1000},{\"type\":\"RGB\",\"id\":13,\"x\":2,\"y\":3,\"color\":616861439,\"width\":4,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255}],\"splitMode\":false,\"sendOnReleaseOn\":true},{\"type\":\"LOGGER\",\"id\":14,\"x\":0,\"y\":6,\"color\":0,\"width\":8,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"ANALOG\",\"min\":0,\"max\":255},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0}],\"period\":\"THREE_MONTHS\",\"max\":0,\"min\":0,\"autoYCoords\":false,\"showLegends\":true},{\"type\":\"LCD\",\"id\":15,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023},{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023}],\"advancedMode\":true,\"textLight\":false,\"frequency\":0}],\"boardType\":\"UNO\",\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":true,\"isActive\":true}]}", profile.toString());
    }

    @Test
    public void testSetWrongWidgetProperty2() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;

        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"My Dashboard\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"pushMode\":false},{\"type\":\"SLIDER\",\"id\":2,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":2,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":3,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":3,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"0\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":4,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":4,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"244\",\"sendOnReleaseOn\":false},{\"type\":\"TIMER\",\"id\":5,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":5,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"dw\\u00005\\u00001\",\"startTime\":0,\"stopTime\":-1},{\"type\":\"LED\",\"id\":6,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":6,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":7,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":5000},{\"type\":\"DIGIT4_DISPLAY\",\"id\":30,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":30,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":100},{\"type\":\"GRAPH\",\"id\":8,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":8,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"frequency\":0,\"isBar\":false},{\"type\":\"NOTIFICATION\",\"id\":9,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"androidTokens\":{\"uid\":\"token\"},\"notifyWhenOffline\":true,\"notifyWhenOfflineIgnorePeriod\":0,\"priority\":\"normal\"},{\"type\":\"TWITTER\",\"id\":10,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"token\":\"token\",\"secret\":\"secret\"},{\"type\":\"RTC\",\"id\":11,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":9,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"LCD\",\"id\":12,\"x\":0,\"y\":0,\"color\":-1,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":0,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"89.888037459418\",\"min\":-100,\"max\":100},{\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"-58.74774244674501\",\"min\":-100,\"max\":100}],\"advancedMode\":false,\"textFormatLine1\":\"pin1 : /pin0/\",\"textFormatLine2\":\"pin2 : /pin1/\",\"textLight\":false,\"frequency\":1000},{\"type\":\"RGB\",\"id\":13,\"x\":2,\"y\":3,\"color\":616861439,\"width\":4,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255}],\"splitMode\":false,\"sendOnReleaseOn\":true},{\"type\":\"LOGGER\",\"id\":14,\"x\":0,\"y\":6,\"color\":0,\"width\":8,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"ANALOG\",\"min\":0,\"max\":255},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0}],\"period\":\"THREE_MONTHS\",\"max\":0,\"min\":0,\"autoYCoords\":false,\"showLegends\":true},{\"type\":\"LCD\",\"id\":15,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023},{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023}],\"advancedMode\":true,\"textLight\":false,\"frequency\":0}],\"boardType\":\"UNO\",\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":true,\"isActive\":true}]}", profile.toString());

        clientPair.hardwareClient.send("setProperty 4 x MyNewLabel");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND_BODY)));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;
        //todo fix
        profile.dashBoards[0].devices = null;

        assertEquals("{\"dashBoards\":[{\"id\":1,\"name\":\"My Dashboard\",\"createdAt\":1,\"updatedAt\":0,\"widgets\":[{\"type\":\"BUTTON\",\"id\":1,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"pushMode\":false},{\"type\":\"SLIDER\",\"id\":2,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":2,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"1\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":3,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":3,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"0\",\"sendOnReleaseOn\":false},{\"type\":\"SLIDER\",\"id\":4,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":4,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"244\",\"sendOnReleaseOn\":false},{\"type\":\"TIMER\",\"id\":5,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"DIGITAL\",\"pin\":5,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"dw\\u00005\\u00001\",\"startTime\":0,\"stopTime\":-1},{\"type\":\"LED\",\"id\":6,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":6,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":7,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":5000},{\"type\":\"DIGIT4_DISPLAY\",\"id\":30,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":30,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"value\":\"3\",\"frequency\":100},{\"type\":\"GRAPH\",\"id\":8,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"label\":\"Some Text\",\"deviceId\":0,\"pinType\":\"ANALOG\",\"pin\":8,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"frequency\":0,\"isBar\":false},{\"type\":\"NOTIFICATION\",\"id\":9,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"androidTokens\":{\"uid\":\"token\"},\"notifyWhenOffline\":true,\"notifyWhenOfflineIgnorePeriod\":0,\"priority\":\"normal\"},{\"type\":\"TWITTER\",\"id\":10,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"token\":\"token\",\"secret\":\"secret\"},{\"type\":\"RTC\",\"id\":11,\"x\":1,\"y\":1,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pinType\":\"VIRTUAL\",\"pin\":9,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"type\":\"LCD\",\"id\":12,\"x\":0,\"y\":0,\"color\":-1,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":0,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"89.888037459418\",\"min\":-100,\"max\":100},{\"pin\":1,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"-58.74774244674501\",\"min\":-100,\"max\":100}],\"advancedMode\":false,\"textFormatLine1\":\"pin1 : /pin0/\",\"textFormatLine2\":\"pin2 : /pin1/\",\"textLight\":false,\"frequency\":1000},{\"type\":\"RGB\",\"id\":13,\"x\":2,\"y\":3,\"color\":616861439,\"width\":4,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255},{\"pin\":13,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"value\":\"60\\u0000143\\u0000158\",\"min\":0,\"max\":255}],\"splitMode\":false,\"sendOnReleaseOn\":true},{\"type\":\"LOGGER\",\"id\":14,\"x\":0,\"y\":6,\"color\":0,\"width\":8,\"height\":3,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":7,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"ANALOG\",\"min\":0,\"max\":255},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0},{\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0}],\"period\":\"THREE_MONTHS\",\"max\":0,\"min\":0,\"autoYCoords\":false,\"showLegends\":true},{\"type\":\"LCD\",\"id\":15,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"deviceId\":0,\"pins\":[{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023},{\"pin\":20,\"pwmMode\":false,\"rangeMappingOn\":false,\"pinType\":\"VIRTUAL\",\"min\":0,\"max\":1023}],\"advancedMode\":true,\"textLight\":false,\"frequency\":0}],\"boardType\":\"UNO\",\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":true,\"isActive\":true}]}", profile.toString());
    }

    @Test
    public void testSetColorForWidget() throws Exception {
        clientPair.hardwareClient.send("setProperty 4 color #23C48E");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new SetWidgetPropertyMessage(1, b("1 4 color #23C48E"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        profile.dashBoards[0].updatedAt = 0;

        Widget widget = profile.dashBoards[0].findWidgetByPin(0, (byte) 4, PinType.VIRTUAL);
        assertEquals(600084223, widget.color);
    }

    @Test
    public void testSetColorShouldNotWorkForNonActiveProject() throws Exception {
        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("setProperty 4 color #23C48E");
        verify(clientPair.hardwareClient.responseMock, after(500).never()).channelRead(any(), eq(ok(1)));
        verify(clientPair.appClient.responseMock, after(500).never()).channelRead(any(), eq(new SetWidgetPropertyMessage(1, b("1 4 color #23C48E"))));
    }

}
