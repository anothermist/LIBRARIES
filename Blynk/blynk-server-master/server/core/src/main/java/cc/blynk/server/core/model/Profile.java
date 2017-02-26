package cc.blynk.server.core.model;

import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.utils.JsonParser;

import java.util.Arrays;

import static cc.blynk.utils.ArrayUtil.EMPTY_DASHBOARDS;

/**
 * User: ddumanskiy
 * Date: 21.11.13
 * Time: 13:04
 */
public class Profile {

    public volatile DashBoard[] dashBoards = EMPTY_DASHBOARDS;

    /**
     * Check if dashboardId is real and exists in user profile.
     */
    public void validateDashId(int dashId) {
        for (DashBoard dashBoard : dashBoards) {
            if (dashBoard.id == dashId) {
                return;
            }
        }

        throw new IllegalCommandException("Requested token for non-existing '" + dashId + "' dash id.");
    }

    public int getDashIndexOrThrow(int dashId) {
        for (int i = 0; i < dashBoards.length; i++) {
            if (dashBoards[i].id == dashId) {
                return i;
            }
        }
        throw new IllegalCommandException("Dashboard with passed id not found.");
    }

    public DashBoard getDashByIdOrThrow(int id) {
        for (DashBoard dashBoard : dashBoards) {
            if (dashBoard.id == id) {
                return dashBoard;
            }
        }
        throw new IllegalCommandException("Dashboard with passed id not found.");
    }

    public DashBoard getDashById(int id) {
        for (DashBoard dashBoard : dashBoards) {
            if (dashBoard.id == id) {
                return dashBoard;
            }
        }
        return null;
    }

    @Override
    public String toString() {
        return JsonParser.toJson(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Profile that = (Profile) o;

        if (!Arrays.equals(dashBoards, that.dashBoards)) return false;

        return true;
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(dashBoards);
    }
}
