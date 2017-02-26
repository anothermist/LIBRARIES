package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.integration.model.tcp.TestHardClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.controls.Timer;
import cc.blynk.server.core.model.widgets.others.eventor.Eventor;
import cc.blynk.server.core.model.widgets.others.eventor.Rule;
import cc.blynk.server.core.model.widgets.others.eventor.TimerTime;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.BaseAction;
import cc.blynk.server.core.model.widgets.others.eventor.model.action.SetPinAction;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.utils.DateTimeUtils;
import cc.blynk.utils.JsonParser;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static cc.blynk.server.workers.timer.TimerWorker.TIMER_MSG_ID;
import static org.junit.Assert.assertNotEquals;
import static org.mockito.Mockito.after;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/2/2015.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class TimerTest extends IntegrationBase {

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
    public void testTimerEvent() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);

        TimerTime timerTime = new TimerTime();

        timerTime.days = new int[] {1,2,3,4,5,6,7};

        //adding 2 seconds just to be sure we no gonna miss timer event
        timerTime.time = LocalTime.now(DateTimeUtils.UTC).toSecondOfDay() + 2;
        timerTime.tzName = DateTimeUtils.UTC;

        Rule rule = new Rule();
        rule.isActive = true;
        rule.triggerTime = timerTime;
        SetPinAction setPinAction = new SetPinAction();
        setPinAction.pin = new Pin();
        setPinAction.pin.pin = 1;
        setPinAction.pin.pinType = PinType.VIRTUAL;
        setPinAction.value = "1";
        rule.actions = new BaseAction[] {
                setPinAction
        };

        Eventor eventor = new Eventor(new Rule[] {
                rule
        });

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(TIMER_MSG_ID, HARDWARE, b("1 vw 1 1"))));
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(TIMER_MSG_ID, HARDWARE, b("vw 1 1"))));
    }

    @Test
    public void testIsTimeMethod() {
        ZonedDateTime currentDateTime = ZonedDateTime.now(DateTimeUtils.UTC).withHour(23);
        //kiev is +2, so as currentDateTime has 23 hour. kiev should be always ahead.
        LocalDateTime userDateTime = currentDateTime.withZoneSameInstant(ZoneId.of("Europe/Kiev")).toLocalDateTime();
        assertNotEquals(currentDateTime.getDayOfWeek(), userDateTime.getDayOfWeek());
    }

    @Test
    public void testTimerEventWithWrongDayDontWork() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);

        ZonedDateTime now = ZonedDateTime.now(DateTimeUtils.UTC);
        int currentDayIndex = now.getDayOfWeek().ordinal();

        TimerTime timerTime = new TimerTime();

        timerTime.days = new int[] {1,2,3,4,5,6,7};
        //removing today day from expected days so timer doesnt work.
        timerTime.days[currentDayIndex] = -1;

        //adding 2 seconds just to be sure we no gonna miss timer event
        timerTime.time = LocalTime.now(DateTimeUtils.UTC).toSecondOfDay() + 1;
        timerTime.tzName = DateTimeUtils.UTC;

        Rule rule = new Rule();
        rule.isActive = true;
        rule.triggerTime = timerTime;
        SetPinAction setPinAction = new SetPinAction();
        setPinAction.pin = new Pin();
        setPinAction.pin.pin = 1;
        setPinAction.pin.pinType = PinType.VIRTUAL;
        setPinAction.value = "1";
        rule.actions = new BaseAction[] {
                setPinAction
        };

        Eventor eventor = new Eventor(new Rule[] {
                rule
        });

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(eventor));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        verify(clientPair.appClient.responseMock, after(700).never()).channelRead(any(), eq(produce(TIMER_MSG_ID, HARDWARE, b("1 vw 1 1"))));
        verify(clientPair.hardwareClient.responseMock, after(700).never()).channelRead(any(), eq(produce(TIMER_MSG_ID, HARDWARE, b("vw 1 1"))));
    }

    @Test
    public void testAddTimerWidgetWithStartTimeTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.width = 2;
        timer.height = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.startValue = b("dw 5 1");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(1500).times(1)).channelRead(any(), any());
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 1"))));
    }

    @Test
    public void testAddTimerWidgetWithStopAndStartTimeTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.width = 2;
        timer.height = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.stopValue = b("dw 5 0");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.stopTime = curTime + 1;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(1500).times(1)).channelRead(any(), any());
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 0"))));
    }

    @Test
    public void testAddTimerWidgetWithStopTimeTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.width = 2;
        timer.height = 1;
        timer.startValue = b("dw 5 1");
        timer.stopValue = b("dw 5 0");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 2;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(2500).times(2)).channelRead(any(), any());
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 0"))));
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 1"))));
    }


    @Test
    public void testAddTimerWidgetWithStopTimeAndRemove() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.width = 2;
        timer.height = 1;
        timer.startValue = b("dw 5 1");
        timer.stopValue = b("dw 5 0");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 2;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("deleteWidget 1\0" + 112);
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        verify(clientPair.hardwareClient.responseMock, after(2500).never()).channelRead(any(), any());
    }

    @Test
    public void testAddFewTimersWidgetWithStartTimeTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.startValue = b("1");
        timer.width = 2;
        timer.height = 1;
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        timer.id = 113;
        timer.startValue = b("2");

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(2500).times(2)).channelRead(any(), any());
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 1"))));
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 2"))));
    }

    @Test
    public void testAddTimerWithSameStartStopTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.width = 2;
        timer.height = 1;
        timer.startValue = b("dw 5 0");
        timer.stopValue = b("dw 5 1");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 1;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(2500).times(2)).channelRead(any(), any());
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 0"))));
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 1"))));
    }

    @Test
    public void testUpdateTimerWidgetWithStopTimeTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.width = 2;
        timer.height = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.startValue = b("dw 5 1");
        timer.stopValue = b("dw 5 0");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 2;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        timer.startValue = b("dw 5 11");
        timer.stopValue = b("dw 5 10");

        clientPair.appClient.send("updateWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(2500).times(2)).channelRead(any(), any());
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 11"))));
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 10"))));
    }

    @Test
    public void testDashTimerNotTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);
        Timer timer = new Timer();
        timer.id = 112;
        timer.x = 1;
        timer.y = 1;
        timer.width = 2;
        timer.height = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.startValue = b("dw 5 1");
        timer.stopValue = b("dw 5 0");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 2;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.toJson(timer));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("deleteDash 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        verify(clientPair.hardwareClient.responseMock, after(2500).times(0)).channelRead(any(), any());
    }



    @Test
    public void testTimerWidgetTriggeredAndSendCommandToCorrectDevice() throws Exception {
        TestHardClient hardClient2 = new TestHardClient("localhost", tcpHardPort);
        hardClient2.start();

        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Timer timer = new Timer();
        timer.id = 1;
        timer.x = 1;
        timer.y = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.startValue = b("dw 5 1");
        timer.stopValue = b("dw 5 0");
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 2;

        DashBoard dashBoard = new DashBoard();
        dashBoard.id = 1;
        dashBoard.name = "Test";
        dashBoard.widgets = new Widget[] {timer};

        clientPair.appClient.send("updateDash " + dashBoard.toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        dashBoard.id = 2;
        clientPair.appClient.send("createDash " + dashBoard.toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        clientPair.appClient.reset();
        clientPair.appClient.send("getToken 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), any());
        hardClient2.send("login " + clientPair.appClient.getBody());
        verify(hardClient2.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        hardClient2.reset();

        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 1"))));
        clientPair.hardwareClient.reset();
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 0"))));

        verify(hardClient2.responseMock, never()).channelRead(any(), any());
        hardClient2.stop().awaitUninterruptibly();
    }

    @Test
    public void testTimerWidgetTriggered() throws Exception {
        Executors.newScheduledThreadPool(1).scheduleAtFixedRate(holder.timerWorker, 0, 1000, TimeUnit.MILLISECONDS);

        clientPair.appClient.send("deactivate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        Timer timer = new Timer();
        timer.id = 1;
        timer.x = 1;
        timer.y = 1;
        timer.pinType = PinType.DIGITAL;
        timer.pin = 5;
        timer.startValue = "1";
        timer.stopValue = "0";
        LocalTime localDateTime = LocalTime.now(ZoneId.of("UTC"));
        int curTime = localDateTime.toSecondOfDay();
        timer.startTime = curTime + 1;
        timer.stopTime = curTime + 2;

        DashBoard dashBoard = new DashBoard();
        dashBoard.id = 1;
        dashBoard.name = "Test";
        dashBoard.widgets = new Widget[] {timer};

        clientPair.appClient.send("updateDash " + dashBoard.toString());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        clientPair.appClient.send("activate 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 1"))));
        clientPair.hardwareClient.reset();
        verify(clientPair.hardwareClient.responseMock, timeout(2000)).channelRead(any(), eq(produce(7777, HARDWARE, b("dw 5 0"))));
    }



}
