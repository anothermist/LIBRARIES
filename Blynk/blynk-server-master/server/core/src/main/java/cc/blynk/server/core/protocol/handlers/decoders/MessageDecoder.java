package cc.blynk.server.core.protocol.handlers.decoders;

import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.model.messages.MessageBase;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.stats.GlobalStats;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.util.CharsetUtil;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.List;

import static cc.blynk.server.core.protocol.model.messages.MessageFactory.*;

/**
 * Decodes input byte array into java message.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class MessageDecoder extends ByteToMessageDecoder {

    protected static final Logger log = LogManager.getLogger(MessageDecoder.class);

    private final GlobalStats stats;

    public MessageDecoder(GlobalStats stats) {
        this.stats = stats;
    }

    @Override
    protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
        if (in.readableBytes() < 5) {
            return;
        }

        in.markReaderIndex();

        final short command = in.readUnsignedByte();
        final int messageId = in.readUnsignedShort();
        final int codeOrLength = in.readUnsignedShort();

        MessageBase message;
        if (command == Command.RESPONSE) {
            message = new ResponseMessage(messageId, codeOrLength);
        } else {
            if (in.readableBytes() < codeOrLength) {
                in.resetReaderIndex();
                return;
            }

            message = produce(messageId, command, in.readSlice(codeOrLength).toString(CharsetUtil.UTF_8));
        }

        log.trace("Incoming {}", message);

        stats.mark(command);

        out.add(message);
    }

}
