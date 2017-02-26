package cc.blynk.server.admin.http.logic.admin;

import cc.blynk.core.http.Response;
import cc.blynk.core.http.annotation.GET;
import cc.blynk.core.http.annotation.Path;
import cc.blynk.core.http.annotation.QueryParam;
import cc.blynk.server.Holder;
import cc.blynk.server.admin.http.response.IpNameResponse;
import cc.blynk.server.admin.http.response.RequestPerSecondResponse;
import cc.blynk.server.core.dao.FileManager;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.core.stats.model.Stat;
import cc.blynk.utils.HttpLogicUtil;
import cc.blynk.utils.JsonParser;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import static cc.blynk.core.http.Response.ok;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
@Path("/stats")
public class StatsLogic extends HttpLogicUtil {

    private final GlobalStats stats;
    private final SessionDao sessionDao;
    private final UserDao userDao;
    private final FileManager fileManager;

    public StatsLogic(Holder holder) {
        this.userDao = holder.userDao;
        this.sessionDao = holder.sessionDao;
        this.stats = holder.stats;
        this.fileManager = holder.fileManager;
    }

    @GET
    @Path("/realtime")
    public Response getReatime() {
       return ok(Collections.singletonList(new Stat(sessionDao, userDao, stats, false)));
    }

    @GET
    @Path("/requestsPerUser")
    public Response getRequestPerUser(@QueryParam("_sortField") String sortField,
                                          @QueryParam("_sortDir") String sortOrder) {
        List<RequestPerSecondResponse> res = new ArrayList<>();
        for (Map.Entry<UserKey, Session> entry : sessionDao.userSession.entrySet()) {
            Session session = entry.getValue();

            int appReqRate = session.getAppRequestRate();
            int hardReqRate = session.getHardRequestRate();

            if (appReqRate > 0 || hardReqRate > 0) {
                res.add(new RequestPerSecondResponse(entry.getKey().name, appReqRate, hardReqRate));
            }
        }
        return ok(sort(res, sortField, sortOrder));
    }

    @GET
    @Path("/messages")
    public Response getMessages(@QueryParam("_sortField") String sortField,
                                    @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertObjectToMap(new Stat(sessionDao, userDao, stats, false).commands), sortField, sortOrder));
    }

    @GET
    @Path("/widgets")
    public Response getWidgets(@QueryParam("_sortField") String sortField,
                                   @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(userDao.getWidgetsUsage()), sortField, sortOrder));
    }

    @GET
    @Path("/projectsPerUser")
    public Response getProjectsPerUser(@QueryParam("_sortField") String sortField,
                                           @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(userDao.getProjectsPerUser()), sortField, sortOrder, true));
    }

    @GET
    @Path("/boards")
    public Response getBoards(@QueryParam("_sortField") String sortField,
                                    @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(userDao.getBoardsUsage()), sortField, sortOrder));
    }

    @GET
    @Path("/facebookLogins")
    public Response getFacebookLogins(@QueryParam("_sortField") String sortField,
                              @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(userDao.getFacebookLogin()), sortField, sortOrder));
    }

    @GET
    @Path("/filledSpace")
    public Response getFilledSpace(@QueryParam("_sortField") String sortField,
                                  @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(userDao.getFilledSpace()), sortField, sortOrder, true));
    }

    @GET
    @Path("/userProfileSize")
    public Response getUserProfileSize(@QueryParam("_sortField") String sortField,
                                   @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(fileManager.getUserProfilesSize()), sortField, sortOrder, true));
    }


    @GET
    @Path("/webHookHosts")
    public Response getWebHookHosts(@QueryParam("_sortField") String sortField,
                                       @QueryParam("_sortDir") String sortOrder) {
        return ok(sort(convertMapToPair(userDao.getWebHookHosts()), sortField, sortOrder, true));
    }

    @GET
    @Path("/ips")
    public Response getIps(@QueryParam("_filters") String filterParam,
                           @QueryParam("_page") int page,
                           @QueryParam("_perPage") int size,
                           @QueryParam("_sortField") String sortField,
                           @QueryParam("_sortDir") String sortOrder) {

        if (filterParam != null) {
            IpFilter filter = JsonParser.readAny(filterParam, IpFilter.class);
            filterParam = filter == null ? null : filter.ip;
        }

        return ok(sort(searchByIP(filterParam), sortField, sortOrder));
    }

    private static class IpFilter {
        public String ip;
    }

    public List<IpNameResponse> searchByIP(String ip) {
        List<IpNameResponse> res = new ArrayList<>();

        for (User user : userDao.users.values()) {
            if (user.lastLoggedIP != null) {
                final String name = user.name + "-" + user.appName;
                if (ip == null) {
                    res.add(new IpNameResponse(name, user.lastLoggedIP));
                    for (DashBoard dashBoard : user.profile.dashBoards) {
                        for (Device device : dashBoard.devices) {
                            if (device.lastLoggedIP != null) {
                                res.add(new IpNameResponse(name, device.lastLoggedIP));
                            }
                        }
                    }
                } else {
                    if (user.lastLoggedIP.contains(ip) || deviceContains(user, ip)) {
                        res.add(new IpNameResponse(name, user.lastLoggedIP));
                    }
                }
            }
        }

        return res;
    }

    private boolean deviceContains(User user, String ip) {
        for (DashBoard dash : user.profile.dashBoards) {
            for (Device device : dash.devices) {
                if (device.lastLoggedIP != null && device.lastLoggedIP.contains(ip)) {
                    return true;
                }
            }
        }
        return false;
    }


}
