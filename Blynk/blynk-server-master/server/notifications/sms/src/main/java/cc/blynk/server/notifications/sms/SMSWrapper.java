package cc.blynk.server.notifications.sms;


import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectReader;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.asynchttpclient.AsyncCompletionHandler;
import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.Param;
import org.asynchttpclient.Response;

import java.util.ArrayList;
import java.util.Properties;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.03.16.
 */
public class SMSWrapper {

    public static final String SMS_PROPERTIES_FILENAME = "sms.properties";

    private static final Logger log = LogManager.getLogger(SMSWrapper.class);

    private final String key;
    private final String secret;
    private final AsyncHttpClient httpclient;

    private final ObjectReader smsResponseReader = new ObjectMapper()
            .configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false)
            .readerFor(SmsResponse.class);

    public SMSWrapper(Properties props, AsyncHttpClient httpclient) {
        this(props.getProperty("nexmo.api.key"), props.getProperty("nexmo.api.secret"), httpclient);
    }

    private SMSWrapper(String key, String secret, AsyncHttpClient httpclient) {
        this.key = key;
        this.secret = secret;
        this.httpclient = httpclient;
    }

    public void send(String to, String text) {
        ArrayList<Param> params = new ArrayList<>();
        params.add(new Param("api_key", key));
        params.add(new Param("api_secret", secret));
        params.add(new Param("from", "Blynk"));
        params.add(new Param("to", to));
        params.add(new Param("text", text));

        httpclient.preparePost("https://rest.nexmo.com/sms/json")
                .setQueryParams(params)
                .execute(new AsyncCompletionHandler<Response>() {
                    @Override
                    public Response onCompleted(org.asynchttpclient.Response response) throws Exception {
                        if (response.getStatusCode() == 200) {
                            SmsResponse smsResponse = smsResponseReader.readValue(response.getResponseBody());
                            if (!smsResponse.messages[0].status.equals("0")) {
                                log.error(smsResponse.messages[0].error);
                            }
                        }
                        return response;
                    }
                });


    }

}
