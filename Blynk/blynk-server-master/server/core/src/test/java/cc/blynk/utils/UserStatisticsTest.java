package cc.blynk.utils;

import cc.blynk.server.core.dao.FileManager;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.widgets.Widget;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Base64;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.zip.Deflater;
import java.util.zip.DeflaterOutputStream;

/**
 * User: ddumanskiy
 * Date: 09.12.13
 * Time: 8:07
 */
@Ignore
public class UserStatisticsTest {

    static FileManager fileManager;
    static Map<UserKey, User> users;

    @BeforeClass
    public static void init() {
        fileManager = new FileManager("/home/doom369/test/root/data");
        users = fileManager.deserialize();
    }

    public static byte[] compress(byte[] data) throws Exception {
        //todo calculate size
        ByteArrayOutputStream baos = new ByteArrayOutputStream(8192);

        try (OutputStream out = new DeflaterOutputStream(baos, new Deflater(Deflater.BEST_COMPRESSION))) {
            out.write(data);
        }
        return baos.toByteArray();
    }

    @Test
    public void read() {
        System.out.println("Registered users : " + users.size());
    }

    @Test
    public void printBoardTypes() {
        System.out.println();
        System.out.println("Board Types :");
        Map<String, Integer> boards = new HashMap<>();
        for (User user : users.values()) {
                for (DashBoard dashBoard : user.profile.dashBoards) {
                    String type = dashBoard.boardType;
                    Integer i = boards.get(type);
                    if (i == null) {
                        i = 0;
                    }
                    boards.put(type, ++i);
                }
        }

        for (Map.Entry<String, Integer> entry : boards.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
    }

    @Test
    public void printWidgetUsage() {
        System.out.println();
        System.out.println("Widget Usage :");
        Map<String, Integer> boards = new HashMap<>();
        for (User user : users.values()) {
                for (DashBoard dashBoard : user.profile.dashBoards) {
                    if (dashBoard.widgets != null) {
                        for (Widget widget : dashBoard.widgets) {
                            Integer i = boards.get(widget.getClass().getSimpleName());
                            if (i == null) {
                                i = 0;
                            }
                            boards.put(widget.getClass().getSimpleName(), ++i);
                        }
                    }
                }
        }

        for (Map.Entry<String, Integer> entry : boards.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }
    }

    @Test
    public void printDashFilling() {
        System.out.println();
        System.out.println("Dashboard Space Usage :");

        List<Integer> all = new ArrayList<>();
        for (User user : users.values()) {
                for (DashBoard dashBoard : user.profile.dashBoards) {
                    if (dashBoard.widgets.length > 3) {
                        int sum = 0;
                        for (Widget widget : dashBoard.widgets) {
                           sum += widget.height * widget.width;
                        }
                        all.add(sum);
                    }
            }
        }

        Collections.sort(all);

        System.out.println("Mediana of cells used : " + all.get(all.size() / 2));
        System.out.println("Avg. percentage of space filling : " + all.get(all.size() / 2) * 100 / 72 + "%");
        //System.out.println("Average filled square per dash : " + (sum / dashes));
        //System.out.println("Percentage : " + (int)((sum / dashes) * 100 / 72));
    }

    @Test
    public void printOutdatedProfiles() {
        long now = System.currentTimeMillis();
        int counter = 0;
        long PERIOD = 1000L * 60 * 60 * 24 * 90;
        for (User user : users.values()) {
            if (user.lastModifiedTs < (now - PERIOD)) {
                counter++;
            }
        }
        System.out.println("Profiles not updated more then 3 months : " + counter);
    }

    @Test
    @Ignore
    public void findLargestDashboard() throws Exception{
        int max = 0;
        String maxDash = null;
        for (User user : users.values()) {
                for (DashBoard dashBoard : user.profile.dashBoards) {
                    if (dashBoard.widgets.length > 0) {
                        String json = JsonParser.toJson(dashBoard);
                        byte[] compressed = compress(json.getBytes());
                        if (compressed.length > max) {
                            max = json.length();
                            maxDash = json;
                        }
                    }
                }
        }

        System.out.println(maxDash);
    }

    @Test
    public void dashesPerUser() {
        int usersCounter = 0;
        int dashesCounter = 0;
        int maxDashes = 0;
        int widgetCount = 0;
        for (User user : users.values()) {
            if (user.profile.dashBoards.length == 0) {
                continue;
            }
            usersCounter++;
            dashesCounter += user.profile.dashBoards.length;
            maxDashes = Math.max(user.profile.dashBoards.length, maxDashes);
            for (DashBoard dash : user.profile.dashBoards) {
                widgetCount += dash.widgets.length;
            }
        }

        System.out.println("Dashboards per user : " + (double) dashesCounter / usersCounter);
        System.out.println("Widgets per user : " + (double) widgetCount / usersCounter);
        System.out.println("Max dashes : " + maxDashes);
    }

    @Test
    @Ignore
    public void testGetByToken() {
        String token = "";
        User user = getByToken(token, users);
        if (user == null) {
            System.out.println("No user");
        } else {
            System.out.println(user.name);
        }
    }

    private User getByToken(String token, Map<UserKey, User> users) {
        for (User user : users.values()) {
            if (user.dashTokens.values().contains(token)) {
                return user;
            }
        }
        return null;
    }

    @Test
    @Ignore
    public void testCompression() throws Exception{
        String largestDash = "{\"id\":2116221194,\"name\":\"Gusmer counter \",\"timestamp\":1431619485,\"widgets\":[{\"type\":\"NOTIFICATION\",\"id\":335157832,\"x\":5,\"y\":8,\"color\":-308477697,\"width\":2,\"height\":1,\"iOSToken\":\"lp7_YoJck4g:APA91bH7lOlsBsZGHwaguGYtkuJGXrcfJEA4i8NYCypwDd971W45ZNkvG7Tzonmla2zzGlB1l07XRmVRShqztbFjvR2L0KPV6gRj2NcrlaHXqVDlWEcaiTPQIRv006z4a3ku6eULSOZT\",\"notifyWhenOffline\":false},{\"type\":\"DIGIT4_DISPLAY\",\"id\":395092900,\"x\":0,\"y\":6,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Injection nr\",\"pinType\":\"VIRTUAL\",\"pin\":1,\"min\":0,\"max\":1023,\"frequency\":1000,\"mappingMode\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":1192914074,\"x\":4,\"y\":3,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Per injection\",\"min\":0,\"max\":1023,\"frequency\":1000,\"mappingMode\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":1178562802,\"x\":6,\"y\":3,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Kg Per Inj.\",\"min\":0,\"max\":1023,\"frequency\":1000,\"mappingMode\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":2113736921,\"x\":4,\"y\":4,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Right heater\",\"min\":0,\"max\":1023,\"frequency\":1000,\"mappingMode\":0},{\"type\":\"BUTTON\",\"id\":2120531847,\"x\":0,\"y\":4,\"color\":616861439,\"width\":2,\"height\":2,\"label\":\"Reset to 0\",\"pushMode\":true},{\"type\":\"BUTTON\",\"id\":1812157287,\"x\":0,\"y\":7,\"color\":616861439,\"width\":2,\"height\":2,\"pinType\":\"VIRTUAL\",\"pin\":0,\"pushMode\":true},{\"type\":\"BUTTON\",\"id\":1126659723,\"x\":6,\"y\":4,\"color\":616861439,\"width\":2,\"height\":2,\"label\":\"Reset to 0\",\"pushMode\":true},{\"type\":\"LED\",\"id\":1377711592,\"x\":2,\"y\":8,\"color\":-308477697,\"width\":1,\"height\":1,\"frequency\":0},{\"type\":\"TIMER\",\"id\":298635238,\"x\":5,\"y\":6,\"color\":-308477697,\"width\":3,\"height\":1,\"label\":\"Time is\",\"startTime\":73958,\"startValue\":\"1\",\"stopTime\":74018,\"stopValue\":\"0\"},{\"type\":\"LED\",\"id\":364399500,\"x\":7,\"y\":7,\"color\":-308477697,\"width\":1,\"height\":1,\"frequency\":0},{\"type\":\"LED\",\"id\":778846748,\"x\":5,\"y\":5,\"color\":-308477697,\"width\":1,\"height\":1,\"label\":\"Of\",\"frequency\":0},{\"type\":\"TIMER\",\"id\":997498635,\"x\":2,\"y\":6,\"color\":-308477697,\"width\":3,\"height\":1,\"label\":\"Timer\",\"startTime\":82800,\"startValue\":\"1\",\"stopTime\":82800,\"stopValue\":\"0\"},{\"type\":\"LED\",\"id\":274842833,\"x\":2,\"y\":7,\"color\":-308477697,\"width\":1,\"height\":1,\"frequency\":0},{\"type\":\"BUTTON\",\"id\":752806213,\"x\":3,\"y\":7,\"color\":616861439,\"width\":2,\"height\":2,\"pushMode\":true},{\"type\":\"LED\",\"id\":357616364,\"x\":3,\"y\":5,\"color\":-308477697,\"width\":1,\"height\":1,\"label\":\"Of\",\"frequency\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":1356688125,\"x\":2,\"y\":4,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Left heater\",\"min\":0,\"max\":60,\"frequency\":1000,\"mappingMode\":0},{\"type\":\"GAUGE\",\"id\":19562774,\"x\":4,\"y\":0,\"color\":-308477697,\"width\":4,\"height\":3,\"label\":\"Presure\",\"frequency\":1000,\"min\":0,\"max\":1023,\"mappingMode\":0},{\"type\":\"LED\",\"id\":189585006,\"x\":7,\"y\":8,\"color\":-308477697,\"width\":1,\"height\":1,\"frequency\":0},{\"type\":\"LED\",\"id\":24032975,\"x\":2,\"y\":5,\"color\":-308477697,\"width\":1,\"height\":1,\"label\":\"On\",\"frequency\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":1564351632,\"x\":0,\"y\":3,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Counter\",\"min\":0,\"max\":6000,\"frequency\":500,\"mappingMode\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":1225335621,\"x\":2,\"y\":3,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Total Kg\",\"min\":0,\"max\":2000,\"frequency\":500,\"mappingMode\":0},{\"type\":\"GAUGE\",\"id\":173315013,\"x\":0,\"y\":0,\"color\":616861439,\"width\":4,\"height\":3,\"label\":\"Pressure\",\"frequency\":1000,\"min\":0,\"max\":1023,\"mappingMode\":0},{\"type\":\"LED\",\"id\":1946992022,\"x\":4,\"y\":5,\"color\":-308477697,\"width\":1,\"height\":1,\"label\":\"On\",\"frequency\":0},{\"type\":\"DIGIT4_DISPLAY\",\"id\":1183934061,\"x\":5,\"y\":7,\"color\":-308477697,\"width\":2,\"height\":1,\"label\":\"Average \",\"min\":0,\"max\":1023,\"frequency\":1000,\"mappingMode\":0}],\"boardType\":\"Arduino Mega\",\"keepScreenOn\":false}";
        System.out.println("Dash json size : " + largestDash.length());
        System.out.println("byte json size : " + largestDash.getBytes().length);

        byte[] compressed = compress(largestDash.getBytes());
        System.out.println("Dash json size compressed : " + compressed.length);
        System.out.println("compression rate : " + ((double) largestDash.length() / compressed.length));

        String res = Base64.getEncoder().encodeToString(compressed);

        System.out.println(res.length());
        System.out.println(res);

        byte[] data = Base64.getDecoder().decode(res);
        byte[] decompressedData = ByteUtils.decompress(data);
        System.out.println(new String(decompressedData));
    }

}
