package cc.blynk.server.api.http.pojo;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 29.12.15.
 */
public class EmailPojo {

    public String to;
    public String title;
    public String subj;

    public EmailPojo() {
    }

    public EmailPojo(String to, String title, String subj) {
        this.to = to;
        this.title = title;
        this.subj = subj;
    }

    @Override
    public String toString() {
        return "EmailPojo{" +
                "to='" + to + '\'' +
                ", title='" + title + '\'' +
                ", subj='" + subj + '\'' +
                '}';
    }
}
