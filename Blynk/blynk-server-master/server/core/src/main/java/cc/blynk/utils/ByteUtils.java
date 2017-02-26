package cc.blynk.utils;

import cc.blynk.server.core.protocol.exceptions.GetGraphDataException;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.zip.DeflaterOutputStream;
import java.util.zip.InflaterInputStream;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.09.15.
 */
public class ByteUtils {

    public static final int REPORTING_RECORD_SIZE_BYTES = 16;

    public static byte[] compress(String value) throws IOException {
        byte[] stringData = value.getBytes(StandardCharsets.UTF_8);
        ByteArrayOutputStream baos = new ByteArrayOutputStream(stringData.length);

        try (OutputStream out = new DeflaterOutputStream(baos)) {
            out.write(stringData);
        }

        return baos.toByteArray();
    }

    public static byte[] compress(byte[][] values) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream(8192);

        try (OutputStream out = new DeflaterOutputStream(baos)) {
            for (byte[] data : values) {
                ByteBuffer bb = ByteBuffer.allocate(4);
                bb.putInt(data.length / REPORTING_RECORD_SIZE_BYTES);
                out.write(bb.array());
                out.write(data);
            }
        } catch (IOException ioe) {
            throw new GetGraphDataException();
        }
        return baos.toByteArray();
    }

    public static byte[] compress(int dashId, byte[][] values) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream(8192);

        try (OutputStream out = new DeflaterOutputStream(baos)) {
            writeInt(out, dashId);
            for (byte[] data : values) {
                writeInt(out, data.length / REPORTING_RECORD_SIZE_BYTES);
                out.write(data);
            }
        }
        return baos.toByteArray();
    }

    private static void writeInt(OutputStream out, int value) throws IOException {
        out.write((value >>> 24) & 0xFF);
        out.write((value >>> 16) & 0xFF);
        out.write((value >>>  8) & 0xFF);
        out.write((value) & 0xFF);
    }

    //for tests only
    public static byte[] decompress(byte[] bytes) {
        try (InputStream in = new InflaterInputStream(new ByteArrayInputStream(bytes))) {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            byte[] buffer = new byte[4096];
            int len;
            while ((len = in.read(buffer)) > 0) {
                baos.write(buffer, 0, len);
            }
            return baos.toByteArray();
        } catch (IOException e) {
            throw new AssertionError(e);
        }
    }

    public static int parseColor(String fieldValue) {
        int decodedColor = Integer.decode(fieldValue);
        return convertARGBtoRGBA(setAlphaComponent(decodedColor, 255));
    }

    private static int convertARGBtoRGBA(int color) {
        final int a = (color & 0xff000000) >> 24;
        final int r = (color & 0x00ff0000) >> 16;
        final int g = (color & 0x0000ff00) >> 8;
        final int b = (color & 0x000000ff);

        return (r << 24) | (g << 16) | (b << 8) | (a & 0xff);
    }

    private static int setAlphaComponent(int color, int alpha) {
        return (color & 0x00ffffff) | (alpha << 24);
    }
}
