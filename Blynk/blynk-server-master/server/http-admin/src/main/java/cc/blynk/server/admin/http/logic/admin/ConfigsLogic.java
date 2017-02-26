package cc.blynk.server.admin.http.logic.admin;

import cc.blynk.core.http.MediaType;
import cc.blynk.core.http.Response;
import cc.blynk.core.http.annotation.Consumes;
import cc.blynk.core.http.annotation.GET;
import cc.blynk.core.http.annotation.PUT;
import cc.blynk.core.http.annotation.Path;
import cc.blynk.core.http.annotation.PathParam;
import cc.blynk.core.http.annotation.QueryParam;
import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.db.DBManager;
import cc.blynk.server.notifications.mail.MailWrapper;
import cc.blynk.server.notifications.push.GCMWrapper;
import cc.blynk.utils.ServerProperties;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import static cc.blynk.core.http.Response.appendTotalCountHeader;
import static cc.blynk.core.http.Response.badRequest;
import static cc.blynk.core.http.Response.ok;
import static cc.blynk.utils.HttpLogicUtil.log;
import static cc.blynk.utils.HttpLogicUtil.sort;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 09.12.15.
 */
@Path("/config")
public class ConfigsLogic {

    private final BlockingIOProcessor blockingIOProcessor;
    private final ServerProperties serverProperties;

    public ConfigsLogic(ServerProperties serverProperties, BlockingIOProcessor blockingIOProcessor) {
        this.blockingIOProcessor = blockingIOProcessor;
        this.serverProperties = serverProperties;
    }

    @GET
    @Path("")
    public Response getConfigs(@QueryParam("_filters") String filterParam,
                             @QueryParam("_page") int page,
                             @QueryParam("_perPage") int size,
                             @QueryParam("_sortField") String sortField,
                             @QueryParam("_sortDir") String sortOrder) {

        List<Config> configs = new ArrayList<>();
        configs.add(new Config(ServerProperties.SERVER_PROPERTIES_FILENAME));
        configs.add(new Config(MailWrapper.MAIL_PROPERTIES_FILENAME));
        configs.add(new Config(GCMWrapper.GCM_PROPERTIES_FILENAME));
        configs.add(new Config(DBManager.DB_PROPERTIES_FILENAME));
        configs.add(new Config("twitter4j.properties"));
        configs.add(new Config("single_token_mail_body.txt"));

        return appendTotalCountHeader(
                                ok(sort(configs, sortField, sortOrder), page, size), configs.size()
        );
    }

    @GET
    @Path("/{name}")
    public Response getUserByName(@PathParam("name") String name) {
        switch (name) {
            case "single_token_mail_body.txt":
                return ok(new Config(name, blockingIOProcessor.tokenBody).toString());
            case ServerProperties.SERVER_PROPERTIES_FILENAME :
                return ok(new Config(name, serverProperties).toString());
            default :
                return badRequest();
        }
    }


    @PUT
    @Consumes(value = MediaType.APPLICATION_JSON)
    @Path("/{name}")
    public Response updateConfig(@PathParam("name") String name,
                               Config updatedConfig) {

        log.info("Updating config {}. New body : ", name);
        log.info("{}", updatedConfig.body);

        switch (name) {
            case "single_token_mail_body.txt":
                blockingIOProcessor.tokenBody = updatedConfig.body;
                break;
            case ServerProperties.SERVER_PROPERTIES_FILENAME :
                Properties properties = readPropertiesFromString(updatedConfig.body);
                serverProperties.putAll(properties);
                break;
        }

        return ok(updatedConfig.toString());
    }

    private static Properties readPropertiesFromString(String propertiesAsString) {
        Properties properties = new Properties();
        try {
            properties.load(new StringReader(propertiesAsString));
        } catch (IOException e) {
            log.error("Error reading properties as string. {}", e.getMessage());
        }
        return properties;
    }

}
