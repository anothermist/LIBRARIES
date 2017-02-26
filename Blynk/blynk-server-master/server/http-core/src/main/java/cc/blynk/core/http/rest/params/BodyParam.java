package cc.blynk.core.http.rest.params;

import cc.blynk.core.http.MediaType;
import cc.blynk.core.http.rest.URIDecoder;
import cc.blynk.utils.JsonParser;
import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public class BodyParam extends Param {

    private static final Logger log = LogManager.getLogger(BodyParam.class);

    private final String expectedContentType;

    public BodyParam(String name, Class<?> type, String expectedContentType) {
        super(name, type);
        this.expectedContentType = expectedContentType;
    }

    @Override
    public Object get(URIDecoder uriDecoder) {
        if (uriDecoder.contentType == null || !uriDecoder.contentType.contains(expectedContentType)) {
            throw new RuntimeException("Unexpected content type. Expecting " + expectedContentType + ".");
        }

        switch (expectedContentType) {
            case MediaType.APPLICATION_JSON :
                String data = "";
                try {
                    data = uriDecoder.getContentAsString();
                    return JsonParser.mapper.readValue(data, type);
                } catch (JsonParseException | JsonMappingException jsonParseError) {
                    log.warn("Error parsing body param : '{}'.", data);
                    throw new RuntimeException("Error parsing body param. " + data);
                } catch (Exception e) {
                    log.error("Unexpected error during parsing body param.", e);
                    throw new RuntimeException("Unexpected error during parsing body param.", e);
                }
            default :
                return uriDecoder.getContentAsString();
        }
    }

}
