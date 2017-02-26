package cc.blynk.utils;

import cc.blynk.server.core.protocol.handlers.DefaultExceptionHandler;
import cc.blynk.server.core.reporting.average.AggregationKey;
import cc.blynk.server.core.reporting.average.AggregationValue;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 04.09.15.
 */
public class ReportingUtil {

    public static final byte[] EMPTY_ARRAY = {};
    private final static Logger log = LogManager.getLogger(ReportingUtil.class);

    public static String getReportingFolder(String dataFolder) {
        return Paths.get(dataFolder, "data").toString();
    }

    @SuppressWarnings("unchecked")
    public static ConcurrentHashMap<AggregationKey, AggregationValue> read(Path path) {
        if (Files.exists(path)) {
            try (InputStream is = Files.newInputStream(path);
                 ObjectInputStream objectinputstream = new ObjectInputStream(is)) {
                return (ConcurrentHashMap<AggregationKey, AggregationValue>) objectinputstream.readObject();
            } catch (Exception e) {
                log.error(e);
            }
        }

        return new ConcurrentHashMap<>();
    }

    public static void write(Path path, Map<AggregationKey, AggregationValue> map) {
        if (map.size() > 0) {
            try (OutputStream os = Files.newOutputStream(path);
                 ObjectOutputStream oos = new ObjectOutputStream(os)) {
                oos.writeObject(map);
            } catch (Exception e) {
                log.error(e);
            }
        }
    }

    public static void createReportingFolder(String reportingFolder, String username) {
        Path reportingPath = Paths.get(reportingFolder, username);
        if (Files.notExists(reportingPath)) {
            try {
                Files.createDirectories(reportingPath);
            } catch (IOException ioe) {
                DefaultExceptionHandler.log.error("Error creating report folder. {}", reportingPath);
            }
        }
    }
}
