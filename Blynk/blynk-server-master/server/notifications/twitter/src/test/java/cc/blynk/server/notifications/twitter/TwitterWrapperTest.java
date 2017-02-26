package cc.blynk.server.notifications.twitter;

import org.junit.Ignore;
import org.junit.Test;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/7/2015.
 */
public class TwitterWrapperTest {

    @Test
    @Ignore
    public void testTweet() throws Exception {
        String token = "PUT_YOUR_TOKEN_HERE";
        String tokenSecret = "PUT_YOUR_TOKEN_SECRET_HERE";
        new TwitterWrapper().send(token, tokenSecret, "Hello444");
    }

}
