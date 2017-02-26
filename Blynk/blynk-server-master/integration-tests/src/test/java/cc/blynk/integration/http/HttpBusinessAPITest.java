package cc.blynk.integration.http;

import cc.blynk.integration.BaseTest;
import cc.blynk.server.Holder;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.utils.properties.GCMProperties;
import cc.blynk.utils.properties.MailProperties;
import cc.blynk.utils.properties.SmsProperties;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Collections;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class HttpBusinessAPITest extends BaseTest {

    private static BaseServer httpServer;
    private static CloseableHttpClient httpclient;
    private static String httpsServerUrl;
    private static Holder localHolder;

    @AfterClass
    public static void shutdown() throws Exception {
        httpclient.close();
        httpServer.close();
        localHolder.close();
    }

    @BeforeClass
    public static void init() throws Exception {
        properties.setProperty("data.folder", getRelativeDataFolder("/business_profile"));
        localHolder = new Holder(properties,
                new MailProperties(Collections.emptyMap()),
                new SmsProperties(Collections.emptyMap()),
                new GCMProperties(Collections.emptyMap())
        );
        httpServer = new HttpAPIServer(localHolder).start();
        httpsServerUrl = String.format("http://localhost:%s/0130aceeb3864280b863c118eb84a8df/query", httpPort);
        httpclient = HttpClients.createDefault();
    }

    //----------------------------GET METHODS SECTION

    @Test
    public void testNoDataProfile() throws Exception {
        HttpGet request = new HttpGet(String.format("http://localhost:%s/1/query", httpPort) + "?groupBy=name&aggregation=count&pin=V1&value=1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[]", result);
        }
    }

    @Test
    public void testGroupByAndCount() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "?groupBy=name&aggregation=count&pin=V1&value=1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[{\"name\":\"parking1\",\"count\":2},{\"name\":\"parking2\",\"count\":1}]", result);
        }

        request = new HttpGet(httpsServerUrl + "?groupBy=name&aggregation=count&value=1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[{\"name\":\"parking1\",\"count\":2},{\"name\":\"parking2\",\"count\":1}]", result);
        }
    }

    @Test
    @Ignore
    public void testGroupByMultipleFieldsAndCount() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "?groupBy=name&groupBy=lat&aggregation=count&pin=V1&value=1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[{\"name\":\"parking2\",\"count\":1,\"lat\":50.4601},{\"name\":\"parking1\",\"count\":2,\"lat\":50.4501}]", result);
        }

        request = new HttpGet(httpsServerUrl + "?groupBy=name&groupBy=lat&groupBy=lon&aggregation=count&pin=V1&value=1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[{\"name\":\"parking1\",\"count\":2,\"lon\":30.5234,\"lat\":50.4501},{\"name\":\"parking2\",\"count\":1,\"lon\":30.5334,\"lat\":50.4601}]", result);
        }
    }

    @Test
    @Ignore
    public void testFilterByValue() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "?value=1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[{\"id\":1,\"name\":\"parking1\",\"metadata\":{\"group\":1,\"lat\":50.4501,\"lon\":30.5234},\"pins\":[{\"value\":\"1\",\"pin\":1,\"pinType\":\"VIRTUAL\"}]},{\"id\":2,\"name\":\"parking1\",\"metadata\":{\"group\":2,\"lat\":50.4501,\"lon\":30.5234},\"pins\":[{\"value\":\"1\",\"pin\":1,\"pinType\":\"VIRTUAL\"}]},{\"id\":3,\"name\":\"parking2\",\"metadata\":{\"group\":3,\"lat\":50.4601,\"lon\":30.5334},\"pins\":[{\"value\":\"1\",\"pin\":1,\"pinType\":\"VIRTUAL\"}]}]", result);
        }
    }

    @Test
    public void testFilterByValueNoResults() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "?value=0");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[]", result);
        }
    }

    @Test
    public void testNoData() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "?groupBy=name&aggregation=count&pin=V1&value=0");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[]", result);
        }
    }

    @Test
    @Ignore
    public void testGetSpecificParking() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "?name=parking2");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String result = consumeText(response);
            assertNotNull(result);
            assertEquals("[{\"id\":3,\"name\":\"parking2\",\"metadata\":{\"group\":3,\"lat\":50.4601,\"lon\":30.5334},\"pins\":[{\"value\":\"1\",\"pin\":1,\"pinType\":\"VIRTUAL\"}]}]", result);
        }
    }

}
