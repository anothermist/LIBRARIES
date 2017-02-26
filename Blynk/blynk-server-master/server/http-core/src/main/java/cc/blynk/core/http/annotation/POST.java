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

@Target({java.lang.annotation.ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@HttpMethod("POST")
@Documented
public @interface POST {}
