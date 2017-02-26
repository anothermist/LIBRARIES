package cc.blynk.server.db.dao;

import cc.blynk.server.db.model.Redeem;
import com.zaxxer.hikari.HikariDataSource;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.List;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.03.16.
 */
public class RedeemDBDao {

    public static final String selectRedeemToken = "SELECT * from redeem where token = ?";
    public static final String updateRedeemToken = "UPDATE redeem SET username = ?, version = 2, isRedeemed = true, ts = NOW() WHERE token = ? and version = 1";
    public static final String insertRedeemToken = "INSERT INTO redeem (token, company, reward) values (?, ?, ?)";

    private static final Logger log = LogManager.getLogger(RedeemDBDao.class);
    private final HikariDataSource ds;

    public RedeemDBDao(HikariDataSource ds) {
        this.ds = ds;
    }

    public Redeem selectRedeemByToken(String token) throws Exception {
        log.info("Redeem select for {}", token);

        ResultSet rs = null;
        try (Connection connection = ds.getConnection();
             PreparedStatement statement = connection.prepareStatement(selectRedeemToken)) {

            statement.setString(1, token);
            rs = statement.executeQuery();
            connection.commit();

            if (rs.next()) {
                return new Redeem(rs.getString("token"), rs.getString("company"),
                        rs.getBoolean("isRedeemed"), rs.getString("username"),
                        rs.getInt("reward"), rs.getInt("version"),
                        rs.getDate("ts")
                );
            }
        } finally {
            if (rs != null) {
                rs.close();
            }
        }

        return null;
    }

    public boolean updateRedeem(String username, String token) throws Exception {
        try (Connection connection = ds.getConnection();
             PreparedStatement statement = connection.prepareStatement(updateRedeemToken)) {

            statement.setString(1, username);
            statement.setString(2, token);
            int updatedRows = statement.executeUpdate();
            connection.commit();
            return updatedRows == 1;
        }
    }

    public void insertRedeems(List<Redeem> redeemList) {
        try (Connection connection = ds.getConnection();
             PreparedStatement ps = connection.prepareStatement(insertRedeemToken)) {

            for (Redeem redeem : redeemList) {
                insert(ps, redeem);
                ps.addBatch();
            }

            ps.executeBatch();
            connection.commit();
        } catch (Exception e) {
            log.error("Error inserting redeems data in DB.", e);
        }
    }

    private static void insert(PreparedStatement ps, Redeem redeem) throws Exception {
        ps.setString(1, redeem.token);
        ps.setString(2, redeem.company);
        ps.setInt(3, redeem.reward);
    }
}
