package cc.blynk.server.db;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.reporting.raw.RawDataProcessor;
import cc.blynk.utils.NumberUtil;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.02.16.
 */
public class RawDataDBTest {

    private static DBManager dbManager;
    private static BlockingIOProcessor blockingIOProcessor;

    @BeforeClass
    public static void init() throws Exception {
        blockingIOProcessor = new BlockingIOProcessor(2, 10000, null);
        dbManager = new DBManager("db-test.properties", blockingIOProcessor);
        assertNotNull(dbManager.getConnection());
    }

    @AfterClass
    public static void close() {
        dbManager.close();
    }

    @Before
    public void cleanAll() throws Exception {
        //clean everything just in case
        dbManager.executeSQL("DELETE FROM reporting_raw_data");
    }

    @Test
    public void testInsertStringAsRawData() throws Exception {
        RawDataProcessor rawDataProcessor = new RawDataProcessor(true);
        rawDataProcessor.collect("test@test.com", 1, 2, 'v', (byte) 3, 1111111111, "Lamp is ON", NumberUtil.NO_RESULT);

        //invoking directly dao to avoid separate thread execution
        dbManager.reportingDBDao.insertRawData(rawDataProcessor.rawStorage);

        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from reporting_raw_data")) {


            while (rs.next()) {
                assertEquals("test@test.com", rs.getString("username"));
                assertEquals(1, rs.getInt("project_id"));
                assertEquals(2, rs.getInt("device_id"));
                assertEquals(3, rs.getByte("pin"));
                assertEquals("v", rs.getString("pinType"));
                assertEquals(1111111111, rs.getLong("ts"));
                assertEquals("Lamp is ON", rs.getString("stringValue"));
                assertNull(rs.getString("doubleValue"));

            }

            connection.commit();
        }
    }

    @Test
    public void testInsertDoubleAsRawData() throws Exception {
        RawDataProcessor rawDataProcessor = new RawDataProcessor(true);
        rawDataProcessor.collect("test@test.com", 1, 2, 'v', (byte) 3, 1111111111, "Lamp is ON", 1.33D);

        //invoking directly dao to avoid separate thread execution
        dbManager.reportingDBDao.insertRawData(rawDataProcessor.rawStorage);

        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from reporting_raw_data")) {


            while (rs.next()) {
                assertEquals("test@test.com", rs.getString("username"));
                assertEquals(1, rs.getInt("project_id"));
                assertEquals(2, rs.getInt("device_id"));
                assertEquals(3, rs.getByte("pin"));
                assertEquals("v", rs.getString("pinType"));
                assertEquals(1111111111, rs.getLong("ts"));
                assertNull(rs.getString("stringValue"));
                assertEquals(1.33D, rs.getDouble("doubleValue"), 0.0000001);

            }

            connection.commit();
        }

    }

    //todo tests for large batches.


}
