package cc.blynk.core.http.handlers;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.07.16.
 */
public class StaticFileEdsWith extends StaticFile {

    public final String folderPathForStatic;

    public StaticFileEdsWith(String folderPathForStatic, String path) {
        super(path, false);
        this.folderPathForStatic = folderPathForStatic;
    }

    public StaticFileEdsWith(String folderPathForStatic, String endsWith, boolean doCaching) {
        super(endsWith, doCaching);
        this.folderPathForStatic = folderPathForStatic;
    }

    @Override
    public boolean isStatic(String url) {
        return url.endsWith(path);
    }
}
