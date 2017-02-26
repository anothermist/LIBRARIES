package cc.blynk.core.http.rest.params;

import cc.blynk.core.http.rest.URIDecoder;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public class ContextParam extends Param {

    public ContextParam(Class<?> type) {
        super(null, type);
    }

    @Override
    public Object get(URIDecoder uriDecoder) {
        return null;
    }

}
