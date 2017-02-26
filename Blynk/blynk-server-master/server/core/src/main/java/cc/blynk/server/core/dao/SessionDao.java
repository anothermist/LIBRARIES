package cc.blynk.server.core.dao;

import cc.blynk.server.core.model.auth.Session;
import io.netty.channel.EventLoop;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * Holds session info related to specific user.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/18/2015.
 */
public class SessionDao {

    private static final Logger log = LogManager.getLogger(SessionDao.class);

    public final ConcurrentMap<UserKey, Session> userSession = new ConcurrentHashMap<>();

    //threadsafe
    public Session getOrCreateSessionByUser(UserKey key, EventLoop initialEventLoop) {
        Session group = userSession.get(key);
        //only one side came
        if (group == null) {
            Session value = new Session(initialEventLoop);
            group = userSession.putIfAbsent(key, value);
            if (group == null) {
                log.trace("Creating unique session for user: {}", key);
                return value;
            }
        }

        return group;
    }

}