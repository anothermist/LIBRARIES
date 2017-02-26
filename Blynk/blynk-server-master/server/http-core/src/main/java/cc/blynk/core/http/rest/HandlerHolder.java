package cc.blynk.core.http.rest;

import cc.blynk.core.http.UriTemplate;
import cc.blynk.core.http.annotation.DELETE;
import cc.blynk.core.http.annotation.GET;
import cc.blynk.core.http.annotation.POST;
import cc.blynk.core.http.annotation.PUT;
import cc.blynk.core.http.rest.params.Param;
import io.netty.handler.codec.http.HttpMethod;

import java.lang.reflect.Method;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public class HandlerHolder {

    public final UriTemplate uriTemplate;

    public HttpMethod httpMethod;

    public final Method method;

    public final Object handler;

    public final Param[] params;

    public HandlerHolder(UriTemplate uriTemplate, Method method, Object handler, int paramsCount) {
        this.uriTemplate = uriTemplate;
        this.method = method;
        this.handler = handler;

        if (method.isAnnotationPresent(GET.class)) {
            this.httpMethod = HttpMethod.GET;
        }
        if (method.isAnnotationPresent(POST.class)) {
            this.httpMethod = HttpMethod.POST;
        }
        if (method.isAnnotationPresent(PUT.class)) {
            this.httpMethod = HttpMethod.PUT;
        }
        if (method.isAnnotationPresent(DELETE.class)) {
            this.httpMethod = HttpMethod.DELETE;
        }

        this.params = new Param[paramsCount];
    }

    public Object[] fetchParams(URIDecoder uriDecoder) {
        Object[] res = new Object[params.length];
        for (int i = 0; i < params.length; i++) {
            res[i] = params[i].get(uriDecoder);
        }

        return res;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        HandlerHolder that = (HandlerHolder) o;

        if (uriTemplate != null ? !uriTemplate.equals(that.uriTemplate) : that.uriTemplate != null) return false;
        if (httpMethod != null ? !httpMethod.equals(that.httpMethod) : that.httpMethod != null) return false;
        return !(method != null ? !method.equals(that.method) : that.method != null);

    }

    @Override
    public int hashCode() {
        int result = uriTemplate != null ? uriTemplate.hashCode() : 0;
        result = 31 * result + (httpMethod != null ? httpMethod.hashCode() : 0);
        result = 31 * result + (method != null ? method.hashCode() : 0);
        return result;
    }
}
