package cc.blynk.server.application.handlers.main.logic.reporting;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.ReportingDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Pin;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.server.core.model.widgets.Widget;
import cc.blynk.server.core.model.widgets.outputs.HistoryGraph;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.protocol.model.messages.StringMessage;
import cc.blynk.server.notifications.mail.MailWrapper;
import cc.blynk.utils.FileUtils;
import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandlerContext;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.nio.file.Path;
import java.util.ArrayList;

import static cc.blynk.server.core.protocol.enums.Response.NOTIFICATION_ERROR;
import static cc.blynk.server.core.protocol.enums.Response.NO_DATA;
import static cc.blynk.utils.BlynkByteBufUtil.makeResponse;
import static cc.blynk.utils.BlynkByteBufUtil.ok;
import static cc.blynk.utils.StringUtils.BODY_SEPARATOR_STRING;
import static cc.blynk.utils.StringUtils.split2Device;

/**
 * Sends graph pins data in csv format via to user email.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/1/2015.
 *
 */
public class ExportGraphDataLogic {

    private static final Logger log = LogManager.getLogger(ExportGraphDataLogic.class);

    private final BlockingIOProcessor blockingIOProcessor;
    private final ReportingDao reportingDao;
    private final MailWrapper mailWrapper;
    private final String csvDownloadUrl;

    public ExportGraphDataLogic(ReportingDao reportingDao, BlockingIOProcessor blockingIOProcessor, MailWrapper mailWrapper, String host, int httpPort) {
        this.reportingDao = reportingDao;
        this.blockingIOProcessor = blockingIOProcessor;
        this.mailWrapper = mailWrapper;
        this.csvDownloadUrl = "http://" + host + ":" + httpPort + "/";
    }

    public void messageReceived(ChannelHandlerContext ctx, User user, StringMessage message) {
        String[] messageParts = message.body.split(BODY_SEPARATOR_STRING);

        if (messageParts.length < 2) {
            throw new IllegalCommandException("Wrong income message format.");
        }

        String[] dashIdAndDeviceId = split2Device(messageParts[0]);
        int dashId = ParseUtil.parseInt(dashIdAndDeviceId[0]);

        //todo new device code. remove after migration.
        int deviceId;
        if (dashIdAndDeviceId.length == 2) {
            deviceId = ParseUtil.parseInt(dashIdAndDeviceId[1]);
        } else {
            deviceId = 0;
        }

        long widgetId = ParseUtil.parseLong(messageParts[1]);

        DashBoard dashBoard = user.profile.getDashByIdOrThrow(dashId);

        Widget widget = dashBoard.getWidgetById(widgetId);
        if (!(widget instanceof HistoryGraph)) {
            throw new IllegalCommandException("Passed wrong widget id.");
        }

        HistoryGraph historyGraph = (HistoryGraph) widget;

        blockingIOProcessor.execute(() -> {
            try {
                String dashName = dashBoard.name == null ? "" : dashBoard.name;
                ArrayList<FileLink> pinsCSVFilePath = new ArrayList<>();
                for (Pin pin : historyGraph.pins) {
                    if (pin != null) {
                        try {
                            Path path = FileUtils.createCSV(reportingDao, user.name, dashId, deviceId, pin.pinType, pin.pin);
                            pinsCSVFilePath.add(new FileLink(path.getFileName(), dashName, pin.pinType, pin.pin));
                        } catch (Exception e) {
                            //ignore eny exception.
                        }
                    }
                }

                if (pinsCSVFilePath.size() == 0) {
                    ctx.writeAndFlush(makeResponse(message.id, NO_DATA), ctx.voidPromise());
                } else {

                    String title = "History graph data for project " + dashName;
                    mailWrapper.sendHtml(user.name, title, makeBody(pinsCSVFilePath));
                    ctx.writeAndFlush(ok(message.id), ctx.voidPromise());
                }

            } catch (Exception e) {
                log.error("Error making csv file for data export. Reason {}", e.getMessage());
                ctx.writeAndFlush(makeResponse(message.id, NOTIFICATION_ERROR), ctx.voidPromise());
            }
        });
    }

    private String makeBody(ArrayList<FileLink> fileUrls) {
        StringBuilder sb = new StringBuilder();
        sb.append("<html><body>");
        for (FileLink link : fileUrls) {
            sb.append(link.toString()).append("<br>");
        }
        sb.append("</body></html>");
        return sb.toString();
    }

    private class FileLink {
        final Path path;
        final String dashName;
        final PinType pinType;
        final byte pin;

        public FileLink(Path path, String dashName, PinType pinType, byte pin) {
            this.path = path;
            this.dashName = dashName;
            this.pinType = pinType;
            this.pin = pin;
        }

        @Override
        public String toString() {
            return "<a href=\"" + csvDownloadUrl + path + "\">" + dashName + " " + pinType.pintTypeChar + pin + "</a>";
        }
    }

}
