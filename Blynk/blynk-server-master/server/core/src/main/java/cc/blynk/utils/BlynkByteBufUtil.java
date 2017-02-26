package cc.blynk.utils;

import cc.blynk.server.core.protocol.enums.Command;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.ByteBufAllocator;
import io.netty.buffer.ByteBufUtil;

import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.model.messages.MessageBase.HEADER_LENGTH;

/**
 * Utility class that creates native netty buffers instead of java objects.
 * This is done in order to allocate less java objects and reduce GC pauses and load.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 10.03.16.
 */
public class BlynkByteBufUtil {

    public static final ByteBufAllocator ALLOCATOR = ByteBufAllocator.DEFAULT;

    public static ByteBuf ok(int msgId) {
        return makeResponse(msgId, OK);
    }

    public static ByteBuf makeResponse(int msgId, int responseCode) {
        return ALLOCATOR.buffer(HEADER_LENGTH)
                .writeByte(Command.RESPONSE)
                .writeShort(msgId)
                .writeShort(responseCode);
    }

    public static ByteBuf makeUTF8StringMessage(short cmd, int msgId, String data) {
        ByteBuf byteBuf = ALLOCATOR.buffer(HEADER_LENGTH + ByteBufUtil.utf8MaxBytes(data));
        byteBuf.writerIndex(HEADER_LENGTH);
        ByteBufUtil.writeUtf8(byteBuf, data);

        return byteBuf.setByte(0, cmd)
               .setShort(1, msgId)
               .setShort(3, byteBuf.writerIndex() - HEADER_LENGTH);
    }

    public static ByteBuf makeASCIIStringMessage(short cmd, int msgId, String data) {
        ByteBuf byteBuf = ALLOCATOR.buffer(HEADER_LENGTH + data.length())
                .writeByte(cmd)
                .writeShort(msgId)
                .writeShort(data.length());

        ByteBufUtil.writeAscii(byteBuf, data);
        return byteBuf;
    }

    public static ByteBuf makeBinaryMessage(short cmd, int msgId, byte[] byteData) {
        return ALLOCATOR.buffer(HEADER_LENGTH + byteData.length)
                .writeByte(cmd)
                .writeShort(msgId)
                .writeShort(byteData.length)
                .writeBytes(byteData);
    }

}
