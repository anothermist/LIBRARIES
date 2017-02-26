package cc.blynk.utils;

import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.03.16.
 */
public class ListUtilsTest {

    private static final int SIZE = 10;

    @Test
    public void testEmptyList() {
        List<Integer> l = new ArrayList<>();
        List<?> res;

        res = ListUtils.subList(l, 1, SIZE);
        assertNotNull(res);
        assertEquals(0, res.size());

        res = ListUtils.subList(l, 10, SIZE);
        assertNotNull(res);
        assertEquals(0, res.size());
    }

    @Test
    public void test1List() {
        List<Integer> l = new ArrayList<>();
        l.add(1);
        List<?> res;

        res = ListUtils.subList(l, 1, SIZE);
        assertNotNull(res);
        assertEquals(1, res.size());
        assertEquals(1, res.get(0));
    }

    @Test
    public void testCorrectResponse() {
        List<Integer> l = new ArrayList<>();
        for (int i = 1; i <= 100; i++) {
            l.add(i);
        }
        List<?> res;

        res = ListUtils.subList(l, 1, SIZE);
        assertNotNull(res);
        assertEquals(10, res.size());
        int index;

        index = 0;
        for (int i = 1; i <= 10; i++) {
            assertEquals(i, res.get(index++));
        }

        res = ListUtils.subList(l, 9, SIZE);
        assertNotNull(res);
        assertEquals(10, res.size());

        index = 0;
        for (int i = 81; i <= 90; i++) {
            assertEquals(i, res.get(index++));
        }

        res = ListUtils.subList(l, 10, SIZE);
        assertNotNull(res);
        assertEquals(10, res.size());

        index = 0;
        for (int i = 91; i <= 100; i++) {
            assertEquals(i, res.get(index++));
        }
    }


    @Test
    public void testCorrectResponse2() {
        List<Integer> l = new ArrayList<>();
        for (int i = 1; i <= 99; i++) {
            l.add(i);
        }
        List<?> res;
        int index;

        res = ListUtils.subList(l, 10, SIZE);
        assertNotNull(res);
        assertEquals(9, res.size());

        index = 0;
        for (int i = 91; i <= 99; i++) {
            assertEquals(i, res.get(index++));
        }
    }


}
