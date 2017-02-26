package cc.blynk.integration.http;

import cc.blynk.integration.BaseTest;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.core.BaseServer;
import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClientConfig;
import org.asynchttpclient.Response;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.List;
import java.util.concurrent.Future;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
@Ignore
public class HttpAPIPinsAsyncClientPerformanceTest extends BaseTest {

    private static BaseServer httpServer;
    private static AsyncHttpClient httpclient;
    private static String httpsServerUrl;

    @AfterClass
    public static void shutdown() throws Exception {
        httpclient.close();
        httpServer.close();
    }

    @Before
    public void init() throws Exception {
        if (httpServer == null) {
            httpServer = new HttpAPIServer(holder).start();
            httpsServerUrl = String.format("http://localhost:%s/", 8080);
            httpclient = new DefaultAsyncHttpClient(
                    new DefaultAsyncHttpClientConfig.Builder()
                            .setUserAgent("")
                            .setKeepAlive(true)
                            .build()
            );
        }
    }

    //----------------------------GET METHODS SECTION

    @Test
    public void testPutGetNonExistingPin() throws Exception {
        Future<Response> f = httpclient.preparePut(httpsServerUrl + "9876f5675bcd4b149ffbc918e45c3534/pin/v10")
                .setHeader("Content-Type", "application/json")
                .setBody("[\"100\"]")
                .execute();
        Response response = f.get();

        assertEquals(200, response.getStatusCode());

        while (true) {
            f = httpclient.prepareGet(httpsServerUrl + "9876f5675bcd4b149ffbc918e45c3534/pin/v10").execute();
            response = f.get();

            assertEquals(200, response.getStatusCode());
            List<String> values = consumeJsonPinValues(response.getResponseBody());
            assertEquals(1, values.size());
            assertEquals("100", values.get(0));
            Thread.sleep(50);
        }

    }

}
