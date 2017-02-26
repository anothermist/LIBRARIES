package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.server.Holder;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.GetServerMessage;
import cc.blynk.server.hardware.HardwareServer;
import cc.blynk.utils.ServerProperties;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;
import redis.clients.jedis.Jedis;

import static cc.blynk.server.core.protocol.enums.Response.DEVICE_NOT_IN_NETWORK;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 5/09/2016.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class LoadBalancingIntegrationTest extends IntegrationBase {

    private BaseServer appServer1;
    private BaseServer hardwareServer1;

    private BaseServer appServer2;
    private BaseServer hardwareServer2;
    private int tcpAppPort2;
    private int plainHardPort2;

    @Before
    public void init() throws Exception {
        hardwareServer1 = new HardwareServer(holder).start();
        appServer1 = new AppServer(holder).start();

        try (Jedis jedis = holder.redisClient.getTokenPool().getResource()) {
            jedis.flushDB();
        }
        try (Jedis jedis = holder.redisClient.getUserPool().getResource()) {
            jedis.flushDB();
        }

        ServerProperties properties2 = new ServerProperties("server2.properties");
        Holder holder2 = new Holder(properties2, twitterWrapper, mailWrapper, gcmWrapper, smsWrapper);
        hardwareServer2 = new HardwareServer(holder2).start();
        appServer2 = new AppServer(holder2).start();
        plainHardPort2 = properties2.getIntProperty("hardware.default.port");
        tcpAppPort2 = properties2.getIntProperty("app.ssl.port");
    }

    @After
    public void shutdown() {
        appServer1.close();
        hardwareServer1.close();

        appServer2.close();
        hardwareServer2.close();
    }

    @Test
    public void test2NewUsersStoredOnDifferentServers() throws Exception {
        TestAppClient appClient1 = new TestAppClient("localhost", tcpAppPort, properties);
        appClient1.start();

        String username = "test_new@gmail.com";
        String pass = "a";
        String appName = "Blynk";

        appClient1.send("getServer " + username + "\0" + appName);
        verify(appClient1.responseMock, timeout(1000)).channelRead(any(), eq(new GetServerMessage(1, "127.0.0.1")));

        appClient1.reset();

        String token = workflowForUser(appClient1, username, pass, appName);
        assertEquals("127.0.0.1", holder.redisClient.getServerByToken(token));
        assertEquals("127.0.0.1", holder.redisClient.getServerByUser(username));

        TestAppClient appClient2 = new TestAppClient("localhost", tcpAppPort2, properties);
        appClient2.start();

        String username2 = "test2_new@gmail.com";

        appClient2.send("getServer " + username2 + "\0" + appName);
        verify(appClient2.responseMock, timeout(1000)).channelRead(any(), eq(new GetServerMessage(1, "localhost")));

        appClient2.reset();


        String token2 = workflowForUser(appClient2, username2, pass, appName);
        assertEquals("localhost", holder.redisClient.getServerByToken(token2));
        assertEquals("localhost", holder.redisClient.getServerByUser(username2));
    }

    private String workflowForUser(TestAppClient appClient, String username, String pass, String appName) throws Exception{
        appClient.send("register " + username + " " + pass + " " + appName);
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(1, OK)));
        appClient.send("login " + username + " " + pass + " Android 1.10.4 " + appName);
        //we should wait until login finished. Only after that we can send commands
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(2, OK)));

        DashBoard dash = new DashBoard();
        dash.id = 1;
        dash.name = "test";
        appClient.send("createDash " + dash.toString());
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(3, OK)));
        appClient.send("activate 1");
        verify(appClient.responseMock, timeout(1000)).channelRead(any(), eq(new ResponseMessage(4, DEVICE_NOT_IN_NETWORK)));

        appClient.reset();
        appClient.send("getToken 1");

        String token = appClient.getBody();
        assertNotNull(token);
        return token;
    }

}
