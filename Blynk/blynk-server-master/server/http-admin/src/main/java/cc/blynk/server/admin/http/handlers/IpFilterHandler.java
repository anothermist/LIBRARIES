package cc.blynk.server.admin.http.handlers;

import cc.blynk.utils.ParseUtil;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.ipfilter.AbstractRemoteAddressFilter;
import io.netty.handler.ipfilter.IpFilterRuleType;
import io.netty.handler.ipfilter.IpSubnetFilterRule;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.Inet6Address;
import java.net.InetSocketAddress;
import java.util.HashSet;
import java.util.Set;

/**
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 16.12.15.
 */
@ChannelHandler.Sharable
public class IpFilterHandler extends AbstractRemoteAddressFilter<InetSocketAddress> {

    private static final Logger log = LogManager.getLogger(IpFilterHandler.class);

    private final Set<String> allowedIPs = new HashSet<>();
    private final Set<IpSubnetFilterRule> rules = new HashSet<>();

    public IpFilterHandler(String[] allowedIPs) {
        if (allowedIPs == null) {
            return;
        }
        for (String allowedIP : allowedIPs) {
            if (allowedIP.contains("/")) {
                String[] split = allowedIP.split("/");
                String ip = split[0];
                int cidr = ParseUtil.parseInt(split[1]);
                this.rules.add(new IpSubnetFilterRule(ip, cidr, IpFilterRuleType.ACCEPT));
            } else {
                this.allowedIPs.add(allowedIP);
            }
        }
    }

    @Override
    protected boolean accept(ChannelHandlerContext ctx, InetSocketAddress remoteAddress) throws Exception {
        if (allowedIPs.size() == 0 && rules.size() == 0) {
            return false;
        }

        if (allowedIPs.contains(remoteAddress.getAddress().getHostAddress())) {
            return true;
        }

        if (remoteAddress.getAddress() instanceof Inet6Address) {
            log.error("Look like you are trying to connect with IPv6 : {}.  While in 'allowed.administrator.ips' " +
                    "you are using IPv4.",
                    remoteAddress.getAddress().getHostAddress());
            return false;
        }

        for (IpSubnetFilterRule rule : rules) {
            if (rule.matches(remoteAddress)) {
                return true;
            }
        }

        return false;
    }
}
