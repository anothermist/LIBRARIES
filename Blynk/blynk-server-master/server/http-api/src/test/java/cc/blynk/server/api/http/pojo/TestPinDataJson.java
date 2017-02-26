package cc.blynk.server.api.http.pojo;

import cc.blynk.utils.JsonParser;
import org.junit.Test;

import static org.junit.Assert.assertNotNull;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 31.08.16.
 */
public class TestPinDataJson {

    @Test
    public void testParseString() throws Exception {
        String pinDataString = "[{\"timestamp\" : 123, \"value\":\"100\"}]";
        PinData[] pinData = JsonParser.mapper.readValue(pinDataString, PinData[].class);
        assertNotNull(pinData);
    }

}
