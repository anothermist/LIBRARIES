package cc.blynk.utils;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Base64;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 22.04.15.
 */
public class SHA256Util {

    private static final Logger log = LogManager.getLogger(SHA256Util.class);

    public static String makeHash(String password, String salt) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(password.getBytes("UTF-8"));

            byte byteData[] = md.digest(makeHash(salt.toLowerCase()));

            return Base64.getEncoder().encodeToString(byteData);
        } catch (NoSuchAlgorithmException | UnsupportedEncodingException e) {
            log.error("Unable to make hash for pass. No hashing.", e);
        }

        return password;
    }

    private static byte[] makeHash(String val) throws NoSuchAlgorithmException, UnsupportedEncodingException {
        return MessageDigest.getInstance("SHA-256").digest(val.getBytes("UTF-8"));
    }

}
