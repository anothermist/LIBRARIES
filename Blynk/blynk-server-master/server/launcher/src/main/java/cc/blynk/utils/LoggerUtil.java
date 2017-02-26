package cc.blynk.utils;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.config.Configuration;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 28.09.15.
 */
public final class LoggerUtil {

    /**
     * - Sets async logger for all logs
     * - Defines logging folder
     * - Sets logging level based on properties
     */
    public static void configureLogging(ServerProperties serverProperties) {
        //required to make all loggers async with LMAX disruptor
        System.setProperty("Log4jContextSelector", "org.apache.logging.log4j.core.async.AsyncLoggerContextSelector");
        System.setProperty("AsyncLogger.RingBufferSize",
                serverProperties.getProperty("async.logger.ring.buffer.size", String.valueOf(8 * 1024)));

        //configurable folder for logs via property.
        if (serverProperties.getProperty("logs.folder") == null) {
            System.out.println("logs.folder property is empty.");
            System.exit(1);
        }
        System.setProperty("logs.folder", serverProperties.getProperty("logs.folder"));

        //changing log level based on properties file
        changeLogLevel(serverProperties.getProperty("log.level"));
    }

    /**
     * Sets desired log level from properties.
     *
     * @param level - desired log level. error|info|debug|trace, etc.
     */
    private static void changeLogLevel(String level) {
        Level newLevel = Level.valueOf(level);
        LoggerContext ctx = (LoggerContext) LogManager.getContext(false);
        Configuration conf = ctx.getConfiguration();
        conf.getLoggerConfig(LogManager.ROOT_LOGGER_NAME).setLevel(newLevel);
        ctx.updateLoggers(conf);
    }

}
