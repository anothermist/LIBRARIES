package cc.blynk.server.hardware.handlers.hardware.mqtt.logic;

import cc.blynk.server.core.dao.ReportingDao;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.auth.Session;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.mqtt.MqttPublishMessage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.nio.charset.StandardCharsets;

import static cc.blynk.server.core.protocol.enums.Command.HARDWARE;
import static cc.blynk.utils.StringUtils.split3;

/**
 * Handler responsible for forwarding messages from hardware to applications.
 * Also handler stores all incoming hardware commands to disk in order to export and
 * analyze data.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class MqttHardwareLogic {

    private static final Logger log = LogManager.getLogger(MqttHardwareLogic.class);

    private final ReportingDao reportingDao;
    private final SessionDao sessionDao;

    public MqttHardwareLogic(SessionDao sessionDao, ReportingDao reportingDao) {
        this.sessionDao = sessionDao;
        this.reportingDao = reportingDao;
    }

    private static boolean isWriteOperation(String body) {
        return body.charAt(1) == 'w';
    }

    public void messageReceived(ChannelHandlerContext ctx, HardwareStateHolder state, MqttPublishMessage msg) {
        Session session = sessionDao.userSession.get(state.userKey);

        String body = msg.payload().readSlice(msg.payload().capacity()).toString(StandardCharsets.UTF_8);

        //just temp solution to simplify demo
        body = body.replace(" ", "\0").replace(" ", "\0");

        //minimum command - "aw 1"
        if (body.length() < 4) {
            //throw new IllegalCommandException("HardwareLogic command body too short.");
            return;
        }

        final int dashId = state.dashId;
        final int deviceId = state.deviceId;

        DashBoard dash = state.user.profile.getDashByIdOrThrow(dashId);

        if (isWriteOperation(body)) {
            //" |\0" - to simplify demonstration
            String[] splitBody = split3(body);

            if (splitBody.length < 3 || splitBody[0].length() == 0) {
                //throw new IllegalCommandException("Write command is wrong.");
                return;
            }

            final PinType pinType = PinType.getPinType(splitBody[0].charAt(0));
            final byte pin = ParseUtil.parseByte(splitBody[1]);
            final String value = splitBody[2];

            if (value.length() == 0) {
                //throw new IllegalCommandException("Hardware write command doesn't have value for pin.");
                return;
            }

            final long now = System.currentTimeMillis();

            reportingDao.process(state.user.name, dashId, deviceId, pin, pinType, value, now);

            dash.update(0, pin, pinType, value, now);
        }

        //todo do not send if no widget pin
        if (dash.isActive) {
            session.sendToApps(HARDWARE, msg.variableHeader().messageId(), dashId, deviceId, body);
        } else {
            log.debug("No active dashboard.");
        }
    }

}
