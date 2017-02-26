package cc.blynk.utils;

import io.netty.handler.ssl.ClientAuth;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.ssl.SslContextBuilder;
import io.netty.handler.ssl.SslProvider;
import io.netty.handler.ssl.util.SelfSignedCertificate;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.net.ssl.SSLException;
import java.io.File;
import java.security.cert.CertificateException;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 28.09.15.
 */
public class SslUtil {

    private final static Logger log = LogManager.getLogger(SslUtil.class);

    public static SslContext initMutualSslContext(String serverCertPath, String serverKeyPath, String serverPass,
                                                  String clientCertPath,
                                                  SslProvider sslProvider) {
        try {
            File serverCert = new File(serverCertPath);
            File serverKey = new File(serverKeyPath);
            File clientCert = new File(clientCertPath);

            if (!serverCert.exists() || !serverKey.exists()) {
                log.warn("ATTENTION. Server certificate paths cert : '{}', key : '{}' - not valid. Using embedded server certs and one way ssl. This is not secure. Please replace it with your own certs.",
                        serverCert.getAbsolutePath(), serverKey.getAbsolutePath());

                return build(sslProvider);
            }

            if (!clientCert.exists()) {
                log.warn("Found server certificates but no client certificate for '{}' path. Using one way ssl.", clientCert.getAbsolutePath());

                return build(serverCert, serverKey, serverPass, sslProvider);
            }

            return build(serverCert, serverKey, serverPass, sslProvider, clientCert);
        } catch (CertificateException | SSLException | IllegalArgumentException e) {
            log.error("Error initializing ssl context. Reason : {}", e.getMessage());
            throw new RuntimeException(e.getMessage());
        }
    }

    public static SslProvider fetchSslProvider(ServerProperties props) {
        return props.getBoolProperty("enable.native.openssl") ? SslProvider.OPENSSL : SslProvider.JDK;
    }

    public static SslContext initSslContext(String serverCertPath, String serverKeyPath, String serverPass,
                                            SslProvider sslProvider) {
        try {
            File serverCert =  new File(serverCertPath);
            File serverKey = new File(serverKeyPath);

            if (!serverCert.exists() || !serverKey.exists()) {
                log.warn("ATTENTION. Certificate {} and key {} paths not valid. Using embedded certs. This is not secure. Please replace it with your own certs.",
                        serverCert.getAbsolutePath(), serverKey.getAbsolutePath());
                return SslUtil.build(sslProvider);
            }

            return SslUtil.build(serverCert, serverKey, serverPass, sslProvider);
        } catch (CertificateException | SSLException | IllegalArgumentException e) {
            log.error("Error initializing ssl context. Reason : {}", e.getMessage());
            throw new RuntimeException(e.getMessage());
        }
    }

    public static SslContext build(SslProvider sslProvider) throws CertificateException, SSLException {
        SelfSignedCertificate ssc = new SelfSignedCertificate();
        return SslContextBuilder.forServer(ssc.certificate(), ssc.privateKey())
                .sslProvider(sslProvider)
                .build();
    }

    public static SslContext build(File serverCert, File serverKey, String serverPass, SslProvider sslProvider) throws SSLException {
        return SslContextBuilder.forServer(serverCert, serverKey, serverPass)
                .sslProvider(sslProvider)
                .build();
    }

    public static SslContext build(File serverCert, File serverKey, String serverPass, SslProvider sslProvider, File clientCert) throws SSLException {
        return SslContextBuilder.forServer(serverCert, serverKey, serverPass)
                .sslProvider(sslProvider)
                .trustManager(clientCert)
                //todo fix that after iOS release
                .clientAuth(ClientAuth.NONE)
                .build();
    }

}
