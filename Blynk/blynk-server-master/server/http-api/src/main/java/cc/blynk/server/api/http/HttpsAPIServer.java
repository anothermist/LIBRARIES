package cc.blynk.server.api.http;

import cc.blynk.core.http.handlers.StaticFile;
import cc.blynk.core.http.handlers.StaticFileEdsWith;
import cc.blynk.core.http.handlers.StaticFileHandler;
import cc.blynk.core.http.handlers.UrlMapperHandler;
import cc.blynk.core.http.rest.HandlerRegistry;
import cc.blynk.server.Holder;
import cc.blynk.server.api.http.handlers.HttpHandler;
import cc.blynk.server.api.http.logic.HttpAPILogic;
import cc.blynk.server.api.http.logic.business.AuthCookieHandler;
import cc.blynk.server.api.http.logic.business.AuthHttpHandler;
import cc.blynk.server.api.http.logic.business.BusinessAuthLogic;
import cc.blynk.server.api.http.logic.business.HttpBusinessAPILogic;
import cc.blynk.server.api.http.logic.business.SessionHolder;
import cc.blynk.server.core.BaseServer;
import cc.blynk.utils.FileUtils;
import cc.blynk.utils.SslUtil;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.stream.ChunkedWriteHandler;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 1/12/2015.
 */
public class HttpsAPIServer extends BaseServer {

    private final ChannelInitializer<SocketChannel> channelInitializer;

    public HttpsAPIServer(Holder holder, boolean isUnpacked) {
        super(holder.props.getIntProperty("https.port"), holder.transportTypeHolder);

        HandlerRegistry.register(new HttpAPILogic(holder));
        HandlerRegistry.register(new HttpBusinessAPILogic(holder));

        final String businessRootPath = holder.props.getProperty("business.rootPath", "/business");

        final SessionHolder sessionHolder = new SessionHolder();

        HandlerRegistry.register(businessRootPath, new BusinessAuthLogic(holder.userDao, holder.sessionDao, holder.fileManager, sessionHolder));

        final SslContext sslCtx = SslUtil.initSslContext(
                holder.props.getProperty("https.cert", holder.props.getProperty("server.ssl.cert")),
                holder.props.getProperty("https.key", holder.props.getProperty("server.ssl.key")),
                holder.props.getProperty("https.key.pass", holder.props.getProperty("server.ssl.key.pass")),
                SslUtil.fetchSslProvider(holder.props));

        channelInitializer = new ChannelInitializer<SocketChannel>() {
            @Override
            protected void initChannel(SocketChannel ch) throws Exception {
                final ChannelPipeline pipeline = ch.pipeline();
                pipeline.addLast("HttpsSslContext", sslCtx.newHandler(ch.alloc()));
                pipeline.addLast("HttpsServerCodec", new HttpServerCodec());
                pipeline.addLast("HttpsObjectAggregator", new HttpObjectAggregator(65536, true));
                pipeline.addLast("HttpsChunckedWriter", new ChunkedWriteHandler());

                pipeline.addLast("HttpsAuthCookie", new AuthCookieHandler(businessRootPath, sessionHolder));
                pipeline.addLast("HttpsUrlMapper", new UrlMapperHandler(businessRootPath, "/static/business/business.html"));
                pipeline.addLast("HttpsUrlMapper2", new UrlMapperHandler("/favicon.ico", "/static/favicon.ico"));
                pipeline.addLast("HttpsStaticFile", new StaticFileHandler(isUnpacked,
                        new StaticFile("/static"), new StaticFileEdsWith(FileUtils.CSV_DIR, ".csv.gz")));
                pipeline.addLast("HttpsAuthHandler", new AuthHttpHandler(holder.tokenManager, holder.sessionDao, holder.stats));
                pipeline.addLast("HttpsHandler", new HttpHandler(holder.tokenManager, holder.sessionDao, holder.stats));
            }
        };
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return channelInitializer;
    }

    @Override
    protected String getServerName() {
        return "HTTPS API";
    }

    @Override
    public void close() {
        System.out.println("Shutting down HTTPS API server...");
        super.close();
    }

}
