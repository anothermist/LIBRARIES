package cc.blynk.integration.http;

import cc.blynk.integration.BaseTest;
import cc.blynk.server.Holder;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.api.http.pojo.EmailPojo;
import cc.blynk.server.api.http.pojo.PushMessagePojo;
import cc.blynk.server.core.BaseServer;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.properties.GCMProperties;
import cc.blynk.utils.properties.MailProperties;
import cc.blynk.utils.properties.SmsProperties;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Collections;
import java.util.List;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class HttpAPIPinsTest extends BaseTest {

    private static BaseServer httpServer;
    private static CloseableHttpClient httpclient;
    private static String httpsServerUrl;
    private static Holder localHolder;

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
        httpclient = HttpClients.createDefault();
    }

    @AfterClass
    public static void shutdown() throws Exception {
        httpclient.close();
        httpServer.close();
        localHolder.close();
    }

    //----------------------------GET METHODS SECTION

    @Test
    public void testGetWithFakeToken() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "dsadasddasdasdasdasdasdas/pin/d8");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("Invalid token.", consumeText(response));
        }
    }

    @Test
    public void testGetWithWrongPathToken() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/w/d8");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(404, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testGetWithWrongPin() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/x8");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("Wrong pin format.", consumeText(response));
        }
    }

    @Test
    public void testGetWithNonExistingPin() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v11");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("Requested pin not exists in app.", consumeText(response));
        }
    }

    @Test
    public void testPutGetNonExistingPin() throws Exception {
        HttpPut put = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");
        put.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(put)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpGet get = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");

        try (CloseableHttpResponse response = httpclient.execute(get)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("100", values.get(0));
        }
    }

    @Test
    public void testMultiPutGetNonExistingPin() throws Exception {
        HttpPut put = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");
        put.setEntity(new StringEntity("[\"100\", \"101\", \"102\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(put)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpGet get = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");

        try (CloseableHttpResponse response = httpclient.execute(get)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(3, values.size());
            assertEquals("100", values.get(0));
            assertEquals("101", values.get(1));
            assertEquals("102", values.get(2));
        }
    }

    @Test
    public void testMultiPutGetNonExistingPinWithNewMethod() throws Exception {
        HttpPut put = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/update/v10");
        put.setEntity(new StringEntity("[\"100\", \"101\", \"102\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(put)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpGet get = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/get/v10");

        try (CloseableHttpResponse response = httpclient.execute(get)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(3, values.size());
            assertEquals("100", values.get(0));
            assertEquals("101", values.get(1));
            assertEquals("102", values.get(2));
        }
    }

    @Test
    public void testGetTimerExistingPin() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/D0");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("1", values.get(0));
        }

    }

    @Test
    public void testGetWithExistingPin() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/D8");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("0", values.get(0));
        }

        request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/d1");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("1", values.get(0));
        }

        request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/d3");
        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("87", values.get(0));
        }
    }

    @Test
    public void testGetWithExistingEmptyPin() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(0, values.size());
        }
    }

    @Test
    public void testGetWithExistingMultiPin() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a15");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(2, values.size());
            assertEquals("1", values.get(0));
            assertEquals("2", values.get(1));
        }
    }

    @Test
    public void testGetForRGBMerge() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v13");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(3, values.size());
            assertEquals("60", values.get(0));
            assertEquals("143", values.get(1));
            assertEquals("158", values.get(2));
        }
    }

    @Test
    public void testGetForJoystickMerge() throws Exception {
        HttpGet request = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v14");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(2, values.size());
            assertEquals("128", values.get(0));
            assertEquals("129", values.get(1));
        }
    }

    //----------------------------PUT METHODS SECTION

    @Test
    public void testPutNoContentType() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/d8");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Unexpected content type. Expecting application/json.", consumeText(response));
        }
    }

    @Test
    public void testPutFakeToken() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "dsadasddasdasdasdasdasdas/pin/d8");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());
        request.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("Invalid token.", consumeText(response));
        }
    }

    @Test
    public void testPutWithWrongPin() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/x8");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());
        request.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("Wrong pin format.", consumeText(response));
        }
    }

    @Test
    public void testPutWithNoWidget() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());
        request.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testPutWithNoWidgetNoPinData() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());
        request.setEntity(new StringEntity("[]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("No pin for update provided.", consumeText(response));
        }
    }

    @Test
    public void testPutWithNoWidgetMultivalue() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/v10");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());
        request.setEntity(new StringEntity("[\"100\", \"101\", \"102\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testPutExtraWithNoWidget() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/extra/pin/v10");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());
        request.setEntity(new StringEntity("[{\"timestamp\" : 123, \"value\":\"100\"}]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testPutWithExistingPin() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14");
        request.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpGet getRequest = new HttpGet(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14");

        try (CloseableHttpResponse response = httpclient.execute(getRequest)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("100", values.get(0));
        }
    }

    @Test
    public void testPutWithExistingPinWrongBody() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14");
        request.setEntity(new StringEntity("\"100\"", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Error parsing body param. \"100\"", consumeText(response));
        }
    }

    @Test
    public void testPutWithExistingPinWrongBody2() throws Exception {
        HttpPut request = new HttpPut(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14");
        request.setEntity(new StringEntity("", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Error parsing body param. ", consumeText(response));
        }
    }

    //----------------------------NOTIFICATION POST METHODS SECTION

    //----------------------------pushes
    @Test
    public void testPostNotifyNoContentType() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/notify");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Unexpected content type. Expecting application/json.", consumeText(response));
        }
    }

    @Test
    public void testPostNotifyNoBody() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/notify");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Error parsing body param. ", consumeText(response));
        }
    }

    @Test
    public void testPostNotifyWithWrongBody() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/notify");
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 256; i++) {
            sb.append(i);
        }
        request.setEntity(new StringEntity("{\"body\":\"" + sb.toString() + "\"}", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
            assertEquals("Body is empty or larger than 255 chars.", consumeText(response));
        }
    }

    @Test
    public void testPostNotifyWithBody() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/notify");
        String message = JsonParser.mapper.writeValueAsString(new PushMessagePojo("This is Push Message"));
        request.setEntity(new StringEntity(message, ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }
    }


    //----------------------------email
    @Test
    public void testPostEmailNoContentType() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/email");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Unexpected content type. Expecting application/json.", consumeText(response));
        }
    }

    @Test
    public void testPostEmailNoBody() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/email");
        request.setHeader("Content-Type", ContentType.APPLICATION_JSON.toString());

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(500, response.getStatusLine().getStatusCode());
            assertEquals("Error parsing body param. ", consumeText(response));
        }
    }

    @Test
    public void testPostEmailWithBody() throws Exception {
        HttpPost request = new HttpPost(httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/email");
        String message = JsonParser.mapper.writeValueAsString(new EmailPojo("pupkin@gmail.com", "Title", "Subject"));
        request.setEntity(new StringEntity(message, ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }
    }

    //------------------------------ SYNC TEST
    @Test
    public void testSync() throws Exception {
        String url = httpsServerUrl + "4ae3851817194e2596cf1b7103603ef8/pin/a14";

        HttpPut request = new HttpPut(url);
        HttpGet getRequest = new HttpGet(url);

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
