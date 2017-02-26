package cc.blynk.server.core.model;

import cc.blynk.server.core.model.auth.User;
import cc.blynk.server.core.model.enums.PinType;
import cc.blynk.utils.JsonParser;
import org.junit.Test;

import java.util.HashMap;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 19.11.16.
 */
public class PinStorageSerializationTest {

    @Test
    public void testSerialize() {
        User user = new User();
        user.name = "123";
        user.profile = new Profile();
        user.profile.dashBoards = new DashBoard[] {
                new DashBoard()
        };
        user.lastModifiedTs = 0;
        user.profile.dashBoards[0].pinsStorage = new HashMap<>();
        PinStorageKey pinStorageKey = new PinStorageKey(0, PinType.VIRTUAL, (byte) 0);
        PinStorageKey pinStorageKey2 = new PinStorageKey(0, PinType.DIGITAL, (byte) 1);
        user.profile.dashBoards[0].pinsStorage.put(pinStorageKey, "0");
        user.profile.dashBoards[0].pinsStorage.put(pinStorageKey2, "1");

        String result = user.toString();
        assertTrue(result.contains("0-v0"));
        assertTrue(result.contains("0-d1"));
    }

    @Test
    public void testDeserialize() throws Exception{
        String expectedString = "{\"name\":\"123\",\"appName\":\"Blynk\",\"lastModifiedTs\":0,\"lastLoggedAt\":0,\"profile\":{\"dashBoards\":[{\"id\":0,\"createdAt\":0,\"updatedAt\":0,\"theme\":\"Blynk\",\"keepScreenOn\":false,\"isShared\":false,\"isActive\":false," +
                "\"pinsStorage\":{\"0-v0\":\"0\",\"0-d1\":\"1\"}" +
                "}]},\"isFacebookUser\":false,\"energy\":2000,\"id\":\"123-Blynk\"}";

        User user = JsonParser.parseUserFromString(expectedString);
        assertNotNull(user);
        assertEquals(2, user.profile.dashBoards[0].pinsStorage.size());

        PinStorageKey pinStorageKey = new PinStorageKey(0, PinType.VIRTUAL, (byte) 0);
        PinStorageKey pinStorageKey2 = new PinStorageKey(0, PinType.DIGITAL, (byte) 1);

        assertEquals("0", user.profile.dashBoards[0].pinsStorage.get(pinStorageKey));
        assertEquals("1", user.profile.dashBoards[0].pinsStorage.get(pinStorageKey2));
    }


}
