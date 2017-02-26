package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.model.device.Status;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateDevice;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.AppSyncMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.utils.JsonParser;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import static cc.blynk.server.core.protocol.enums.Command.GET_ENERGY;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
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
public class AppSyncWorkflowTest extends IntegrationBase {

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
    public void testLCDOnActivateSendsCorrectBodySimpleMode() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"type\":\"LCD\",\"id\":1923810267,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"" +
                "pins\":[" +
                "{\"pin\":10,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023, \"value\":\"10\"}," +
                "{\"pin\":11,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023, \"value\":\"11\"}]," +
                "\"advancedMode\":false,\"textLight\":false,\"textLightOn\":false,\"frequency\":1000}");

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.reset();
        clientPair.appClient.send("appsync 1");

        verify(clientPair.appClient.responseMock, timeout(500).times(13)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 10 10"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 11 11"))));


        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
    }

    @Test
    public void testLCDOnActivateSendsCorrectBodyAdvancedMode() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"type\":\"LCD\",\"id\":1923810267,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"" +
                "pins\":[" +
                "{\"pin\":10,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023}," +
                "{\"pin\":11,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023}]," +
                "\"advancedMode\":true,\"textLight\":false,\"textLightOn\":false,\"frequency\":1000}");

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 10 p x y 10");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(1, b("1 vw 10 p x y 10"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("appsync 1");

        verify(clientPair.appClient.responseMock, timeout(500).times(12)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(1111, b("1 vw 10 p x y 10"))));


        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
    }


    @Test
    public void testTerminalSendsSyncOnActivate() throws Exception {
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        assertEquals(16, profile.dashBoards[0].widgets.length);

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "7500")));

        clientPair.appClient.send("createWidget 1\0{\"id\":102, \"width\":1, \"height\":1, \"x\":5, \"y\":0, \"tabId\":0, \"label\":\"Some Text\", \"type\":\"TERMINAL\", \"pinType\":\"VIRTUAL\", \"pin\":17}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(3)));

        clientPair.hardwareClient.send("hardware vw 17 a");
        clientPair.hardwareClient.send("hardware vw 17 b");
        clientPair.hardwareClient.send("hardware vw 17 c");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 17 a"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 17 b"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, HARDWARE, b("1 vw 17 c"))));

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(4)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(5)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 17 a"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 17 b"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 17 c"))));
    }

    @Test
    public void testLCDSendsSyncOnActivate() throws Exception {
        clientPair.hardwareClient.send("hardware vw 20 p 0 0 Hello");
        clientPair.hardwareClient.send("hardware vw 20 p 0 1 World");

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 20 p 0 0 Hello"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 20 p 0 1 World"))));

        clientPair.appClient.send("appsync 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 0 Hello"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 1 World"))));
    }

    @Test
    public void testLCDSendsSyncOnActivate2() throws Exception {
        clientPair.hardwareClient.send("hardware vw 20 p 0 0 H1");
        clientPair.hardwareClient.send("hardware vw 20 p 0 1 H2");
        clientPair.hardwareClient.send("hardware vw 20 p 0 2 H3");
        clientPair.hardwareClient.send("hardware vw 20 p 0 3 H4");
        clientPair.hardwareClient.send("hardware vw 20 p 0 4 H5");
        clientPair.hardwareClient.send("hardware vw 20 p 0 5 H6");
        clientPair.hardwareClient.send("hardware vw 20 p 0 6 H7");

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 20 p 0 0 H1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 20 p 0 1 H2"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, HARDWARE, b("1 vw 20 p 0 2 H3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(4, HARDWARE, b("1 vw 20 p 0 3 H4"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(5, HARDWARE, b("1 vw 20 p 0 4 H5"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(6, HARDWARE, b("1 vw 20 p 0 5 H6"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(7, HARDWARE, b("1 vw 20 p 0 6 H7"))));

        clientPair.appClient.send("appsync 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 1 H2"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 2 H3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 3 H4"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 4 H5"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 5 H6"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 20 p 0 6 H7"))));
    }

    @Test
    public void testActivateAndGetSync() throws Exception {
        clientPair.appClient.send("appsync 1");

        verify(clientPair.appClient.responseMock, timeout(500).times(11)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
    }

    @Test
    //https://github.com/blynkkk/blynk-server/issues/443
    public void testSyncWidgetValueOverlapsWithPinStorage() throws Exception {
        clientPair.hardwareClient.send("hardware vw 125 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(1, b("1 vw 125 1"))));
        clientPair.appClient.reset();

        clientPair.appClient.send("appsync 1");

        verify(clientPair.appClient.responseMock, timeout(500).times(12)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 125 1"))));


        clientPair.appClient.send("createWidget 1\0{\"id\":88, \"width\":1, \"height\":1, \"deviceId\":0, \"x\":0, \"y\":0, \"label\":\"Button\", \"type\":\"BUTTON\", \"pinType\":\"VIRTUAL\", \"pin\":125}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));
        clientPair.appClient.reset();

        clientPair.hardwareClient.send("hardware vw 125 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(2, b("1 vw 125 2"))));
        clientPair.appClient.reset();

        clientPair.appClient.send("appsync 1");

        verify(clientPair.appClient.responseMock, timeout(500).times(12)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 125 2"))));
    }

    @Test
    public void testActivateAndGetSyncForSpecificDeviceId() throws Exception {
        clientPair.appClient.send("appsync 1-0");

        verify(clientPair.appClient.responseMock, timeout(500).times(11)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
    }

    @Test
    public void testActivateAndGetSyncForTimeInput() throws Exception {
        clientPair.appClient.send(("createWidget 1\0{\"type\":\"TIME_INPUT\",\"id\":99, \"pin\":99, \"pinType\":\"VIRTUAL\", " +
                "\"x\":0,\"y\":0,\"width\":1,\"height\":1}"));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("hardware 1 vw " + b("99 82800 82860 Europe/Kiev 1"));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(1)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 99 82800 82860 Europe/Kiev 1"))));

        clientPair.appClient.reset();

        clientPair.appClient.send("appsync 1-0");

        verify(clientPair.appClient.responseMock, timeout(500).times(12)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 2 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 3 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 dw 5 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 4 244"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 7 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 aw 30 3"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 0 89.888037459418"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 1 -58.74774244674501"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 13 60 143 158"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1 vw 99 82800 82860 Europe/Kiev 1"))));
    }

    @Test
    public void testActivateAndGetSyncForNonExistingDeviceId() throws Exception {
        clientPair.appClient.send("appsync 1-1");

        verify(clientPair.appClient.responseMock, timeout(500).times(1)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));
    }

    @Test
    public void testLCDOnActivateSendsCorrectBodySimpleModeAndAnotherDevice() throws Exception {
        Device device1 = new Device(1, "My Device", "ESP8266");
        device1.status = Status.OFFLINE;

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        clientPair.appClient.send("createWidget 1\0{\"deviceId\":1,\"type\":\"LCD\",\"id\":1923810267,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"" +
                "pins\":[" +
                "{\"pin\":10,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023, \"value\":\"10\"}," +
                "{\"pin\":11,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023, \"value\":\"11\"}]," +
                "\"advancedMode\":false,\"textLight\":false,\"textLightOn\":false,\"frequency\":1000}");

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        clientPair.appClient.reset();
        clientPair.appClient.send("appsync 1-1");

        verify(clientPair.appClient.responseMock, timeout(500).times(3)).channelRead(any(), any());

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1-1 vw 10 10"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1-1 vw 11 11"))));
    }

}
