package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.ClientPair;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.ui.table.Row;
import cc.blynk.server.core.model.widgets.ui.table.Table;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.utils.JsonParser;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 7/09/2016.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class TableCommandsTest extends IntegrationBase {

    private BaseServer httpServer;
    private BaseServer appServer;
    private BaseServer hardwareServer;
    private ClientPair clientPair;

    @Before
    public void init() throws Exception {
        httpServer = new HttpAPIServer(holder).start();
        hardwareServer = new HardwareServer(holder).start();
        appServer = new AppServer(holder).start();

        if (clientPair == null) {
            clientPair = initAppAndHardPair(tcpAppPort, tcpHardPort, properties);
        }
        clientPair.hardwareClient.reset();
        clientPair.appClient.reset();
    }

    @After
    public void shutdown() {
        httpServer.close();
        appServer.close();
        hardwareServer.close();
        clientPair.stop();
    }

    @Test
    public void testAllTableCommands() throws Exception {
        Table table = new Table();
        table.pin = 123;
        table.pinType = PinType.VIRTUAL;
        table.isClickableRows = true;
        table.isReoderingAllowed = true;
        table.height = 2;
        table.width = 2;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(table));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 123 clr");
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), any());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 123 clr"))));

        clientPair.hardwareClient.send("hardware vw 123 add 0 Row0 row0");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(2, HARDWARE, b("1 vw 123 add 0 Row0 row0"))));

        table = loadTable();
        Row row;

        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(1, table.rows.size());
        row = table.rows.get(0);
        assertNotNull(row);
        assertEquals(0, row.id);
        assertEquals("Row0", row.name);
        assertEquals("row0", row.value);
        assertTrue(row.isSelected);
        assertEquals(0, table.currentRowIndex);

        clientPair.hardwareClient.send("hardware vw 123 pick 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(3, HARDWARE, b("1 vw 123 pick 2"))));

        table = loadTable();
        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(1, table.rows.size());
        row = table.rows.get(0);
        assertNotNull(row);
        assertEquals(0, table.currentRowIndex);

        clientPair.hardwareClient.send("hardware vw 123 add 1 Row1 row1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(4, HARDWARE, b("1 vw 123 add 1 Row1 row1"))));
        clientPair.hardwareClient.send("hardware vw 123 add 2 Row2 row2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(5, HARDWARE, b("1 vw 123 add 2 Row2 row2"))));
        clientPair.hardwareClient.send("hardware vw 123 pick 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(6, HARDWARE, b("1 vw 123 pick 2"))));

        table = loadTable();
        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(3, table.rows.size());
        row = table.rows.get(0);
        assertNotNull(row);
        assertEquals(2, table.currentRowIndex);

        clientPair.hardwareClient.send("hardware vw 123 deselect 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(7, HARDWARE, b("1 vw 123 deselect 1"))));

        table = loadTable();
        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(3, table.rows.size());
        row = table.rows.get(1);
        assertNotNull(row);
        assertFalse(row.isSelected);

        clientPair.hardwareClient.send("hardware vw 123 select 1");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(8, HARDWARE, b("1 vw 123 select 1"))));

        table = loadTable();
        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(3, table.rows.size());
        row = table.rows.get(1);
        assertNotNull(row);
        assertTrue(row.isSelected);

        clientPair.hardwareClient.send("hardware vw 123 order 0 2");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(9, HARDWARE, b("1 vw 123 order 0 2"))));

        table = loadTable();
        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(3, table.rows.size());

        assertEquals(1, table.rows.get(0).id);
        assertEquals(2, table.rows.get(1).id);
        assertEquals(0, table.rows.get(2).id);

        clientPair.hardwareClient.send("hardware vw 123 clr");
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(10, HARDWARE, b("1 vw 123 clr"))));
        table = loadTable();
        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(0, table.rows.size());
    }

    @Test
    public void testTableRowLimit() throws Exception {
        System.setProperty("table.rows.pool.size", "5");

        Table table = new Table();
        table.pin = 123;
        table.pinType = PinType.VIRTUAL;
        table.isClickableRows = true;
        table.isReoderingAllowed = true;
        table.width = 2;
        table.height = 2;

        clientPair.appClient.send("createWidget 1\0" + JsonParser.mapper.writeValueAsString(table));
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(ok(1)));

        clientPair.hardwareClient.send("hardware vw 123 clr");
        verify(clientPair.hardwareClient.responseMock, timeout(500).times(0)).channelRead(any(), any());
        verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(1, HARDWARE, b("1 vw 123 clr"))));

        for (int i = 1; i <= 6; i++) {
            String cmd = "vw 123 add " + i + " Row0 row0";
            clientPair.hardwareClient.send("hardware " + cmd);
            verify(clientPair.appClient.responseMock, timeout(500)).channelRead(any(), eq(produce(i + 1, HARDWARE, b("1 " + cmd))));
        }


        table = loadTable();
        Row row;

        assertNotNull(table);
        assertNotNull(table.rows);
        assertEquals(5, table.rows.size());
        for (int i = 2; i <= 6; i++) {
            row = table.rows.get(i - 2);
            assertNotNull(row);
            assertEquals(i, row.id);
        }
    }

    private Table loadTable() throws Exception {
        clientPair.appClient.reset();
        clientPair.appClient.send("loadProfileGzipped");
        Profile profile = parseProfile(clientPair.appClient.getBody());
        return (Table) profile.dashBoards[0].findWidgetByPin(0, (byte) 123, PinType.VIRTUAL);
    }

}
