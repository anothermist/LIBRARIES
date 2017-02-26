package cc.blynk.server.core.model.widgets.ui.table;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.09.16.
 */
public class Row {

    public int id;

    public String name;

    public String value;

    public boolean isSelected;

    public Row() {
    }

    public Row(int id, String name, String value) {
        this.id = id;
        this.name = name;
        this.value = value;
        this.isSelected = true;
    }
}
