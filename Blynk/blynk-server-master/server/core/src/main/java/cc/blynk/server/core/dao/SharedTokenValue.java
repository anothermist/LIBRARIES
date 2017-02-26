package cc.blynk.server.core.dao;

import cc.blynk.server.core.model.auth.User;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 17.11.16.
 */
public class SharedTokenValue {

    public final User user;

    public final int dashId;

    public SharedTokenValue(User user, int dashId) {
        this.user = user;
        this.dashId = dashId;
    }

}
