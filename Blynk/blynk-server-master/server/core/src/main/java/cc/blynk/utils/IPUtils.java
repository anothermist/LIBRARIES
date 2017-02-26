package cc.blynk.utils;

import io.netty.channel.Channel;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 13.03.16.
 */
public class IPUtils {

    private static final Logger log = LogManager.getLogger(IPUtils.class);

    public static String resolveHostIP(String netInterface) {
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface networkInterface = interfaces.nextElement();
                if (networkInterface.getDisplayName().startsWith(netInterface)) {
                    Enumeration<InetAddress> ips = networkInterface.getInetAddresses();
                    while (ips.hasMoreElements()) {
                        InetAddress inetAddress = ips.nextElement();
                        if (inetAddress instanceof Inet4Address) {
                            return inetAddress.getHostAddress();
                        }
                    }
                    return networkInterface.getDisplayName();
                }
            }
            return InetAddress.getLocalHost().getHostAddress();
        } catch (Exception e) {
            log.warn("Problem resolving current host IP.", e.getMessage());
            return "127.0.0.1";
        }
    }

    public static String getIp(Channel channel) {
        try {
            InetSocketAddress socketAddress = (InetSocketAddress) channel.remoteAddress();
            return socketAddress.getAddress().getHostAddress();
        } catch (Exception e) {
            log.warn("Error getting IP. {}", channel.remoteAddress());
        }
        return null;
    }

}
