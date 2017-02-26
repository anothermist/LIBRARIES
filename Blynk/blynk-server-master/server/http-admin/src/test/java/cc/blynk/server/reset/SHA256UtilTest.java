package cc.blynk.server.reset;

import cc.blynk.utils.SHA256Util;
import org.junit.Test;

import static org.junit.Assert.*;

public class SHA256UtilTest {

    @Test
    public void testPasswordHash() {
        final String hashedPassword = SHA256Util.makeHash("123", "test@gmail.com");
        assertEquals("/pyQf3JCj5XoczfsYJ4LUb+y0DONGMl/AFzLiBTo8LA=", hashedPassword);
    }
}
