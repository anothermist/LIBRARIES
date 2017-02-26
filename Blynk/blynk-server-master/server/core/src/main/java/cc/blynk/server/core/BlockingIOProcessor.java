package cc.blynk.server.core;

import java.io.Closeable;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * Wrapper around ThreadPoolExecutor that should perform blocking IO operations.
 * Due to async nature of netty performing Blocking operations withing netty pipeline
 * will cause performance issues. So Blocking operations should always
 * executed via this wrapper.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.04.15.
 */
public class BlockingIOProcessor implements Closeable {

    public static final String TOKEN_MAIL_BODY = "single_token_mail_body.txt";
    private final ThreadPoolExecutor executor;
    public volatile String tokenBody;

    public BlockingIOProcessor(int poolSize, int maxQueueSize, String tokenBody) {
        this.executor = new ThreadPoolExecutor(
                poolSize, poolSize,
                0L, TimeUnit.MILLISECONDS,
                new ArrayBlockingQueue<>(maxQueueSize)
        );
        this.tokenBody = tokenBody;
    }

    public void execute(Runnable task) {
        executor.execute(task);
    }

    @Override
    public void close() {
        executor.shutdown();
    }

    public int getActiveCount() {
        return executor.getActiveCount();
    }

}
