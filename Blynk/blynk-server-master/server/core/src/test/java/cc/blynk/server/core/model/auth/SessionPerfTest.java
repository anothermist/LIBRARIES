package cc.blynk.server.core.model.auth;

import cc.blynk.server.Limits;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import cc.blynk.utils.ServerProperties;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPromise;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.config.Configuration;
import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.BenchmarkMode;
import org.openjdk.jmh.annotations.Fork;
import org.openjdk.jmh.annotations.Measurement;
import org.openjdk.jmh.annotations.Mode;
import org.openjdk.jmh.annotations.OutputTimeUnit;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.annotations.TearDown;
import org.openjdk.jmh.annotations.Warmup;

import java.util.Collections;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 5, time = 1, timeUnit = TimeUnit.SECONDS)
@Measurement(iterations = 5, time = 1, timeUnit = TimeUnit.SECONDS)
public class SessionPerfTest {
    static final int DASH_ID = 1;

    Session session1;
    Session session2;
    Session session3;
    Session session4;
    int i1;
    int i2;
    int i3;
    int i4;

    @Setup
    public void setup() {
        // disable logging
        LoggerContext ctx = (LoggerContext) LogManager.getContext(false);
        Configuration conf = ctx.getConfiguration();
        conf.getLoggerConfig(LogManager.ROOT_LOGGER_NAME).setLevel(org.apache.logging.log4j.Level.OFF);
        ctx.updateLoggers(conf);

        // create handler with HardwareStateHolder
        User user = new User();
        user.dashTokens.putIfAbsent(DASH_ID, "1");

        HardwareStateHolder hardwareStateHolder = new HardwareStateHolder(0, DASH_ID, user, "1");

        // create 1 hardware channel and a session
        session1 = new Session(null);
        session1.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));

        // create two hardware channels and a session
        session2 = new Session(null);
        session2.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
        session2.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));

        // create 3 hardware channels and a session
        session3 = new Session(null);
        session3.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
        session3.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
        session3.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));

        // create 4 hardware channels and a session
        session4 = new Session(null);
        session4.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
        session4.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
        session4.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
        session4.hardwareChannels.add(new EmbeddedChannel(newChannelHandler(hardwareStateHolder)));
    }

    @Benchmark
    public void sendMessageToHardware_1Channel() {
        session1.sendMessageToHardware(DASH_ID, (short) i1, i1, "body:" + (i1++));
    }

    @Benchmark
    public void sendMessageToHardware_2Channels() {
        session2.sendMessageToHardware(DASH_ID, (short) i2, i2, "body:" + (i2++));
    }

    @Benchmark
    public void sendMessageToHardware_3Channels() {
        session3.sendMessageToHardware(DASH_ID, (short) i3, i3, "body:" + (i3++));
    }

    @Benchmark
    public void sendMessageToHardware_4Channels() {
        session4.sendMessageToHardware(DASH_ID, (short) i4, i4, "body:" + (i4++));
    }

    @TearDown
    public void teardown() {
        session1.closeAll();
        session2.closeAll();
        session3.closeAll();
        session4.closeAll();
    }

    private ChannelHandler newChannelHandler(final HardwareStateHolder hardwareStateHolder) {
        return new BaseSimpleChannelInboundHandler(new Limits(new ServerProperties(Collections.emptyMap())), hardwareStateHolder) {
            @Override
            public void messageReceived(ChannelHandlerContext ctx, Object msg) {
                throw new UnsupportedOperationException();
            }
        };
    }

    private class EmbeddedChannel extends io.netty.channel.embedded.EmbeddedChannel {
        public EmbeddedChannel(ChannelHandler... handlers) {
            super(handlers);
        }

        @Override
        public ChannelFuture writeAndFlush(Object msg) {
            return null;
        }

        @Override
        public ChannelFuture writeAndFlush(Object msg, ChannelPromise promise) {
            return null;
        }
    }
}
