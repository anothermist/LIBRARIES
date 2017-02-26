package cc.blynk.server.core.stats.metrics;

import java.util.concurrent.TimeUnit;

import static java.lang.Math.*;

/**
 * Class used to restrict user in request rate. Mostly copied from codahale metrics for performance improvement.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 16.03.15.
 */
public class InstanceLoadMeter {

    private static final long TICK_INTERVAL = TimeUnit.SECONDS.toMillis(1);
    private static final double TICK_INTERVAL_DOUBLE = (double) TICK_INTERVAL;

    private static final double ALPHA = 1 - exp(-1/60.0);

    private long lastTick;
    private long count = 0;
    private boolean initialized = false;
    private double rate = 0.0;

    public InstanceLoadMeter() {
        this.lastTick = System.currentTimeMillis();
    }

    /**
     * Mark the occurrence of a given number of events.
     */
    public void mark() {
        tickIfNecessary();
        count++;
    }

    private void tickIfNecessary() {
        final long newTick = System.currentTimeMillis();
        final long age = newTick - lastTick;
        if (age > TICK_INTERVAL) {
            lastTick = newTick - age % TICK_INTERVAL;
            final long requiredTicks = age / TICK_INTERVAL;
            for (long i = 0; i < requiredTicks; i++) {
                tick();
            }
        }
    }

    public double getOneMinuteRateNoTick() {
        return rate * TICK_INTERVAL_DOUBLE;
    }

    public double getOneMinuteRate() {
        tickIfNecessary();
        return rate * TICK_INTERVAL_DOUBLE;
    }

    private void tick() {
        final double instantRate = count / TICK_INTERVAL_DOUBLE;
        count = 0;
        if (initialized) {
            rate += (ALPHA * (instantRate - rate));
        } else {
            rate = instantRate;
            initialized = true;
        }
    }

}
