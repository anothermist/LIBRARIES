package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.hardware.HardwareServer;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import static cc.blynk.server.core.protocol.enums.Command.GET_ENERGY;
import static cc.blynk.server.core.protocol.enums.Response.ENERGY_LIMIT;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
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
public class EnergyWorkflowTest extends IntegrationBase {

    private BaseServer appServer;
    private BaseServer hardwareServer;
    private ClientPair clientPair;

    @Before
    public void init() throws Exception {
        this.hardwareServer = new HardwareServer(holder).start();
        this.appServer = new AppServer(holder).start();

        this.clientPair = initAppAndHardPair(4500);
    }

    @After
    public void shutdown() {
        this.appServer.close();
        this.hardwareServer.close();
        this.clientPair.stop();
    }

    @Test
    public void testReach1500LimitOfEnergy() throws Exception {
        clientPair.appClient.send("createDash {\"id\":2, \"createdAt\":1458856800001, \"name\":\"test board\"}\"");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        for (int i = 2; i < 12; i++) {
            clientPair.appClient.send("createWidget 2\0{\"id\":X, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":2}".replace("X", "" + i));
            verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(i, OK)));
        }

        clientPair.appClient.send("createWidget 2\0{\"id\":100, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(12, ENERGY_LIMIT)));
    }

    @Test
    public void testGetEnergy() throws Exception {
        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, GET_ENERGY, "2000")));
    }

    @Test
    public void testAddEnergy() throws Exception {
        clientPair.appClient.send("addEnergy 1000" + "\0" + "123");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "3000")));
    }

    @Test
    public void testEnergyAfterCreateRemoveProject() throws Exception {
        clientPair.appClient.send("createDash {\"id\":2, \"createdAt\":1458856800001, \"name\":\"test board\"}\"");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "2000")));

        clientPair.appClient.send("deleteDash 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(4, GET_ENERGY, "2000")));
    }


    @Test
    public void testEnergyAfterCreateRemoveWidget() throws Exception {
        clientPair.appClient.send("createDash {\"id\":2, \"createdAt\":1458856800001, \"name\":\"test board\"}\"");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(1, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, GET_ENERGY, "2000")));

        clientPair.appClient.send("createWidget 2\0{\"id\":2, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(3, OK)));

        clientPair.appClient.send("createWidget 2\0{\"id\":3, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(4, OK)));

        clientPair.appClient.send("createWidget 2\0{\"id\":4, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(5, OK)));

        clientPair.appClient.send("createWidget 2\0{\"id\":5, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(6, OK)));

        clientPair.appClient.send("createWidget 2\0{\"id\":6, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"LCD\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(7, OK)));

        clientPair.appClient.send("createWidget 2\0{\"id\":7, \"width\":1, \"height\":1, \"x\":2, \"y\":2, \"label\":\"Some Text 2\", \"type\":\"BUTTON\", \"pinType\":\"DIGITAL\", \"pin\":2}");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(8, ENERGY_LIMIT)));

        clientPair.appClient.send("deleteDash 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(new ResponseMessage(9, OK)));

        clientPair.appClient.send("getEnergy");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(10, GET_ENERGY, "2000")));
    }

}
