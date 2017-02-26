package cc.blynk.server.storage;

import cc.blynk.server.core.model.graph.GraphKey;
import cc.blynk.server.core.reporting.average.AverageAggregatorProcessor;
import cc.blynk.utils.FileUtils;
import org.junit.Ignore;
import org.junit.Test;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.07.15.
 */
public class StorageTest {

    @Test
    @Ignore
    public void generateLogFile() throws IOException {
        Path path = Paths.get("/home/doom369/test-data.log");
        long ts  = System.currentTimeMillis();
        int i = 0;
        try (BufferedWriter bw = Files.newBufferedWriter(path)) {
            //200 req/sec current load.
            for (int j = 0; j < 24 * 3600; j++) {
                GraphKey storeMessage = new GraphKey(100000, ("aw 10 " + i++).split(" "), ts++);
                bw.write(storeMessage.toCSV());
                bw.write("\n");
            }
        }
    }

    @Test
    @Ignore
    public void generateDailyHistoryData() throws IOException {
        Path path = Paths.get("/home/doom369/blynk/data/dmitriy@blynk.cc/daily_data.bin");
        //now - 365 days.
        long ts  = (System.currentTimeMillis() / AverageAggregatorProcessor.DAY - 365);
        for (int i = 0; i < 365; i++ ) {
            FileUtils.write(path, i, (ts + i) * AverageAggregatorProcessor.DAY);
        }
    }

    @Test
    @Ignore
    public void generateHourlyHistoryData() throws IOException {
        int count = 7 * 24;
        Path path = Paths.get("/home/doom369/blynk/data/dmitriy@blynk.cc/hourly_data.bin");
        //now - 1 week.
        long ts  = (System.currentTimeMillis() / AverageAggregatorProcessor.HOUR - count);
        for (int i = 0; i < count; i++ ) {
            FileUtils.write(path, i, (ts + i) * AverageAggregatorProcessor.HOUR);
        }
    }


}
