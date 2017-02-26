package cc.blynk.server.core.protocol.model.messages;

import cc.blynk.server.core.protocol.exceptions.UnsupportedCommandException;
import cc.blynk.server.core.protocol.model.messages.appllication.ActivateDashboardMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.AddEnergy;
import cc.blynk.server.core.protocol.model.messages.appllication.AddPushToken;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateDashMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateDevice;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateTag;
import cc.blynk.server.core.protocol.model.messages.appllication.CreateWidget;
import cc.blynk.server.core.protocol.model.messages.appllication.DeActivateDashboardMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.DeleteDashMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.DeleteDevice;
import cc.blynk.server.core.protocol.model.messages.appllication.DeleteTag;
import cc.blynk.server.core.protocol.model.messages.appllication.DeleteWidget;
import cc.blynk.server.core.protocol.model.messages.appllication.ExportDataMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.GetDevices;
import cc.blynk.server.core.protocol.model.messages.appllication.GetEnergy;
import cc.blynk.server.core.protocol.model.messages.appllication.GetGraphDataStringMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.GetServerMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.GetTags;
import cc.blynk.server.core.protocol.model.messages.appllication.GetTokenMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.LoadProfileGzippedStringMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.LoginMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.RedeemMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.RefreshTokenMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.RegisterMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.SetWidgetPropertyMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.UpdateDashMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.UpdateDevice;
import cc.blynk.server.core.protocol.model.messages.appllication.UpdateTag;
import cc.blynk.server.core.protocol.model.messages.appllication.UpdateWidget;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.AppSyncMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.GetShareTokenMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.GetSharedDashMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.RefreshShareTokenMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.ShareLoginMessage;
import cc.blynk.server.core.protocol.model.messages.appllication.sharing.SharingMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareConnectedMessage;
import cc.blynk.server.core.protocol.model.messages.common.HardwareMessage;
import cc.blynk.server.core.protocol.model.messages.common.PingMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.AppConnectedMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.BlynkInternalMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.BridgeMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.ConnectRedirectMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.HardwareSyncMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.MailMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.PushMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.SMSMessage;
import cc.blynk.server.core.protocol.model.messages.hardware.TwitMessage;

import static cc.blynk.server.core.protocol.enums.Command.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 */
public class MessageFactory {

    public static MessageBase produce(int messageId, short command, String body) {
        switch (command) {
            case REGISTER :
                return new RegisterMessage(messageId, body);
            case LOGIN :
                return new LoginMessage(messageId, body);
            case LOAD_PROFILE_GZIPPED :
                return new LoadProfileGzippedStringMessage(messageId, body);
            case APP_SYNC:
                return new AppSyncMessage(messageId, body);
            case SHARING :
                return new SharingMessage(messageId, body);
            case GET_TOKEN :
                return new GetTokenMessage(messageId, body);
            case PING :
                return new PingMessage(messageId);
            case ACTIVATE_DASHBOARD :
                return new ActivateDashboardMessage(messageId, body);
            case DEACTIVATE_DASHBOARD :
                return new DeActivateDashboardMessage(messageId, body);
            case REFRESH_TOKEN :
                return new RefreshTokenMessage(messageId, body);
            case GET_GRAPH_DATA :
                return new GetGraphDataStringMessage(messageId, body);
            case EXPORT_GRAPH_DATA :
                return new ExportDataMessage(messageId, body);
            case SET_WIDGET_PROPERTY :
                return new SetWidgetPropertyMessage(messageId, body);

            case TWEET :
                return new TwitMessage(messageId, body);
            case EMAIL :
                return new MailMessage(messageId, body);
            case PUSH_NOTIFICATION :
                return new PushMessage(messageId, body);
            case SMS :
                return new SMSMessage(messageId, body);

            case BRIDGE :
                return new BridgeMessage(messageId, body);
            case SHARE_LOGIN :
                return new ShareLoginMessage(messageId, body);
            case GET_SHARE_TOKEN :
                return new GetShareTokenMessage(messageId, body);
            case REFRESH_SHARE_TOKEN :
                return new RefreshShareTokenMessage(messageId, body);
            case GET_SHARED_DASH :
                return new GetSharedDashMessage(messageId, body);
            case HARDWARE :
                return new HardwareMessage(messageId, body);
            case HARDWARE_CONNECTED :
                return new HardwareConnectedMessage(messageId, body);
            case REDEEM :
                return new RedeemMessage(messageId, body);

            case CREATE_DASH :
                return new CreateDashMessage(messageId, body);
            case UPDATE_DASH:
                return new UpdateDashMessage(messageId, body);
            case DELETE_DASH :
                return new DeleteDashMessage(messageId, body);

            case ADD_PUSH_TOKEN :
                return new AddPushToken(messageId, body);
            case HARDWARE_SYNC :
                return new HardwareSyncMessage(messageId, body);
            case BLYNK_INTERNAL:
                return new BlynkInternalMessage(messageId, body);

            case CREATE_WIDGET :
                return new CreateWidget(messageId, body);
            case UPDATE_WIDGET :
                return new UpdateWidget(messageId, body);
            case DELETE_WIDGET :
                return new DeleteWidget(messageId, body);

            case CREATE_DEVICE :
                return new CreateDevice(messageId, body);
            case UPDATE_DEVICE :
                return new UpdateDevice(messageId, body);
            case DELETE_DEVICE :
                return new DeleteDevice(messageId, body);
            case GET_DEVICES :
                return new GetDevices(messageId, body);

            case CREATE_TAG :
                return new CreateTag(messageId, body);
            case UPDATE_TAG :
                return new UpdateTag(messageId, body);
            case DELETE_TAG :
                return new DeleteTag(messageId, body);
            case GET_TAGS :
                return new GetTags(messageId, body);

            case GET_ENERGY :
                return new GetEnergy(messageId, body);
            case ADD_ENERGY :
                return new AddEnergy(messageId, body);

            case GET_SERVER :
                return new GetServerMessage(messageId, body);
            case CONNECT_REDIRECT :
                return new ConnectRedirectMessage(messageId, body);

            case APP_CONNECTED :
                return new AppConnectedMessage(messageId);

            default: throw new UnsupportedCommandException("Command not supported. Code : " + command, messageId);
        }
    }

}
