package cc.blynk.utils;

import cc.blynk.core.http.model.NameCountResponse;
import cc.blynk.server.core.stats.model.CommandStat;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.lang.reflect.Field;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
public class HttpLogicUtil {

    public static final Logger log = LogManager.getLogger(HttpLogicUtil.class);

    @SuppressWarnings("unchecked")
    public static List<?> sort(List<?> list, String field, String order, boolean nameAsInt) {
        if (list.size() == 0) {
            return list;
        }

        try {
            Comparator c = new GenericComparator(list.get(0).getClass(), field, nameAsInt);
            Collections.sort(list, "ASC".equals(order) ? c : Collections.reverseOrder(c));
        } catch (NoSuchFieldException e) {
            log.warn("No order field '{}'", field);
        } catch (Exception e) {
            log.error("Problem sorting.", e);
        }

        return list;
    }

    public static List<?> sort(List<?> list, String field, String order) {
        return sort(list, field, order, false);
    }

    public static List<NameCountResponse> convertMapToPair(Map<String, ?> map) {
        return map.entrySet().stream().map(NameCountResponse::new).collect(Collectors.toList());
    }

    @SuppressWarnings("unchecked")
    public static List<NameCountResponse> convertObjectToMap(CommandStat commandStat) {
        return convertMapToPair(JsonParser.mapper.convertValue(commandStat, Map.class));
    }

    /**
     * The Blynk Project.
     * Created by Dmitriy Dumanskiy.
     * Created on 10.12.15.
     */
    public static class GenericComparator implements Comparator {

        private final Class<?> fieldType;
        private final Field field;
        private final boolean nameAsInt;

        public GenericComparator(Class<?> type, String sortField, boolean nameAsInt) throws NoSuchFieldException{
            this.field = type.getField(sortField);
            this.fieldType = field.getType();
            this.nameAsInt = nameAsInt;
        }

        @Override
        public int compare(Object o1, Object o2) {
            try {
                Object v1 = field.get(o1);
                Object v2 = field.get(o2);

                return compareActual(v1, v2, fieldType);
            } catch (Exception e) {
                throw new RuntimeException("Error on compare during sorting. Type : " + e.getMessage());
            }
        }

        private int compareActual(Object v1, Object v2, Class<?> returnType) {
            if (returnType == int.class || returnType == Integer.class) {
                return Integer.compare((int) v1, (int) v2);
            }
            if (returnType == long.class || returnType == Long.class) {
                return Long.compare((long) v1, (long) v2);
            }
            if (returnType == String.class) {
                if (nameAsInt) {
                    return Integer.valueOf((String) v1).compareTo(Integer.valueOf((String) v2));
                } else {
                    return ((String) v1).compareTo((String) v2);
                }
            }

            throw new RuntimeException("Unexpected field type. Type : " + returnType.getName());
        }

    }
}
