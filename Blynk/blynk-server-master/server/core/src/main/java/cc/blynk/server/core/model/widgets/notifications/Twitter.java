package cc.blynk.server.core.model.widgets.notifications;

import cc.blynk.server.core.model.widgets.NoPinWidget;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 21.03.15.
 */
public class Twitter extends NoPinWidget {

    private static final int MAX_TWITTER_BODY_SIZE = 140;

    public String token;

    public String secret;

    public String username;

    public static boolean isWrongBody(String body) {
       return body == null || body.isEmpty() || body.length() > MAX_TWITTER_BODY_SIZE;
    }

    @Override
    public int getPrice() {
        return 0;
    }
}
