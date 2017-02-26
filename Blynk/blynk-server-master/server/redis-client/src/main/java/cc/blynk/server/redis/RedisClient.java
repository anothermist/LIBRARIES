package cc.blynk.server.redis;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.Protocol;

import java.io.Closeable;
import java.util.Properties;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 14.10.16.
 */
public class RedisClient implements Closeable {

    public static final String REDIS_PROPERTIES = "redis.properties";
    public static final int USER_DB_INDEX = 0;
    public static final int TOKEN_DB_INDEX = 1;

    private static final Logger log = LogManager.getLogger(RedisClient.class);

    private final JedisPool tokenPool;
    private final JedisPool userPool;
    private final boolean isEnabled;

    public RedisClient(Properties props, String region) {
        this(props.getProperty("redis.host"), props.getProperty("redis.pass"), props.getProperty("redis.port"), "local".equals(region));
    }

    protected RedisClient(String host, String pass, String port, boolean isLocalMode) {
        this(host, pass, (port == null ? Protocol.DEFAULT_PORT : Integer.parseInt(port)), isLocalMode);
    }

    protected RedisClient(String host, String pass, int port, boolean isLocalMode) {
        this.isEnabled = !isLocalMode;
        if (isEnabled) {
            JedisPoolConfig config = new JedisPoolConfig();
            config.setMaxTotal(10);
            this.userPool = new JedisPool(config, host, port, Protocol.DEFAULT_TIMEOUT, pass, USER_DB_INDEX);
            this.tokenPool = new JedisPool(config, host, port, Protocol.DEFAULT_TIMEOUT, pass, TOKEN_DB_INDEX);
            checkConnected();
            log.info("Redis pool successfully initialized on {}:{}", host, port);
        } else {
            this.userPool = null;
            this.tokenPool = null;
        }
    }

    private void checkConnected() {
        try (Jedis jedis = userPool.getResource()) {
            //that fine. just check connection. do nothing.
        }
    }

    public String getServerByUser(String user) {
        if (isEnabled) {
            try (Jedis jedis = userPool.getResource()) {
                return jedis.get(user);
            } catch (Exception e) {
                log.error("Error getting server by user {}", user, e);
            }
        }
        return null;
    }

    public String getServerByToken(String token) {
        if (isEnabled) {
            try (Jedis jedis = tokenPool.getResource()) {
                return jedis.get(token);
            } catch (Exception e) {
                log.error("Error getting server by token {}.", token, e);
            }
        }
        return null;
    }

    public void assignServerToToken(String token, String server) {
        if (isEnabled) {
            try (Jedis jedis = tokenPool.getResource()) {
                jedis.set(token, server);
            } catch (Exception e) {
                log.error("Error setting server {} to token {}.", server, token, e);
            }
        }
    }

    public void assignServerToUser(String username, String server) {
        if (isEnabled) {
            try (Jedis jedis = userPool.getResource()) {
                jedis.set(username, server);
            } catch (Exception e) {
                log.error("Error setting server {} to user {}.", server, username, e);
            }
        }
    }

    public void removeToken(String... tokens) {
        if (isEnabled) {
            try (Jedis jedis = tokenPool.getResource()) {
                jedis.del(tokens);
            } catch (Exception e) {
                log.error("Error removing tokens {}.", tokens, e);
            }
        }
    }

    public JedisPool getTokenPool() {
        return tokenPool;
    }

    public JedisPool getUserPool() {
        return userPool;
    }

    @Override
    public void close() {
        if (isEnabled) {
            System.out.println("Stopping Redis...");
            try {
                userPool.destroy();
            } catch (Exception e) {
                //ignore
            }
            try {
                tokenPool.destroy();
            } catch (Exception e) {
                //ignore
            }
        }
    }
}
