package cc.blynk.server.core.protocol.handlers.encoders;

import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.model.messages.MessageBase;
import cc.blynk.server.core.protocol.model.messages.ResponseWithBodyMessage;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;

/**
 * Encodes java message into a bytes array.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class MessageEncoder extends MessageToByteEncoder<MessageBase> {

    private final GlobalStats stats;

    public MessageEncoder(GlobalStats stats) {
        this.stats = stats;
    }

    @Override
    protected void encode(ChannelHandlerContext ctx, MessageBase message, ByteBuf out) throws Exception {
        out.writeByte(message.command);
        out.writeShort(message.id);

        if (message.command == Command.RESPONSE) {
            out.writeShort(message.length);
            if (message.length == Response.DEVICE_WENT_OFFLINE) {
                out.writeInt(((ResponseWithBodyMessage) message).dashId);
            }
        } else {
            stats.mark(message.command);

            byte[] body = message.getBytes();
            out.writeShort(body.length);
            if (body.length > 0) {
                out.writeBytes(body);
            }
        }
    }
}
