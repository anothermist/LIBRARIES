package cc.blynk.server.core.dao;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 22.09.15.
 */
class SharedTokenManager {

    private final ConcurrentMap<String, SharedTokenValue> cache;

    private static final Logger log = LogManager.getLogger(SharedTokenManager.class);

    public SharedTokenManager(Iterable<User> users) {
        this.cache = new ConcurrentHashMap<String, SharedTokenValue>() {{
            for (User user : users) {
                for (DashBoard dashBoard : user.profile.dashBoards) {
                    if (dashBoard.sharedToken != null) {
                        put(dashBoard.sharedToken, new SharedTokenValue(user, dashBoard.id));
                    }
                }
            }
        }};
    }

    public void assignToken(User user, DashBoard dash, String newToken) {
        // Clean old token from cache if exists.
        String oldToken = dash.sharedToken;
        if (oldToken != null) {
            cache.remove(oldToken);
        }

        //assign new token
        dash.sharedToken = newToken;
        dash.updatedAt = System.currentTimeMillis();
        user.lastModifiedTs = dash.updatedAt;

        cache.put(newToken, new SharedTokenValue(user, dash.id));

        log.info("Generated shared token for user {} and dashId {} is {}.", user.name, dash.id, newToken);
    }

    public SharedTokenValue getUserByToken(String token) {
        return cache.get(token);
    }

    void deleteProject(DashBoard dash) {
        if (dash.sharedToken != null) {
            cache.remove(dash.sharedToken);
            log.info("Deleted {} shared token.", dash.sharedToken);
        }
    }

}
