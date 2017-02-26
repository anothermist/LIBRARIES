package cc.blynk.server.db;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.enums.GraphType;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.reporting.average.AggregationKey;
import cc.blynk.server.core.reporting.average.AggregationValue;
import cc.blynk.server.core.reporting.average.AverageAggregatorProcessor;
import cc.blynk.server.db.dao.ReportingDBDao;
import cc.blynk.server.db.model.Purchase;
import cc.blynk.server.db.model.Redeem;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.postgresql.copy.CopyManager;
import org.postgresql.core.BaseConnection;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.zip.GZIPOutputStream;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.02.16.
 */
public class DBManagerTest {

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
        dbManager.executeSQL("DELETE FROM users");
        dbManager.executeSQL("DELETE FROM reporting_average_minute");
        dbManager.executeSQL("DELETE FROM reporting_average_hourly");
        dbManager.executeSQL("DELETE FROM reporting_average_daily");
        dbManager.executeSQL("DELETE FROM purchase");
        dbManager.executeSQL("DELETE FROM redeem");
    }

    @Test
    public void test() throws Exception {
        assertNotNull(dbManager.getConnection());
    }

    @Test
    public void testInsert1000RecordsAndSelect() throws Exception {
        int a = 0;

        String userName = "test@gmail.com";

        long start = System.currentTimeMillis();
        long minute = (start / AverageAggregatorProcessor.MINUTE) * AverageAggregatorProcessor.MINUTE;
        long startMinute = minute;

        try (Connection connection = dbManager.getConnection();
             PreparedStatement ps = connection.prepareStatement(ReportingDBDao.insertMinute)) {

            for (int i = 0; i < 1000; i++) {
                ReportingDBDao.prepareReportingInsert(ps, userName, 1, 2, (byte) 0, 'v', minute, (double) i);
                ps.addBatch();
                minute += AverageAggregatorProcessor.MINUTE;
                a++;
            }

            ps.executeBatch();
            connection.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("Finished : " + (System.currentTimeMillis() - start)  + " millis. Executed : " + a);


        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from reporting_average_minute order by ts ASC")) {

            int i = 0;
            while (rs.next()) {
                assertEquals(userName, rs.getString("username"));
                assertEquals(1, rs.getInt("project_id"));
                assertEquals(2, rs.getInt("device_id"));
                assertEquals(0, rs.getByte("pin"));
                assertEquals("v", rs.getString("pinType"));
                assertEquals(startMinute, rs.getLong("ts"));
                assertEquals((double) i, rs.getDouble("value"), 0.0001);
                startMinute += AverageAggregatorProcessor.MINUTE;
                i++;
            }
            connection.commit();
        }
    }

    @Test
    @Ignore
    public void testCopy100RecordsIntoFile() throws Exception {
        System.out.println("Starting");

        int a = 0;

        long start = System.currentTimeMillis();
        try (Connection connection = dbManager.getConnection();
             PreparedStatement ps = connection.prepareStatement(ReportingDBDao.insertMinute)) {

            String userName = "test@gmail.com";
            long minute = (System.currentTimeMillis() / AverageAggregatorProcessor.MINUTE) * AverageAggregatorProcessor.MINUTE;

            for (int i = 0; i < 100; i++) {
                ReportingDBDao.prepareReportingInsert(ps, userName, 1, 0, (byte) 0, 'v', minute, (double) i);
                ps.addBatch();
                minute += AverageAggregatorProcessor.MINUTE;
                a++;
            }

            ps.executeBatch();
            connection.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("Finished : " + (System.currentTimeMillis() - start)  + " millis. Executed : " + a);


        try (Connection connection = dbManager.getConnection();
             Writer gzipWriter = new OutputStreamWriter(new GZIPOutputStream(new FileOutputStream(new File("/home/doom369/output.csv.gz"))), "UTF-8")) {

            CopyManager copyManager = new CopyManager(connection.unwrap(BaseConnection.class));


            String selectQuery = "select pintype || pin, ts, value from reporting_average_minute where project_id = 1 and username = 'test@gmail.com'";
            long res = copyManager.copyOut("COPY (" + selectQuery + " ) TO STDOUT WITH (FORMAT CSV)", gzipWriter);
            System.out.println(res);
        }


    }

    @Test
    public void testDeleteWorksAsExpected() throws Exception {
        long minute;
        try (Connection connection = dbManager.getConnection();
             PreparedStatement ps = connection.prepareStatement(ReportingDBDao.insertMinute)) {

            minute = (System.currentTimeMillis() / AverageAggregatorProcessor.MINUTE) * AverageAggregatorProcessor.MINUTE;

            for (int i = 0; i < 370; i++) {
                ReportingDBDao.prepareReportingInsert(ps, "test1111@gmail.com", 1, 0, (byte) 0, 'v', minute, (double) i);
                ps.addBatch();
                minute += AverageAggregatorProcessor.MINUTE;
            }

            ps.executeBatch();
            connection.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }

        //todo finish.
        //todo this breaks testInsert1000RecordsAndSelect() test
        //Instant now = Instant.ofEpochMilli(minute);
        //dbManager.cleanOldReportingRecords(now);

    }


    @Test
    public void testManyConnections() throws Exception {
        Map<AggregationKey, AggregationValue> map = new ConcurrentHashMap<>();
        AggregationValue value = new AggregationValue();
        value.update(1);
        long ts = System.currentTimeMillis();
        for (int i = 0; i < 60; i++) {
            map.put(new AggregationKey("test@test.com", i, 0, PinType.ANALOG.pintTypeChar, (byte) i, ts), value);
            dbManager.insertReporting(map, GraphType.MINUTE);
            dbManager.insertReporting(map, GraphType.HOURLY);
            dbManager.insertReporting(map, GraphType.DAILY);

            map.clear();
        }

        while (blockingIOProcessor.getActiveCount() > 0) {
            Thread.sleep(100);
        }

    }


    @Test
    @Ignore("Ignored cause travis postgres is old and doesn't support upserts")
    public void testUpsertForDifferentApps() throws Exception {
        ArrayList<User> users = new ArrayList<>();
        users.add(new User("test1@gmail.com", "pass", "testapp2", "local", false));
        users.add(new User("test1@gmail.com", "pass", "testapp1", "local", false));
        dbManager.userDBDao.save(users);
        ConcurrentMap<UserKey, User> dbUsers = dbManager.userDBDao.getAllUsers();
        assertEquals(2, dbUsers.size());
    }

    @Test
    @Ignore("Ignored cause travis postgres is old and doesn't support upserts")
    public void testUpsertAndSelect() throws Exception {
        ArrayList<User> users = new ArrayList<>();
        for (int i = 0; i < 10000; i++) {
            users.add(new User("test" + i + "@gmail.com", "pass", AppName.BLYNK, "local", false));
        }
        //dbManager.saveUsers(users);
        dbManager.userDBDao.save(users);

        ConcurrentMap<UserKey, User> dbUsers = dbManager.userDBDao.getAllUsers();
        System.out.println("Records : " + dbUsers.size());
    }

    @Test
    @Ignore("Ignored cause travis postgres is old and doesn't support upserts")
    public void testUpsertUser() throws Exception {
        ArrayList<User> users = new ArrayList<>();
        User user = new User("test@gmail.com", "pass", AppName.BLYNK, "local", false);
        user.lastModifiedTs = 0;
        users.add(user);
        user = new User("test@gmail.com", "pass2", AppName.BLYNK, "local", false);
        user.lastModifiedTs = 0;
        users.add(user);
        user = new User("test2@gmail.com", "pass2", AppName.BLYNK, "local", false);
        user.lastModifiedTs = 0;
        users.add(user);

        dbManager.userDBDao.save(users);

        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from users where username = 'test@gmail.com'")) {
            while (rs.next()) {
                assertEquals("test@gmail.com", rs.getString("username"));
                assertEquals("local", rs.getString("region"));
                assertEquals("{\"name\":\"test@gmail.com\",\"appName\":\"Blynk\",\"region\":\"local\",\"pass\":\"pass2\",\"lastModifiedTs\":0,\"lastLoggedAt\":0,\"profile\":{},\"isFacebookUser\":false,\"energy\":2000}", rs.getString("json"));
            }
            connection.commit();
        }
    }

    @Test
    public void testRedeem() throws Exception {
        assertNull(dbManager.selectRedeemByToken("123"));
        String token = UUID.randomUUID().toString().replace("-", "");
        dbManager.executeSQL("insert into redeem (token) values('" + token + "')");
        assertNotNull(dbManager.selectRedeemByToken(token));
        assertNull(dbManager.selectRedeemByToken("123"));
    }

    @Test
    public void testPurchase() throws Exception {
        dbManager.insertPurchase(new Purchase("test@gmail.com", 1000, "123456"));


        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from purchase")) {

            while (rs.next()) {
                assertEquals("test@gmail.com", rs.getString("username"));
                assertEquals(1000, rs.getInt("reward"));
                assertEquals("123456", rs.getString("transactionId"));
                assertEquals(0.99D, rs.getDouble("price"), 0.1D);
                assertNotNull(rs.getDate("ts"));
            }

            connection.commit();
        }
    }

    @Test
    public void testOptimisticLockingRedeem() throws Exception {
        String token = UUID.randomUUID().toString().replace("-", "");
        dbManager.executeSQL("insert into redeem (token) values('" + token + "')");

        Redeem redeem = dbManager.selectRedeemByToken(token);
        assertNotNull(redeem);
        assertEquals(redeem.token, token);
        assertFalse(redeem.isRedeemed);
        assertEquals(1, redeem.version);
        assertNull(redeem.ts);

        assertTrue(dbManager.updateRedeem("user@user.com", token));
        assertFalse(dbManager.updateRedeem("user@user.com", token));

        redeem = dbManager.selectRedeemByToken(token);
        assertNotNull(redeem);
        assertEquals(redeem.token, token);
        assertTrue(redeem.isRedeemed);
        assertEquals(2, redeem.version);
        assertEquals("user@user.com", redeem.username);
        assertNotNull(redeem.ts);
    }

    @Test
    public void testSelect() {
        long ts = 1455924480000L;
        try (Connection connection = dbManager.getConnection();
             PreparedStatement ps = connection.prepareStatement(ReportingDBDao.selectMinute)) {

            ReportingDBDao.prepareReportingSelect(ps, ts, 2);
             ResultSet rs = ps.executeQuery();


            while(rs.next()) {
                System.out.println(rs.getLong("ts") + " " + rs.getDouble("value"));
            }

            rs.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }


}
