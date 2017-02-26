package cc.blynk.integration.http;

import cc.blynk.integration.BaseTest;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.List;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.01.16.
 */
@RunWith(MockitoJUnitRunner.class)
@Ignore
public class HttpAPIPerformanceTest extends BaseTest {

    private static String httpsServerUrl;

    @Before
    public void init() throws Exception {
        httpsServerUrl = "http://104.40.224.211/";

    }

    @Test
    public void testKeepAlive() throws Exception {
        makeTest();
    }

    private void makeTest() {
        String url = httpsServerUrl + "6896a7fc35e24524a18d712de4936d62/pin/v0";

        HttpPut request = new HttpPut(url);
        request.setHeader("Connection", "keep-alive");

        HttpGet getRequest = new HttpGet(url);
        getRequest.setHeader("Connection", "keep-alive");

        CloseableHttpClient httpclient = HttpClients.custom()
                .setConnectionReuseStrategy((response, context) -> true)
                .setKeepAliveStrategy((response, context) -> 10000000).build();

        try {
            int i = 0;
            while (true) {
                request.setEntity(new StringEntity("[\"" + ++i + "\"]", ContentType.APPLICATION_JSON));

                try (CloseableHttpResponse response = httpclient.execute(request)) {
                    assertEquals(200, response.getStatusLine().getStatusCode());
                    EntityUtils.consume(response.getEntity());
                }

                try (CloseableHttpResponse response2 = httpclient.execute(getRequest)) {
                    assertEquals(200, response2.getStatusLine().getStatusCode());
                    List<String> values = consumeJsonPinValues(response2);
                    assertEquals(1, values.size());
                    assertEquals(String.valueOf(i), values.get(0));
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
