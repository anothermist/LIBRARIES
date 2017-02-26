package cc.blynk.server.api.http.logic.business;

import cc.blynk.server.core.model.auth.User;
import io.netty.handler.codec.http.cookie.Cookie;

import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 12.05.16.
 */
public class SessionHolder {

    private final ConcurrentMap<String, User> httpSession = new ConcurrentHashMap<>();

    public String generateNewSession(User user) {
        String sessionId = UUID.randomUUID().toString();
        httpSession.put(sessionId, user);
        return sessionId;
    }

    public boolean isValid(Cookie cookie) {
        return cookie.name().equals(Cookies.SESSION_COOKIE);
    }

    public User getUser(String token) {
        return httpSession.get(token);
    }

}
