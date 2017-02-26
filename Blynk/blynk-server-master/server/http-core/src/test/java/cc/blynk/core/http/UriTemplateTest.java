package cc.blynk.core.http;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 05.01.16.
 */
public class UriTemplateTest {

    @Test
    public void testCorrectMatch() {
        UriTemplate template1 = new UriTemplate("http://example.com/admin/users/{name}");
        UriTemplate template2 = new UriTemplate("http://example.com/admin/users/changePass/{name}");

        assertFalse(template1.matches("http://example.com/admin/users/changePass/dmitriy@blynk.cc"));
        assertTrue(template1.matches("http://example.com/admin/users/dmitriy@blynk.cc"));

        assertTrue(template2.matches("http://example.com/admin/users/changePass/dmitriy@blynk.cc"));
        assertFalse(template2.matches("http://example.com/admin/users/dmitriy@blynk.cc"));
    }

}
