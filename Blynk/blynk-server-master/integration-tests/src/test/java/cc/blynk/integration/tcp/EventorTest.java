package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.OnePinWidget;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.model.widgets.others.eventor.Rule;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.BaseAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.SetPinAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.WaitAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.notification.MailAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.notification.NotifyAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.notification.TwitAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.BaseCondition;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.Between;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.Equal;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.GreaterThan;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.GreaterThanOrEqual;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.LessThan;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.LessThanOrEqual;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.NotBetween;
import cc.blynk.server.core.model.widgets.others.eventor.model.condition.NotEqual;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandBodyException;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
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
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/2/2015.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class EventorTest extends IntegrationBase {

    private BaseServer appServer;
    private BaseServer hardwareServer;
    private ClientPair clientPair;

    private static Rule buildRule(String s) {
        //example "if V1 > 37 then setpin V2 123"

        String[] splitted = s.split(" ");

        //"V1"
        Pin triggerPin = parsePin(splitted[1]);
                                                       //>                               37
        BaseCondition ifCondition = resolveCondition(splitted[2], Double.parseDouble(splitted[3]));

        Pin pin = null;
        String value;
        try {
            //V2
            pin = parsePin(splitted[6]);
            //123
            value = splitted[7];
        } catch (IllegalCommandBodyException e) {
            value = splitted[6];
        }

                                            //setpin
        BaseAction action = resolveAction(splitted[5], pin, value);

        Rule rule = new Rule(triggerPin, ifCondition, new BaseAction[] { action });
        rule.isActive = true;
        return rule;
    }

    private static Pin parsePin(String pinString) {
        PinType pinType = PinType.getPinType(pinString.charAt(0));
        byte pin = Byte.parseByte(pinString.substring(1));
        return new Pin(pin, pinType);
    }

    private static BaseAction resolveAction(String action, Pin pin, String value) {
        switch (action) {
            case "setpin" :
                return new SetPinAction(pin, value);
            case "wait" :
                return new WaitAction();
            case "notify" :
                return new NotifyAction(value);
            case "mail" :
                return new MailAction(value);
            case "twit" :
                return new TwitAction(value);

            default: throw new RuntimeException("Not supported action. " + action);
        }
    }

    private static BaseCondition resolveCondition(String conditionString, double value) {
        switch (conditionString) {
            case ">" :
                return new GreaterThan(value);
            case ">=" :
                return new GreaterThanOrEqual(value);
            case "<" :
                return new LessThan(value);
            case "<=" :
                return new LessThanOrEqual(value);
            case "=" :
                return new Equal(value);
            case "!=" :
                return new NotEqual(value);

            default: throw new RuntimeException("Not supported operation. " + conditionString);
        }
    }

    public static Eventor oneRuleEventor(String ruleString) {
        Rule rule = buildRule(ruleString);
        return new Eventor(new Rule[] {rule});
    }

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
    public void printAllInJson() throws Exception {
        Eventor tempEventor = oneRuleEventor("if v1 != 37 then setpin v2 123");
        //replace with between
        tempEventor.rules[0].condition = new Between(10, 12);

        Eventor tempEventor2 = oneRuleEventor("if v1 != 37 then setpin v2 123");
        //replace with between
        tempEventor2.rules[0].condition = new NotBetween(10, 12);


        Eventor[] eventors = new Eventor[]{
                oneRuleEventor("if v1 > 37 then setpin v2 123"),
         oneRuleEventor("if v1 >= 37 then setpin v2 123"),
        oneRuleEventor("if v1 <= 37 then setpin v2 123"),
        oneRuleEventor("if v1 = 37 then setpin v2 123"),
        oneRuleEventor("if v1 < 37 then setpin v2 123"),
         oneRuleEventor("if v1 != 37 then setpin v2 123"),
                tempEventor,
                tempEventor2
        };

        for (Eventor eventor : eventors) {
            System.out.println(JsonParser.mapper.writeValueAsString(eventor));
        }

        Pin pin = new Pin(1, PinType.VIRTUAL);

        BaseAction[] actions = new BaseAction[] {
                new SetPinAction(pin, "pinValuetoSEt"),
                new WaitAction(360, new SetPinAction(pin, "pinValueToSet")),
                new NotifyAction("Hello!!!"),
                new MailAction("Hello mail")
        };

        for (BaseAction action : actions) {
            System.out.println(JsonParser.mapper.writeValueAsString(action));
        }
    }

    @Test
    public void testSimpleRule1() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 > 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 38");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 38"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule1AndDashUpdatedValue() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 > 37 then setpin v4 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 38");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 38"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 4 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 4 123"))));

        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        assertNotNull(profile);
        OnePinWidget widget = (OnePinWidget) profile.dashBoards[0].findWidgetByPin(0, (byte) 4, PinType.VIRTUAL);
        assertNotNull(widget);
        assertEquals("123", widget.value);
    }

    @Test
    public void testSimpleRule2() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 >= 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule3() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 <= 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule4() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 = 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule5() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 < 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule6() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 != 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule7() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 != 37 then setpin v2 123");

        //replace with between
        eventor.rules[0].condition = new Between(10, 12);

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 11");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 11"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule8() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 != 37 then setpin v2 123");

        //replace with between
        eventor.rules[0].condition = new NotBetween(10, 12);

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 9");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 9"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRule8Notify() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 = 37 then notify Yo!!!!!");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Yo!!!!!", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testSimpleRule8NotifyAndFormat() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 = 37 then notify Temperatureis:/pin/.");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));

        ArgumentCaptor<AndroidGCMMessage> objectArgumentCaptor = ArgumentCaptor.forClass(AndroidGCMMessage.class);
        verify(gcmWrapper, timeout(500).times(1)).send(objectArgumentCaptor.capture(), any(), any());
        AndroidGCMMessage message = objectArgumentCaptor.getValue();

        String expectedJson = new AndroidGCMMessage("token", Priority.normal, "Temperatureis:37.", 1).toJson();
        assertEquals(expectedJson, message.toJson());
    }

    @Test
    public void testSimpleRule9Twit() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 = 37 then twit Yo!!!!!");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));

        verify(twitterWrapper, timeout(500)).send(eq("token"), eq("secret"), eq("Yo!!!!!"));
    }


    @Test
    public void testSimpleRuleCreateUpdateConditionWorks() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 >= 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 37"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));

        eventor = oneRuleEventor("if v1 >= 37 then setpin v2 124");
        clientPair.appClient.send("updateWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 124"))));
        verify(clientPair.appClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 124"))));

        clientPair.hardwareClient.send("hardware vw 1 37");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, HARDWARE, b("1 vw 1 37"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 124"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 124"))));
    }

    @Test
    public void testPinModeForEventorAndSetPinAction() throws Exception {
        clientPair.appClient.send("activate 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in"))));
        reset(clientPair.hardwareClient.responseMock);

        Eventor eventor = oneRuleEventor("if v1 > 37 then setpin d9 1");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in 9 out"))));
        //reset(clientPair.hardwareClient.responseMock);

        clientPair.hardwareClient.send("hardware vw 1 38");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 38"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("dw 9 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 dw 9 1"))));
    }

    @Test
    public void testTriggerOnlyOnceOnCondition() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 < 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));

        clientPair.hardwareClient.reset();
        clientPair.appClient.reset();

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));

        clientPair.hardwareClient.send("hardware vw 1 38");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, HARDWARE, b("1 vw 1 38"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500).times(0)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(4, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testEventorWorksForMultipleHardware() throws Exception {
        TestHardClient hardClient = new TestHardClient("localhost", tcpHardPort);
        hardClient.start();

        clientPair.appClient.send("getToken 1");
        String token = clientPair.appClient.getBody();
        hardClient.send("login " + token);
        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));
        clientPair.appClient.reset();

        Eventor eventor = oneRuleEventor("if v1 < 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 36");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 36"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
        verify(hardClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
    }

    @Test
    public void testPinModeForPWMPinForEventorAndSetPinAction() throws Exception {
        clientPair.appClient.send("activate 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in"))));
        reset(clientPair.hardwareClient.responseMock);

        Eventor eventor = oneRuleEventor("if v1 > 37 then setpin d9 255");
        //here is special case. right now eventor for digital pins supports only LOW/HIGH values
        //that's why eventor doesn't work with PWM pins, as they handled as analog, where HIGH doesn't work.
        SetPinAction setPinAction = (SetPinAction) eventor.rules[0].actions[0];
        setPinAction.pin.pwmMode = true;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("pm 1 out 2 out 3 out 5 out 6 in 7 in 30 in 8 in 9 out"))));

        clientPair.hardwareClient.send("hardware vw 1 38");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 38"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("aw 9 255"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 aw 9 255"))));
    }

    @Test
    public void testSimpleRule2WorksFromAppSide() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 >= 37 then setpin v2 123");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.appClient.send("hardware 1 vw 1 37");
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("vw 1 37"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 2 123"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 2 123"))));
    }

    @Test
    public void testSimpleRuleWith2Actions() throws Exception {
        Eventor eventor = oneRuleEventor("if v1 > 37 then setpin v2 123");
        eventor.rules[0].actions = new BaseAction[] {
                new SetPinAction((byte)0, PinType.VIRTUAL, "0"),
                new SetPinAction((byte)1, PinType.VIRTUAL, "1")
        };

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 1 38");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 1 38"))));

        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 0 0"))));
        verify(clientPair.hardwareClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("vw 1 1"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 0 0"))));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(888, HARDWARE, b("1 vw 1 1"))));
    }

}
