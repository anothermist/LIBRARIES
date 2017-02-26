#!/usr/bin/env bash
sudo iptables -A INPUT -i lo -j ACCEPT
sudo iptables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 22 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 443 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 8443 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 8442 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 8441 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 8777 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 587 -j ACCEPT
sudo iptables -A INPUT -j DROP