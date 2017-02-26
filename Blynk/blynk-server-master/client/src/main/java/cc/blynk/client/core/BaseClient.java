package cc.blynk.client.core;

import cc.blynk.client.CommandParser;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.model.messages.MessageBase;
import cc.blynk.utils.SHA256Util;
import cc.blynk.utils.ServerProperties;
import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ConnectTimeoutException;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.channels.UnresolvedAddressException;
import java.util.Collections;
import java.util.Random;

import static cc.blynk.server.core.protocol.model.messages.MessageFactory.produce;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 1/31/2015.
 */
public abstract class BaseClient {

    protected static final Logger log = LogManager.getLogger(BaseClient.class);

    protected final ServerProperties props;
    protected final String host;
    protected final int port;
    protected final Random random;
    protected Channel channel;
    protected NioEventLoopGroup nioEventLoopGroup;

    public BaseClient(String host, int port, Random messageIdGenerator) {
        this(host, port, messageIdGenerator, new NioEventLoopGroup(1));
    }

    public BaseClient(String host, int port, Random messageIdGenerator, ServerProperties serverProperties) {
        this.host = host;
        this.port = port;
        this.random = messageIdGenerator;
        this.props = serverProperties;
        this.nioEventLoopGroup = new NioEventLoopGroup(1);
    }

    public BaseClient(String host, int port, Random messageIdGenerator, NioEventLoopGroup nioEventLoopGroup) {
        this.host = host;
        this.port = port;
        this.random = messageIdGenerator;
        this.props = new ServerProperties(Collections.emptyMap());
        this.nioEventLoopGroup = nioEventLoopGroup;
    }

    public static MessageBase produceMessageBaseOnUserInput(String line, int msgId) {
        String[] input = line.split(" ", 2);

        short command;

        try {
            command = CommandParser.parseCommand(input[0]);
        } catch (IllegalArgumentException e) {
            log.error("Command not supported {}", input[0]);
            return null;
        }

        String body = input.length == 1 ? "" : input[1];
        if (command == Command.REGISTER || command == Command.LOGIN) {
            String[] userPass = body.split(" ", 3);
            if (userPass.length > 1) {
                String username = userPass[0];
                String pass = userPass[1];
                body = username + "\0" + SHA256Util.makeHash(pass, username) + (userPass.length == 3 ? "\0" + userPass[2].replaceAll(" ", "\0") : "");
            }
        }
        if (command == Command.SHARE_LOGIN || command == Command.GET_GRAPH_DATA) {
            body = body.replaceAll(" ", "\0");
        }
        if (command == Command.HARDWARE || command == Command.BRIDGE || command == Command.EMAIL ||
                command == Command.SHARING || command == Command.EXPORT_GRAPH_DATA || command == Command.SET_WIDGET_PROPERTY
                || command == Command.HARDWARE_SYNC) {
            body = body.replaceAll(" ", "\0");
        }
        return produce(msgId, command, body);
    }

    public void start(BufferedReader commandInputStream) {
        this.nioEventLoopGroup = new NioEventLoopGroup(1);
        try {
            Bootstrap b = new Bootstrap();
            b.group(nioEventLoopGroup)
                    .channel(NioSocketChannel.class)
                    .option(ChannelOption.SO_KEEPALIVE, true)
                    .handler(getChannelInitializer());

            // Start the connection attempt.
            this.channel = b.connect(host, port).sync().channel();
            readUserInput(commandInputStream);
        } catch (UnresolvedAddressException uae) {
            log.error("Host name '{}' is invalid. Please make sure it is correct name.", host);
        } catch (ConnectTimeoutException cte) {
            log.error("Timeout exceeded when connecting to '{}:{}'. Please make sure host available and port is open on target host.", host, port);
        } catch (IOException | InterruptedException e) {
            log.error("Error running client. Shutting down.", e);
        } catch (Exception e) {
            log.error(e);
        } finally {
            // The connection is closed automatically on shutdown.
            nioEventLoopGroup.shutdownGracefully();
        }
    }

    public void start() {
        Bootstrap b = new Bootstrap();
        b.group(nioEventLoopGroup).channel(NioSocketChannel.class).handler(getChannelInitializer());

        try {
            // Start the connection attempt.
            this.channel = b.connect(host, port).sync().channel();
        } catch (InterruptedException e) {
            log.error(e);
        }
    }

    protected abstract ChannelInitializer<SocketChannel> getChannelInitializer();

    private void readUserInput(BufferedReader commandInputStream) throws IOException {
        String line;
        while ((line = commandInputStream.readLine()) != null) {
            // If user typed the 'quit' command, wait until the server closes the connection.
            if ("quit".equals(line.toLowerCase())) {
                log.info("Got 'quit' command. Closing client.");
                channel.close();
                break;
            }

            MessageBase msg = produceMessageBaseOnUserInput(line, (short) random.nextInt(Short.MAX_VALUE));
            if (msg == null) {
                continue;
            }

            send(msg);
        }
    }

    public void send(Object msg) {
        channel.writeAndFlush(msg);
    }

    public boolean isClosed() {
        return !channel.isOpen();
    }

    public ChannelFuture stop() {
        ChannelFuture channelFuture = channel.close().awaitUninterruptibly();
        nioEventLoopGroup.shutdownGracefully();
        return channelFuture;
    }
}
