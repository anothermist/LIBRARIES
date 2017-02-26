package cc.blynk.core.http.handlers;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;
import io.netty.handler.codec.http.FullHttpRequest;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.05.16.
 */
public class UrlMapperHandler extends ChannelInboundHandlerAdapter {

    private final String initUrl;
    private final String mapToUrl;

    public UrlMapperHandler(String initUrl, String mapToUrl) {
        this.initUrl = initUrl;
        this.mapToUrl = mapToUrl;
    }

    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        if (msg instanceof FullHttpRequest) {
            FullHttpRequest request = (FullHttpRequest) msg;
            if (request.uri().equals(initUrl)) {
                request.setUri(mapToUrl);
            }
        }

        super.channelRead(ctx, msg);
    }

}
