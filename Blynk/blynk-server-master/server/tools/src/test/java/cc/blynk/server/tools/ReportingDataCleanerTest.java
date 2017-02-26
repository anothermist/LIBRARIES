package cc.blynk.server.tools;

import cc.blynk.utils.FileUtils;
import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 05.02.17.
 */
public class ReportingDataCleanerTest {

    private static Path reportingPath = Paths.get("/tmp/test_reporting");
    private static Path userPath = Paths.get("/tmp/test_reporting/test@test.gmail.com");

    @Before
    public void init() throws Exception {
        deleteDirectory(userPath.toFile());
        Files.createDirectories(userPath);
    }

    @Test
    public void testDoNotOverrideSmallFile() throws Exception {
        Path userFile = Paths.get(userPath.toString(), "123_minute.bin");
        int count = 1;
        fillWithData(userFile, count);

        ReportingDataCleaner.main(new String[] {reportingPath.toString()});

        assertEquals(16 * count, Files.size(userFile));
    }

    @Test
    public void testDoNotOverrideSmallFile2() throws Exception {
        Path userFile = Paths.get(userPath.toString(), "123_minute.bin");
        int count = 360;
        fillWithData(userFile, count);

        ReportingDataCleaner.main(new String[] {reportingPath.toString()});

        assertEquals(16 * count, Files.size(userFile));
    }

    @Test
    public void testOverrideCorrectFlowAndCheckContent() throws Exception {
        Path userFile = Paths.get(userPath.toString(), "123_minute.bin");
        int count = 361;
        fillWithData(userFile, count);

        ReportingDataCleaner.main(new String[] {reportingPath.toString()});

        assertEquals(16 * 360, Files.size(userFile));

        ByteBuffer userReportingData = FileUtils.read(Paths.get(userPath.toString(), "123_minute.bin"), 360);
        userReportingData.flip();
        for (int i = 1; i < 361; i++) {
            double value = userReportingData.getDouble();
            long ts = userReportingData.getLong();
            assertEquals(i, (int) value);
            assertEquals(ts, i);
        }
    }

    @Test
    public void testOverrideCorrectFlowAndCheckContent2() throws Exception {
        Path userFile = Paths.get(userPath.toString(), "123_minute.bin");
        int count = 720;
        fillWithData(userFile, count);

        ReportingDataCleaner.main(new String[] {reportingPath.toString()});

        assertEquals(16 * 360, Files.size(userFile));

        ByteBuffer userReportingData = FileUtils.read(Paths.get(userPath.toString(), "123_minute.bin"), 360);
        userReportingData.flip();
        for (int i = 360; i < 720; i++) {
            double value = userReportingData.getDouble();
            long ts = userReportingData.getLong();
            assertEquals(i, (int) value);
            assertEquals(ts, i);
        }
    }


    private static void fillWithData(Path path, int count) throws Exception {
        for (int i = 0; i < count; i++) {
            FileUtils.write(path, (double) i, (long) i);
        }
    }

    public static boolean deleteDirectory(File dir) {
        if (dir.isDirectory()) {
            File[] children = dir.listFiles();

            if (children != null) {
                for (File aChildren : children) {
                    boolean success = deleteDirectory(aChildren);
                    if (!success) {
                        return false;
                    }
                }
            }
        }

        return dir.delete();
    }

}
