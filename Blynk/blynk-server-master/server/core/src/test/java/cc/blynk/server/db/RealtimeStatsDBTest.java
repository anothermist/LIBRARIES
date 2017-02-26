package cc.blynk.server.db;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.reporting.average.AverageAggregatorProcessor;
import cc.blynk.server.core.stats.model.CommandStat;
import cc.blynk.server.core.stats.model.HttpStat;
import cc.blynk.server.core.stats.model.Stat;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.02.16.
 */
public class RealtimeStatsDBTest {

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
        dbManager.executeSQL("DELETE FROM reporting_app_stat_minute");
        dbManager.executeSQL("DELETE FROM reporting_app_command_stat_minute");
        dbManager.executeSQL("DELETE FROM reporting_http_command_stat_minute");
    }

    @Test
    public void testRealTimeStatsInsertWroks() throws Exception {
        String region = "ua";
        long now = System.currentTimeMillis();

        Stat stat = new Stat(1,2,3,4,5,6,7,8,9,10,now);
        int i;

        final HttpStat hs = stat.http;
        i = 0;
        hs.isHardwareConnected = i++;
        hs.isAppConnected = i++;
        hs.getPinData = i++;
        hs.updatePinData = i++;
        hs.email = i++;
        hs.notify = i++;
        hs.getProject = i++;
        hs.qr = i++;
        hs.getHistoryPinData = i++;
        hs.total = i;

        final CommandStat cs = stat.commands;
        i = 0;
        cs.response = i++;
        cs.register = i++;
        cs.login = i++;
        cs.loadProfile = i++;
        cs.appSync = i++;
        cs.sharing = i++;
        cs.getToken = i++;
        cs.ping = i++;
        cs.activate = i++;
        cs.deactivate = i++;
        cs.refreshToken = i++;
        cs.getGraphData = i++;
        cs.exportGraphData = i++;
        cs.setWidgetProperty = i++;
        cs.bridge = i++;
        cs.hardware = i++;
        cs.getSharedDash = i++;
        cs.getShareToken = i++;
        cs.refreshShareToken = i++;
        cs.shareLogin = i++;
        cs.createProject = i++;
        cs.updateProject = i++;
        cs.deleteProject = i++;
        cs.hardwareSync = i++;
        cs.internal = i++;
        cs.sms = i++;
        cs.tweet = i++;
        cs.email = i++;
        cs.push = i++;
        cs.addPushToken = i++;
        cs.createWidget = i++;
        cs.updateWidget = i++;
        cs.deleteWidget = i++;
        cs.createDevice = i++;
        cs.updateDevice = i++;
        cs.deleteDevice = i++;
        cs.getDevices = i++;
        cs.createTag = i++;
        cs.updateTag = i++;
        cs.deleteTag = i++;
        cs.getTags = i++;
        cs.addEnergy = i++;
        cs.getEnergy = i++;
        cs.getServer = i++;
        cs.connectRedirect = i++;
        cs.webSockets = i++;
        cs.eventor = i++;
        cs.webhooks = i++;
        cs.appTotal = i++;
        cs.hardTotal = i;

        dbManager.reportingDBDao.insertStat(region, stat);

        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from reporting_app_stat_minute")) {


            while (rs.next()) {
                assertEquals(region, rs.getString("region"));
                assertEquals((now / AverageAggregatorProcessor.MINUTE) * AverageAggregatorProcessor.MINUTE, rs.getLong("ts"));

                assertEquals(1, rs.getInt("minute_rate"));
                assertEquals(2, rs.getInt("registrations"));
                assertEquals(3, rs.getInt("active"));
                assertEquals(4, rs.getInt("active_week"));
                assertEquals(5, rs.getInt("active_month"));
                assertEquals(6, rs.getInt("connected"));
                assertEquals(7, rs.getInt("online_apps"));
                assertEquals(8, rs.getInt("total_online_apps"));
                assertEquals(9, rs.getInt("online_hards"));
                assertEquals(10, rs.getInt("total_online_hards"));
            }

            connection.commit();
        }

        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from reporting_http_command_stat_minute")) {
            i = 0;
            while (rs.next()) {
                assertEquals(region, rs.getString("region"));
                assertEquals((now / AverageAggregatorProcessor.MINUTE) * AverageAggregatorProcessor.MINUTE, rs.getLong("ts"));

                assertEquals(i++, rs.getInt("is_hardware_connected"));
                assertEquals(i++, rs.getInt("is_app_connected"));
                assertEquals(i++, rs.getInt("get_pin_data"));
                assertEquals(i++, rs.getInt("update_pin"));
                assertEquals(i++, rs.getInt("email"));
                assertEquals(i++, rs.getInt("push"));
                assertEquals(i++, rs.getInt("get_project"));
                assertEquals(i++, rs.getInt("qr"));
                assertEquals(i++, rs.getInt("get_history_pin_data"));
                assertEquals(i++, rs.getInt("total"));
            }

            connection.commit();
        }

        try (Connection connection = dbManager.getConnection();
             Statement statement = connection.createStatement();
             ResultSet rs = statement.executeQuery("select * from reporting_app_command_stat_minute")) {
            i = 0;
            while (rs.next()) {
                assertEquals(region, rs.getString("region"));
                assertEquals((now / AverageAggregatorProcessor.MINUTE) * AverageAggregatorProcessor.MINUTE, rs.getLong("ts"));

                assertEquals(i++, rs.getInt("response"));
                assertEquals(i++, rs.getInt("register"));
                assertEquals(i++, rs.getInt("login"));
                assertEquals(i++, rs.getInt("load_profile"));
                assertEquals(i++, rs.getInt("app_sync"));
                assertEquals(i++, rs.getInt("sharing"));
                assertEquals(i++, rs.getInt("get_token"));
                assertEquals(i++, rs.getInt("ping"));
                assertEquals(i++, rs.getInt("activate"));
                assertEquals(i++, rs.getInt("deactivate"));
                assertEquals(i++, rs.getInt("refresh_token"));
                assertEquals(i++, rs.getInt("get_graph_data"));
                assertEquals(i++, rs.getInt("export_graph_data"));
                assertEquals(i++, rs.getInt("set_widget_property"));
                assertEquals(i++, rs.getInt("bridge"));
                assertEquals(i++, rs.getInt("hardware"));
                assertEquals(i++, rs.getInt("get_share_dash"));
                assertEquals(i++, rs.getInt("get_share_token"));
                assertEquals(i++, rs.getInt("refresh_share_token"));
                assertEquals(i++, rs.getInt("share_login"));
                assertEquals(i++, rs.getInt("create_project"));
                assertEquals(i++, rs.getInt("update_project"));
                assertEquals(i++, rs.getInt("delete_project"));
                assertEquals(i++, rs.getInt("hardware_sync"));
                assertEquals(i++, rs.getInt("internal"));
                assertEquals(i++, rs.getInt("sms"));
                assertEquals(i++, rs.getInt("tweet"));
                assertEquals(i++, rs.getInt("email"));
                assertEquals(i++, rs.getInt("push"));
                assertEquals(i++, rs.getInt("add_push_token"));
                assertEquals(i++, rs.getInt("create_widget"));
                assertEquals(i++, rs.getInt("update_widget"));
                assertEquals(i++, rs.getInt("delete_widget"));
                assertEquals(i++, rs.getInt("create_device"));
                assertEquals(i++, rs.getInt("update_device"));
                assertEquals(i++, rs.getInt("delete_device"));
                assertEquals(i++, rs.getInt("get_devices"));
                assertEquals(i++, rs.getInt("create_tag"));
                assertEquals(i++, rs.getInt("update_tag"));
                assertEquals(i++, rs.getInt("delete_tag"));
                assertEquals(i++, rs.getInt("get_tags"));
                assertEquals(i++, rs.getInt("add_energy"));
                assertEquals(i++, rs.getInt("get_energy"));
                assertEquals(i++, rs.getInt("get_server"));
                assertEquals(i++, rs.getInt("connect_redirect"));
                assertEquals(i++, rs.getInt("web_sockets"));
                assertEquals(i++, rs.getInt("eventor"));
                assertEquals(i++, rs.getInt("webhooks"));
                assertEquals(i++, rs.getInt("appTotal"));
                assertEquals(i, rs.getInt("hardTotal"));
            }

            connection.commit();
        }


    }


}
