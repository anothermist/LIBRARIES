package cc.blynk.server.hardware.handlers;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.model.messages.hardware.BlynkInternalMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.hardware.handlers.hardware.logic.BlynkInternalLogic;
import cc.blynk.utils.ServerProperties;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufAllocator;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelPipeline;
import io.netty.handler.timeout.ReadTimeoutHandler;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.Collections;

import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 04.12.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class BlynkInternalLogicTest {

    ServerProperties props = new ServerProperties(Collections.emptyMap());

    @Mock
    private ChannelHandlerContext ctx;

    @Mock
    private ChannelPipeline pipeline;

    @Mock
    private ByteBufAllocator allocator;

    @Mock
    private ByteBuf byteBuf;

    @Test
    public void testCorrectBehavior() {
        BlynkInternalLogic logic = new BlynkInternalLogic(props.getIntProperty("hard.socket.idle.timeout", 0));

        when(ctx.pipeline()).thenReturn(pipeline);
        when(ctx.alloc()).thenReturn(allocator);
        when(allocator.ioBuffer(anyInt())).thenReturn(byteBuf);
        when(byteBuf.writeByte(eq(0))).thenReturn(byteBuf);
        when(byteBuf.writeShort(eq(1))).thenReturn(byteBuf);
        when(byteBuf.writeShort(eq(200))).thenReturn(byteBuf);

        User user = new User();
        user.profile = new Profile();
        DashBoard dashBoard = new DashBoard();
        dashBoard.id = 1;
        user.profile.dashBoards = new DashBoard[] {dashBoard};
        HardwareStateHolder hardwareStateHolder = new HardwareStateHolder(1, 0, user, null);

        BlynkInternalMessage hardwareInfoLogic = new BlynkInternalMessage(1, "ver 0.3.2-beta h-beat 60 buff-in 256 dev ESP8266".replaceAll(" ", "\0"));
        logic.messageReceived(ctx, hardwareStateHolder, hardwareInfoLogic);

        verify(pipeline).remove(ReadTimeoutHandler.class);
        verify(pipeline).addFirst(any(ReadTimeoutHandler.class));
        verify(ctx).writeAndFlush(any(), any());
    }

}
