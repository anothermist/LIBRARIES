package cc.blynk.server.admin.http.response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 10.12.15.
 */
public final class IpNameResponse {

    public final String ip;

    public final String name;

    public IpNameResponse(String name, String ip) {
        this.name = name;
        this.ip = ip;
    }


}
