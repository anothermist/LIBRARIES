package cc.blynk.core.http.annotation;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.09.16.
 */
import java.lang.annotation.Documented;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Inherited
@Target({java.lang.annotation.ElementType.TYPE, java.lang.annotation.ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Consumes
{
    String[] value() default {"*/*"};
}
