package cc.blynk.utils;

import org.junit.Test;

import java.util.concurrent.ThreadLocalRandom;

import static cc.blynk.utils.NumberUtil.NO_RESULT;
import static cc.blynk.utils.NumberUtil.parseDouble;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.01.17.
 */
public class NumberUtilTest {

    @Test
    public void testCorrectResultForInt() {
        for (int i = 0; i < 10_000; i++) {
            int random = ThreadLocalRandom.current().nextInt(Integer.MIN_VALUE, Integer.MAX_VALUE);
            double parsed = parseDouble(String.valueOf(random));
            //System.out.println(random);
            assertEquals(random, parsed, 0.0000000001);
        }
    }

    @Test
    public void testCorrectResultForDouble() {
        for (int i = 0; i < 10_000; i++) {
            double random = ThreadLocalRandom.current().nextDouble(-100000, 100000);
            double parsed = parseDouble(String.valueOf(random));
            //System.out.println(random);
            assertEquals(random, parsed, 0.0000000001);
        }
    }

    @Test
    public void testCorrectResultForDouble2() {
        for (int i = 0; i < 10_000; i++) {
            double random = ThreadLocalRandom.current().nextDouble();
            double parsed = parseDouble(String.valueOf(random));
            //System.out.println(random);
            assertEquals(random, parsed, 0.0000000001);
        }
    }

    @Test(expected = NullPointerException.class)
    public void testExpectError() {
        parseDouble(null);
    }

    @Test
    public void testExpectError2() {
        assertTrue(parseDouble("") == NO_RESULT);
    }

    @Test
    public void testExpectError3() {
        assertTrue(parseDouble("123.123F") == NO_RESULT);
    }

    @Test
    public void testExpectError4() {
        assertTrue(parseDouble("p 123.123") == NO_RESULT);
    }

    @Test
    public void testExpectError5() {
        assertTrue(parseDouble("p 123.123") == NO_RESULT);
    }

    @Test
    public void testCustomValue() {
        double d;
        d = parseDouble("0");
        assertEquals(d, 0, 0.0000000001);

        d = parseDouble("0.0");
        assertEquals(d, 0, 0.0000000001);

        d = parseDouble("1.0");
        assertEquals(d, 1.0, 0.0000000001);

        d = parseDouble("+1.0");
        assertEquals(d, 1.0, 0.0000000001);

        d = parseDouble("-1.0");
        assertEquals(d, -1.0, 0.0000000001);
    }
}
