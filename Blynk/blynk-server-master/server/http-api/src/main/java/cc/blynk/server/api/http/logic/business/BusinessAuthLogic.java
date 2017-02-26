package cc.blynk.server.api.http.logic.business;

import cc.blynk.core.http.MediaType;
import cc.blynk.core.http.Response;
import cc.blynk.core.http.annotation.Consumes;
import cc.blynk.core.http.annotation.FormParam;
import cc.blynk.core.http.annotation.POST;
import cc.blynk.core.http.annotation.Path;
import cc.blynk.server.core.dao.FileManager;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.auth.User;
import io.netty.handler.codec.http.cookie.Cookie;
import io.netty.handler.codec.http.cookie.DefaultCookie;
import io.netty.handler.codec.http.cookie.ServerCookieEncoder;

import static cc.blynk.core.http.Response.redirect;
import static io.netty.handler.codec.http.HttpHeaderNames.SET_COOKIE;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
@Path("")
public class BusinessAuthLogic {

    private final UserDao userDao;
    private final SessionHolder sessionHolder;
    private final SessionDao sessionDao;
    private final FileManager fileManager;

    public BusinessAuthLogic(UserDao userDao, SessionDao sessionDao, FileManager fileManager, SessionHolder sessionHolder) {
        this.userDao = userDao;
        this.fileManager = fileManager;
        this.sessionDao = sessionDao;
        this.sessionHolder = sessionHolder;
    }

    @POST
    @Consumes(value = MediaType.APPLICATION_FORM_URLENCODED)
    @Path("/login")
    public Response login(@FormParam("email") String email,
                          @FormParam("password") String password) {

        if (email == null || password == null) {
            return redirect("/business");
        }

        User user = userDao.getByName(email, AppName.BLYNK);

        if (user == null) {
            return redirect("/business");
        }

        if (!password.equals(user.pass)) {
            return redirect("/business");
        }

        Response response = redirect("/business");

        Cookie cookie = makeDefaultSessionCookie(sessionHolder.generateNewSession(user), 86400);
        response.headers().add(SET_COOKIE, ServerCookieEncoder.STRICT.encode(cookie));

        return response;
    }

    @POST
    @Path("/logout")
    public Response logout() {
        Response response = redirect("/business");
        Cookie cookie = makeDefaultSessionCookie("", 0);
        response.headers().add(SET_COOKIE, ServerCookieEncoder.STRICT.encode(cookie));
        return response;
    }

    private static Cookie makeDefaultSessionCookie(String sessionId, int maxAge) {
        DefaultCookie cookie = new DefaultCookie(Cookies.SESSION_COOKIE, sessionId);
        cookie.setMaxAge(maxAge);
        return cookie;
    }

}
