package cc.blynk.core.http.rest;

import cc.blynk.core.http.MediaType;
import io.netty.buffer.ByteBuf;
import io.netty.handler.codec.http.HttpContent;
import io.netty.handler.codec.http.HttpHeaderNames;
import io.netty.handler.codec.http.HttpMethod;
import io.netty.handler.codec.http.HttpRequest;
import io.netty.handler.codec.http.QueryStringDecoder;
import io.netty.handler.codec.http.multipart.DefaultHttpDataFactory;
import io.netty.handler.codec.http.multipart.HttpPostRequestDecoder;
import io.netty.handler.codec.http.multipart.InterfaceHttpData;

import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Map;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 06.12.15.
 */
public class URIDecoder extends QueryStringDecoder {

    public final String[] paths;
    public Map<String, String> pathData;
    public String contentType;
    public Map<String, String> headers;

    private HttpPostRequestDecoder decoder;
    private ByteBuf bodyData;

    public URIDecoder(HttpRequest httpRequest) {
        super(httpRequest.uri());
        this.paths = path().split("/");
        if (httpRequest.method() == HttpMethod.PUT || httpRequest.method() == HttpMethod.POST) {
            if (httpRequest instanceof HttpContent) {
                this.contentType = httpRequest.headers().get(HttpHeaderNames.CONTENT_TYPE);
                if (contentType != null && contentType.equals(MediaType.APPLICATION_FORM_URLENCODED)) {
                    this.decoder = new HttpPostRequestDecoder(new DefaultHttpDataFactory(false), httpRequest);
                } else {
                    this.bodyData = ((HttpContent) httpRequest).content();
                }
            }
        }
    }

    public List<InterfaceHttpData> getBodyHttpDatas() {
        return decoder.getBodyHttpDatas();
    }

    public String getContentAsString() {
        return bodyData.toString(StandardCharsets.UTF_8);
    }

    public ByteBuf getBodyData() {
        return bodyData;
    }
}
