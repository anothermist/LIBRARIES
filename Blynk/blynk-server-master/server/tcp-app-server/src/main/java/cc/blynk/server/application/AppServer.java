package cc.blynk.server.application;

import cc.blynk.server.Holder;
import cc.blynk.server.application.handlers.main.AppChannelStateHandler;
import cc.blynk.server.application.handlers.main.auth.AppLoginHandler;
import cc.blynk.server.application.handlers.main.auth.GetServerHandler;
import cc.blynk.server.application.handlers.main.auth.RegisterHandler;
import cc.blynk.server.application.handlers.sharing.auth.AppShareLoginHandler;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.protocol.handlers.decoders.MessageDecoder;
import cc.blynk.server.core.protocol.handlers.encoders.MessageEncoder;
import cc.blynk.server.handlers.common.UserNotLoggedHandler;
import cc.blynk.utils.SslUtil;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.ssl.SslContext;
import io.netty.handler.timeout.ReadTimeoutHandler;

/**
 * Class responsible for handling all Application connections and netty pipeline initialization.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class AppServer extends BaseServer {

    private final ChannelInitializer<SocketChannel> channelInitializer;

    public AppServer(Holder holder) {
        super(holder.props.getIntProperty("app.ssl.port"), holder.transportTypeHolder);

        final String[] loadBalancingIPs = holder.props.getCommaSeparatedValueAsArray("load.balancing.ips");
        final String[] allowedUsers = holder.props.getCommaSeparatedValueAsArray("allowed.users.list");

        final AppChannelStateHandler appChannelStateHandler = new AppChannelStateHandler(holder.sessionDao);
        final RegisterHandler registerHandler = new RegisterHandler(holder.userDao, allowedUsers);
        final AppLoginHandler appLoginHandler = new AppLoginHandler(holder);
        final AppShareLoginHandler appShareLoginHandler = new AppShareLoginHandler(holder);
        final UserNotLoggedHandler userNotLoggedHandler = new UserNotLoggedHandler();
        final GetServerHandler getServerHandler = new GetServerHandler(holder, loadBalancingIPs);

        final SslContext sslCtx = SslUtil.initMutualSslContext(
                holder.props.getProperty("server.ssl.cert"),
                holder.props.getProperty("server.ssl.key"),
                holder.props.getProperty("server.ssl.key.pass"),
                holder.props.getProperty("client.ssl.cert"),
                SslUtil.fetchSslProvider(holder.props));

        int appTimeoutSecs = holder.props.getIntProperty("app.socket.idle.timeout", 0);
        log.debug("app.socket.idle.timeout = {}", appTimeoutSecs);

        this.channelInitializer = new ChannelInitializer<SocketChannel>() {
            @Override
            protected void initChannel(SocketChannel ch) throws Exception {
                final ChannelPipeline pipeline = ch.pipeline();

                if (appTimeoutSecs > 0) {
                    pipeline.addLast("AReadTimeout", new ReadTimeoutHandler(appTimeoutSecs));
                }

                pipeline.addLast("ASSL", sslCtx.newHandler(ch.alloc()));
                pipeline.addLast("AChannelState", appChannelStateHandler);
                pipeline.addLast("AMessageDecoder", new MessageDecoder(holder.stats));
                pipeline.addLast("AMessageEncoder", new MessageEncoder(holder.stats));
                pipeline.addLast("AGetServer", getServerHandler);
                pipeline.addLast("ARegister", registerHandler);
                pipeline.addLast("ALogin", appLoginHandler);
                pipeline.addLast("AShareLogin", appShareLoginHandler);
                pipeline.addLast("ANotLogged", userNotLoggedHandler);
            }
        };
    }

    @Override
    public ChannelInitializer<SocketChannel> getChannelInitializer() {
        return channelInitializer;
    }

    @Override
    protected String getServerName() {
        return "Application";
    }

    @Override
    public void close() {
        System.out.println("Shutting down Application SSL server...");
        super.close();
    }

}
