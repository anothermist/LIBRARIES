package cc.blynk.server.workers;

import cc.blynk.server.Holder;
import cc.blynk.server.core.BaseServer;

import java.util.concurrent.ScheduledExecutorService;

/**
 * Used to close and store all important info to disk.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 25.03.15.
 */
public class ShutdownHookWorker implements Runnable {

    private final BaseServer[] servers;
    private final Holder holder;
    private final ProfileSaverWorker profileSaverWorker;
    private final ScheduledExecutorService scheduler;

    public ShutdownHookWorker(BaseServer[] servers, Holder holder,
                              ScheduledExecutorService scheduler,
                              ProfileSaverWorker profileSaverWorker) {
        this.servers = servers;
        this.holder = holder;
        this.profileSaverWorker = profileSaverWorker;
        this.scheduler = scheduler;
    }

    @Override
    public void run() {
        System.out.println("Catch shutdown hook.");

        System.out.println("Stopping servers...");
        for (BaseServer server : servers) {
            try {
                server.close();
            } catch (Throwable t) {
                System.out.println("Error on serve shutdown : " + t.getCause());
            }
        }

        System.out.println("Stopping scheduler...");
        scheduler.shutdown();

        System.out.println("Saving user profiles...");
        profileSaverWorker.close();

        System.out.println("Stopping aggregator...");
        holder.close();

        System.out.println("Done.");
    }

}
