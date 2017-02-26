package cc.blynk.utils.structure;

import cc.blynk.utils.ParseUtil;

import java.util.LinkedList;

/**
 *
 * FIFO limited array.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.09.16.
 */
public class TableLimitedQueue<T> extends LinkedList<T> {

    private static final int POOL_SIZE = ParseUtil.parseInt(System.getProperty("table.rows.pool.size", "100"));

    private final int limit;

    public TableLimitedQueue() {
        this.limit = POOL_SIZE;
    }

    protected TableLimitedQueue(int limit) {
        this.limit = limit;
    }

    @Override
    public boolean add(T o) {
        super.add(o);
        if (size() > limit) {
            super.remove();
        }
        return true;
    }

    public void order(int oldIndex, int newIndex) {
        T e = remove(oldIndex);
        add(newIndex, e);
    }


}
