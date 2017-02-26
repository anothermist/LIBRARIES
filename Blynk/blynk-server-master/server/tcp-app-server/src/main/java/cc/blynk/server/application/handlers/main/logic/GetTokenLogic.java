package cc.blynk.server.application.handlers.main.logic;

import cc.blynk.server.Holder;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.device.Device;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.ParseUtil;
import cc.blynk.utils.TokenGeneratorUtil;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.GET_TOKEN;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class GetTokenLogic {

    private final TokenManager tokenManager;

    public GetTokenLogic(Holder holder) {
        this.tokenManager = holder.tokenManager;
    }

    public void messageReceived(ChannelHandlerContext ctx, User user, StringMessage message) {
        String dashBoardIdString = message.body;

        int dashId = ParseUtil.parseInt(dashBoardIdString);
        int deviceId = 0;

        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);

        Device device = dash.getDeviceById(deviceId);
        String token = device == null ? null : device.token;

        //if token not exists. generate new one
        if (token == null) {
            //todo back compatibility code. remove in future
            dash.devices = new Device[] {
                new Device(deviceId, dash.boardType, dash.boardType)
            };
            //

            token = TokenGeneratorUtil.generateNewToken();
            tokenManager.assignToken(user, dashId, deviceId, token);
        }

        ctx.writeAndFlush(makeUTF8StringMessage(GET_TOKEN, message.id, token), ctx.voidPromise());
    }
}
