package cc.blynk.utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

/**
 * Simply reads full file from disk as String
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/12/2015.
 */
public class FileLoaderUtil {

    private static final String NEW_LINE = System.getProperty("line.separator");

    private static Path getFileInCurrentDir(String filename) {
        return Paths.get(System.getProperty("user.dir"), filename);
    }

    /**
     * First loads file from class path after that from current folder.
     * So file in current folder is always overrides properties in classpath.
     *
     * @param fileName - name of properties file, for example "twitter4j.properties"
     */
    public static String readFileAsString(String fileName) {
        if (!fileName.startsWith("/")) {
            fileName = "/" + fileName;
        }

        String result = null;

        try (InputStream classPath = FileLoaderUtil.class.getResourceAsStream(fileName)) {
            if (classPath != null) {
                result = load(classPath);
            }
        } catch (Exception e) {
            throw new RuntimeException("Error getting properties file : " + fileName, e);
        }

        Path curDirPath = getFileInCurrentDir(fileName);
        if (Files.exists(curDirPath)) {
            try {
                List<String> stringList = Files.readAllLines(curDirPath);
                StringBuilder responseData = new StringBuilder();
                for (String s : stringList) {
                    responseData.append(s).append(NEW_LINE);
                }
                result = responseData.toString();
            } catch (Exception e) {
                throw new RuntimeException("Error getting properties file : " + fileName, e);
            }
        }

        return result;
    }

    private static String load(InputStream is) throws IOException{
        try (BufferedReader in = new BufferedReader(new InputStreamReader(is))) {
            String line;
            StringBuilder responseData = new StringBuilder();
            while ((line = in.readLine()) != null) {
                responseData.append(line).append(NEW_LINE);
            }
            return responseData.toString();
        }
    }

}
