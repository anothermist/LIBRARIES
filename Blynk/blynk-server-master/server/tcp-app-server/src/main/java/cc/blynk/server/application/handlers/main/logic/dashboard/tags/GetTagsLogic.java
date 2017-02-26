package cc.blynk.server.application.handlers.main.logic.dashboard.tags;

import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.utils.JsonParser;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import static cc.blynk.server.core.protocol.enums.Command.GET_TAGS;
import static cc.blynk.utils.BlynkByteBufUtil.makeUTF8StringMessage;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 01.02.16.
 */
public class GetTagsLogic {

    private static final Logger log = LogManager.getLogger(GetTagsLogic.class);

    public static void messageReceived(ChannelHandlerContext ctx, User user, StringMessage message) {
        int dashId = ParseUtil.parseInt(message.body) ;

        DashBoard dash = user.profile.getDashByIdOrThrow(dashId);

        String response = JsonParser.toJson(dash.tags);
        if (response == null) {
            response = "[]";
        }

        if (ctx.channel().isWritable()) {
            ctx.writeAndFlush(makeUTF8StringMessage(GET_TAGS, message.id, response), ctx.voidPromise());
        }
    }

}
