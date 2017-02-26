package cc.blynk.utils;

import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.security.CodeSource;
import java.util.ArrayList;
import java.util.Properties;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Utility class to work with jar file. Used in order to find all static resources
 * within jar file and helps extract them into file system.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.12.15.
 */
public final class JarUtil {

    /**
     * Unpacks all files from staticFolder of jar and puts them to current folder within staticFolder path.
     *
     * @param staticFolder - path to resources
     * @throws Exception
     */
    public static boolean unpackStaticFiles(String staticFolder) throws Exception {
        ArrayList<String> staticResources = find(staticFolder);

        if (staticResources.size() == 0) {
            return false;
        }

        for (String staticFile : staticResources) {
            try (InputStream is = JarUtil.class.getResourceAsStream("/" + staticFile)) {
                Path newStaticFile = ServerProperties.getFileInCurrentDir(staticFile);

                Files.deleteIfExists(newStaticFile);
                Files.createDirectories(newStaticFile);

                Files.copy(is, newStaticFile, StandardCopyOption.REPLACE_EXISTING);
            }
        }

        return true;
    }

    /**
     * Returns list of resources that were found in staticResourcesFolder
     *
     * @param staticResourcesFolder - resource folder
     * @return - absolute path to resources within staticResourcesFolder
     * @throws Exception
     */
    private static ArrayList<String> find(String staticResourcesFolder) throws Exception {
        CodeSource src = JarUtil.class.getProtectionDomain().getCodeSource();
        ArrayList<String> staticResources = new ArrayList<>();

        if (src != null) {
            URL jar = src.getLocation();
            try (ZipInputStream zip = new ZipInputStream(jar.openStream())) {
                ZipEntry ze;

                while ((ze = zip.getNextEntry()) != null) {
                    String entryName = ze.getName();
                    if (entryName.startsWith(staticResourcesFolder) && isResource(entryName)) {
                        staticResources.add(entryName);
                    }
                }
            }
        }

        return staticResources;
    }

    /**
     * Extension which defines what files are 'resources'.
     *
     * @param filename - name of file
     * @return - true is this file is resource that should be unpacked
     */
    private static boolean isResource(String filename) {
        return filename.endsWith(".js") ||
               filename.endsWith(".css") ||
               filename.endsWith(".html") ||
               filename.endsWith(".ico") ||
               filename.endsWith(".png");
    }

    /**
     * Gets server version from jar file.
     *
     * @return server version
     */
    public static String getServerVersion() {
        try (InputStream is = JarUtil.class.getResourceAsStream("/META-INF/maven/cc.blynk.server/launcher/pom.properties")) {
            Properties properties = new Properties();
            properties.load(is);
            return properties.getProperty("version", "");
        } catch (Exception e) {
            return "";
        }
    }
}
