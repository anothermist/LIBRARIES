package cc.blynk.integration.http;

import cc.blynk.integration.BaseTest;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.core.BaseServer;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.junit.After;
import org.junit.Before;
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
public class HttpAPIKeepAliveServerTest extends BaseTest {

    private BaseServer httpServer;
    private CloseableHttpClient httpclient;
    private String httpServerUrl;

    @After
    public void shutdown() throws Exception {
        httpclient.close();
        httpServer.close();
    }

    @Before
    public void init() throws Exception {
        httpServer = new HttpAPIServer(holder).start();
        httpServerUrl = String.format("http://localhost:%s/", httpPort);

        //this http client doesn't close HTTP connection.
        httpclient = HttpClients.custom()
                .setConnectionReuseStrategy((response, context) -> true)
                .setKeepAliveStrategy((response, context) -> 10000000).build();
    }

    @Override
    public String getDataFolder() {
        return getRelativeDataFolder("/profiles");
    }

    @Test
    public void testKeepAlive() throws Exception {
        String url = httpServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14";

        HttpPut request = new HttpPut(url);
        request.setHeader("Connection", "keep-alive");

        HttpGet getRequest = new HttpGet(url);
        getRequest.setHeader("Connection", "keep-alive");

        for (int i = 0; i < 100; i++) {
            request.setEntity(new StringEntity("[\""+ i + "\"]", ContentType.APPLICATION_JSON));

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
    }

}
