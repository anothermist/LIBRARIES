package cc.blynk.server.reset;

import cc.blynk.server.api.http.pojo.TokenUser;
import cc.blynk.server.api.http.pojo.TokensPool;
import cc.blynk.server.core.model.AppName;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

import static org.junit.Assert.*;

@RunWith(MockitoJUnitRunner.class)
public class TokensPoolTest {

    @Test
    public void addTokenTest() {
        final TokenUser user = new TokenUser("test.gmail.com", AppName.BLYNK);
        final String token = "123";
        final TokensPool tokensPool = new TokensPool();
        tokensPool.addToken(token, user);
        assertEquals(user, tokensPool.getUser(token));
    }

    @Test
    public void addTokenTwiceTest() {
        final TokenUser user = new TokenUser("test.gmail.com", AppName.BLYNK);
        final String token = "123";
        final TokensPool tokensPool = new TokensPool();
        tokensPool.addToken(token, user);
        tokensPool.addToken(token, user);
        assertEquals(1, tokensPool.size());
    }

    @Test
    public void remoteTokenTest() {
        final TokenUser user = new TokenUser("test.gmail.com", AppName.BLYNK);
        final String token = "123";
        final TokensPool tokensPool = new TokensPool();
        tokensPool.addToken(token, user);
        assertEquals(user, tokensPool.getUser(token));
        tokensPool.removeToken(token);
        assertEquals(0, tokensPool.size());
        assertNull(tokensPool.getUser(token));
    }
}
