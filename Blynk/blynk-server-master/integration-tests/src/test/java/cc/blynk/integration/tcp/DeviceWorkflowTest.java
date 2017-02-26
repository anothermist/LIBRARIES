package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.model.device.Status;
import cc.blynk.server.core.model.device.Tag;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateDevice;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateTag;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.AppSyncMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareConnectedMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.server.notifications.push.android.AndroidGCMMessage;
import cc.blynk.server.notifications.push.enums.Priority;
import cc.blynk.utils.JsonParser;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.runners.MockitoJUnitRunner;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.DEVICE_NOT_IN_NETWORK;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
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
public class DeviceWorkflowTest extends IntegrationBase {

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
    public void testDeleteGraphCommandWorks() throws Exception {
        clientPair.appClient.send("getgraphdata 1-0 d 8 del");

        verify(clientPair.appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
    }

    @Test
    public void testSendHardwareCommandToMultipleDevices() throws Exception {
        Device device0 = new Device(0, "My Dashboard", "UNO");
        device0.status = Status.ONLINE;
        Device device1 = new Device(1, "My Device", "ESP8266");
        device1.status = Status.OFFLINE;

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        clientPair.appClient.reset();

        clientPair.appClient.send("getDevices 1");
        String response = clientPair.appClient.getBody();

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(2, devices.length);

        assertEqualDevice(device0, devices[0]);
        assertEqualDevice(device1, devices[1]);

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + devices[1].token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        device1.status = Status.ONLINE;

        clientPair.appClient.send("hardware 1 vw 100 100");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(2, b("vw 100 100"))));
        verify(hardClient2.responseMock, never()).channelRead(any(), eq(new HardwareMessage(2, b("vw 1 100"))));

