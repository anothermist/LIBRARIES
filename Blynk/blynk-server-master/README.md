# What is Blynk?
Blynk is a platform with iOS and Android apps to control Arduino, ESP8266, Raspberry Pi and the likes over the Internet.  
You can easily build graphic interfaces for all your projects by simply dragging and dropping widgets.
If you need more information, please follow these links:
* [Blynk site](http://www.blynk.cc)
* [Blynk docs](http://docs.blynk.cc)
* [Blynk community](http://community.blynk.cc)
* [Facebook](http://www.fb.com/blynkapp)
* [Twitter](http://twitter.com/blynk_app)
* [App Store](https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8)
* [Google Play](https://play.google.com/store/apps/details?id=cc.blynk)
* [Blynk library](https://github.com/blynkkk/blynk-library)
* [Kickstarter](https://www.kickstarter.com/projects/167134865/blynk-build-an-app-for-your-arduino-project-in-5-m/description)

![Dashboard settings](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/dash_settings.png)
![Widgets Box](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/widgets_box.png)
![Button edit](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/button_edit.png)
![terminal edit](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/terminal_edit.png)
![Dashboard](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/dash.png)
![Dashboard2](https://github.com/blynkkk/blynk-server/blob/master/docs/overview/dash2.png)

# Blynk server
Blynk Server is an Open-Source [Netty](https://github.com/netty/netty) based Java server, responsible for forwarding messages between Blynk mobile application and various microcontroller boards (i.e. Arduino, Raspberry Pi. etc).
**Download latest server build [here](https://github.com/blynkkk/blynk-server/releases).**

[![GitHub version](https://img.shields.io/github/release/blynkkk/blynk-server.svg)](https://github.com/blynkkk/blynk-server/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/blynkkk/blynk-server/total.svg)](https://github.com/blynkkk/blynk-server/releases/latest)
[ ![Build Status](https://travis-ci.org/blynkkk/blynk-server.svg?branch=master)](https://travis-ci.org/blynkkk/blynk-server)

# GETTING STARTED

## Requirements
Java 8 required. (OpenJDK, Oracle). 

Ubuntu installation instructions [here](https://github.com/blynkkk/blynk-server#install-java-for-ubuntu).

For Windows just download from [here](http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html) and install. 

## Quick local server setup

+ Make sure you are using Java 8

        java -version
        Output: java version "1.8.0_40"

+ Run the server on default 'hardware port 8442' and default 'application port 8443' (SSL port)

        java -jar server-0.22.3.jar -dataFolder /path
        
That's it! 

**NOTE : ```/path``` should be real existing path to folder where you want to store all your data.**

+ As output you will see something like that :

        Blynk Server successfully started.
        All server output is stored in current folder in 'logs/blynk.log' file.

## Quick local server setup on Raspberry PI

+ Login to Raspberry Pi via ssh;
+ Install java 8 : 
        
        sudo apt-get install oracle-java8-jdk
        
+ Make sure you are using Java 8

        java -version
        Output: java version "1.8.0_40"
        
+ Download Blynk server jar file (or manually copy it to raspberry via ssh and scp command) : 
   
        wget "https://github.com/blynkkk/blynk-server/releases/download/v0.22.3/server-0.22.3.jar"

+ Run the server on default 'hardware port 8442' and default 'application port 8443' (SSL port)

        java -jar server-0.22.3.jar -dataFolder /home/pi/Blynk        
        
That's it! 

+ As output you will see something like that :

        Blynk Server successfully started.
        All server output is stored in current folder in 'logs/blynk.log' file.
       
## Enabling server auto restart on unix-like systems
        
+ To enable server auto restart find /etc/init.d/rc.local file and add :

        java -jar /home/pi/server-0.22.3.jar -dataFolder /home/pi/Blynk &
        
+ Or in case above approach doesn't work for you, execute 
       
        crontab -e

add the following line

        @reboot java -jar /home/pi/server-0.22.3.jar -dataFolder /home/pi/Blynk &
        
save and exit.

## Enabling server auto restart on Windows

+ Create bat file :

        start-blynk.bat

+ Put in it one line : 

        java -jar server-0.22.3.jar -dataFolder /home/pi/Blynk
        
+ Put bat file to windows startup folder

You can also use [this](https://github.com/blynkkk/blynk-server/tree/master/scripts/win) ready script to run server.

## Update instruction for unix-like systems

Server update should always go before app update. In order to update your server with new version you need to kill old process and start new one.

+ Find process id of Blynk server

        ps -aux | grep java
        
+ You will see something like that
 
        username   10539  1.0 12.1 3325808 428948 pts/76 Sl   Jan22   9:11 java -jar server-0.22.3.jar   
        
+ Kill old process

        kill 10539
        
10539 - blynk server process id from command output above.
 
+ Start new server [as usual](https://github.com/blynkkk/blynk-server#quick-local-server-setup)

After this steps you are ready to update app. Server version downgrade is not supported. Please **do not** revert your server 
to lower versions. Tou may loss data in that case.

## Update instruction for Windows

+ Open Task Manager;

+ Find Java process;

+ Stop process;

+ Start new server [as usual](https://github.com/blynkkk/blynk-server#quick-local-server-setup)
                
## App and sketch changes

+ Specify custom server path in your application

![Custom server icon](https://github.com/blynkkk/blynk-server/blob/master/docs/login.png)
![Server properties menu](https://github.com/blynkkk/blynk-server/blob/master/docs/custom.png)

+ Change your ethernet sketch from

    ```
    Blynk.begin(auth);
    ```
    
    to
    
    ```
    Blynk.begin(auth, "your_host");
    ```
    
    or to
    
    ```
    Blynk.begin(auth, IPAddress(xxx,xxx,xxx,xxx));
    ```
        
+ Change your WIFI sketch from
        
    ```
    Blynk.begin(auth, SSID, pass));
    ```
   
    to
    
    ```
    Blynk.begin(auth, SSID, pass, "your_host");
    ```
    
    or to
    
    ```
    Blynk.begin(auth, SSID, pass, IPAddress(XXX,XXX,XXX,XXX));
    ```
        
+ Change your rasp PI javascript from

    ```
    var blynk = new Blynk.Blynk(AUTH, options = {connector : new Blynk.TcpClient()});
    ```
    
    to
    
    ```
    var blynk = new Blynk.Blynk(AUTH, options= {addr:"xxx.xxx.xxx.xxx"});
    ```
        
+ or in case of USB when running blynk-ser.sh provide '-s' option with address of your local server

        ./blynk-ser.sh -s you_host_or_IP
        
        
WARNING : Blynk is growing very quickly. And both apps and server constantly updated. In order to avoid problems during 
updates either turn off auto-update for Blynk app either update both local server and blynk app at same time to avoid possible
migration issues.

WARNING : Blynk local server is not Blynk cloud server. They are not related at all. So you have to create new account when using 
Blynk local server.

## Advanced local server setup
For more flexibility you can extend server with more options by creating ```server.properties``` file in same folder as ```server.jar```. 
Example could be found [here](https://github.com/blynkkk/blynk-server/blob/master/server/core/src/main/resources/server.properties).
You could also specify any path to ```server.properties``` file via command line argument ```-serverConfig```. You can 
do the same with ```mail.properties``` via ```-mailConfig``` and ```sms.properties``` via ```-smsConfig```.
 
For example :

    java -jar server-0.22.3.jar -dataFolder /home/pi/Blynk -serverConfig /home/pi/someFolder/server.properties

Possible server options:

+ Application mutual ssl/tls port

        app.ssl.port=8443
        
        
+ Hardware plain tcp/ip port

        hardware.default.port=8442
        

+ Hardware ssl/tls port (for hardware that supports SSL/TLS sockets)

        hardware.ssl.port=8441
        
        
+ For simplicity Blynk already provides server jar with build-in SSL certificates, so you have working server out of the box via SSL/TLS sockets. But as certificate and it's private key are in public this is totally not secure. So in order to fix that you need to provide your own certificates. And change below properties with path to your cert. and private key and it's password. See how to generate self-signed certificates [here](https://github.com/blynkkk/blynk-server#generate-ssl-certificates)

        #points to cert and key that placed in same folder as running jar.
        
        server.ssl.cert=./server_embedded.crt
        server.ssl.key=./server_embedded.pem
        server.ssl.key.pass=pupkin123
        

+ Web socket ssl/tls port
        
        ssl.websocket.port=8081
      
        
+ Web sockets plain tcp/ip port
        
        tcp.websocket.port=8082
        
        
+ Https port
        
        https.port=9443
        
        
+ Http port
        
        http.port=8080
        
        
+ User profiles folder. Folder in which all users profiles will be stored. By default System.getProperty("java.io.tmpdir")/blynk used. Will be created if not exists

        data.folder=/tmp/blynk
        

+ Folder for all application logs. Will be created if it doesn't exist. "." is dir from which you are running script.

        logs.folder=./logs
        

+ Log debug level. Possible values: trace|debug|info|error. Defines how precise logging will be. From left to right -> maximum logging to minimum

        log.level=trace
        

+ Maximum allowed number of user dashboards.

        user.dashboard.max.limit=100
        

+ 100 Req/sec rate limit per user. You also may want to extend this limit on [hardware side](https://github.com/blynkkk/blynk-library/blob/f4e132652906d63d683abeed89f5d6ebe369e37a/Blynk/BlynkConfig.h#L42).

        user.message.quota.limit=100
        

+ this setting defines how often you can send mail/tweet/push or any other notification. Specified in seconds
        
        notifications.frequency.user.quota.limit=60
        

+ Maximum allowed user profile size. In Kb's.

        user.profile.max.size=128
        
        
+ Number of strings to store in terminal widget (terminal history data)

        terminal.strings.pool.size=25
        

+ Maximum allowed number of notification queue. Queue responsible for processing email, pushes, twits sending. Because of performance issue - those queue is processed in separate thread, this is required due to blocking nature of all above operations. Usually limit shouldn't be reached
        
        notifications.queue.limit=10000
        
        
+ Number of threads for performing blocking operations - push, twits, emails, db queries. Recommended to hold this value low unless you have to perform a lot of blocking operations.

        blocking.processor.thread.pool.limit=5
        

+ Period for flushing all user DB to disk. In millis

        profile.save.worker.period=60000
        

+ Specifies maximum period of time when application socket could be idle. After which socket will be closed due to non activity. In seconds. Leave it empty for infinity timeout

        app.socket.idle.timeout=600
        

+ Specifies maximum period of time when hardware socket could be idle. After which socket will be closed due to non activity. In seconds. Leave it empty for infinity timeout

        hard.socket.idle.timeout=15
        
        
+ Mostly required for local servers setup in case user want to log raw data in CSV format. See [raw data] (https://github.com/blynkkk/blynk-server#raw-data-storage) section for more info.
        
        enable.raw.data.store=true
        
        
+ Administration UI https port
        
        administration.https.port=7443
        
        
+ Url for opening admin page. Must start from "/". For "/admin" url path will look like that "https://127.0.0.1:7443/admin". 

        admin.rootPath=/admin
        
        
+ Comma separated list of administrator IPs. Allow access to admin UI only for those IPs. You may set it for 0.0.0.0/0 to allow access for all. You may use CIDR notation. For instance, 192.168.0.53/24.
        
        allowed.administrator.ips=127.0.0.1
        

+ Host for reset password redirect. By default current server IP is taken from "eth" network interface. Could be replaced with more friendly hostname. It is recommended to override this property with your server IP to avoid possible problems of host resolving.
        
        reset-pass.http.host=blynk-cloud.com
        
        
+ Comma separated list of users allowed to create accounts. Leave it empty if no restriction required.
        
        allowed.users.list=allowed1@gmail.com,allowed2@gmail.com
        
## Administration UI

Blynk server also has administration panel where you could monitor your server. It could be accessible with URL.

        https://your_ip:7443/admin
        
![Administration UI](https://github.com/blynkkk/blynk-server/blob/master/docs/admin_panel.png)
        
You can change it with next options :
        
        admin.rootPath
        allowed.administrator.ips
        administration.https.port
        
WARNING : default ```allowed.administrator.ips``` setting allows access only from ```localhost```. In other words 
administration will not be available from any other machine except those one you are running server on.
        
## HTTP/S RESTful
Blynk HTTP/S RESTful API allows to easily read and write values to/from Pins in Blynk apps and Hardware. 
Http API description could be found [here](http://docs.blynkapi.apiary.io).
        
### Enabling mail on Local server
In order to enable mail notifications on Local server you need to provide own mail credentials. To do that you need to create file ```mail.properties``` within same folder where server.jar is.
Mail properties :

        mail.smtp.auth=true
        mail.smtp.starttls.enable=true
        mail.smtp.host=smtp.gmail.com
        mail.smtp.port=587
        mail.smtp.username=YOUR_EMAIL_HERE
        mail.smtp.password=YOUR_EMAIL_PASS_HERE
        
See example [here](https://github.com/blynkkk/blynk-server/blob/master/server/notifications/email/src/main/resources/mail.properties).

NOTE : you'll need to setup Gmail to allow less secured applications. Go [here](https://www.google.com/settings/security/lesssecureapps) and then click "Allow less secure apps".


### Enabling sms on local server
In order to enable sms notifications on Local Server you need to provide credentials for sms gateway (for now Blynk server
supports only 1 provider - [Nexmo] (https://www.nexmo.com/)). So to do that you need to create file ```sms.properties``` 
within same folder where server.jar is.

        nexmo.api.key=
        nexmo.api.secret=
        
And fill above properties with credentials you'll get from Nexmo. (Account -> Settings -> API settings).
You also can send sms via email in case your cell provider supports that. See [discussion](http://community.blynk.cc/t/sms-notification-for-important-alert/2542) for more details.
        

### Raw data storage
By default raw data storage is enabled. So any write (Blynk.virtualWrite) command will stored on disk. 
The default path is "data" folder within [data.folder] (https://github.com/blynkkk/blynk-server#advanced-local-server-setup) property of server properties.

File name format is 
        
        dashBoardId_pin.csv

For instance
 
        data/1_v5.csv
        
Which means in 1_v5.csv file stored raw data for virtual pin 5 of dashboard with id 1.

Data format is

        value,timestamp
        
For instance

        10,1438022081332
        
Where 10 - value of pin, and 1438022081332 - the difference, measured in milliseconds, between the current time and midnight, January 1, 1970 UTC.
To display the date/time in excel you may use formula:

        =((COLUMN/(60*60*24)/1000+25569))

Raw data files are rotated every day and gzipped.

WARNING : this will changed in near future. 

### Generate Let's Encrypt SSL/TLS Certificates

+ First install [certbot] (https://github.com/certbot/certbot) on your server (machine where you gonna run Blynk Server)

        wget https://dl.eff.org/certbot-auto
        chmod a+x certbot-auto
        
+ Generate and verify certificates (your server should be connected to internet and have open 80/443 ports)

        ./certbot-auto certonly --agree-tos --email YOUR_EMAIL --standalone -d YOUR_HOST

For example 

        ./certbot-auto certonly --agree-tos --email pupkin@blynk.cc --standalone -d blynk.cc

+ Add password to your let's encrypt certificate and reformat it:

        mkdir /srv/blynk-data
        cp /etc/letsencrypt/live/YOUR_HOST/fullchain.pem /srv/blynk-data
        cp /etc/letsencrypt/live/YOUR_HOST/privkey.pem /srv/blynk-data
        cd /srv/blynk-data
        openssl pkcs8 -topk8 -inform PEM -outform PEM -in privkey.pem -out privkey_pass.pem

+ Then add to your ```server.properties``` file (in folder with server.jar)

        server.ssl.cert=/srv/blynk-data/fullchain.pem
        server.ssl.key=/srv/blynk-data/privkey_pass.pem
        server.ssl.key.pass=your_password
        
        https.cert=/srv/blynk-data/fullchain.pem
        https.key=/srv/blynk-data/privkey_pass.pem
        https.key.pass=our_password

### Generate own SSL certificates

+ Generate self-signed certificate and key

        openssl req -x509 -nodes -days 1825 -newkey rsa:2048 -keyout server.key -out server.crt
        
+ Convert server.key to PKCS#8 private key file in PEM format

        openssl pkcs8 -topk8 -inform PEM -outform PEM -in server.key -out server.pem
        
WARNING : you should have password for certificate. Certificates without pass are not accepted. 
In case you connect hardware via [USB script](https://github.com/blynkkk/blynk-library/tree/master/scripts) you have to provide an option '-s' pointing to "common name" (hostname) you did specified during certificate generation.
        
As output you'll retrieve server.crt and server.pem files that you need to provide for server.ssl properties.

### Install java for Ubuntu

        sudo apt-add-repository ppa:webupd8team/java
        sudo apt-get update
        sudo apt-get install oracle-java8-installer
        
### Port forwarding for HTTP/S API

        sudo iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-port 8080
        sudo iptables -t nat -A PREROUTING -p tcp --dport 443 -j REDIRECT --to-port 9443

### Enabling QR generation on server
        
        sudo apt-get install libxrender1
        
Add JVM param during server launch
        
        -Djava.awt.headless=true

### Behind wifi router
If you want to run Blynk server behind WiFi-router and want it to be accessible from the Internet, you have to add port-forwarding rule on your router. This is required in order to forward all of the requests that come to the router within the local network to Blynk server.

### How to build
Blynk has a bunch of integration tests that require DB, so you have to skip tests during build.

        mvn clean install -Dmaven.test.skip=true
        
### Minimal requirements
Blynk server is very powerful it can handle 1000 devices on single raspberry PI.

- Any OS that can run java
- At least 30 MB of RAM (could be less with tuning)

## Licensing
[GNU GPL license](https://github.com/blynkkk/blynk-server/blob/master/license.txt)
