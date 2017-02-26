package cc.blynk.server.notifications.twitter;

import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.auth.AccessToken;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/6/2015.
 */
public class TwitterWrapper {

    // The factory instance is re-usable and thread safe.
    private final TwitterFactory factory = new TwitterFactory();

    public void send(String token, String secret, String message) throws TwitterException {
        AccessToken accessToken = new AccessToken(token, secret);
        Twitter twitter = factory.getInstance();
        twitter.setOAuthAccessToken(accessToken);
        twitter.updateStatus(message);
    }

}
