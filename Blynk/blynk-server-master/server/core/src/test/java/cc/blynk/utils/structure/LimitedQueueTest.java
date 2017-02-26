package cc.blynk.utils.structure;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 24.05.16.
 */
public class LimitedQueueTest {

    @Test
    public void addLimitTest() {
        TableLimitedQueue<String> list = new TableLimitedQueue<>(2);
        list.add("1");
        list.add("2");
        list.add("3");
        assertEquals(2, list.size());
        assertEquals("2", list.get(0));
        assertEquals("3", list.get(1));
    }

    @Test
    public void orderTest() {
        TableLimitedQueue<String> list = makeList();

        list.order(0, 1);
        assertEquals(3, list.size());
        assertEquals("2", list.get(0));
        assertEquals("1", list.get(1));
        assertEquals("3", list.get(2));

        list = makeList();

        list.order(0, 2);
        assertEquals(3, list.size());
        assertEquals("2", list.get(0));
        assertEquals("3", list.get(1));
        assertEquals("1", list.get(2));

        list = makeList();

        list.order(1, 2);
        assertEquals(3, list.size());
        assertEquals("1", list.get(0));
        assertEquals("3", list.get(1));
        assertEquals("2", list.get(2));

        list = makeList();

        list.order(2, 0);
        assertEquals(3, list.size());
        assertEquals("3", list.get(0));
        assertEquals("1", list.get(1));
        assertEquals("2", list.get(2));
    }

    private static TableLimitedQueue<String> makeList() {
        return new TableLimitedQueue<String>(3) {{
            add("1");
            add("2");
            add("3");
        }
        };
    }

}
