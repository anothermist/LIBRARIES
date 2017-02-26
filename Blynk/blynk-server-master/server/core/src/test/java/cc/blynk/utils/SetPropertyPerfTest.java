package cc.blynk.utils;

import cc.blynk.server.core.model.auth.User;
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

import java.lang.reflect.Field;
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
public class SetPropertyPerfTest {

    public User user;
    public String fieldName;

    @Setup
    public void setup(BenchmarkParams params) {
        user = new User();
        fieldName = "name";
    }

    @Benchmark
    public void reflection() {
        try {
            Field field = User.class.getDeclaredField(fieldName);
            Class fieldType = field.getType();
            field.set(user, ReflectionUtil.castTo(fieldType, "My New Name"));
        } catch (Exception e) {
            System.out.println("asd");
        }
    }

    @Benchmark
    public void regularJava() {
        switch (fieldName) {
            case "name" :
                user.name = "My New Name";
        }
    }


}