        clientPair.appClient.send("hardware 1-0 vw 100 101");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(3, b("vw 100 101"))));
        verify(hardClient2.responseMock, never()).channelRead(any(), eq(new HardwareMessage(3, b("vw 1 101"))));

        clientPair.appClient.send("hardware 1-1 vw 100 102");
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(4, b("vw 100 102"))));
        verify(clientPair.hardwareClient.responseMock, never()).channelRead(any(), eq(new HardwareMessage(4, b("vw 100 102"))));
    }

    @Test
    public void testDeviceWentOfflineMessage() throws Exception {
        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        hardClient2.stop().await();

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Your My Device went offline. \"My Dashboard\" project is disconnected.", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testSendHardwareCommandToAppFromMultipleDevices() throws Exception {
        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        clientPair.appClient.reset();

        clientPair.appClient.send("getDevices 1");
        String response = clientPair.appClient.getBody();

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + devices[1].token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareConnectedMessage(1, "1-1")));

        clientPair.hardwareClient.send("hardware vw 100 101");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(1, b("1 vw 100 101"))));

        hardClient2.send("hardware vw 100 100");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(2, b("1-1 vw 100 100"))));
    }

    @Test
    public void testSendDeviceSpecificPMMessage() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":188, \"width\":1, \"height\":1, \"deviceId\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":1}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody(2);
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(2, device.toString())));

        TestHardClient hardClient = new TestHardClient("localhost", tcpHardPort);
        hardClient.start();

        hardClient.send("login " + device.token);
        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareConnectedMessage(1, "1-1")));

        String expectedBody = "pm 1 out";
        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b(expectedBody))));
        verify(hardClient.responseMock, times(2)).channelRead(any(), any());
        hardClient.stop().awaitUninterruptibly();
    }

    @Test
    public void testSendPMOnActivateForMultiDevices() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":188, \"width\":1, \"height\":1, \"deviceId\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":33}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody(2);
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(2, device.toString())));

        TestHardClient hardClient = new TestHardClient("localhost", tcpHardPort);
        hardClient.start();

        hardClient.send("login " + device.token);
        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareConnectedMessage(1, "1-1")));

        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 33 out"))));
        verify(hardClient.responseMock, times(2)).channelRead(any(), any());

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        hardClient.reset();
        clientPair.hardwareClient.reset();

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 33 out"))));
        verify(hardClient.responseMock, times(1)).channelRead(any(), any());

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in"))));
        verify(clientPair.hardwareClient.responseMock, times(1)).channelRead(any(), any());


        hardClient.stop().awaitUninterruptibly();
    }

    @Test
    public void testActivateForMultiDevices() throws Exception {
        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, DEVICE_NOT_IN_NETWORK)));
    }

    @Test
    public void testTagWorks() throws Exception {
        Tag tag = new Tag(100_000, "My New Tag");
        tag.deviceIds = new int[] {1};

        clientPair.appClient.send("createTag 1\0" + tag.toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateTag(1, tag.toString())));

        clientPair.appClient.send("createWidget 1\0{\"id\":188, \"width\":1, \"height\":1, \"deviceId\":100000, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":33, \"value\":1}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody(3);
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(3, device.toString())));


        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.reset();

        clientPair.appClient.send("hardware 1-100000 dw 33 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(new HardwareMessage(3, b("dw 33 10"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(1, b("dw 33 1"))));

        tag.deviceIds = new int[] {0, 1};

        clientPair.appClient.send("updateTag 1\0" + tag.toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        clientPair.appClient.send("hardware 1-100000 dw 33 10");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(3, b("dw 33 10"))));
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new HardwareMessage(3, b("dw 33 10"))));
    }

    @Test
    public void testActivateAndGetSyncForMultiDevices() throws Exception {
        clientPair.appClient.send("createWidget 1\0{\"id\":188, \"width\":1, \"height\":1, \"deviceId\":1, \"x\":0, \"y\":0, \"label\":\"Some Text\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":33, \"value\":1}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody(2);
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(2, device.toString())));

        clientPair.appClient.reset();
        clientPair.appClient.send("activate 1");

        verify(clientPair.appClient.responseMock, timeout(500).times(13)).channelRead(any(), any());

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
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new AppSyncMessage(b("1-1 dw 33 1"))));
    }

    @Test
    public void testOfflineOnlineStatusForMultiDevices() throws Exception {
        Device device0 = new Device(0, "My Dashboard", "UNO");
        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        device0.status = Status.ONLINE;
        device1.status = Status.ONLINE;

        clientPair.appClient.reset();
        clientPair.appClient.send("getDevices 1");
        String response = clientPair.appClient.getBody();

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(2, devices.length);

        assertEqualDevice(device0, devices[0]);
        assertEqualDevice(device1, devices[1]);

        hardClient2.stop().await();
        device1.status = Status.OFFLINE;

        clientPair.appClient.reset();
        clientPair.appClient.send("getDevices 1");
        response = clientPair.appClient.getBody();

        devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(2, devices.length);

        assertEqualDevice(device0, devices[0]);
        assertEqualDevice(device1, devices[1]);
    }

    @Test
    public void testCorrectOnlineStatusForDisconnect() throws Exception {
        Device device0 = new Device(0, "My Dashboard", "UNO");
        device0.status = Status.ONLINE;

        clientPair.appClient.send("getDevices 1");
        String response = clientPair.appClient.getBody();

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(1, devices.length);

        assertEqualDevice(device0, devices[0]);

        clientPair.hardwareClient.stop().await();
        device0.status = Status.OFFLINE;

        clientPair.appClient.send("getDevices 1");
        response = clientPair.appClient.getBody(2);

        devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(1, devices.length);

        assertEqualDevice(device0, devices[0]);
    }

    @Test
    public void testCorrectOnlineStatusForReconnect() throws Exception {
        Device device0 = new Device(0, "My Dashboard", "UNO");
        device0.status = Status.ONLINE;

        clientPair.appClient.send("getDevices 1");
        String response = clientPair.appClient.getBody();

        Device[] devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(1, devices.length);

        assertEqualDevice(device0, devices[0]);

        clientPair.hardwareClient.stop().await();

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + devices[0].token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.reset();

        clientPair.appClient.send("getDevices 1");
        response = clientPair.appClient.getBody();

        devices = JsonParser.mapper.readValue(response, Device[].class);
        assertNotNull(devices);
        assertEquals(1, devices.length);

        assertEqualDevice(device0, devices[0]);
    }


    @Test
    public void testHardwareChannelClosedOnDashRemoval() throws Exception {
        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("deleteDash 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        assertTrue(clientPair.hardwareClient.isClosed());
        assertTrue(hardClient2.isClosed());
    }

    @Test
    public void testHardwareChannelClosedOnDeviceRemoval() throws Exception {
        Device device1 = new Device(1, "My Device", "ESP8266");

        clientPair.appClient.send("createDevice 1\0" + device1.toString());
        String createdDevice = clientPair.appClient.getBody();
        Device device = JsonParser.parseDevice(createdDevice);
        assertNotNull(device);
        assertNotNull(device.token);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new CreateDevice(1, device.toString())));

        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        hardClient2.send("login " + device.token);
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new HardwareConnectedMessage(1, "1-1")));

        clientPair.appClient.send("deleteDevice 1\0" + "1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        assertFalse(clientPair.hardwareClient.isClosed());
        assertTrue(hardClient2.isClosed());
    }


    private static void assertEqualDevice(Device expected, Device real) {
        assertEquals(expected.id, real.id);
        //assertEquals(expected.name, real.name);
        assertEquals(expected.boardType, real.boardType);
        assertNotNull(real.token);
        assertEquals(expected.status, real.status);
    }

}
