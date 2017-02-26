#!/usr/bin/env bash

sudo apt-get update
sudo apt-get install fail2ban
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local

sudo apt-get install sendmail

sudo sh -c 'echo "deb http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" >> /etc/apt/sources.list.d/pgdg.list'
wget -q https://www.postgresql.org/media/keys/ACCC4CF8.asc -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install postgresql postgresql-contrib
sudo su - postgres

apt-get install ipset
ipset create blacklist hash:ip hashsize 4096
iptables -I INPUT -m set --match-set blacklist src -j DROP
iptables -I FORWARD -m set --match-set blacklist src -j DROP