package cc.blynk.server.api.http.logic;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.notifications.Notification;
import cc.blynk.server.core.model.widgets.notifications.Twitter;
import org.junit.Test;

import java.util.concurrent.ConcurrentHashMap;

import static org.junit.Assert.assertEquals;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 08.07.16.
 */
public class TestHideSecureInfoForCloning {

    @Test
    public void testHideInfo() throws Exception {
        DashBoard dashBoard = new DashBoard();
        dashBoard.name = "123";
        dashBoard.widgets = new Widget[2];
        Twitter twitter = new Twitter();
        twitter.secret = "secret";
        twitter.token = "token";
        twitter.username = "username";
        dashBoard.widgets[0] = twitter;
        Notification notification = new Notification();
        notification.iOSTokens = new ConcurrentHashMap<>();
        notification.iOSTokens.put("uid", "token");
        notification.androidTokens = new ConcurrentHashMap<>();
        notification.androidTokens.put("uid2", "token2");
        dashBoard.widgets[1] = notification;

        assertEquals("{\"id\":0,\"name\":\"123\",\"createdAt\":0,\"updatedAt\":0,\"widgets\":[{\"type\":\"TWITTER\",\"id\":0,\"x\":0,\"y\":0,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0},{\"type\":\"NOTIFICATION\",\"id\":0,\"x\":0,\"y\":0,\"color\":0,\"width\":0,\"height\":0,\"tabId\":0,\"notifyWhenOffline\":false,\"notifyWhenOfflineIgnorePeriod\":0,\"priority\":\"normal\"}],\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isAppConnectedOn\":false,\"isShared\":false,\"isActive\":false}", HttpAPILogic.dashboardCloneWriter.writeValueAsString(dashBoard));
    }

}
