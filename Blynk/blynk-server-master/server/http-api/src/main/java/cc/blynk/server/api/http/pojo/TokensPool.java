package cc.blynk.server.api.http.pojo;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * The Blynk project
 * Created by Andrew Zakordonets
 * Date : 12/05/2015.
 */
public final class TokensPool {

    private static final Logger log = LogManager.getLogger(TokensPool.class);
    private static final int TOKEN_EXPIRATION_DEFAULT_PERIOD_IN_MILLIS = 60 * 60 * 1000;
    private final ConcurrentMap<String, TokenUser> holder;

    public TokensPool() {
        this.holder = new ConcurrentHashMap<>();
    }

    public void addToken(String token, TokenUser user) {
        log.info("Adding token for {} user to the pool", user.email);
        cleanupOldTokens();
        holder.put(token, user);
    }

    public TokenUser getUser(String token) {
        cleanupOldTokens();
        return holder.get(token);
    }

    public void removeToken(String token) {
        holder.remove(token);
    }

    public int size() {
        return holder.size();
    }

    private void cleanupOldTokens() {
        final long now = System.currentTimeMillis();
        for (Iterator<Map.Entry<String, TokenUser>> iterator = holder.entrySet().iterator(); iterator.hasNext();) {
            Map.Entry<String, TokenUser> entry = iterator.next();
            if (entry.getValue().createdAt + TOKEN_EXPIRATION_DEFAULT_PERIOD_IN_MILLIS < now) {
                iterator.remove();
            }
        }
    }

}
