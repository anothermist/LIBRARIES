package cc.blynk.server.core.stats;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.LongAdder;

import static java.lang.Math.*;

/**
 * copy paste from codahale metrics
 */
public class Meter {

    private static final int INTERVAL = 5;
    private static final long TICK_INTERVAL = TimeUnit.SECONDS.toNanos(INTERVAL);
    private static final double SECONDS_PER_MINUTE = 60.0;
    private static final int ONE_MINUTE = 1;

    private static final double M1_ALPHA = 1 - exp(-INTERVAL / SECONDS_PER_MINUTE / ONE_MINUTE);
    private final EWMA m1Rate = new EWMA(M1_ALPHA, INTERVAL, TimeUnit.SECONDS);

    private final LongAdder count = new LongAdder();
    private final AtomicLong lastTick;

    /**
     * Creates a new {@link Meter}.
     *
     */
    public Meter() {
        this.lastTick = new AtomicLong(System.nanoTime());
    }

    /**
     * Mark the occurrence of a given number of events.
     *
     * @param n the number of events
     */
    public void mark(long n) {
        tickIfNecessary();
        count.add(n);
        m1Rate.update(n);
    }

    private void tickIfNecessary() {
        final long oldTick = lastTick.get();
        final long newTick = System.nanoTime();
        final long age = newTick - oldTick;
        if (age > TICK_INTERVAL) {
            final long newIntervalStartTick = newTick - age % TICK_INTERVAL;
            if (lastTick.compareAndSet(oldTick, newIntervalStartTick)) {
                final long requiredTicks = age / TICK_INTERVAL;
                for (long i = 0; i < requiredTicks; i++) {
                    m1Rate.tick();
                }
            }
        }
    }

    public long getCount() {
        return count.sum();
    }

    public double getOneMinuteRate() {
        tickIfNecessary();
        return m1Rate.getRate(TimeUnit.SECONDS);
    }
}

