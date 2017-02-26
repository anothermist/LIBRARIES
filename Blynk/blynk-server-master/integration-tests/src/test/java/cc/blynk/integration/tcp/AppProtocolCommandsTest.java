package cc.blynk.integration.tcp;

import cc.blynk.integration.IntegrationBase;
import cc.blynk.integration.model.MockHolder;
import cc.blynk.integration.model.tcp.TestAppClient;
import cc.blynk.server.application.AppServer;
import cc.blynk.server.core.BaseServer;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.protocol.enums.Command;
import cc.blynk.server.core.protocol.model.messages.ResponseMessage;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.db.DBManager;
import cc.blynk.server.workers.ProfileSaverWorker;
import cc.blynk.utils.ReflectionUtil;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;
import org.mockito.stubbing.OngoingStubbing;

import java.io.BufferedReader;
import java.util.Collection;
import java.util.Collections;
import java.util.Map;

import static cc.blynk.server.core.protocol.enums.Command.APP_SYNC;
import static cc.blynk.server.core.protocol.enums.Response.ILLEGAL_COMMAND;
import static cc.blynk.server.core.protocol.enums.Response.OK;
import static cc.blynk.server.core.protocol.enums.Response.QUOTA_LIMIT;
import static cc.blynk.server.core.protocol.enums.Response.USER_NOT_AUTHENTICATED;
import static cc.blynk.server.core.protocol.enums.Response.USER_NOT_REGISTERED;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 2/2/2015.
 *
 * Basic integration test. Allows to test base commands workflow. Thus netty is asynchronous
 * test is little bit complex, but I don't know right now how to make it better and simpler.
 *
 */
@RunWith(MockitoJUnitRunner.class)
public class AppProtocolCommandsTest extends IntegrationBase {

    @Mock
    public BufferedReader bufferedReader;
    private BaseServer appServer;

    @Before
    public void init() throws Exception {
        this.appServer = new AppServer(holder).start();

        ProfileSaverWorker profileSaverWorker = new ProfileSaverWorker(holder.userDao, holder.fileManager, new DBManager(blockingIOProcessor));
        new Thread(profileSaverWorker).start();
    }

    @After
    public void shutdown() {
        this.appServer.close();
    }

    @Test
    public void testCommandsIndexOk() {
        Map<Short, String> commandCodes = ReflectionUtil.generateMapOfValueNameShort(Command.class);
        Collection<Short> codes = commandCodes.keySet();
        Short maxShort = Collections.max(codes);
        GlobalStats stat = new GlobalStats();
        stat.mark(maxShort);
    }

    @Test
    public void testAppNotRegistered() throws Exception {
        makeCommands("login dmitriy@mail.ua 1").check(new ResponseMessage(1, USER_NOT_REGISTERED));
    }

    @Test
    public void testIllegalCommandForHardLoginOnAppChannel() throws Exception {
        makeCommands("login dasdsadasdasdasdasdas").check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    public void testLogin2Times() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands(1, "login dmitriy@mail.ua 1", "login dmitriy@mail.ua 1").check(1, OK);
    }

    @Test
    public void testLogin2TimesAndWork() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands(2, "login dmitriy@mail.ua 1", "login dmitriy@mail.ua 1", "getToken 1").check(1, OK).check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    public void testGetTokenForNonExistentDashId() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 1", "getToken 1").check(OK).check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    public void testRefreshToken() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        Profile profile = parseProfile(readTestUserProfile());

        makeCommands("login dmitriy@mail.ua 1", "addEnergy 5000" + "\0" + "123", "createDash " + profile.dashBoards[0]).check(3, OK);

        //todo fix?
        makeCommands("login dmitriy@mail.ua 1", "getToken 1", "refreshToken 1")
                .check(OK);
                //.check(2, produce(1, REFRESH_TOKEN, "12345678901234567890123456789012"));
    }

    @Test
    @Ignore
    //todo fix
    public void testProfileWithManyDashes() throws Exception {
        makeCommands("register dmitriy@mail.ua 1", "login dmitriy@mail.ua 1", "addEnergy 5000").check(3, OK);

        Profile profile = parseProfile(readTestUserProfile("user_profile_json_many_dashes.txt"));

        String[] cmds = new String[profile.dashBoards.length + 1];
        cmds[0] = "login dmitriy@mail.ua 1";
        for (int i = 0; i < profile.dashBoards.length; i++) {
            cmds[i + 1] = "createDash " + profile.dashBoards[i];
        }

        makeCommands(cmds).check(profile.dashBoards.length, OK).check(new ResponseMessage(1, QUOTA_LIMIT));
    }

    @Test
    public void testPassNotValid() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 2").check(new ResponseMessage(1, USER_NOT_AUTHENTICATED));
    }

    @Test
    public void testActivateWrongFormat() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 1", "activate ").check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    @Ignore
    //todo fix?
    public void testActivateWorks() throws Exception {
        String userProfileString = readTestUserProfile();

        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 1", "saveProfile " + userProfileString).check(2, OK);

        makeCommands("activate 1").check(10, APP_SYNC);
    }

    @Test
    public void testActivateWrongDashId() throws Exception {
        Profile profile = parseProfile(readTestUserProfile());

        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 1", "addEnergy 5000" + "\0" + "123", "createDash " + profile.dashBoards[0], "activate 2").check(3, OK).check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    public void testActivateBadId() throws Exception {
        Profile profile = parseProfile(readTestUserProfile());

        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 1", "addEnergy 5000" + "\0" + "123", "createDash " + profile.dashBoards[0], "activate xxx").check(3, OK).check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    public void testTryHardLoginInAppChannel() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login adsadasdasdasdas").check(new ResponseMessage(1, ILLEGAL_COMMAND));
    }

    @Test
    //all commands together cause all operations requires register and then login =(.
    public void testPingOk() throws Exception {
        makeCommands("register dmitriy@mail.ua 1").check(OK);

        makeCommands("login dmitriy@mail.ua 1", "ping").check(2, OK);
    }

    /**
     * 1) Creates client socket;
     * 2) Sends commands;
     * 3) Sleeps for 100ms between every command send;
     * 4) Checks that sever response is OK;
     * 5) Closing socket.
     */
    private MockHolder makeCommands(int expected, String... commands) throws Exception {
        TestAppClient appClient = new TestAppClient("localhost", tcpAppPort);

        OngoingStubbing<String> ongoingStubbing = when(bufferedReader.readLine());
        for (String cmd : commands) {
            ongoingStubbing = ongoingStubbing.thenReturn(cmd);
        }

        ongoingStubbing.thenAnswer(invocation -> {
            //todo think how to avoid this
            sleep(400);
            return "quit";
        });

        appClient.start(bufferedReader);

        verify(appClient.responseMock, times(expected)).channelRead(any(), any());
        return new MockHolder(appClient.responseMock);
    }

    private MockHolder makeCommands(String... commands) throws Exception {
        return makeCommands(commands.length, commands);
    }

}
