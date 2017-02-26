package cc.blynk.server.workers;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.utils.FileLoaderUtil;
import cc.blynk.utils.ServerProperties;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.nio.file.*;

/**
 *
 * Hook that monitors server.properties file for modification.
 * In case of change - reloads "reloadable" properties in realtime.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.10.15.
 */
public class FileChangeWatcherWorker implements Runnable {

    private static final Logger log = LogManager.getLogger(FileChangeWatcherWorker.class);

    private final String fileName;
    private final Path propsFileFolder;
    private final BlockingIOProcessor blockingIOProcessor;

    public FileChangeWatcherWorker(String fileName, BlockingIOProcessor blockingIOProcessor) {
        this.fileName = fileName;
        this.propsFileFolder = getCurrentDir();
        this.blockingIOProcessor = blockingIOProcessor;
    }

    private static Path getCurrentDir() {
        return Paths.get(System.getProperty("user.dir"));
    }

    @Override
    public void run() {
        try {
            WatchService watchService = FileSystems.getDefault().newWatchService();
            propsFileFolder.register(watchService, StandardWatchEventKinds.ENTRY_MODIFY);
            while (true) {
                WatchKey wk = watchService.take();
                for (WatchEvent<?> event : wk.pollEvents()) {
                    Path changed = (Path) event.context();
                    Path changedFile = propsFileFolder.resolve(changed.toString());
                    if (changed.getFileName().toString().endsWith(fileName) && Files.exists(changedFile)) {
                        log.info("File '{}' changed. Updating values.", changedFile);
                        blockingIOProcessor.tokenBody = FileLoaderUtil.readFileAsString(fileName);
                    }
                }
                // reset the key
                boolean valid = wk.reset();
                if (!valid) {
                    log.info("Key has been not unregistered.");
                }
            }
        } catch (IOException | InterruptedException e) {
            log.warn("Error monitoring '{}' file. Reloadable properties are not enabled.", ServerProperties.SERVER_PROPERTIES_FILENAME, e);
        }
    }

}
