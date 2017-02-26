package cc.blynk.integration.http;

import cc.blynk.integration.BaseTest;
import cc.blynk.server.Holder;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.utils.properties.GCMProperties;
import cc.blynk.utils.properties.MailProperties;
import cc.blynk.utils.properties.SmsProperties;
import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClientConfig;
import org.asynchttpclient.Response;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Future;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class HttpAPIPinsAsyncClientTest extends BaseTest {

    private static BaseServer httpServer;
    private static AsyncHttpClient httpclient;
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
        properties.setProperty("data.folder", getRelativeDataFolder("/profiles"));
        localHolder = new Holder(properties,
                new MailProperties(Collections.emptyMap()),
                new SmsProperties(Collections.emptyMap()),
                new GCMProperties(Collections.emptyMap())
        );
        httpServer = new HttpAPIServer(localHolder).start();
        httpsServerUrl = String.format("http://localhost:%s/", httpPort);
        httpclient = new DefaultAsyncHttpClient(
                new DefaultAsyncHttpClientConfig.Builder()
                        .setUserAgent(null)
                        .setKeepAlive(false)
                        .build()
        );
    }

    //----------------------------GET METHODS SECTION

    @Test
    public void testGetWithFakeToken() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "dsadasddasdasdasdasdasdas/get/d8").execute();
        Response response = f.get();
        assertEquals(400, response.getStatusCode());
        assertEquals("Invalid token.", response.getResponseBody());
    }

    @Test
    public void testGetWithWrongPathToken() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/w/d8").execute();
        assertEquals(404, f.get().getStatusCode());
    }

    @Test
    public void testGetWithWrongPin() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/x8").execute();
        Response response = f.get();
        assertEquals(400, response.getStatusCode());
        assertEquals("Wrong pin format.", response.getResponseBody());
    }

    @Test
    public void testGetWithNonExistingPin() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/v11").execute();
        Response response = f.get();
        assertEquals(400, response.getStatusCode());
        assertEquals("Requested pin not exists in app.", response.getResponseBody());
    }

    @Test
    public void testPutViaGetRequestSingleValue() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/update/v11?value=10").execute();
        Response response = f.get();
        assertEquals(200, response.getStatusCode());


        f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/v11").execute();
        response = f.get();

        assertEquals(200, response.getStatusCode());
        List<String> values = consumeJsonPinValues(response.getResponseBody());
        assertEquals(1, values.size());
        assertEquals("10", values.get(0));
    }

    @Test
    public void testPutViaGetRequestMultipleValue() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/update/v11?value=10&value=11").execute();
        Response response = f.get();
        assertEquals(200, response.getStatusCode());


        f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/v11").execute();
        response = f.get();

        assertEquals(200, response.getStatusCode());
        List<String> values = consumeJsonPinValues(response.getResponseBody());
        assertEquals(2, values.size());
        assertEquals("10", values.get(0));
        assertEquals("11", values.get(1));
    }

    @Test
    public void testPutGetNonExistingPin() throws Exception {
        Future<Response> f = httpclient.preparePut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10")
                .setHeader("Content-Type", "application/json")
                .setBody("[\"100\"]")
                .execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());

        f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/v10").execute();
        response = f.get();

        assertEquals(200, response.getStatusCode());
        List<String> values = consumeJsonPinValues(response.getResponseBody());
        assertEquals(1, values.size());
        assertEquals("100", values.get(0));

    }

    @Test
    public void testMultiPutGetNonExistingPin() throws Exception {
        Future<Response> f = httpclient.preparePut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10")
                .setHeader("Content-Type", "application/json")
                .setBody("[\"100\", \"101\", \"102\"]")
                .execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());

        f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/v10").execute();
        response = f.get();

        assertEquals(200, response.getStatusCode());
        List<String> values = consumeJsonPinValues(response.getResponseBody());
        assertEquals(3, values.size());
        assertEquals("100", values.get(0));
        assertEquals("101", values.get(1));
        assertEquals("102", values.get(2));
    }

    @Test
    public void testGetPinData() throws Exception {
        Future<Response> f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/update/v111?value=10").execute();
        Response response = f.get();
        assertEquals(200, response.getStatusCode());

        f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/data/v111").execute();
        response = f.get();
        assertEquals(400, response.getStatusCode());
        assertEquals("No data for pin.", response.getResponseBody());

        f = httpclient.prepareGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/data/z111").execute();
        response = f.get();
        assertEquals(400, response.getStatusCode());
        assertEquals("Wrong pin format.", response.getResponseBody());
    }

}
