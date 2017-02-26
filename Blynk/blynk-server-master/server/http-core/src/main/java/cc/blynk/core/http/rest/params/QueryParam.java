package cc.blynk.core.http.rest.params;

import cc.blynk.core.http.rest.URIDecoder;

import java.lang.reflect.Array;
import java.util.List;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public class QueryParam extends Param {

    public QueryParam(String name, Class<?> type) {
        super(name, type);
    }

    @Override
    public Object get(URIDecoder uriDecoder) {
        List<String> params = uriDecoder.parameters().get(name);
        if (params == null) {
            return null;
        }

        if (type == List.class) {
            return params;
        }

        if (type.isArray()) {
            //don't know how to make it better
            if (type.getComponentType() == String.class) {
                return params.toArray((String[]) Array.newInstance(type.getComponentType(), params.size()));
            } else {
                throw new IllegalStateException("Not supported.");
            }
        }

        return convertTo(params.get(0));
    }

}
