package cc.blynk.core.http.annotation;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.09.16.
 */
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Target({java.lang.annotation.ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface HttpMethod
{
    public static final String GET = "GET";
    public static final String POST = "POST";
    public static final String PUT = "PUT";
    public static final String DELETE = "DELETE";
    public static final String HEAD = "HEAD";
    public static final String OPTIONS = "OPTIONS";

    String value();
}
