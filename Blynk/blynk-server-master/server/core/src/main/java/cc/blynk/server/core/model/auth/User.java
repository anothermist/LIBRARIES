package cc.blynk.server.core.model.auth;

import cc.blynk.server.core.model.AppName;
import cc.blynk.server.core.model.Profile;
import cc.blynk.server.core.protocol.exceptions.EnergyLimitException;
import cc.blynk.utils.JsonParser;
import com.fasterxml.jackson.annotation.JsonProperty;

import java.util.concurrent.ConcurrentMap;

/**
 * User: ddumanskiy
 * Date: 8/11/13
 * Time: 4:03 PM
 */
public class User {

    private static final int INITIAL_ENERGY_AMOUNT = 2000;

    //todo remove after migration
    public ConcurrentMap<Integer, String> dashTokens;

    //key fields
	public String name;
    public String appName;
    public String region;

    public volatile String pass;

    //used mostly to understand if user profile was changed, all other fields update ignored as it is not so important
    public volatile long lastModifiedTs;

    public String lastLoggedIP;
    public long lastLoggedAt;

    public Profile profile;

    public boolean isFacebookUser;

    public volatile int energy;

    public transient int emailMessages;
    public transient long emailSentTs;

    public User() {
        this.lastModifiedTs = System.currentTimeMillis();
        this.profile = new Profile();
        this.energy = INITIAL_ENERGY_AMOUNT;
        this.isFacebookUser = false;
        this.appName = AppName.BLYNK;
    }

    public User(String name, String pass, String appName, String region, boolean isFacebookUser) {
        this();
        this.name = name;
        this.pass = pass;
        this.appName = appName;
        this.region = region;
        this.isFacebookUser = isFacebookUser;
    }

    @JsonProperty("id")
    private String id() {
        return name + "-" + appName;
    }

    public void subtractEnergy(int price) {
        if (AppName.BLYNK.equals(appName) && price > energy) {
            throw new EnergyLimitException("Not enough energy.");
        }
        //non-atomic. we are fine with that
        this.energy -= price;
        this.lastModifiedTs = System.currentTimeMillis();
    }

    public void recycleEnergy(int price) {
        purchaseEnergy(price);
    }

    public void purchaseEnergy(int price) {
        //non-atomic. we are fine with that
        this.energy += price;
        this.lastModifiedTs = System.currentTimeMillis();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        User user = (User) o;

		return !(name != null ? !name.equals(user.name) : user.name != null);

	}

    @Override
    public int hashCode() {
        return name != null ? name.hashCode() : 0;
    }

    @Override
    public String toString() {
        return JsonParser.toJson(this);
    }

}
