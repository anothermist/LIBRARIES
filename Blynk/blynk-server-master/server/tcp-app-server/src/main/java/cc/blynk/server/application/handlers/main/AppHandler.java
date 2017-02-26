package cc.blynk.server.application.handlers.main;

import cc.blynk.server.Holder;
import cc.blynk.server.application.handlers.main.auth.AppStateHolder;
import cc.blynk.server.application.handlers.main.logic.ActivateDashboardLogic;
import cc.blynk.server.application.handlers.main.logic.AddEnergyLogic;
import cc.blynk.server.application.handlers.main.logic.AddPushLogic;
import cc.blynk.server.application.handlers.main.logic.AppMailLogic;
import cc.blynk.server.application.handlers.main.logic.AppSyncLogic;
import cc.blynk.server.application.handlers.main.logic.DeActivateDashboardLogic;
import cc.blynk.server.application.handlers.main.logic.GetEnergyLogic;
import cc.blynk.server.application.handlers.main.logic.GetTokenLogic;
import cc.blynk.server.application.handlers.main.logic.HardwareAppLogic;
import cc.blynk.server.application.handlers.main.logic.LoadProfileGzippedLogic;
import cc.blynk.server.application.handlers.main.logic.RedeemLogic;
import cc.blynk.server.application.handlers.main.logic.RefreshTokenLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.CreateDashLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.DeleteDashLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.UpdateDashLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.device.CreateDeviceLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.device.DeleteDeviceLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.device.GetDevicesLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.device.UpdateDeviceLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.tags.CreateTagLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.tags.DeleteTagLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.tags.GetTagsLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.tags.UpdateTagLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.widget.CreateWidgetLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.widget.DeleteWidgetLogic;
import cc.blynk.server.application.handlers.main.logic.dashboard.widget.UpdateWidgetLogic;
import cc.blynk.server.application.handlers.main.logic.reporting.ExportGraphDataLogic;
import cc.blynk.server.application.handlers.main.logic.reporting.GetGraphDataLogic;
import cc.blynk.server.application.handlers.main.logic.sharing.GetShareTokenLogic;
import cc.blynk.server.application.handlers.main.logic.sharing.GetSharedDashLogic;
import cc.blynk.server.application.handlers.main.logic.sharing.RefreshShareTokenLogic;
import cc.blynk.server.application.handlers.main.logic.sharing.ShareLogic;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.handlers.BaseSimpleChannelInboundHandler;
import cc.blynk.server.handlers.common.PingLogic;
import io.netty.channel.ChannelHandlerContext;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class AppHandler extends BaseSimpleChannelInboundHandler<StringMessage> {

    public final AppStateHolder state;
    private final GetTokenLogic token;
    private final HardwareAppLogic hardwareApp;
    private final RefreshTokenLogic refreshToken;
    private final GetGraphDataLogic graphData;
    private final ExportGraphDataLogic exportGraphData;
    private final AppMailLogic appMailLogic;
    private final GetShareTokenLogic getShareTokenLogic;
    private final RefreshShareTokenLogic refreshShareTokenLogic;
    private final GetSharedDashLogic getSharedDashLogic;
    private final CreateDashLogic createDashLogic;
    private final UpdateDashLogic updateDashLogic;
    private final ActivateDashboardLogic activateDashboardLogic;
    private final DeActivateDashboardLogic deActivateDashboardLogic;
    private final CreateWidgetLogic createWidgetLogic;
    private final UpdateWidgetLogic updateWidgetLogic;
    private final DeleteWidgetLogic deleteWidgetLogic;
    private final DeleteDashLogic deleteDashLogic;
    private final ShareLogic shareLogic;
    private final RedeemLogic redeemLogic;
    private final AddEnergyLogic addEnergyLogic;
    private final CreateDeviceLogic createDeviceLogic;
    private final UpdateDeviceLogic updateDeviceLogic;
    private final DeleteDeviceLogic deleteDeviceLogic;
    private final GlobalStats stats;

    public AppHandler(Holder holder, AppStateHolder state) {
        super(holder.limits, state);
        this.token = new GetTokenLogic(holder);
        this.hardwareApp = new HardwareAppLogic(holder, state.user.name);
        this.refreshToken = new RefreshTokenLogic(holder);
        this.graphData = new GetGraphDataLogic(holder.reportingDao, holder.blockingIOProcessor);
        this.exportGraphData = new ExportGraphDataLogic(holder.reportingDao, holder.blockingIOProcessor, holder.mailWrapper,
                holder.currentIp,
                holder.props.getIntProperty("http.port"));
        this.appMailLogic = new AppMailLogic(holder);
        this.getShareTokenLogic = new GetShareTokenLogic(holder.tokenManager);
        this.refreshShareTokenLogic = new RefreshShareTokenLogic(holder.tokenManager, holder.sessionDao);
        this.getSharedDashLogic = new GetSharedDashLogic(holder.tokenManager);

        this.createDashLogic = new CreateDashLogic(holder.timerWorker, holder.tokenManager, holder.limits.DASHBOARDS_LIMIT, holder.limits.PROFILE_SIZE_LIMIT_BYTES);
        this.updateDashLogic = new UpdateDashLogic(holder.timerWorker, holder.limits.PROFILE_SIZE_LIMIT_BYTES);

        this.activateDashboardLogic = new ActivateDashboardLogic(holder.sessionDao);
        this.deActivateDashboardLogic = new DeActivateDashboardLogic(holder.sessionDao);

        this.createWidgetLogic = new CreateWidgetLogic(holder.limits.WIDGET_SIZE_LIMIT_BYTES, holder.timerWorker);
        this.updateWidgetLogic = new UpdateWidgetLogic(holder.limits.WIDGET_SIZE_LIMIT_BYTES, holder.timerWorker);
        this.deleteWidgetLogic = new DeleteWidgetLogic(holder.timerWorker);
        this.deleteDashLogic = new DeleteDashLogic(holder);

        this.createDeviceLogic = new CreateDeviceLogic(holder);
        this.updateDeviceLogic = new UpdateDeviceLogic();
        this.deleteDeviceLogic = new DeleteDeviceLogic(holder.tokenManager, holder.sessionDao);

        this.shareLogic = new ShareLogic(holder.sessionDao);
        this.redeemLogic = new RedeemLogic(holder.dbManager, holder.blockingIOProcessor);
        this.addEnergyLogic = new AddEnergyLogic(holder.dbManager, holder.blockingIOProcessor);

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
            case ACTIVATE_DASHBOARD :
                activateDashboardLogic.messageReceived(ctx, state, msg);
                break;
            case DEACTIVATE_DASHBOARD :
                deActivateDashboardLogic.messageReceived(ctx, state, msg);
                break;
            case LOAD_PROFILE_GZIPPED :
                LoadProfileGzippedLogic.messageReceived(ctx, state.user, msg);
                break;
            case SHARING :
                shareLogic.messageReceived(ctx, state, msg);
                break;
            case GET_TOKEN :
                token.messageReceived(ctx, state.user, msg);
                break;
            case ADD_PUSH_TOKEN :
                AddPushLogic.messageReceived(ctx, state, msg);
                break;
            case REFRESH_TOKEN :
                refreshToken.messageReceived(ctx, state.user, msg);
                break;
            case GET_GRAPH_DATA :
                graphData.messageReceived(ctx, state.user, msg);
                break;
            case EXPORT_GRAPH_DATA :
                exportGraphData.messageReceived(ctx, state.user, msg);
                break;
            case PING :
                PingLogic.messageReceived(ctx, msg.id);
                break;
            case GET_SHARE_TOKEN :
                getShareTokenLogic.messageReceived(ctx, state.user, msg);
                break;
            case REFRESH_SHARE_TOKEN :
                refreshShareTokenLogic.messageReceived(ctx, state, msg);
                break;
            case GET_SHARED_DASH :
                getSharedDashLogic.messageReceived(ctx, msg);
                break;
            case EMAIL :
                appMailLogic.messageReceived(ctx, state.user, msg);
                break;
            case CREATE_DASH :
                createDashLogic.messageReceived(ctx, state, msg);
                break;
            case UPDATE_DASH:
                updateDashLogic.messageReceived(ctx, state, msg);
                break;
            case DELETE_DASH :
                deleteDashLogic.messageReceived(ctx, state, msg);
                break;
            case CREATE_WIDGET :
                createWidgetLogic.messageReceived(ctx, state, msg);
                break;
            case UPDATE_WIDGET :
                updateWidgetLogic.messageReceived(ctx, state, msg);
                break;
            case DELETE_WIDGET :
                deleteWidgetLogic.messageReceived(ctx, state, msg);
                break;
            case REDEEM :
                redeemLogic.messageReceived(ctx, state.user, msg);
                break;
            case GET_ENERGY :
                GetEnergyLogic.messageReceived(ctx, state.user, msg);
                break;
            case ADD_ENERGY :
                addEnergyLogic.messageReceived(ctx, state.user, msg);
                break;
            case CREATE_DEVICE :
                createDeviceLogic.messageReceived(ctx, state.user, msg);
                break;
            case UPDATE_DEVICE :
                updateDeviceLogic.messageReceived(ctx, state.user, msg);
                break;
            case DELETE_DEVICE :
                deleteDeviceLogic.messageReceived(ctx, state, msg);
                break;
            case GET_DEVICES :
                GetDevicesLogic.messageReceived(ctx, state.user, msg);
                break;
            case CREATE_TAG :
                CreateTagLogic.messageReceived(ctx, state.user, msg);
                break;
            case UPDATE_TAG :
                UpdateTagLogic.messageReceived(ctx, state.user, msg);
                break;
            case DELETE_TAG :
                DeleteTagLogic.messageReceived(ctx, state.user, msg);
                break;
            case GET_TAGS :
                GetTagsLogic.messageReceived(ctx, state.user, msg);
                break;
            case APP_SYNC :
                AppSyncLogic.messageReceived(ctx, state, msg);
        }
    }

}
