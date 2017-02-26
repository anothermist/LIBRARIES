package cc.blynk.server.api.http;

import cc.blynk.core.http.handlers.StaticFile;
import cc.blynk.core.http.handlers.StaticFileEdsWith;
import cc.blynk.core.http.handlers.StaticFileHandler;
import cc.blynk.core.http.handlers.UrlMapperHandler;
import cc.blynk.core.http.rest.HandlerRegistry;
import cc.blynk.server.Holder;
import cc.blynk.server.api.http.handlers.HttpHandler;
import cc.blynk.server.api.http.logic.HttpAPILogic;
import cc.blynk.server.api.http.logic.ResetPasswordLogic;
import cc.blynk.server.api.http.logic.business.HttpBusinessAPILogic;
import cc.blynk.server.api.http.logic.ide.IDEAuthLogic;
import cc.blynk.server.core.BaseServer;
import cc.blynk.utils.FileUtils;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.stream.ChunkedWriteHandler;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 1/12/2015.
 */
public class HttpAPIServer extends BaseServer {

    private final ChannelInitializer<SocketChannel> channelInitializer;

    public HttpAPIServer(Holder holder) {
        super(holder.props.getIntProperty("http.port"), holder.transportTypeHolder);

        HandlerRegistry.register(new ResetPasswordLogic(holder.props, holder.userDao, holder.mailWrapper));
        HandlerRegistry.register(new HttpAPILogic(holder));
        HandlerRegistry.register(new HttpBusinessAPILogic(holder));
        HandlerRegistry.register(new IDEAuthLogic(holder.userDao, holder.redisClient));

        channelInitializer = new ChannelInitializer<SocketChannel>() {
            @Override
            protected void initChannel(SocketChannel ch) throws Exception {
                final ChannelPipeline pipeline = ch.pipeline();
                pipeline.addLast("HttpServerCodec", new HttpServerCodec());
                pipeline.addLast("HttpObjectAggregator", new HttpObjectAggregator(65536, true));
                pipeline.addLast("HttpChunkedWrite", new ChunkedWriteHandler());
                pipeline.addLast("HttpUrlMapper", new UrlMapperHandler("/favicon.ico", "/static/favicon.ico"));
                pipeline.addLast("HttpStaticFile", new StaticFileHandler(true,
                        new StaticFile("/static"), new StaticFileEdsWith(FileUtils.CSV_DIR, ".csv.gz")));
                pipeline.addLast("HttpHandler", new HttpHandler(holder.tokenManager, holder.sessionDao, holder.stats));
            }
        };
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return channelInitializer;
    }

    @Override
    protected String getServerName() {
        return "HTTP API";
    }

    @Override
    public void close() {
        System.out.println("Shutting down HTTP API server...");
        super.close();
    }

}
