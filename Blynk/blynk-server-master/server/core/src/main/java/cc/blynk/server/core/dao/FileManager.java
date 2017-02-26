package cc.blynk.server.core.dao;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.utils.FileUtils;
import cc.blynk.utils.JsonParser;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.util.function.Function.identity;


/**
 * Class responsible for saving/reading user data to/from disk.
 *
 * User: ddumanskiy
 * Date: 8/11/13
 * Time: 6:53 PM
 */
public class FileManager {

    private static final Logger log = LogManager.getLogger(FileManager.class);

    /**
     * Folder where all user profiles are stored locally.
     */
    private Path dataDir;

    private static final String DELETED_DATA_DIR_NAME = "deleted";
    private static final String BACKUP_DATA_DIR_NAME = "backup";
    private Path deletedDataDir;
    private Path backupDataDir;

    public FileManager(String dataFolder) {
        if (dataFolder == null || dataFolder.isEmpty() || dataFolder.equals("/path")) {
            System.out.println("WARNING : '" + dataFolder + "' does not exists. Please specify correct -dataFolder parameter.");
            dataFolder = Paths.get(System.getProperty("java.io.tmpdir"), "blynk").toString();
            System.out.println("Your data may be lost during server restart. Using temp folder : " + dataFolder);
        }
        try {
            Path dataFolderPath = Paths.get(dataFolder);
            this.dataDir = createDir(dataFolderPath);
            this.deletedDataDir = createDir(Paths.get(dataFolder, DELETED_DATA_DIR_NAME));
            this.backupDataDir = createDir(Paths.get(dataFolder, BACKUP_DATA_DIR_NAME));
        } catch (RuntimeException e) {
            Path tempDir = Paths.get(System.getProperty("java.io.tmpdir"), "blynk");

            System.out.println("WARNING : could not find folder '" + dataFolder + "'. Please specify correct -dataFolder parameter.");
            System.out.println("Your data may be lost during server restart. Using temp folder : " + tempDir.toString());

            this.dataDir = createDir(tempDir);
            this.deletedDataDir = createDir(Paths.get(this.dataDir.toString(), DELETED_DATA_DIR_NAME));
            this.backupDataDir = createDir(Paths.get(this.dataDir.toString(), BACKUP_DATA_DIR_NAME));
        }

        log.info("Using data dir '{}'", dataDir);
    }

    private static Path createDir(Path dataDir) {
        try {
            Files.createDirectories(dataDir);
        } catch (IOException ioe) {
            log.error("Error creating data folder '{}'", dataDir);
            throw new RuntimeException("Error creating data folder '" + dataDir + "'");
        }
        return dataDir;
    }

    public Path getDataDir() {
        return dataDir;
    }

    public Path generateFileName(String userName, String appName) {
        return Paths.get(dataDir.toString(), userName + "." + appName + ".user");
    }

    public Path generateBackupFileName(String userName, String appName) {
        return Paths.get(backupDataDir.toString(), userName + "." + appName + ".user." +
                new SimpleDateFormat("yyyy-MM-dd").format(new Date()));
    }

    public Path generateOldFileName(String userName) {
        return Paths.get(dataDir.toString(), "u_" + userName + ".user");
    }

    public boolean delete(String name, String appName) {
        Path file = generateFileName(name, appName);
        return FileUtils.move(file, this.deletedDataDir);
    }

    public void overrideUserFile(User user) throws IOException {
        Path path = generateFileName(user.name, user.appName);

        JsonParser.writeUser(path.toFile(), user);

        removeOldFile(user.name);
    }

    private void removeOldFile(String username) {
        //this oldFileName is migration code. should be removed in future versions
        Path oldFileName = generateOldFileName(username);
        try {
            Files.deleteIfExists(oldFileName);
        } catch (Exception e) {
            log.error("Error removing old file. {}", oldFileName, e);
        }
    }

    /**
     * Loads all user profiles one by one from disk using dataDir as starting point.
     *
     * @return mapping between username and it's profile.
     */
    public ConcurrentMap<UserKey, User> deserialize() {
        log.debug("Starting reading user DB.");

        final File[] files = dataDir.toFile().listFiles();

        if (files != null) {
            ConcurrentMap<UserKey, User> tempUsers = Arrays.stream(files).parallel()
                    .filter(file -> file.isFile() && file.getName().endsWith(".user"))
                    .flatMap(file -> {
                        try {
                            User user = JsonParser.parseUserFromFile(file);
                            //todo this is migration code. remove during next deploy.
                            migrateOldProfile(user);

                            return Stream.of(user);
                        } catch (IOException ioe) {
                            log.error("Error parsing file '{}'. Error : {}", file, ioe.getMessage());
                        }
                        return Stream.empty();
                    })
                    .collect(Collectors.toConcurrentMap(UserKey::new, identity(), (user1, user2) -> user2));

            log.debug("Reading user DB finished.");
            return tempUsers;
        }

        log.debug("Reading user DB finished.");
        return new ConcurrentHashMap<>();
    }

    public static void migrateOldProfile(User user) {
        for (DashBoard dashBoard : user.profile.dashBoards) {
            final Integer dashId = dashBoard.id;
            if (user.dashTokens != null) {
                String token = user.dashTokens.get(dashId);
                if (token != null && !token.isEmpty()) {
                    dashBoard.devices = new Device[]{
                            new Device(0, dashBoard.boardType, dashBoard.boardType, token, null)
                    };
                    user.dashTokens.remove(dashId);
                }
            }

            if (dashBoard.devices != null) {
                for (Device device : dashBoard.devices) {
                    device.status = null;
                }
            }

            for (Widget widget : dashBoard.widgets) {
                dashBoard.cleanPinStorage(widget);
            }
        }
    }

    public Map<String, Integer> getUserProfilesSize() {
        Map<String, Integer> userProfileSize = new HashMap<>();
        File[] files = dataDir.toFile().listFiles();
        if (files != null) {
            for (File file : files) {
                if (file.isFile() && file.getName().endsWith(".user")) {
                    userProfileSize.put(file.getName(), (int) file.length());
                }
            }
        }
        return userProfileSize;
    }

}
