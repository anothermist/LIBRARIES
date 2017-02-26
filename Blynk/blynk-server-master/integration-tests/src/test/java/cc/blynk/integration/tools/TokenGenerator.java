package cc.blynk.integration.tools;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.db.DBManager;
import cc.blynk.server.db.model.Redeem;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.UUID;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 08.02.16.
 */
public class TokenGenerator {

    public static void main(String[] args) throws Exception {
        //List<String> tokens = Files.readAllLines(Paths.get("/home/doom369/Downloads/x.csv"));
        Set<String> tokens = generate(10);

        List<Redeem> redeems = new ArrayList<>(tokens.size());
        for (String token : tokens) {
            redeems.add(new Redeem(token, "SparkFun", 15000));
        }

        DBManager dbManager = new DBManager("db-test.properties", new BlockingIOProcessor(1, 100, null));
        dbManager.insertRedeems(redeems);
    }

    private static Set<String> generate(int amount) {
        Set<String> tokens = new HashSet<>();

        for (int i = 0; i < amount; i++ ) {
            String token = UUID.randomUUID().toString().replace("-", "");
            tokens.add(token);
            System.out.println(token);
        }

        return tokens;
    }

    private static void write(String outputPath, Set<String> tokens) throws IOException {
        Path path = Paths.get(outputPath);
        write(path, tokens);
    }

    private static void write(Path path, Set<String> tokens) throws IOException {
        try (BufferedWriter writer = Files.newBufferedWriter(path)) {
            for (String token : tokens) {
                writer.write(token);
                writer.newLine();
            }
        }
    }

}
