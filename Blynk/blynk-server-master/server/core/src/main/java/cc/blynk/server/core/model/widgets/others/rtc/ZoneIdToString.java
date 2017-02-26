package cc.blynk.server.core.model.widgets.others.rtc;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.SerializerProvider;

import java.io.IOException;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 04.09.16.
 */
public class ZoneIdToString extends JsonSerializer<Object> {

    @Override
    public void serialize(Object value, JsonGenerator jsonGenerator, SerializerProvider serializers) throws IOException {
        String result = value.toString();
        jsonGenerator.writeObject(result);
    }

}
