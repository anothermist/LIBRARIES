package cc.blynk.client;

import cc.blynk.client.core.ActiveHardwareClient;
import cc.blynk.client.core.AppClient;
import cc.blynk.client.core.HardwareClient;
import cc.blynk.client.enums.ClientMode;
import cc.blynk.utils.ParseUtil;
import org.apache.commons.cli.BasicParser;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import java.io.BufferedReader;
import java.io.InputStreamReader;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 11.03.15.
 */
public class ClientLauncher {

    static final String DEFAULT_HOST = "localhost";
    static final int DEFAULT_HARDWARE_PORT = 8442;
    static final int DEFAULT_APPLICATION_PORT = 8443;

    private static final Options options = new Options();

    static {
        options.addOption("host", true, "Server host or ip.")
               .addOption("port", true, "Port client should connect to.")
               .addOption("mode", true, "Client mode. 'hardware' or 'app'.")
               .addOption("tokens", true, "Tokens");
    }

    public static void main(String[] args) throws ParseException {
        CommandLine cmd = new BasicParser().parse(options, args);

        ClientMode mode = ClientMode.parse(cmd.getOptionValue("mode", ClientMode.HARDWARE.name()));
        String host = cmd.getOptionValue("host", DEFAULT_HOST);
        int port = ParseUtil.parseInt(cmd.getOptionValue("port",
                        (mode == ClientMode.APP ? String.valueOf(DEFAULT_APPLICATION_PORT) : String.valueOf(DEFAULT_HARDWARE_PORT)))
        );

        switch (mode) {
            case APP :
                new AppClient(host, port).start(new BufferedReader(new InputStreamReader(System.in)));
                break;
            case HARDWARE :
                new HardwareClient(host, port).start(new BufferedReader(new InputStreamReader(System.in)));
                break;
            default :
                String tokensFullString = cmd.getOptionValue("tokens");
                if (tokensFullString == null) {
                    throw new RuntimeException("Tokens required for TEST mode.");
                }
                String[] tokens = tokensFullString.split(",");
                for (String token : tokens) {
                    new ActiveHardwareClient(host, port).start(token);
                }
        }
    }

}
