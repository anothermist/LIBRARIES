package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateDevice;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.utils.JsonParser;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import static cc.blynk.server.core.model.widgets.FrequencyWidget.READING_MSG_ID;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND_BODY;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.junit.Assert.assertNotNull;
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
public class ReadingWorkflowTest extends IntegrationBase {

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
    public void testNoReadingWidgetOnPin() throws Exception {
        clientPair.appClient.send("hardware 1 ar 111");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, ILLEGAL_COMMAND_BODY)));
    }

    @Test
    public void testReadingCommandNotAcceptedAnymoreFromApp() throws Exception {
        clientPair.appClient.send("hardware 1 ar 7");
        verify(clientPair.hardwareClient.responseMock, after(500).never()).channelRead(any(), any());
    }

    @Test
    public void testServerSendReadingCommandWithReadingWorkerEnabled() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 1000, TimeUnit.MILLISECONDS);
        verify(clientPair.hardwareClient.responseMock, timeout(1000)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("ar 7"))));
        verify(clientPair.hardwareClient.responseMock, timeout(1000)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("ar 30"))));
    }

    @Test
    public void testServerSendReadingCommandCorrectly() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":155, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":100}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 500, TimeUnit.MILLISECONDS);

        verify(clientPair.hardwareClient.responseMock, after(600).times(2)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
    }

    @Test
    public void testServerDontSendReadingCommandsForNonActiveDash() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":155, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":100}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 500, TimeUnit.MILLISECONDS);

        verify(clientPair.hardwareClient.responseMock, after(1000).never()).channelRead(any(), any());
    }

    @Test
    public void testSendReadCommandsForLCD() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"type\":\"LCD\",\"id\":1923810267,\"x\":0,\"y\":6,\"color\":600084223,\"width\":8,\"height\":2,\"tabId\":0,\"" +
                "pins\":[" +
                "{\"pin\":100,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023}," +
                "{\"pin\":101,\"pinType\":\"VIRTUAL\",\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":1023}]," +
                "\"advancedMode\":false,\"textLight\":false,\"textLightOn\":false,\"frequency\":900}");

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 1000, TimeUnit.MILLISECONDS);

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));

        clientPair.hardwareClient.reset();

        verify(clientPair.hardwareClient.responseMock, timeout(1000)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(clientPair.hardwareClient.responseMock, timeout(1000)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));
    }

    @Test
    public void testSendReadForMultipleDevices() throws Exception {
        Device device2 = new Device(2, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device2.toString());
        String createdDevice = clientPair.appClient.getBody();

        device2 = JsonParser.parseDevice(createdDevice);
        assertNotNull(device2);
        assertNotNull(device2.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device2.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device2.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        clientPair.appClient.reset();


        clientPair.appClient.send("createWidget 1\0{\"id\":155, \"deviceId\":0, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":100}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("createWidget 1\0{\"id\":156, \"deviceId\":2, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":101}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 500, TimeUnit.MILLISECONDS);

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));
    }

    @Test
    public void testSendReadForDeviceSelector() throws Exception {
        Device device2 = new Device(2, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device2.toString());
        String createdDevice = clientPair.appClient.getBody();

        device2 = JsonParser.parseDevice(createdDevice);
        assertNotNull(device2);
        assertNotNull(device2.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device2.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device2.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        clientPair.appClient.reset();

        clientPair.appClient.send("createWidget 1\0{\"id\":200000, \"width\":1, \"value\":2, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"DEVICE_SELECTOR\"}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("createWidget 1\0{\"id\":155, \"deviceId\":200000, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":100}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 500, TimeUnit.MILLISECONDS);

        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(clientPair.hardwareClient.responseMock, after(100).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));

        clientPair.hardwareClient.reset();
        hardClient2.reset();

        clientPair.appClient.send("hardware 1 vu 200000 0");

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(hardClient2.responseMock, after(100).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
    }

    @Test
    public void testSendReadForMultipleDevices2() throws Exception {
        Device device2 = new Device(2, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device2.toString());
        String createdDevice = clientPair.appClient.getBody();

        device2 = JsonParser.parseDevice(createdDevice);
        assertNotNull(device2);
        assertNotNull(device2.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device2.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device2.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        clientPair.appClient.reset();


        clientPair.appClient.send("createWidget 1\0{\"id\":155, \"deviceId\":0, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":100}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));
        clientPair.appClient.send("createWidget 1\0{\"id\":156, \"deviceId\":0, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":101}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));


        clientPair.appClient.send("createWidget 1\0{\"id\":157, \"deviceId\":2, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":102}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(3)));
        clientPair.appClient.send("createWidget 1\0{\"id\":158, \"deviceId\":2, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":103}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(4)));

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 500, TimeUnit.MILLISECONDS);

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 102"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 103"))));

        hardClient2.reset();
        clientPair.hardwareClient.reset();

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 102"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 103"))));

        clientPair.appClient.send("deactivate 1");

        hardClient2.reset();
        clientPair.hardwareClient.reset();

        verify(clientPair.hardwareClient.responseMock, after(500).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(clientPair.hardwareClient.responseMock, after(500).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));
        verify(hardClient2.responseMock, after(500).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 102"))));
        verify(hardClient2.responseMock, after(500).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 103"))));
    }

    @Test
    public void testSendReadOnlyForOnlineApp() throws Exception {
        Device device2 = new Device(2, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device2.toString());
        String createdDevice = clientPair.appClient.getBody();

        device2 = JsonParser.parseDevice(createdDevice);
        assertNotNull(device2);
        assertNotNull(device2.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device2.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device2.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        clientPair.appClient.reset();


        clientPair.appClient.send("createWidget 1\0{\"id\":155, \"deviceId\":0, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":100}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("createWidget 1\0{\"id\":156, \"deviceId\":2, \"frequency\":400, \"width\":1, \"height\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"GAUGE\", \"pinType\":\"VIRTUAL\", \"pin\":101}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        clientPair.appClient.stop().await();
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.readingWidgetsWorker, 0, 500, TimeUnit.MILLISECONDS);

        verify(clientPair.hardwareClient.responseMock, after(500).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 100"))));
        verify(hardClient2.responseMock, after(500).never()).channelRead(any(), eq(produce(READING_MSG_ID, HARDWARE, b("vr 101"))));
    }

}