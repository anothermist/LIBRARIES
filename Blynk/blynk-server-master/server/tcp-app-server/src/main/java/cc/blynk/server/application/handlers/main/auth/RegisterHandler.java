package cc.blynk.server.application.handlers.main.auth;

import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.protocol.handlers.DefaultExceptionHandler;
import cc.blynk.server.core.protocol.model.messages.appllication.RegisterMessage;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import org.apache.commons.validator.routines.EmailValidator;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND;
import static cc.blynk.server.core.protocol.enums.Response.NOT_ALLOWED;
import static cc.blynk.server.core.protocol.enums.Response.USER_ALREADY_REGISTERED;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;

/**
 * Process register message.
 * Divides input string by nil char on 3 parts:
 * "username" "password" "appName".
 * Checks if user not registered yet. If not - registering.
 *
 * For instance, incoming register message may be : "user@mail.ua my_password"
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
@ChannelHandler.Sharable
public class RegisterHandler extends SimpleChannelInboundHandler<RegisterMessage> implements DefaultExceptionHandler {

    private final UserDao userDao;
    private final Set<String> allowedUsers;

    public RegisterHandler(UserDao userDao, String[] allowedUsersArray) {
        this.userDao = userDao;
        if (allowedUsersArray != null && allowedUsersArray.length > 0 &&
                allowedUsersArray[0] != null && !allowedUsersArray[0].isEmpty()) {
            allowedUsers = new HashSet<>(Arrays.asList(allowedUsersArray));
            log.debug("Created allowed user list : {}", allowedUsersArray);
        } else {
            allowedUsers = null;
        }
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, RegisterMessage message) throws Exception {
        //warn: split may be optimized
        String[] messageParts = message.body.split("\0");

        //expecting message with 3 parts, described above in comment.
        if (messageParts.length < 2) {
            log.error("Register Handler. Wrong income message format. {}", message);
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND), ctx.voidPromise());
            return;
        }

        String userName = messageParts[0].trim().toLowerCase();
        String pass = messageParts[1];
        String appName = messageParts.length == 3 ? messageParts[2] : AppName.BLYNK;
        log.info("Trying register user : {}, app : {}", userName, appName);

        if (userName.length() > 255 || !EmailValidator.getInstance().isValid(userName)) {
            log.error("Register Handler. Wrong email: {}", userName);
            ctx.writeAndFlush(makeResponse(message.id, ILLEGAL_COMMAND), ctx.voidPromise());
            return;
        }

        if (userDao.isUserExists(userName, appName)) {
            log.warn("User with name {} already exists.", userName);
            ctx.writeAndFlush(makeResponse(message.id, USER_ALREADY_REGISTERED), ctx.voidPromise());
            return;
        }

        if (allowedUsers != null && !allowedUsers.contains(userName)) {
            log.warn("User with name {} not allowed to register.", userName);
            ctx.writeAndFlush(makeResponse(message.id, NOT_ALLOWED), ctx.voidPromise());
            return;
        }

        userDao.add(userName, pass, appName);

        log.info("Registered {}.", userName);

        ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
    }

}
