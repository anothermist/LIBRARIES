package cc.blynk.server.api.http.logic.ide;

import cc.blynk.core.http.Response;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.redis.RedisClient;
import io.netty.handler.codec.http.HttpResponseStatus;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import java.util.HashMap;
import java.util.Map;

import static cc.blynk.server.api.http.logic.ide.IDEAuthLogic.IDE_AUTHORIZE_ENDPOINT;
import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.when;

/**
 * @author gig.
 */
@RunWith(MockitoJUnitRunner.class)
public class IDEAuthLogicTest {

    public static final String TEST_USER1 = "test1@user.com";
    public static final String TEST_USER2 = "test2@user.com";
    public static final String PASS = "pass";
    public static final String SERVER_NAME = "127.0.0.1";
    public static final String REDIRECT = String.format("//%s%s", SERVER_NAME, IDE_AUTHORIZE_ENDPOINT);

    @Mock
    private UserDao userDao;

    @Mock
    private RedisClient redisClient;

    private IDEAuthLogic ideAuthLogic;

    private final User user = new User(TEST_USER1, PASS, "", "", false);

    @Before
    public void setUp() throws Exception {
        ideAuthLogic = new IDEAuthLogic(userDao, redisClient);
        when(userDao.getByName(TEST_USER1, AppName.BLYNK)).thenReturn(user);
        when(userDao.getByName(TEST_USER2, AppName.BLYNK)).thenReturn(null);
        when(redisClient.getServerByUser(TEST_USER2)).thenReturn(SERVER_NAME);
    }

    @Test
    public void authorizeOk() throws Exception {
        Response response = ideAuthLogic.authorize(getMap(TEST_USER1, PASS));

        assertEquals(HttpResponseStatus.OK, response.status());
    }

    @Test
    public void authorizeBad() throws Exception {
        Response response = ideAuthLogic.authorize(getMap(TEST_USER1, "wrong pass"));

        assertEquals(HttpResponseStatus.BAD_REQUEST, response.status());
    }

    @Test
    public void authorizeRedirect() throws Exception {
        Response response = ideAuthLogic.authorize(getMap(TEST_USER2, PASS));

        assertEquals(HttpResponseStatus.MOVED_PERMANENTLY, response.status());
        assertEquals(REDIRECT, response.headers().get("location"));
    }

    private Map<String, String> getMap(String email, String pass) {
        Map<String, String> map = new HashMap<>();
        map.put("email", email);
        map.put("password", pass);
        return map;
    }

}