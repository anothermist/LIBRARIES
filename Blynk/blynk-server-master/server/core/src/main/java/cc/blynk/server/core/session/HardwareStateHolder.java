package cc.blynk.server.core.session;

import cc.blynk.server.core.model.auth.User;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.09.15.
 */
public final class HardwareStateHolder extends StateHolderBase {

    public final int dashId;
    public final int deviceId;
    public final String token;

    public HardwareStateHolder(int dashId, int deviceId, User user, String token) {
        super(user);
        this.dashId = dashId;
        this.deviceId = deviceId;
        this.token = token;
    }

    @Override
    public boolean contains(String sharedToken) {
        return false;
    }
}
