package cc.blynk.server.admin.http.handlers;

import org.junit.Test;

import java.net.InetSocketAddress;

import static org.junit.Assert.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 26.01.16.
 */
public class IpFilterHandlerTest {

    private static InetSocketAddress newSockAddress(String ipAddress) {
        return new InetSocketAddress(ipAddress, 1234);
    }

    @Test
    public void testSingleIPFilterWork() throws Exception {
        String[] data = new String[1];
        data[0] = "192.168.0.50";
        IpFilterHandler ipFilterHandler = new IpFilterHandler(data);

        assertTrue(ipFilterHandler.accept(null, newSockAddress("192.168.0.50")));
        assertFalse(ipFilterHandler.accept(null, newSockAddress("192.168.0.51")));
        assertFalse(ipFilterHandler.accept(null, newSockAddress("192.168.1.50")));
    }

    @Test
    public void testCIDRNotationIPFilterWork() throws Exception {
        String[] data = new String[2];
        data[0] = "192.168.100.100";
        data[1] = "192.168.0.50/24";
        IpFilterHandler ipFilterHandler = new IpFilterHandler(data);

        for (int i = 0; i <= 255; i++) {
            assertTrue(ipFilterHandler.accept(null, (newSockAddress(String.format("192.168.0.%d", i)))));
        }
        assertTrue(ipFilterHandler.accept(null, (newSockAddress("192.168.100.100"))));

        assertFalse(ipFilterHandler.accept(null, (newSockAddress("192.168.1.0"))));
        assertFalse(ipFilterHandler.accept(null, (newSockAddress("192.168.100.0"))));
        assertFalse(ipFilterHandler.accept(null, (newSockAddress("192.168.100.101"))));
    }

}
