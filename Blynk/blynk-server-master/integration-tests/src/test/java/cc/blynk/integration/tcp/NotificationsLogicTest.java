package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.widgets.notifications.Notification;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.ResponseWithBodyMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.server.notifications.push.android.AndroidGCMMessage;
import cc.blynk.server.notifications.push.enums.Priority;
import io.netty.channel.ChannelFuture;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Map;

import static cc.blynk.server.core.protocol.enums.Response.DEVICE_WENT_OFFLINE;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.server.core.protocol.enums.Response.OK;
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
public class NotificationsLogicTest extends IntegrationBase {

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
    public void addPushTokenWrongInput()  throws Exception  {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);

        appClient.start();

        appClient.send("register test@test.com 1");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("login test@test.com 1 Android" + "\0" + "RC13");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.send("createDash {\"id\":1, \"createdAt\":1, \"name\":\"test board\"}\"");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        appClient.send("addPushToken 1\0uid\0token");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, NOT_ALLOWED)));
    }

    @Test
    public void addPushTokenWorksForAndroid() throws Exception {
        clientPair.appClient.send("addPushToken 1\0uid1\0token1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody(2));

        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        assertNotNull(notification);
        assertEquals(2, notification.androidTokens.size());
        assertEquals(0, notification.iOSTokens.size());

        assertTrue(notification.androidTokens.containsKey("uid1"));
        assertTrue(notification.androidTokens.containsValue("token1"));
    }

    @Test
    public void addPushTokenNotOverridedOnProfileSave() throws Exception {
        clientPair.appClient.send("addPushToken 1\0uid1\0token1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody(2));

        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        assertNotNull(notification);
        assertEquals(2, notification.androidTokens.size());
        assertEquals(0, notification.iOSTokens.size());

        assertTrue(notification.androidTokens.containsKey("uid1"));
        assertTrue(notification.androidTokens.containsValue("token1"));

        clientPair.appClient.send("updateDash " + profile.getDashById(1).toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("loadProfileGzipped");
        profile = parseProfile(clientPair.appClient.getBody(4));

        notification = profile.getDashById(1).getWidgetByType(Notification.class);
        assertNotNull(notification);
        assertEquals(2, notification.androidTokens.size());
        assertEquals(0, notification.iOSTokens.size());

        assertTrue(notification.androidTokens.containsKey("uid1"));
        assertTrue(notification.androidTokens.containsValue("token1"));
    }

    @Test
    public void addPushTokenWorksForIos() throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort, properties);

        appClient.start();

        appClient.send("login " + DEFAULT_TEST_USER +" 1 iOS" + "\0" + "1.10.2");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        appClient.send("addPushToken 1\0uid2\0token2");
        verify(appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        appClient.reset();

        appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(appClient.getBody());

        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        assertNotNull(notification);
        assertEquals(1, notification.androidTokens.size());
        assertEquals(1, notification.iOSTokens.size());
        Map.Entry<String, String> entry = notification.iOSTokens.entrySet().iterator().next();
        assertEquals("uid2", entry.getKey());
        assertEquals("token2", entry.getValue());
    }

    @Test
    public void testHardwareDeviceWentOffline() throws Exception {
        Profile profile = parseProfile(readTestUserProfile());
        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        notification.notifyWhenOffline = false;

        clientPair.appClient.send("updateDash " + profile.getDashById(1).toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.await();

        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseWithBodyMessage(0, Command.RESPONSE, DEVICE_WENT_OFFLINE, 1)));
    }

    @Test
    public void testHardwareDeviceWentOfflineAndPushWorks() throws Exception {
        Profile profile = parseProfile(readTestUserProfile());
        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        notification.notifyWhenOffline = true;

        clientPair.appClient.send("updateDash " + profile.getDashById(1).toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.await();

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Your UNO went offline. \"My Dashboard\" project is disconnected.", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testHardwareDeviceWentOfflineAndPushDelayedWorks() throws Exception {
        Profile profile = parseProfile(readTestUserProfile());
        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        notification.notifyWhenOffline = true;
        notification.notifyWhenOfflineIgnorePeriod = 1000;

        long now = System.currentTimeMillis();

        clientPair.appClient.send("updateDash " + profile.getDashById(1).toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.await();

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);

        verify(gcmWrapper, timeout(1100).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();
        assertTrue(System.currentTimeMillis() - now > notification.notifyWhenOfflineIgnorePeriod );

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Your UNO went offline. \"My Dashboard\" project is disconnected.", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testHardwareDeviceWentOfflineAndPushDelayedNotTriggeredDueToReconnect() throws Exception {
        Profile profile = parseProfile(readTestUserProfile());
        Notification notification = profile.getDashById(1).getWidgetByType(Notification.class);
        notification.notifyWhenOffline = true;
        notification.notifyWhenOfflineIgnorePeriod = 1000;

        clientPair.appClient.send("updateDash " + profile.getDashById(1).toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        ChannelFuture channelFuture = clientPair.hardwareClient.stop();
        channelFuture.await();


        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody(2);

        TestHardClient newHardClient = new TestHardClient("localhost", tcpHardPort);
        newHardClient.start();
        newHardClient.send("login " + token);
        verify(newHardClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, after(1500).never()).send(objectArgumentCaptor.capture(), any(), any());
    }

    @Test
    public void testCreateNewNotificationWidget() throws Exception  {
        clientPair.appClient.send("deleteWidget 1\0" + "9");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("createWidget 1\0{\"id\":9, \"x\":1, \"y\":1, \"width\":1, \"height\":1, \"type\":\"NOTIFICATION\", \"notifyWhenOfflineIgnorePeriod\":0, \"priority\":\"high\", \"notifyWhenOffline\":true}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        clientPair.appClient.send("addPushToken 1\0uid1\0token1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(3)));

        clientPair.appClient.send("updateWidget 1\0{\"id\":9, \"x\":1, \"y\":1, \"width\":1, \"height\":1, \"type\":\"NOTIFICATION\", \"notifyWhenOfflineIgnorePeriod\":0, \"priority\":\"high\", \"notifyWhenOffline\":false}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(2)));

        clientPair.hardwareClient.send("push 123");

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);

        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token1", Priority.high, "123", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }


}
