package cc.blynk.server.db.dao;

import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.utils.JsonParser;
import com.zaxxer.hikari.HikariDataSource;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.03.16.
 */
public class UserDBDao {

    public static final String upsertUser = "INSERT INTO users (username, appName, region, json) VALUES (?, ?, ?, ?) ON CONFLICT (username, appName) DO UPDATE SET json = EXCLUDED.json, region = EXCLUDED.region";
    public static final String selectAllUsers = "SELECT * from users";

    private static final Logger log = LogManager.getLogger(UserDBDao.class);
    private final HikariDataSource ds;

    public UserDBDao(HikariDataSource ds) {
        this.ds = ds;
    }

    public ConcurrentMap<UserKey, User> getAllUsers() throws Exception {
        ResultSet rs = null;
        ConcurrentMap<UserKey, User> users = new ConcurrentHashMap<>();

        try (Connection connection = ds.getConnection();
             Statement statement = connection.createStatement()) {

            rs = statement.executeQuery(selectAllUsers);

            while (rs.next()) {
                User user = JsonParser.parseUserFromString(rs.getString("json"));
                user.appName = rs.getString("appName");
                user.region = rs.getString("region");
                users.put(new UserKey(user), user);
            }
            connection.commit();
        } finally {
            if (rs != null) {
                rs.close();
            }
        }

        log.info("Loaded {} users.", users.size());

        return users;
    }

    public void save(ArrayList<User> users) {
        long start = System.currentTimeMillis();
        log.info("Storing users...");

        try (Connection connection = ds.getConnection();
             PreparedStatement ps = connection.prepareStatement(upsertUser)) {

            for (User user : users) {
                ps.setString(1, user.name);
                ps.setString(2, user.appName);
                ps.setString(3, user.region);
                ps.setString(4, user.toString());
                ps.addBatch();
            }

            ps.executeBatch();
            connection.commit();
        } catch (Exception e) {
            log.error("Error upserting users in DB.", e);
        }
        log.info("Storing users finished. Time {}. Users saved {}", System.currentTimeMillis() - start, users.size());
    }
}
