package cc.blynk.integration;

import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.widgets.MultiPinWidget;
import cc.blynk.server.core.model.widgets.OnePinWidget;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.GetTokenMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareConnectedMessage;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.ServerProperties;
import cc.blynk.utils.StringUtils;
import com.fasterxml.jackson.databind.ObjectReader;
import org.mockito.ArgumentCaptor;

import java.io.InputStream;
import java.util.List;

import static cc.blynk.server.core.protocol.enums.Response.OK;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/4/2015.
 */
public abstract class IntegrationBase extends BaseTest {

    public static final String DEFAULT_TEST_USER = "dima@mail.ua";
    private static final ObjectReader profileReader = JsonParser.init().readerFor(Profile.class);

    public static Profile parseProfile(InputStream reader) throws Exception {
        return profileReader.readValue(reader);
    }

    public static Profile parseProfile(String reader) throws Exception {
        return profileReader.readValue(reader);
    }

    public static String readTestUserProfile(String fileName) throws Exception{
        if (fileName == null) {
            fileName = "user_profile_json.txt";
        }
        InputStream is = IntegrationBase.class.getResourceAsStream("/json_test/" + fileName);
        Profile profile = parseProfile(is);
        return profile.toString();
    }

    public static String readTestUserProfile() throws Exception {
        return readTestUserProfile(null);
    }

    protected static GetTokenMessage getGetTokenMessage(List<Object> arguments) {
        for (Object obj : arguments) {
            if (obj instanceof GetTokenMessage) {
                return (GetTokenMessage) obj;
            }
        }
        throw new RuntimeException("Get token message wasn't retrieved.");
    }

    public static void saveProfile(TestAppClient appClient, DashBoard... dashBoards) {
        for (DashBoard dash : dashBoards) {
            appClient.send("createDash " + dash.toString());
        }
    }

    public static String b(String body) {
        return body.replaceAll(" ", StringUtils.BODY_SEPARATOR_STRING);
    }

    public static ResponseMessage ok(int msgId) {
        return new ResponseMessage(msgId, 200);
    }

    public static ClientPair initAppAndHardPair(String host, int appPort, int hardPort, String user, String jsonProfile,
                                                ServerProperties properties, int energy) throws Exception {

        TestAppClient appClient = new TestAppClient(host, appPort, properties);
        TestHardClient hardClient = new TestHardClient(host, hardPort);

        return initAppAndHardPair(appClient, hardClient, user, jsonProfile, energy);
    }

    public static ClientPair initAppAndHardPair(TestAppClient appClient, TestHardClient hardClient, String user,
                                                String jsonProfile, int energy) throws Exception {

        appClient.start();
        hardClient.start();

        String userProfileString = readTestUserProfile(jsonProfile);
        Profile profile = parseProfile(userProfileString);

        int expectedSyncCommandsCount = 0;
        for (Widget widget : profile.dashBoards[0].widgets) {
            if (widget instanceof OnePinWidget) {
                if (((OnePinWidget) widget).makeHardwareBody() != null) {
                    expectedSyncCommandsCount++;
                }
            } else if (widget instanceof MultiPinWidget) {
                MultiPinWidget multiPinWidget = (MultiPinWidget) widget;
                if (multiPinWidget.pins != null) {
                    if (multiPinWidget.isSplitMode()) {
                        for (Pin pin : multiPinWidget.pins) {
                            if (pin.notEmpty()) {
                                expectedSyncCommandsCount++;
                            }
                        }
                    } else {
                        if (multiPinWidget.pins[0].notEmpty()) {
                            expectedSyncCommandsCount++;
                        }
                    }
                }
            }
        }

        int dashId = profile.dashBoards[0].id;

        appClient.send("register " + user);
        appClient.send("login " + user + " Android" + "\0" + "1.10.4");
        appClient.send("addEnergy " + energy + "\0" + "123");
        //we should wait until login finished. Only after that we can send commands
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        saveProfile(appClient, profile.dashBoards);

        appClient.send("activate " + dashId);
        appClient.send("getToken " + dashId);

        ArgumentCaptor<Object> objectArgumentCaptor = ArgumentCaptor.forClass(Object.class);
        verify(appClient.responseMock, timeout(2000).times(5 + profile.dashBoards.length + expectedSyncCommandsCount)).channelRead(any(), objectArgumentCaptor.capture());

        List<Object> arguments = objectArgumentCaptor.getAllValues();
        String token = getGetTokenMessage(arguments).body;

        hardClient.send("login " + token);
        verify(hardClient.responseMock, timeout(2000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        verify(appClient.responseMock, timeout(2000)).channelRead(any(), eq(new HardwareConnectedMessage(1, "" + dashId + "-0")));

        appClient.reset();
        hardClient.reset();

        return new ClientPair(appClient, hardClient, token);
    }

    public static ClientPair initAppAndHardPair(int tcpAppPort, int tcpHartPort, ServerProperties properties) throws Exception {
        return initAppAndHardPair("localhost", tcpAppPort, tcpHartPort, DEFAULT_TEST_USER + " 1", null, properties, 10000);
    }

    public static ClientPair initAppAndHardPair() throws Exception {
        return initAppAndHardPair("localhost", tcpAppPort, tcpHardPort, DEFAULT_TEST_USER + " 1", null, properties, 10000);
    }

    public static ClientPair initAppAndHardPair(int energy) throws Exception {
        return initAppAndHardPair("localhost", tcpAppPort, tcpHardPort, DEFAULT_TEST_USER + " 1", null, properties, energy);
    }

    public static ClientPair initAppAndHardPair(String jsonProfile) throws Exception {
        return initAppAndHardPair("localhost", tcpAppPort, tcpHardPort, DEFAULT_TEST_USER + " 1", jsonProfile, properties, 10000);
    }

}
