package cc.blynk.server.application.handlers.main.auth;

import cc.blynk.server.Holder;
import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.protocol.enums.Response;
import cc.blynk.server.core.protocol.model.messages.appllication.GetServerMessage;
import cc.blynk.server.redis.RedisClient;
import cc.blynk.utils.StringUtils;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import org.apache.commons.validator.routines.EmailValidator;

import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND_BODY;
import static cc.blynk.utils.BlynkByteBufUtil.makeASCIIStringMessage;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;


/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.10.16.
 */
@ChannelHandler.Sharable
public class GetServerHandler extends SimpleChannelInboundHandler<GetServerMessage> {

    private final String[] loadBalancingIps;
    private final BlockingIOProcessor blockingIOProcessor;
    private final RedisClient redisClient;
    private final UserDao userDao;
    private final String currentIp;

    public GetServerHandler(Holder holder, String[] ips) {
        super();
        this.loadBalancingIps = ips;
        this.blockingIOProcessor = holder.blockingIOProcessor;
        this.redisClient = holder.redisClient;
        this.userDao = holder.userDao;
        this.currentIp = holder.currentIp;
    }

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, GetServerMessage msg) throws Exception {
        final String[] parts = StringUtils.split2(msg.body);

        if (parts.length < 2) {
            ctx.writeAndFlush(makeResponse(msg.id, Response.ILLEGAL_COMMAND), ctx.voidPromise());
            return;
        }

        final String username = parts[0];
        final String appName = parts[1];

        if (username == null || username.isEmpty() || appName == null || appName.isEmpty()) {
            ctx.writeAndFlush(makeResponse(msg.id, Response.ILLEGAL_COMMAND), ctx.voidPromise());
            return;
        }

        if (username.length() > 255 || appName.length() > 100 || !EmailValidator.getInstance().isValid(username)) {
            ctx.writeAndFlush(makeResponse(msg.id, ILLEGAL_COMMAND_BODY), ctx.voidPromise());
            return;
        }

        if (userDao.contains(username, appName)) {
            //user exists on current server. so returning ip of current server
            ctx.writeAndFlush(makeASCIIStringMessage(msg.command, msg.id, currentIp), ctx.voidPromise());
        } else {
            //user is on other server
            blockingIOProcessor.execute(() -> {
                String userServer = redisClient.getServerByUser(username);
                if (userServer == null) {
                    //user not registered yet anywhere
                    redisClient.assignServerToUser(username, currentIp);
                    userServer = currentIp;
                }

                ctx.writeAndFlush(makeASCIIStringMessage(msg.command, msg.id, userServer), ctx.voidPromise());
            });
        }
    }

}
