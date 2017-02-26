package cc.blynk.utils;

import java.util.List;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 06.12.15.
 */
public class ListUtils {

    public static List<?> subList(List<?> list, int page, int size) {
        return list.subList(
                Math.min(list.size(), (page - 1)  * size),
                Math.min(list.size(), size * page)
        );

        //below doesn't work with java 1.8_32 due to java bug.
        /*
        return list.stream()
                .skip((page - 1)  * size)
                .limit(size)
                .collect(Collectors.toList());
        */
    }


}
