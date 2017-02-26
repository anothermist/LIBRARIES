package cc.blynk.server.core.dao;

import cc.blynk.server.core.model.enums.GraphType;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.protocol.exceptions.NoDataException;
import cc.blynk.server.core.reporting.GraphPinRequest;
import cc.blynk.server.core.reporting.average.AverageAggregatorProcessor;
import cc.blynk.server.core.reporting.raw.RawDataProcessor;
import cc.blynk.utils.FileUtils;
import cc.blynk.utils.NumberUtil;
import cc.blynk.utils.ServerProperties;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.Closeable;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static cc.blynk.utils.ReportingUtil.EMPTY_ARRAY;
import static cc.blynk.utils.StringUtils.DEVICE_SEPARATOR;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/18/2015.
 */
public class ReportingDao implements Closeable {

    private static final Logger log = LogManager.getLogger(ReportingDao.class);

    public final AverageAggregatorProcessor averageAggregator;
    public final RawDataProcessor rawDataProcessor;

    private final String dataFolder;

    private final boolean ENABLE_RAW_DB_DATA_STORE;

    //for test only
    public ReportingDao(String reportingFolder, AverageAggregatorProcessor averageAggregator, ServerProperties serverProperties) {
        this.averageAggregator = averageAggregator;
        this.dataFolder = reportingFolder;
        this.ENABLE_RAW_DB_DATA_STORE = serverProperties.getBoolProperty("enable.raw.db.data.store");
        this.rawDataProcessor = new RawDataProcessor(ENABLE_RAW_DB_DATA_STORE);
    }

    public ReportingDao(String reportingFolder , ServerProperties serverProperties) {
        this.averageAggregator = new AverageAggregatorProcessor(reportingFolder);
        this.dataFolder = reportingFolder;
        this.ENABLE_RAW_DB_DATA_STORE = serverProperties.getBoolProperty("enable.raw.db.data.store");
        this.rawDataProcessor = new RawDataProcessor(ENABLE_RAW_DB_DATA_STORE);
    }

    public static String generateFilename(int dashId, int deviceId, char pinType, byte pin, GraphType type) {
        switch (type) {
            case MINUTE :
                return formatMinute(dashId, deviceId, pinType, pin);
            case HOURLY :
                return formatHour(dashId, deviceId, pinType, pin);
            default :
                return formatDaily(dashId, deviceId, pinType, pin);
        }
    }

    public static ByteBuffer getByteBufferFromDisk(String dataFolder, String username, int dashId, int deviceId, PinType pinType, byte pin, int count, GraphType type) {
        Path userDataFile = Paths.get(dataFolder, username, generateFilename(dashId, deviceId, pinType.pintTypeChar, pin, type));
        if (Files.notExists(userDataFile)) {
            return null;
        }

        try {
            return FileUtils.read(userDataFile, count);
        } catch (IOException ioe) {
            log.error(ioe);
        }

        return null;
    }

    private static boolean checkNoData(byte[][] data) {
        boolean noData = true;

        for (byte[] pinData : data) {
            noData = noData && pinData.length == 0;
        }

        return noData;
    }

    public ByteBuffer getByteBufferFromDisk(String username, int dashId, int deviceId, PinType pinType, byte pin, int count, GraphType type) {
        return getByteBufferFromDisk(dataFolder, username, dashId, deviceId, pinType, pin, count, type);
    }

    public void delete(String username, int dashId, int deviceId, PinType pinType, byte pin) {
        log.debug("Removing {}{} pin data for dashId {}, deviceId {}.", pinType.pintTypeChar, pin, dashId, deviceId);
        Path userDataMinuteFile = Paths.get(dataFolder, username, formatMinute(dashId, deviceId, pinType.pintTypeChar, pin));
        Path userDataHourlyFile = Paths.get(dataFolder, username, formatHour(dashId, deviceId, pinType.pintTypeChar, pin));
        Path userDataDailyFile = Paths.get(dataFolder, username, formatDaily(dashId, deviceId, pinType.pintTypeChar, pin));
        FileUtils.deleteQuietly(userDataMinuteFile);
        FileUtils.deleteQuietly(userDataHourlyFile);
        FileUtils.deleteQuietly(userDataDailyFile);
    }

    protected static String formatMinute(int dashId, int deviceId, char pinType, byte pin) {
        return format("minute", dashId, deviceId, pinType, pin);
    }

    protected static String formatHour(int dashId, int deviceId, char pinType, byte pin) {
        return format("hourly", dashId, deviceId, pinType, pin);
    }

    protected static String formatDaily(int dashId, int deviceId, char pinType, byte pin) {
        return format("daily", dashId, deviceId, pinType, pin);
    }

    private static String format(String type, int dashId, int deviceId, char pinType, byte pin) {
        //todo this is back compatibility code. should be removed in future versions.
        if (deviceId == 0) {
            return "history_" + dashId + "_" + pinType + pin + "_" + type + ".bin";
        }
        return "history_" + dashId + DEVICE_SEPARATOR + deviceId + "_" + pinType + pin + "_" + type + ".bin";
    }

    public void process(String username, int dashId, int deviceId, byte pin, PinType pinType, String value, long ts) {
        try {
            double doubleVal = NumberUtil.parseDouble(value);
            process(username, dashId, deviceId, pin, pinType, value, ts, doubleVal);
        } catch (Exception e) {
            //just in case
            log.trace("Error collecting reporting entry.");
        }
    }

    private void process(String username, int dashId, int deviceId, byte pin, PinType pinType, String value, long ts, double doubleVal) {
        if (ENABLE_RAW_DB_DATA_STORE) {
            rawDataProcessor.collect(username, dashId, deviceId, pinType.pintTypeChar, pin, ts, value, doubleVal);
        }

        //not a number, nothing to aggregate
        if (doubleVal == NumberUtil.NO_RESULT) {
            return;
        }

        averageAggregator.collect(username, dashId, deviceId, pinType.pintTypeChar, pin, ts, doubleVal);
    }

    public byte[][] getAllFromDisk(String username, GraphPinRequest[] requestedPins) {
        byte[][] values = new byte[requestedPins.length][];

        for (int i = 0; i < requestedPins.length; i++) {
            final ByteBuffer byteBuffer = getByteBufferFromDisk(username,
                    requestedPins[i].dashId, requestedPins[i].deviceId, requestedPins[i].pinType,
                    requestedPins[i].pin, requestedPins[i].count, requestedPins[i].type);
            values[i] =  byteBuffer == null ? EMPTY_ARRAY : byteBuffer.array();
        }


        if (checkNoData(values)) {
            throw new NoDataException();
        }

        return values;
    }

    @Override
    public void close() {
        System.out.println("Stopping aggregator...");
        this.averageAggregator.close();
    }
}
