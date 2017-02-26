package cc.blynk.client.handlers.hardware;

import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.model.messages.MessageFactory;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;

import java.util.concurrent.ThreadLocalRandom;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 29.07.15.
 */
@ChannelHandler.Sharable
public class HardwareEchoHandler extends SimpleChannelInboundHandler<HardwareMessage> {

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, HardwareMessage msg) throws Exception {
        if (msg.body.charAt(1) == 'm') {
            return;
        }

        String[] split = msg.body.split("\0");

        PinType pinType = PinType.getPinType(split[0].charAt(0));

        byte pin = Byte.parseByte(split[1]);
        //String value = split[2];

        switch (msg.body.charAt(1)) {
            case 'r' :
                read(ctx, pinType, pin, msg.id);
                break;
            case 'w' :
                //write();
                break;
            default:
                break;
        }
    }

    private void read(ChannelHandlerContext ctx, PinType pinType, byte pin, int msgId) {
        String value = "";
        if (pinType == PinType.VIRTUAL) {
            if (pin == 0) {
                value = String.valueOf(ThreadLocalRandom.current().nextDouble(100));
            }
            if (pin == 1) {
                value = String.valueOf(ThreadLocalRandom.current().nextInt(-100, 0));
            }
            if (pin == 2) {
                value = String.valueOf(ThreadLocalRandom.current().nextInt(100));
            }
            if (pin == 3) {
                value = String.valueOf(ThreadLocalRandom.current().nextDouble(-100, 100));
            }
            if (pin == 11) {
                ctx.writeAndFlush(MessageFactory.produce(msgId, Command.PUSH_NOTIFICATION, "You pressed button on V11"));
            }
            if (pin == 12) {
                value = String.valueOf("1234567890" + ThreadLocalRandom.current().nextDouble(100));
            }
            if (pin == 13) {
                value = String.valueOf("123456789012345678901234567890" + ThreadLocalRandom.current().nextDouble(100));
            }

        }

        if (!"".equals(value)) {
            ctx.writeAndFlush(MessageFactory.produce(msgId, Command.HARDWARE,
                    String.valueOf(pinType.pintTypeChar) + 'w' + '\0' + pin + '\0' + value));
        }
    }

}
