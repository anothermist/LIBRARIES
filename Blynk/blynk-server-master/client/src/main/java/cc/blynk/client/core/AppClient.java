package cc.blynk.client.core;

import cc.blynk.client.handlers.ClientReplayingMessageDecoder;
import cc.blynk.server.core.protocol.handlers.encoders.MessageEncoder;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.utils.ServerProperties;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;
import io.netty.handler.ssl.SslProvider;
import io.netty.handler.ssl.util.InsecureTrustManagerFactory;

import javax.net.ssl.SSLException;
import java.io.File;
import java.util.Random;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.03.15.
 */
public class AppClient extends BaseClient {

    protected SslContext sslCtx;

    public AppClient(String host, int port) {
        super(host, port, new Random());
    }

    protected AppClient(String host, int port, Random msgIdGenerator, ServerProperties properties) {
        super(host, port, msgIdGenerator, properties);
        log.info("Creating app client. Host {}, sslPort : {}", host, port);
        File serverCert = makeCertificateFile("server.ssl.cert");
        File clientCert = makeCertificateFile("client.ssl.cert");
        File clientKey = makeCertificateFile("client.ssl.key");
        try {
            if (!serverCert.exists() || !clientCert.exists() || !clientKey.exists()) {
                log.info("Enabling one-way auth with no certs checks.");
                this.sslCtx = SslContextBuilder.forClient().sslProvider(SslProvider.JDK)
                        .trustManager(InsecureTrustManagerFactory.INSTANCE)
                        .build();
            } else {
                log.info("Enabling mutual auth.");
                String clientPass = props.getProperty("client.ssl.key.pass");
                this.sslCtx = SslContextBuilder.forClient()
                        .sslProvider(SslProvider.JDK)
                        .trustManager(serverCert)
                        .keyManager(clientCert, clientKey, clientPass)
                        .build();
            }
        } catch (SSLException e) {
            log.error("Error initializing SSL context. Reason : {}", e.getMessage());
            log.debug(e);
            throw new RuntimeException(e);
        }
    }

    private File makeCertificateFile(String propertyName) {
        String path = props.getProperty(propertyName);
        if (path == null || path.isEmpty()) {
            path = "";
        }
        File file = new File(path);
        if (!file.exists()) {
            log.warn("{} file was not found at {} location", propertyName, path);
        }
        return file;
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return new ChannelInitializer<SocketChannel>() {
            @Override
            public void initChannel(SocketChannel ch) throws Exception {
                ChannelPipeline pipeline = ch.pipeline();
                if (sslCtx != null) {
                    pipeline.addLast(sslCtx.newHandler(ch.alloc(), host, port));
                }
                pipeline.addLast(new ClientReplayingMessageDecoder());
                pipeline.addLast(new MessageEncoder(new GlobalStats()));
            }
        };
    }
}
