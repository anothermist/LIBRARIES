package cc.blynk.integration.https;

import cc.blynk.integration.BaseTest;
import cc.blynk.integration.model.http.ResponseUserEntity;
import cc.blynk.server.admin.http.HttpsAdminServer;
import cc.blynk.server.api.http.HttpAPIServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.utils.JsonParser;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class HttpsAdminServerTest extends BaseTest {

    private static BaseServer httpServer;
    private BaseServer httpAdminServer;
    private CloseableHttpClient httpclient;
    private String httpsAdminServerUrl;
    private String httpServerUrl;

    //administration ports
    public static int administrationPort;

    @After
    public void shutdown() {
        httpAdminServer.close();
        httpServer.close();
    }

    @Before
    public void init() throws Exception {
        administrationPort = properties.getIntProperty("administration.https.port");
        this.httpAdminServer = new HttpsAdminServer(holder, true).start();

        httpsAdminServerUrl = String.format("https://localhost:%s/admin", administrationPort);
        httpServerUrl = String.format("http://localhost:%s/", httpPort);

        SSLContext sslcontext = initUnsecuredSSLContext();

        // Allow TLSv1 protocol only
        SSLConnectionSocketFactory sslsf = new SSLConnectionSocketFactory(sslcontext, new MyHostVerifier());
        this.httpclient = HttpClients.custom().setSSLSocketFactory(sslsf).build();

        httpServer = new HttpAPIServer(holder).start();
    }

    @Override
    public String getDataFolder() {
        return getRelativeDataFolder("/profiles");
    }

    private SSLContext initUnsecuredSSLContext() throws NoSuchAlgorithmException, KeyManagementException {
        X509TrustManager tm = new X509TrustManager() {
            @Override
            public void checkClientTrusted(java.security.cert.X509Certificate[] x509Certificates, String s) throws java.security.cert.CertificateException {

            }

            @Override
            public void checkServerTrusted(java.security.cert.X509Certificate[] x509Certificates, String s) throws java.security.cert.CertificateException {

            }

            @Override
            public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                return null;
            }
        };

        SSLContext context = SSLContext.getInstance("TLS");
        context.init(null, new TrustManager[]{ tm }, null);

        return context;
    }

    @Test
    public void testChangePassNoUser() throws Exception {
        String testUser = "dima@dima.ua";
        HttpPut request = new HttpPut(httpsAdminServerUrl + "/users/" + "xxx/" + testUser);
        request.setEntity(new StringEntity(new ResponseUserEntity("123").toString(), ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(404, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testGetUserFromAdminPage() throws Exception {
        String testUser = "dmitriy@blynk.cc";
        String appName = "Blynk";
        HttpGet request = new HttpGet(httpsAdminServerUrl + "/users/" + testUser + "-" + appName);

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            String jsonProfile = consumeText(response);
            assertNotNull(jsonProfile);
            User user = JsonParser.readAny(jsonProfile, User.class);
            assertNotNull(user);
            assertEquals(testUser, user.name);
            assertNotNull(user.profile.dashBoards);
            assertEquals(5, user.profile.dashBoards.length);
        }
    }

    @Test
    public void testAssignNewTokenForNonExistingToken() throws Exception {
        HttpGet request = new HttpGet(httpsAdminServerUrl + "/users/token/assign?old=123&new=123");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testAssignNewToken() throws Exception {
        HttpGet request = new HttpGet(httpsAdminServerUrl + "/users/token/assign?old=4ae3851817194e2596cf1b7103603ef8&new=123");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpPut put = new HttpPut(httpServerUrl + "123/pin/v10");
        put.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(put)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpGet get = new HttpGet(httpServerUrl + "123/pin/v10");

        try (CloseableHttpResponse response = httpclient.execute(get)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("100", values.get(0));
        }

        request = new HttpGet(httpsAdminServerUrl + "/users/token/assign?old=4ae3851817194e2596cf1b7103603ef8&new=124");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(400, response.getStatusLine().getStatusCode());
        }
    }

    @Test
    public void testForceAssignNewToken() throws Exception {
        HttpGet request = new HttpGet(httpsAdminServerUrl + "/users/token/force?username=dmitriy@blynk.cc&app=Blynk&dashId=79780619&deviceId=0&new=123");

        try (CloseableHttpResponse response = httpclient.execute(request)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpPut put = new HttpPut(httpServerUrl + "123/pin/v10");
        put.setEntity(new StringEntity("[\"100\"]", ContentType.APPLICATION_JSON));

        try (CloseableHttpResponse response = httpclient.execute(put)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
        }

        HttpGet get = new HttpGet(httpServerUrl + "123/pin/v10");

        try (CloseableHttpResponse response = httpclient.execute(get)) {
            assertEquals(200, response.getStatusLine().getStatusCode());
            List<String> values = consumeJsonPinValues(response);
            assertEquals(1, values.size());
            assertEquals("100", values.get(0));
        }
    }

    private class MyHostVerifier implements HostnameVerifier {
        @Override
        public boolean verify(String s, SSLSession sslSession) {
            return true;
        }
    }

}
