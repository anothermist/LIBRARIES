package cc.blynk.server.db.dao;

import cc.blynk.server.db.model.Purchase;
import com.zaxxer.hikari.HikariDataSource;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.sql.Connection;
import java.sql.PreparedStatement;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.03.16.
 */
public class PurchaseDBDao {

    public static final String insertPurchase = "INSERT INTO purchase (username, reward, transactionId, price) values (?, ?, ?, ?)";

    private static final Logger log = LogManager.getLogger(PurchaseDBDao.class);
    private final HikariDataSource ds;

    public PurchaseDBDao(HikariDataSource ds) {
        this.ds = ds;
    }

    public void insertPurchase(Purchase purchase) {
        try (Connection connection = ds.getConnection();
             PreparedStatement ps = connection.prepareStatement(insertPurchase)) {

            insert(ps, purchase);
            ps.executeUpdate();

            connection.commit();
        } catch (Throwable e) {
            log.error("Error inserting purchase data in DB.", e);
        }
    }

    private static void insert(PreparedStatement ps, Purchase purchase) throws Exception {
        ps.setString(1, purchase.username);
        ps.setInt(2, purchase.reward);
        ps.setString(3, purchase.transactionId);
        ps.setDouble(4, purchase.price);
    }
}
