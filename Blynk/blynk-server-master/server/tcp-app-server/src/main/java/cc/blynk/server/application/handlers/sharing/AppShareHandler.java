package cc.blynk.server.application.handlers.sharing;

import cc.blynk.server.Holder;
import cc.blynk.server.application.handlers.main.logic.AddPushLogic;
import cc.blynk.server.application.handlers.main.logic.AppSyncLogic;
import cc.blynk.server.application.handlers.main.logic.LoadProfileGzippedLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.device.GetDevicesLogic;
import cc.blynk.server.application.handlers.main.logic.reporting.GetGraphDataLogic;
import cc.blynk.server.application.handlers.sharing.auth.AppShareStateHolder;
import cc.blynk.server.application.handlers.sharing.logic.HardwareAppShareLogic;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import cc.blynk.server.handlers.common.PingLogic;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.ADD_PUSH_TOKEN;
import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.server.core.protocol.enums.Command.GET_DEVICES;
import static cc.blynk.server.core.protocol.enums.Command.GET_GRAPH_DATA;
import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.server.core.protocol.enums.Command.LOAD_PROFILE_GZIPPED;
import static cc.blynk.server.core.protocol.enums.Command.PING;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class AppShareHandler extends BaseSimpleChannelInboundHandler<StringMessage> {

    public final AppShareStateHolder state;
    private final HardwareAppShareLogic hardwareApp;
    private final GetGraphDataLogic graphData;
    private final GlobalStats stats;

    public AppShareHandler(Holder holder, AppShareStateHolder state) {
        super(holder.limits, state);
        this.hardwareApp = new HardwareAppShareLogic(holder.sessionDao);
        this.graphData = new GetGraphDataLogic(holder.reportingDao, holder.blockingIOProcessor);
        this.state = state;
        this.stats = holder.stats;
    }

    @Override
    public void messageReceived(ChannelHandlerContext ctx, StringMessage msg) {
        this.stats.incrementAppStat();
        switch (msg.command) {
            case HARDWARE:
                hardwareApp.messageReceived(ctx, state, msg);
                break;
            case LOAD_PROFILE_GZIPPED :
                LoadProfileGzippedLogic.messageReceived(ctx, state.user, msg);
                break;
            //todo add test for that case
            case ADD_PUSH_TOKEN :
                AddPushLogic.messageReceived(ctx, state, msg);
                break;
            case GET_GRAPH_DATA :
                graphData.messageReceived(ctx, state.user, msg);
                break;
            case GET_DEVICES :
                GetDevicesLogic.messageReceived(ctx, state.user, msg);
                break;
            case PING :
                PingLogic.messageReceived(ctx, msg.id);
                break;
            case APP_SYNC :
                AppSyncLogic.messageReceived(ctx, state, msg);
                break;
        }
    }

}
