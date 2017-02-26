package cc.blynk.core.http.rest.params;

import cc.blynk.core.http.rest.URIDecoder;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public abstract class Param {

    protected final String name;

    protected final Class<?> type;

    public Param(String name, Class<?> type) {
        this.name = name;
        this.type = type;
    }

    public abstract Object get(URIDecoder uriDecoder);

    Object convertTo(String value) {
        if (type == long.class) {
            return Long.valueOf(value);
        }
        if (type == int.class || type == Integer.class) {
            return Integer.valueOf(value);
        }
        if (type == short.class || type == Short.class) {
            return Short.valueOf(value);
        }
        if (type == boolean.class) {
            return Boolean.valueOf(value);
        }
        return value;
    }
}
