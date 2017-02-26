package cc.blynk.server.workers;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.FileManager;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.dao.UserKey;
import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.db.DBManager;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import static org.mockito.Mockito.any;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 3/3/2015.
 */
@RunWith(MockitoJUnitRunner.class)
public class ProfileSaverWorkerTest {

    @Mock
    private UserDao userDao;

    @Mock
    private FileManager fileManager;

    @Mock
    private GlobalStats stats;

    private BlockingIOProcessor blockingIOProcessor = new BlockingIOProcessor(1, 1, null);

    @Test
    public void testCorrectProfilesAreSaved() throws IOException {
        ProfileSaverWorker profileSaverWorker = new ProfileSaverWorker(userDao, fileManager, new DBManager(blockingIOProcessor));

        User user1 = new User("1", "", AppName.BLYNK, "local", false);
        User user2 = new User("2", "", AppName.BLYNK, "local", false);
        User user3 = new User("3", "", AppName.BLYNK, "local", false);
        User user4 = new User("4", "", AppName.BLYNK, "local", false);

        ConcurrentMap<UserKey, User> userMap = new ConcurrentHashMap<>();
        userMap.put(new UserKey(user1), user1);
        userMap.put(new UserKey(user2), user2);
        userMap.put(new UserKey(user3), user3);
        userMap.put(new UserKey(user4), user4);

        when(userDao.getUsers()).thenReturn(userMap);
        profileSaverWorker.run();

        verify(fileManager, times(4)).overrideUserFile(any());
        verify(fileManager).overrideUserFile(user1);
        verify(fileManager).overrideUserFile(user2);
        verify(fileManager).overrideUserFile(user3);
        verify(fileManager).overrideUserFile(user4);
    }

    @Test
    public void testNoProfileChanges() throws Exception {
        User user1 = new User("1", "", AppName.BLYNK, "local", false);
        User user2 = new User("2", "", AppName.BLYNK, "local", false);
        User user3 = new User("3", "", AppName.BLYNK, "local", false);
        User user4 = new User("4", "", AppName.BLYNK, "local", false);

        Map<UserKey, User> userMap = new HashMap<>();
        userMap.put(new UserKey("1", AppName.BLYNK), user1);
        userMap.put(new UserKey("2", AppName.BLYNK), user2);
        userMap.put(new UserKey("3", AppName.BLYNK), user3);
        userMap.put(new UserKey("4", AppName.BLYNK), user4);

        Thread.sleep(1);

        ProfileSaverWorker profileSaverWorker = new ProfileSaverWorker(userDao, fileManager, new DBManager(blockingIOProcessor));

        when(userDao.getUsers()).thenReturn(userMap);
        profileSaverWorker.run();

        verifyNoMoreInteractions(fileManager);
    }

}
