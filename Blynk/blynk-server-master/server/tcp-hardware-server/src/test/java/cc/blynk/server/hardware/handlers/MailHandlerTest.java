package cc.blynk.server.hardware.handlers;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.model.DashBoard;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.widgets.notifications.Mail;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.exceptions.IllegalCommandException;
import cc.blynk.server.core.protocol.exceptions.NotAllowedException;
import cc.blynk.server.core.protocol.model.messages.MessageFactory;
import cc.blynk.server.core.protocol.model.messages.hardware.MailMessage;
import cc.blynk.server.core.session.HardwareStateHolder;
import cc.blynk.server.hardware.handlers.hardware.logic.MailLogic;
import cc.blynk.server.notifications.mail.MailWrapper;
import cc.blynk.utils.ServerProperties;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import static org.mockito.Mockito.when;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 07.04.15.
 */
@RunWith(MockitoJUnitRunner.class)
public class MailHandlerTest {

    @Mock
    private BlockingIOProcessor blockingIOProcessor;

    @Mock
    private MailWrapper mailWrapper;

    private final MailLogic mailHandler = new MailLogic(blockingIOProcessor, mailWrapper, 1);

	@Mock
	private ChannelHandlerContext ctx;

	@Mock
	private UserDao userDao;

	@Mock
	private SessionDao sessionDao;

	@Mock
	private ServerProperties serverProperties;

    @Mock
    private User user;

    @Mock
    private Profile profile;

    @Mock
    private DashBoard dashBoard;

    @Mock
    private Channel channel;

    @Test(expected = NotAllowedException.class)
	public void testNoEmailWidget() throws InterruptedException {
		MailMessage mailMessage = (MailMessage) MessageFactory.produce(1, Command.EMAIL, "body");

        user.profile = profile;
        when(profile.getDashByIdOrThrow(1)).thenReturn(dashBoard);
        when(dashBoard.getWidgetByType(Mail.class)).thenReturn(null);

        HardwareStateHolder state = new HardwareStateHolder(1, 0, user, "x");
        mailHandler.messageReceived(ctx, state, mailMessage);
    }

    @Test(expected = IllegalCommandException.class)
	public void testNoToBody() throws InterruptedException {
		MailMessage mailMessage = (MailMessage) MessageFactory.produce(1, Command.EMAIL, "".replaceAll(" ", "\0"));

        user.profile = profile;
        when(profile.getDashByIdOrThrow(1)).thenReturn(dashBoard);
        Mail mail = new Mail();
        when(dashBoard.getWidgetByType(Mail.class)).thenReturn(mail);
        dashBoard.isActive = true;

        HardwareStateHolder state = new HardwareStateHolder(1, 0, user, "x");
        mailHandler.messageReceived(ctx, state, mailMessage);
    }

    @Test(expected = IllegalCommandException.class)
	public void testNoBody() throws InterruptedException {
		MailMessage mailMessage = (MailMessage) MessageFactory.produce(1, Command.EMAIL, "body".replaceAll(" ", "\0"));

        user.profile = profile;
        when(profile.getDashByIdOrThrow(1)).thenReturn(dashBoard);
        when(dashBoard.getWidgetByType(Mail.class)).thenReturn(new Mail());
        dashBoard.isActive = true;

        HardwareStateHolder state = new HardwareStateHolder(1, 0, user, "x");
        mailHandler.messageReceived(ctx, state, mailMessage);
    }

}
