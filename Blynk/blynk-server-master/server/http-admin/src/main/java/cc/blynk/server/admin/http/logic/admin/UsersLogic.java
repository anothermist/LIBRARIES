package cc.blynk.server.admin.http.logic.admin;

import cc.blynk.core.http.MediaType;
import cc.blynk.core.http.Response;
import cc.blynk.core.http.annotation.Consumes;
import cc.blynk.core.http.annotation.DELETE;
import cc.blynk.core.http.annotation.GET;
import cc.blynk.core.http.annotation.PUT;
import cc.blynk.core.http.annotation.Path;
import cc.blynk.core.http.annotation.PathParam;
import cc.blynk.core.http.annotation.QueryParam;
import cc.blynk.core.http.model.Filter;
import cc.blynk.server.Holder;
import cc.blynk.server.core.dao.FileManager;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.dao.TokenValue;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.utils.HttpLogicUtil;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.SHA256Util;

import java.util.List;

import static cc.blynk.core.http.Response.appendTotalCountHeader;
import static cc.blynk.core.http.Response.badRequest;
import static cc.blynk.core.http.Response.ok;
import static io.netty.handler.codec.http.HttpResponseStatus.NOT_FOUND;
import static io.netty.handler.codec.http.HttpVersion.HTTP_1_1;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 03.12.15.
 */
@Path("/users")
public class UsersLogic extends HttpLogicUtil {

    private final UserDao userDao;
    private final SessionDao sessionDao;
    private final FileManager fileManager;
    private final TokenManager tokenManager;

    public UsersLogic(Holder holder) {
        this.userDao = holder.userDao;
        this.fileManager = holder.fileManager;
        this.sessionDao = holder.sessionDao;
        this.tokenManager = holder.tokenManager;
    }

    //for tests only
    protected UsersLogic(UserDao userDao, SessionDao sessionDao, FileManager fileManager, TokenManager tokenManager) {
        this.userDao = userDao;
        this.fileManager = fileManager;
        this.sessionDao = sessionDao;
        this.tokenManager = tokenManager;
    }

    @GET
    @Path("")
    public Response getUsers(@QueryParam("_filters") String filterParam,
                                 @QueryParam("_page") int page,
                                 @QueryParam("_perPage") int size,
                                 @QueryParam("_sortField") String sortField,
                                 @QueryParam("_sortDir") String sortOrder) {
        if (filterParam != null) {
            Filter filter = JsonParser.readAny(filterParam, Filter.class);
            filterParam = filter == null ? null : filter.name;
        }

        List<User> users = userDao.searchByUsername(filterParam, AppName.ALL);
        return appendTotalCountHeader(
                ok(sort(users, sortField, sortOrder), page, size), users.size()
        );
    }

    @GET
    @Path("/{id}")
    public Response getUserByName(@PathParam("id") String id) {
        String[] parts =  slitByLast(id);
        String name = parts[0];
        String appName = parts[1];
        return ok(userDao.getByName(name, appName));
    }

    @GET
    @Path("/names/getAll")
    public Response getAllUserNames() {
        return ok(userDao.users.keySet());
    }

    @GET
    @Path("/token/assign")
    public Response assignToken(@QueryParam("old") String oldToken, @QueryParam("new") String newToken) {
        TokenValue tokenValue = tokenManager.getUserByToken(oldToken);

        if (tokenValue == null) {
            return badRequest("This token not exists.");
        }

        tokenManager.assignToken(tokenValue.user, tokenValue.dashId, tokenValue.deviceId, newToken);
        return ok();
    }

    @GET
    @Path("/token/force")
    public Response forceToken(@QueryParam("username") String username,
                                    @QueryParam("app") String app,
                                    @QueryParam("dashId") int dashId,
                                    @QueryParam("deviceId") int deviceId,
                                    @QueryParam("new") String newToken) {

        User user = userDao.getUsers().get(new UserKey(username, app));

        if (user == null) {
            return badRequest("No user with such name.");
        }

        tokenManager.assignToken(user, dashId, deviceId, newToken);
        return ok();
    }

    @PUT
    @Consumes(value = MediaType.APPLICATION_JSON)
    @Path("/{id}")
    public Response updateUser(@PathParam("id") String id,
                                   User updatedUser) {

        log.debug("Updating user {}", id);

        String[] parts =  slitByLast(id);
        String name = parts[0];
        String appName = parts[1];

        User oldUser = userDao.getByName(name, appName);

        //if pass was changed, call hash function.
        if (!updatedUser.pass.equals(oldUser.pass)) {
            log.debug("Updating pass for {}.", updatedUser.name);
            updatedUser.pass = SHA256Util.makeHash(updatedUser.pass, updatedUser.name);
        }

        //user name was changed
        if (!updatedUser.name.equals(oldUser.name)) {
            deleteUserByName(id);
            for (DashBoard dashBoard : oldUser.profile.dashBoards) {
                for (Device device : dashBoard.devices) {
                    tokenManager.assignToken(updatedUser, dashBoard.id, device.id, device.token);
                }
            }
        }

        userDao.add(updatedUser);
        updatedUser.lastModifiedTs = System.currentTimeMillis();
        log.debug("Adding new user {}", updatedUser.name);


        return ok(updatedUser);
    }

    @DELETE
    @Path("/{id}")
    public Response deleteUserByName(@PathParam("id") String id) {
        String[] parts =  slitByLast(id);
        String name = parts[0];
        String appName = parts[1];

        UserKey userKey = new UserKey(name, appName);
        User user = userDao.delete(userKey);
        if (user == null) {
            return new Response(HTTP_1_1, NOT_FOUND);
        }

        if (!fileManager.delete(name, appName)) {
            return new Response(HTTP_1_1, NOT_FOUND);
        }

        Session session = sessionDao.userSession.remove(userKey);
        if (session != null) {
            session.closeAll();
        }

        log.info("User {} successfully removed.", name);

        return ok();
    }

    private String[] slitByLast(String id) {
        int i = id.lastIndexOf("-");
        return new String[] {
                id.substring(0, i),
                id.substring(i + 1)
        };
    }

}
