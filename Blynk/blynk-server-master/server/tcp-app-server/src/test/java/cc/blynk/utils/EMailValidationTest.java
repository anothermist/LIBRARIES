package cc.blynk.utils;

import org.apache.commons.validator.routines.EmailValidator;
import org.junit.Ignore;
import org.junit.Test;

import java.io.File;

import static org.junit.Assert.*;

/**
 * User: ddumanskiy
 * Date: 8/11/13
 * Time: 6:43 PM
 */
public class EMailValidationTest {

    @Test
    public void testAllValid() {
        String[] mailList = new String[] {
                "xxxx.yyy@rsa.rohde-schwarz.com",
                "1@mail.ru",
                "google@gmail.com",
                "dsasd234e021-0+@mail.ua",
                "ddd@yahoo.com",
                "mmmm@yahoo.com",
                "mmmmm-100@yahoo.com",
                "mmmmm.100@yahoo.com",
                "mmmm111@mmmm.com",
                "mmmm-100@mmmm.net",
                "mmmm.100@mmmm.com.au",
                "mmmm@1.com",
                "mmmm@gmail.com.com",
                "mmmm+100@gmail.com",
                "bla@bla.com.ua",
                "bla@bla.cc",
                "mmmm-100@yahoo-test.com"
        };

        for (String email : mailList) {
            assertTrue(email, EmailValidator.getInstance().isValid(email));
        }
    }

    @Test
    @Ignore("to avoid publishing of emails you need to specify your data")
    public void testExistingUsers() {
        File dataDir = new File("/home/doom369/prod_data/data");
        File[] files = dataDir.listFiles();
        if (files != null) {
            for (File user : files) {
                String email = user.getName().substring(2, user.getName().length() - 5);
                //if (email.endsWith(".con") || email.endsWith(".cpm") || email.endsWith(".comcom") || email.endsWith("fe")
                //        || email.endsWith(".hshs") || email.endsWith(".aa") || email.endsWith(".cim")) {
                //    continue;
                //}
                assertTrue(email, EmailValidator.getInstance().isValid(email));
            }
        }
    }

    @Test
    public void testAllInValid() {
        String[] mailList = new String[] {
                "mmmm",
                "mmmm@.com.my",
                "mmm@hey.con",
                "mmm@hey.cpm",
                "mmm@hey.comcom",
                "mmm@hey.fe",
                "mmm@hey.hshs",
                "mmm@hey.aa",
                "mmm@hey.cim",
                "mmmm123@.com.com",
                ".mmmm@mmmm.com",
                "mmmm()*@gmail.com",
                "mmmm..2002@gmail.com",
                "mmmm.@gmail.com",
                "mmmm@mmmm@gmail.com",
                "bla@bla"
        };

        for (String email : mailList) {
            assertFalse(email, EmailValidator.getInstance().isValid(email));
        }
    }

}
