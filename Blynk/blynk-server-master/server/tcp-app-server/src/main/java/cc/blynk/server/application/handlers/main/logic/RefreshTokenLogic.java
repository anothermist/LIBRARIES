package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.Holder;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import cc.blynk.utils.StringUtils;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.REFRESH_TOKEN;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class RefreshTokenLogic {

    private final TokenManager tokenManager;

    public RefreshTokenLogic(Holder holder) {
        this.tokenManager = holder.tokenManager;
    }

    public void messageReceived(ChannelHandlerContext ctx, User user, StringMessage message) {
        String[] split = StringUtils.split2(message.body);

        int dashId = ParseUtil.parseInt(split[0]);
        int deviceId = 0;

        //new value for multi devices
        if (split.length == 2) {
            deviceId = ParseUtil.parseInt(split[1]);
        }

        user.profile.validateDashId(dashId);

        String token = tokenManager.refreshToken(user, dashId, deviceId);

        ctx.writeAndFlush(makeUTF8StringMessage(REFRESH_TOKEN, message.id, token), ctx.voidPromise());
    }
}
