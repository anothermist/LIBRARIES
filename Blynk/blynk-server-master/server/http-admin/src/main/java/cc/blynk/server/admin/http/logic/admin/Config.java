package cc.blynk.server.admin.http.logic.admin;

import cc.blynk.utils.JsonParser;
import cc.blynk.utils.ServerProperties;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 04.04.16.
 */
public class Config {

    String name;
    String body;

    public Config() {
    }

    public Config(String name) {
        this.name = name;
    }

    public Config(String name, String body) {
        this.name = name;
        this.body = body;
    }

    public Config(String name, ServerProperties serverProperties) {
        this.name = name;
        //return only editable options
        this.body = makeProperties(serverProperties,
                "allowed.administrator.ips",
                "user.dashboard.max.limit",
                "user.profile.max.size");
    }

    private static String makeProperties(ServerProperties properties, String... propertyNames) {
        StringBuilder sb = new StringBuilder();
        for (String name : propertyNames) {
            sb.append(name).append(" = ").append(properties.getProperty(name)).append("\n");
        }
        return sb.toString();
    }

    @Override
    public String toString() {
        try {
            return JsonParser.mapper.writeValueAsString(this);
        } catch (Exception e) {
            return "{}";
        }
    }
}
