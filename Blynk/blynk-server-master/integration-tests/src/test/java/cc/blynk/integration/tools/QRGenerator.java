package cc.blynk.integration.tools;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.db.DBManager;
import cc.blynk.server.db.model.Redeem;
import net.glxn.qrgen.core.image.ImageType;
import net.glxn.qrgen.javase.QRCode;

import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 22.03.16.
 */
public class QRGenerator {

    public static void main(String[] args) throws Exception {
        DBManager dbManager = new DBManager("db-test.properties", new BlockingIOProcessor(1, 100, null));
        List<Redeem> redeems;

        redeems = generateQR(361, "/home/doom369/QR/blynk25", "Blynk", 25000);
        dbManager.insertRedeems(redeems);

        redeems = generateQR(1950, "/home/doom369/QR/blynk100", "Blynk", 100000);
        dbManager.insertRedeems(redeems);

        redeems = generateQR(90, "/home/doom369/QR/bluz", "Bluz", 100000);
        dbManager.insertRedeems(redeems);

        redeems = generateQR(210, "/home/doom369/QR/oak", "Digistump Oak", 100000);
        dbManager.insertRedeems(redeems);

        redeems = generateQR(160, "/home/doom369/QR/onion", "Onion Omega", 100000);
        dbManager.insertRedeems(redeems);
    }

    private static List<Redeem> generateQR(int count, String outputFolder, String campaign, int reward) throws Exception {
        List<Redeem> redeems = new ArrayList<>(count);
        for (int i = 0; i < count; i++) {
            String token = UUID.randomUUID().toString().replace("-", "");

            Redeem redeem = new Redeem(token, campaign, reward);
            redeems.add(redeem);

            Path path = Paths.get(outputFolder, String.format("%d.jpg", i));
            generateQR(redeem.formatToken(), path);
        }
        return redeems;
    }

    private static void generateQR(String text, Path outputFile) throws Exception {
        try (OutputStream out = Files.newOutputStream(outputFile)) {
            QRCode.from(text).to(ImageType.JPG).writeTo(out);
        }
    }

}
