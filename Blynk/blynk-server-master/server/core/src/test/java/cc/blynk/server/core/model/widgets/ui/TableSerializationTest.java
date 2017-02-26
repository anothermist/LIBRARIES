package cc.blynk.server.core.model.widgets.ui;

import cc.blynk.server.core.model.widgets.ui.table.Column;
import cc.blynk.server.core.model.widgets.ui.table.Row;
import cc.blynk.server.core.model.widgets.ui.table.Table;
import cc.blynk.utils.JsonParser;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 8/20/16.
 */
public class TableSerializationTest {

    @Test
    public void testTableNoRowsJson() throws Exception {
        Table table = new Table();
        table.columns = new Column[3];
        table.columns[0] = new Column();
        table.columns[0].name = "indicator";
        table.columns[1] = new Column();
        table.columns[1].name = "name";
        table.columns[2] = new Column();
        table.columns[2].name = "value";

        String json = JsonParser.mapper.writeValueAsString(table);
        assertEquals("{\"type\":\"TABLE\",\"id\":0,\"x\":0,\"y\":0,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"columns\":[{\"name\":\"indicator\"},{\"name\":\"name\"},{\"name\":\"value\"}],\"rows\":[],\"currentRowIndex\":0,\"isReoderingAllowed\":false,\"isClickableRows\":false}", json);
    }

    @Test
    public void testTableSingleRowJson() throws Exception {
        Table table = new Table();
        table.columns = new Column[3];
        table.columns[0] = new Column();
        table.columns[0].name = "indicator";
        table.columns[1] = new Column();
        table.columns[1].name = "name";
        table.columns[2] = new Column();
        table.columns[2].name = "value";

        Row row = new Row();
        row.name = "Adskiy trash";
        row.value = "6:33";
        row.isSelected = false;
        row.id = 1;
        table.rows.add(row);

        String json = JsonParser.mapper.writeValueAsString(table);
        assertEquals("{\"type\":\"TABLE\",\"id\":0,\"x\":0,\"y\":0,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"columns\":[{\"name\":\"indicator\"},{\"name\":\"name\"},{\"name\":\"value\"}],\"rows\":[{\"id\":1,\"name\":\"Adskiy trash\",\"value\":\"6:33\",\"isSelected\":false}],\"currentRowIndex\":0,\"isReoderingAllowed\":false,\"isClickableRows\":false}", json);
    }


    @Test
    public void testTableMultiRowJson() throws Exception {
        Table table = new Table();
        table.columns = new Column[3];
        table.columns[0] = new Column();
        table.columns[0].name = "indicator";
        table.columns[1] = new Column();
        table.columns[1].name = "name";
        table.columns[2] = new Column();
        table.columns[2].name = "value";

        Row row = new Row();
        row.name = "Adskiy trash";
        row.value = "6:33";
        row.isSelected = false;
        row.id = 1;
        table.rows.add(row);

        Row row2 = new Row();
        row2.name = "Adskiy trash2";
        row2.value = "6:332";
        row2.isSelected = false;
        row2.id = 2;
        table.rows.add(row2);

        String json = JsonParser.mapper.writeValueAsString(table);
        assertEquals("{\"type\":\"TABLE\",\"id\":0,\"x\":0,\"y\":0,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"deviceId\":0,\"pin\":-1,\"pwmMode\":false,\"rangeMappingOn\":false,\"min\":0,\"max\":0,\"columns\":[{\"name\":\"indicator\"},{\"name\":\"name\"},{\"name\":\"value\"}],\"rows\":[{\"id\":1,\"name\":\"Adskiy trash\",\"value\":\"6:33\",\"isSelected\":false},{\"id\":2,\"name\":\"Adskiy trash2\",\"value\":\"6:332\",\"isSelected\":false}],\"currentRowIndex\":0,\"isReoderingAllowed\":false,\"isClickableRows\":false}", json);
    }

}
