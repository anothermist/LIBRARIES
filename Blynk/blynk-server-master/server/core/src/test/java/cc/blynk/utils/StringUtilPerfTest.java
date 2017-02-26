package cc.blynk.utils;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.BenchmarkMode;
import org.openjdk.jmh.annotations.Fork;
import org.openjdk.jmh.annotations.Measurement;
import org.openjdk.jmh.annotations.Mode;
import org.openjdk.jmh.annotations.OutputTimeUnit;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.annotations.Warmup;
import org.openjdk.jmh.infra.BenchmarkParams;

import java.util.concurrent.TimeUnit;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 16.01.16.
 */
@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.NANOSECONDS)
@State(Scope.Thread)
@Fork(1)
@Warmup(iterations = 5, time = 1, timeUnit = TimeUnit.SECONDS)
@Measurement(iterations = 5, time = 1, timeUnit = TimeUnit.SECONDS)
public class StringUtilPerfTest {

     /*
     * Should your benchmark require returning multiple results, you have to
     * consider two options (detailed below).
     *
     * NOTE: If you are only producing a single result, it is more readable to
     * use the implicit return, as in JMHSample_08_DeadCode. Do not make your benchmark
     * code less readable with explicit Blackholes!
     */

    public String vw_1;
    public String aw_1_2;
    public String vw_99_900;
    public String vw_99_22222;
    public String aw_100_900;
    public String aw_10_long_text;

    @Setup
    public void setup(BenchmarkParams params) {
        vw_1 = "vw 1".replaceAll(" ", "\0");
        aw_1_2 = "aw 1 2".replaceAll(" ", "\0");
        vw_99_900 = "vw 99 900".replaceAll(" ", "\0");
        vw_99_22222 = "vw 99 22222.32".replaceAll(" ", "\0");
        aw_100_900 = "aw 100 200".replaceAll(" ", "\0");
        aw_10_long_text = "aw 10  dsfdsfdsfdsfdsfdsfdsfdsfd gfdsgdfg dfg dfg dfsgdf gdfs gdfsg dfsg dfsg dfs".replaceAll(" ", "\0");
    }

    @Benchmark
    public String[] split3_vw_1() {
        return vw_1.split("\0", 3);
    }

    @Benchmark
    public String[] customSplit3_vw_1() {
        return StringUtils.split3(vw_1);
    }

    @Benchmark
    public String[] split3_aw_1_2() {
        return aw_1_2.split("\0", 3);
    }

    @Benchmark
    public String[] customSplit3_aw_1_2() {
        return StringUtils.split3(aw_1_2);
    }

    @Benchmark
    public String[] split3_vw_99_900() {
        return vw_99_900.split("\0", 3);
    }

    @Benchmark
    public String[] customSplit3_vw_99_900() {
        return StringUtils.split3(vw_99_900);
    }

    @Benchmark
    public String[] split3_vw_99_22222() {
        return vw_99_22222.split("\0", 3);
    }

    @Benchmark
    public String[] customSplit3_vw_99_22222() {
        return StringUtils.split3(vw_99_22222);
    }

    @Benchmark
    public String[] split3_aw_100_900() {
        return aw_100_900.split("\0", 3);
    }

    @Benchmark
    public String[] customSplit3_aw_100_900() {
        return StringUtils.split3(aw_100_900);
    }

    @Benchmark
    public String[] split3_aw_10_long_text() {
        return aw_10_long_text.split("\0", 3);
    }

    @Benchmark
    public String[] customSplit3_aw_10_long_text() {
        return StringUtils.split3(aw_10_long_text);
    }

}
