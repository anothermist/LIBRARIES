package cc.blynk.core.http.rest.params;

import cc.blynk.core.http.rest.URIDecoder;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public class PathParam extends Param {

    public PathParam(String name, Class<?> type) {
        super(name, type);
    }

    @Override
    public Object get(URIDecoder uriDecoder) {
        return convertTo(uriDecoder.pathData.get(name));
    }

}
