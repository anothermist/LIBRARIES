package cc.blynk.server.core.stats.model;

import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.utils.JsonParser;

import java.util.Map;
import java.util.concurrent.atomic.LongAdder;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.07.15.
 */
public class Stat {

    private final static long ONE_DAY = 24 * 60 * 60 * 1000;
    private final static long ONE_WEEK = 7 * ONE_DAY;
    private final static long ONE_MONTH = 30 * ONE_DAY;

    public final CommandStat commands = new CommandStat();
    public final HttpStat http = new HttpStat();

    public final int oneMinRate;
    public final int registrations;
    public final int active;
    public final int activeWeek;
    public final int activeMonth;
    public final int connected;
    public final int onlineApps;
    public final int totalOnlineApps;
    public final int onlineHards;
    public final int totalOnlineHards;
    public final transient long ts;

    //for tests only
    public Stat(int oneMinRate, int registrations, int active, int activeWeek,
                int activeMonth, int connected, int onlineApps,
                int totalOnlineApps, int onlineHards, int totalOnlineHards, long ts) {
        this.oneMinRate = oneMinRate;
        this.registrations = registrations;
        this.active = active;
        this.activeWeek = activeWeek;
        this.activeMonth = activeMonth;
        this.connected = connected;
        this.onlineApps = onlineApps;
        this.totalOnlineApps = totalOnlineApps;
        this.onlineHards = onlineHards;
        this.totalOnlineHards = totalOnlineHards;
        this.ts = ts;
    }

    public Stat(SessionDao sessionDao, UserDao userDao, GlobalStats globalStats, boolean reset) {
        //yeap, some stats updates may be lost (because of sumThenReset()),
        //but we don't care, cause this is just for general monitoring
        for (Short command : Command.valuesName.keySet()) {
            LongAdder longAdder = globalStats.specificCounters[command];
            int val = (int) (reset ? longAdder.sumThenReset() : longAdder.sum());

            this.http.assign(command, val);
            this.commands.assign(command, val);
        }

        this.commands.appTotal = (int) globalStats.getTotalAppCounter(reset);
        this.commands.hardTotal = (int) globalStats.getTotalHardCounter(reset);

        this.oneMinRate = (int) globalStats.totalMessages.getOneMinuteRate();
        int connectedSessions = 0;

        int hardActive = 0;
        int totalOnlineHards = 0;

        int appActive = 0;
        int totalOnlineApps = 0;

        int active = 0;
        int activeWeek = 0;
        int activeMonth = 0;

        this.ts = System.currentTimeMillis();
        for (Map.Entry<UserKey, Session> entry: sessionDao.userSession.entrySet()) {
            Session session = entry.getValue();

            if (session.isHardwareConnected() && session.isAppConnected()) {
                connectedSessions++;
            }
            if (session.isHardwareConnected()) {
                hardActive++;
                totalOnlineHards += session.hardwareChannels.size();
            }
            if (session.isAppConnected()) {
                appActive++;
                totalOnlineApps += session.appChannels.size();
            }
            UserKey userKey = entry.getKey();
            User user = userDao.users.get(userKey);

            if (user != null) {
                if (this.ts - user.lastModifiedTs < ONE_DAY || dashUpdated(user, this.ts, ONE_DAY)) {
                    active++;
                    activeWeek++;
                    activeMonth++;
                    continue;
                }
                if (this.ts - user.lastModifiedTs < ONE_WEEK || dashUpdated(user, this.ts, ONE_WEEK)) {
                    activeWeek++;
                    activeMonth++;
                    continue;
                }
                if (this.ts - user.lastModifiedTs < ONE_MONTH || dashUpdated(user, this.ts, ONE_MONTH)) {
                    activeMonth++;
                }
            }
        }

        this.connected = connectedSessions;
        this.onlineApps = appActive;
        this.totalOnlineApps = totalOnlineApps;
        this.onlineHards = hardActive;
        this.totalOnlineHards = totalOnlineHards;

        this.active = active;
        this.activeWeek = activeWeek;
        this.activeMonth = activeMonth;
        this.registrations = userDao.users.size();
    }

    private boolean dashUpdated(User user, long now, long period) {
        for (DashBoard dash : user.profile.dashBoards) {
            if (now - dash.updatedAt < period) {
                return true;
            }
        }
        return false;
    }

    public String toJson() {
        return JsonParser.toJson(this);
    }
}
