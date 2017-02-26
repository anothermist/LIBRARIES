package cc.blynk.utils;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.auth.FacebookTokenResponse;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.model.device.Tag;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.notifications.Notification;
import cc.blynk.server.core.model.widgets.notifications.Twitter;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandBodyException;
import cc.blynk.server.core.stats.model.Stat;
import cc.blynk.utils.serialization.NotificationIgnoreMixIn;
import cc.blynk.utils.serialization.TwitterIgnoreMixIn;
import com.fasterxml.jackson.annotation.JsonAutoDetect;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.PropertyAccessor;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectReader;
import com.fasterxml.jackson.databind.ObjectWriter;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.File;
import java.io.IOException;
import java.util.StringJoiner;

/**
 * User: ddumanskiy
 * Date: 21.11.13
 * Time: 15:31
 */
public final class JsonParser {

    private static final Logger log = LogManager.getLogger(JsonParser.class);

    //it is threadsafe
    public static final ObjectMapper mapper = init();

    private static final ObjectReader userReader = mapper.readerFor(User.class);
    private static final ObjectReader dashboardReader = mapper.readerFor(DashBoard.class);
    private static final ObjectReader widgetReader = mapper.readerFor(Widget.class);
    private static final ObjectReader deviceReader = mapper.readerFor(Device.class);
    private static final ObjectReader tagReader = mapper.readerFor(Tag.class);
    private static final ObjectReader facebookTokenReader = mapper.readerFor(FacebookTokenResponse.class);

    private static final ObjectWriter userWriter = mapper.writerFor(User.class);
    private static final ObjectWriter profileWriter = mapper.writerFor(Profile.class);
    private static final ObjectWriter dashboardWriter = mapper.writerFor(DashBoard.class);
    private static final ObjectWriter deviceWriter = mapper.writerFor(Device.class);
    //new init() here is necessary
    private static final ObjectWriter sharedDashboardWriter = init()
            .addMixIn(Notification.class, NotificationIgnoreMixIn.class)
            .addMixIn(Twitter.class, TwitterIgnoreMixIn.class).writerFor(DashBoard.class);

    private static final ObjectWriter statWriter = init().writerWithDefaultPrettyPrinter().forType(Stat.class);

    public static ObjectMapper init() {
        return new ObjectMapper()
                .configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false)
                .setSerializationInclusion(JsonInclude.Include.NON_NULL)
                .setSerializationInclusion(JsonInclude.Include.NON_EMPTY)
                .setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.NONE)
                .setVisibility(PropertyAccessor.FIELD, JsonAutoDetect.Visibility.ANY);
    }

    public static String toJson(User user) {
        return toJson(userWriter, user);
    }

    public static String toJson(Profile profile) {
        return toJson(profileWriter, profile);
    }

    public static String toJson(DashBoard dashBoard) {
        return toJson(dashboardWriter, dashBoard);
    }

    public static String toJsonSharedDashboard(DashBoard dashBoard) {
        return toJson(sharedDashboardWriter, dashBoard);
    }

    public static String toJson(Device device) {
        return toJson(deviceWriter, device);
    }

    public static String toJson(Stat stat) {
        return toJson(statWriter, stat);
    }

    public static void writeUser(File file, User user) throws IOException {
        userWriter.writeValue(file, user);
    }

    private static String toJson(ObjectWriter writer, Object o) {
        try {
            return writer.writeValueAsString(o);
        } catch (Exception e) {
            log.error("Error jsoning object.", e);
        }
        return "{}";
    }

    public static String toJson(Object o) {
        try {
            return mapper.writeValueAsString(o);
        } catch (Exception e) {
            log.error("Error jsoning object.", e);
        }
        return null;
    }

    public static <T> T readAny(String val, Class<T> c) {
        try {
            return mapper.readValue(val, c);
        } catch (Exception e) {
            log.error("Error reading json object.", e);
        }
        return null;
    }

    public static User parseUserFromFile(File userFile) throws IOException {
        return userReader.readValue(userFile);
    }

    public static User parseUserFromString(String userString) throws IOException {
        return userReader.readValue(userString);
    }

    public static FacebookTokenResponse parseFacebookTokenResponse(String response) throws IOException {
        return facebookTokenReader.readValue(response);
    }

    public static DashBoard parseDashboard(String reader) {
        try {
            return dashboardReader.readValue(reader);
        } catch (IOException e) {
            log.error(e.getMessage());
            throw new IllegalCommandBodyException("Error parsing dashboard.");
        }
    }

    public static Widget parseWidget(String reader) {
        try {
            return widgetReader.readValue(reader);
        } catch (IOException e) {
            log.error(e.getMessage());
            throw new IllegalCommandBodyException("Error parsing widget.");
        }
    }

    public static Device parseDevice(String reader) {
        try {
            return deviceReader.readValue(reader);
        } catch (IOException e) {
            log.error(e.getMessage());
            throw new IllegalCommandBodyException("Error parsing device.");
        }
    }

    public static Tag parseTag(String reader) {
        try {
            return tagReader.readValue(reader);
        } catch (IOException e) {
            log.error(e.getMessage());
            throw new IllegalCommandBodyException("Error parsing tag.");
        }
    }


    public static String valueToJsonAsString(String[] values) {
        StringJoiner sj = new StringJoiner(",", "[", "]");
        for (String value : values) {
            sj.add(makeJsonStringValue(value));
        }
        return sj.toString();
    }

    public static String valueToJsonAsString(String value) {
        return "[\"" + value  + "\"]";
    }

    private static String makeJsonStringValue(String value) {
        return "\"" + value  + "\"";
    }

}
