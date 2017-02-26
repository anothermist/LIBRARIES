package cc.blynk.core.http.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 23.09.16.
 */
@Target({java.lang.annotation.ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@HttpMethod("DELETE")
@Documented
public @interface DELETE {}
