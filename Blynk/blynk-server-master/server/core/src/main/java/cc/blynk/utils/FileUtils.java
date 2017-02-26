package cc.blynk.utils;

import cc.blynk.server.core.dao.ReportingDao;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.enums.GraphType;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandBodyException;
import cc.blynk.server.core.protocol.exceptions.NoDataException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.nio.ByteBuffer;
import java.nio.channels.SeekableByteChannel;
import java.nio.file.Files;
import java.nio.file.OpenOption;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.nio.file.StandardOpenOption;
import java.util.zip.GZIPOutputStream;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 04.01.16.
 */
public class FileUtils {

    public static final String CSV_DIR = Paths.get(System.getProperty("java.io.tmpdir"), "blynk").toString();

    private final static Logger log = LogManager.getLogger(FileUtils.class);

    static {
        try {
            Files.createDirectories(Paths.get(CSV_DIR));
        } catch (IOException ioe) {
            log.error("Error creating temp '{}' folder for csv export data.", CSV_DIR);
        }
    }

    public static void deleteQuietly(Path path) {
        try {
            Files.deleteIfExists(path);
        } catch (Exception ignored) {
            //ignore
        }
    }

    public static boolean move(Path source, Path target) {
        try {
            Path targetFile = Paths.get(target.toString(), source.getFileName().toString());
            Files.move(source, targetFile, StandardCopyOption.REPLACE_EXISTING);
        } catch (IOException e) {
            log.debug("Failed to move file. {}" , e.getMessage());
            return false;
        }
        return true;
    }

    /**
     * Writes ByteBuffer with value (double 8 bytes),
     * timestamp (long 8 bytes) data to disk as csv file and gzips it.
     *
     * @param onePinData - reporting data
     * @param path - path to file to store data
     * @throws IOException
     */
    public static void makeGzippedCSVFile(ByteBuffer onePinData, Path path) throws IOException {
        try (OutputStream output = Files.newOutputStream(path);
             Writer writer = new OutputStreamWriter(new GZIPOutputStream(output), "UTF-8")) {

            while (onePinData.remaining() > 0) {
                double value = onePinData.getDouble();
                long ts = onePinData.getLong();

                writer.write("" + value);
                writer.write(',');
                writer.write("" + ts);
                writer.write('\n');
            }
        }
    }

    public static final int DEFAULT_FETCH_COUNT = 60 * 24 * 30;

    public static Path createCSV(ReportingDao reportingDao, String username, int dashId, int deviceId, PinType pinType, byte pin) throws Exception {
        if (pinType == null || pin == Pin.NO_PIN) {
            throw new IllegalCommandBodyException("Wrong pin format.");
        }

        //data for 1 month
        ByteBuffer onePinData = reportingDao.getByteBufferFromDisk(username, dashId, deviceId, pinType, pin, DEFAULT_FETCH_COUNT, GraphType.MINUTE);
        if (onePinData == null) {
            throw new NoDataException();
        }

        onePinData.flip();
        Path path = generateExportCSVPath(username, dashId, pinType, pin);
        makeGzippedCSVFile(onePinData, path);
        return path;
    }

    private static Path generateExportCSVPath(String username, int dashId, PinType pinType, byte pin) {
        return Paths.get(CSV_DIR, format(username, dashId, pinType, pin));
    }

    //"%s_%s_%c%d.csv.gz"
    private static String format(String username, int dashId, PinType pinType, byte pin) {
        return username + "_" + dashId + "_" + pinType.pintTypeChar + pin + ".csv.gz";
    }

    /**
     * Simply writes single reporting entry to disk (16 bytes).
     * Reporting entry is value (double) and timestamp (long)
     *
     * @param reportingPath - path to user specific reporting file
     * @param value - sensor data
     * @param ts - time when entry was created
     * @throws IOException
     */
    public static void write(Path reportingPath, double value, long ts) throws IOException {
        write(reportingPath, value, ts, StandardOpenOption.CREATE, StandardOpenOption.APPEND);
    }

    public static void write(Path reportingPath, double value, long ts, OpenOption... options) throws IOException {
        try (DataOutputStream dos = new DataOutputStream(
                Files.newOutputStream(reportingPath, options))) {
            dos.writeDouble(value);
            dos.writeLong(ts);
            dos.flush();
        }
    }

    private static final int SIZE_OF_REPORT_ENTRY = 16;

    /**
     * Read bunch of last records from file.
     *
     * @param userDataFile - file to read
     * @param count = number of records to read
     * @return - byte buffer with data
     * @throws IOException
     */
    public static ByteBuffer read(Path userDataFile, int count) throws IOException {
        try (SeekableByteChannel channel = Files.newByteChannel(userDataFile, StandardOpenOption.READ)) {
            final int size = (int) Files.size(userDataFile);
            final int dataSize = count * SIZE_OF_REPORT_ENTRY;
            final int readDataSize = Math.min(dataSize, size);

            ByteBuffer buf = ByteBuffer.allocate(readDataSize);
            channel.position(Math.max(0, size - dataSize));
            channel.read(buf);
            return buf;
        }
    }
}
