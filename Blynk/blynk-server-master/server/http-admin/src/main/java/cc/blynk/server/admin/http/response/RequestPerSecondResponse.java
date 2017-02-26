package cc.blynk.server.admin.http.response;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 10.12.15.
 */
public class RequestPerSecondResponse {

    public final String name;

    public final int appRate;

    public final int hardRate;

    public RequestPerSecondResponse(String name, int appRate, int hardRate) {
        this.name = name;
        this.appRate = appRate;
        this.hardRate = hardRate;
    }


}
