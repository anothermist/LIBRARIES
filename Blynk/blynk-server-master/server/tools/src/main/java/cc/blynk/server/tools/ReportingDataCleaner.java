package cc.blynk.server.tools;

import cc.blynk.utils.FileUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 05.02.17.
 */
public class ReportingDataCleaner {

    public static void main(String[] args) {
        String reportingFolder = args[0];
        Path reportingPath = Paths.get(reportingFolder);
        if (Files.exists(reportingPath)) {
            System.out.println("Starting processing " + reportingPath.toString());
            start(reportingPath);
        } else {
            System.out.println(reportingPath.toString() + " not exists.");
        }
    }

    private static void start(Path reportingPath) {
        File[] allReporting = reportingPath.toFile().listFiles();
        if (allReporting == null || allReporting.length == 0) {
            System.out.println("No files.");
            return;
        }

        System.out.println("Directories number : " + allReporting.length);

        final int COUNT = 360;
        int filesCount = 0;
        int overrideCount = 0;

        for (File userDirectory : allReporting) {
            if (userDirectory.isDirectory()) {
                File[] userFiles = userDirectory.listFiles();
                if (userFiles == null || userFiles.length == 0) {
                    System.out.println(userDirectory + " is empty.");
                    continue;
                }
                for (File file : userFiles) {
                    if (filesCount != 0 && filesCount % 1000 == 0) {
                        System.out.println("Visited " + filesCount + " files.");
                    }
                    long fileSize = file.length();
                    if (file.getPath().endsWith("minute.bin") && fileSize > COUNT * 16) {
                        System.out.println("Found " + file.getPath() + ". Size : " + fileSize);
                        try {
                            Path path = file.toPath();
                            ByteBuffer userReportingData = FileUtils.read(path, COUNT);
                            userReportingData.flip();
                            write(file, userReportingData);
                            System.out.println("Successfully copied. Truncated : " + (fileSize - userReportingData.position()));
                            overrideCount++;
                        } catch (Exception e) {
                            System.out.println("Error reading file " + file.getAbsolutePath());
                            System.out.println("Skipping.");
                        }
                    }

                    filesCount++;
                }
            }
        }

        System.out.println("Visited : " + filesCount + ". Overrided : " + overrideCount);
    }

    private static void write(File file, ByteBuffer data) throws Exception {
        try (FileChannel channel = new FileOutputStream(file, false).getChannel()) {
            channel.write(data);
        }
    }

}
