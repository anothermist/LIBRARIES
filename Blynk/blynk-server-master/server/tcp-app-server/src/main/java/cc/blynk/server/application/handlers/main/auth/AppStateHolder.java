package cc.blynk.server.application.handlers.main.auth;

import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.session.StateHolderBase;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.09.15.
 */
public class AppStateHolder extends StateHolderBase {

    public final OsType osType;
    public final String version;

    public AppStateHolder(User user, OsType osType, String version) {
        super(user);
        this.osType = osType;
        this.version = version;
    }

    @Override
    public boolean contains(String sharedToken) {
        return true;
    }
}
