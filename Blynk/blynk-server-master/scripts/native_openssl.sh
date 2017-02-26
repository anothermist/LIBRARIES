#!/usr/bin/env bash
wget http://apache.ip-connect.vn.ua/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz
sudo tar -xzvf apache-maven-3.3.9-bin.tar.gz
export M2_HOME=/root/apache-maven-3.3.9
export M2=$M2_HOME/bin
export PATH=$M2:$PATH

sudo apt-get install git
sudo apt-get install autoconf automake libtool make tar libapr1-dev libssl-dev
git clone https://github.com/netty/netty-tcnative.git
cd netty-tcnative
git checkout netty-tcnative-parent-1.1.33.Fork26
~/apache-maven-3.3.9/bin/mvn clean install