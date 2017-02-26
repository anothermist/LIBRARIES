package cc.blynk.server;

import cc.blynk.server.core.BlockingIOProcessor;
import cc.blynk.server.core.dao.FileManager;
import cc.blynk.server.core.dao.ReportingDao;
import cc.blynk.server.core.dao.SessionDao;
import cc.blynk.server.core.dao.TokenManager;
import cc.blynk.server.core.dao.UserDao;
import cc.blynk.server.core.processors.EventorProcessor;
import cc.blynk.server.core.stats.GlobalStats;
import cc.blynk.server.db.DBManager;
import cc.blynk.server.notifications.mail.MailWrapper;
import cc.blynk.server.notifications.push.GCMWrapper;
import cc.blynk.server.notifications.sms.SMSWrapper;
import cc.blynk.server.notifications.twitter.TwitterWrapper;
import cc.blynk.server.redis.RedisClient;
import cc.blynk.server.transport.TransportTypeHolder;
import cc.blynk.server.workers.ReadingWidgetsWorker;
import cc.blynk.server.workers.timer.TimerWorker;
import cc.blynk.utils.FileLoaderUtil;
import cc.blynk.utils.IPUtils;
import cc.blynk.utils.ServerProperties;
import org.asynchttpclient.DefaultAsyncHttpClient;
import org.asynchttpclient.DefaultAsyncHttpClientConfig;

import java.io.Closeable;

import static cc.blynk.utils.ReportingUtil.getReportingFolder;

/**
 * Just a holder for all necessary objects for server instance creation.
 *
 * The Blynk Project.
 * Created by Dmitriy Dumanskiy.
 * Created on 28.09.15.
 */
public class Holder implements Closeable {

    public final FileManager fileManager;

    public final SessionDao sessionDao;

    public final UserDao userDao;

    public final TokenManager tokenManager;

    public final ReportingDao reportingDao;

    public final RedisClient redisClient;

    public final DBManager dbManager;

    public final GlobalStats stats;

    public final ServerProperties props;

    public final BlockingIOProcessor blockingIOProcessor;
    public final TransportTypeHolder transportTypeHolder;
    public final TwitterWrapper twitterWrapper;
    public final MailWrapper mailWrapper;
    public final GCMWrapper gcmWrapper;
    public final SMSWrapper smsWrapper;
    public final String region;
    public final TimerWorker timerWorker;
    public final ReadingWidgetsWorker readingWidgetsWorker;

    public final EventorProcessor eventorProcessor;
    public final DefaultAsyncHttpClient asyncHttpClient;

    public final Limits limits;

    public final String currentIp;

    public Holder(ServerProperties serverProperties, ServerProperties mailProperties,
                  ServerProperties smsProperties, ServerProperties gcmProperties) {
        this.props = serverProperties;

        this.region = serverProperties.getProperty("region", "local");
        String netInterface = serverProperties.getProperty("net.interface", "eth");
        this.currentIp = serverProperties.getProperty("reset-pass.http.host", IPUtils.resolveHostIP(netInterface));

        this.redisClient = new RedisClient(new ServerProperties(RedisClient.REDIS_PROPERTIES), region);

        String dataFolder = serverProperties.getProperty("data.folder");
        this.fileManager = new FileManager(dataFolder);
        this.sessionDao = new SessionDao();
        this.userDao = new UserDao(fileManager.deserialize(), this.region);
        this.blockingIOProcessor = new BlockingIOProcessor(
                serverProperties.getIntProperty("blocking.processor.thread.pool.limit", 5),
                serverProperties.getIntProperty("notifications.queue.limit", 10000),
                FileLoaderUtil.readFileAsString(BlockingIOProcessor.TOKEN_MAIL_BODY)
        );
        this.tokenManager = new TokenManager(this.userDao.users, blockingIOProcessor, redisClient, currentIp);
        this.stats = new GlobalStats();
        final String reportingFolder = getReportingFolder(dataFolder);
        this.reportingDao = new ReportingDao(reportingFolder, serverProperties);

        this.transportTypeHolder = new TransportTypeHolder(serverProperties);

        this.asyncHttpClient = new DefaultAsyncHttpClient(new DefaultAsyncHttpClientConfig.Builder()
                .setUserAgent(null)
                .setEventLoopGroup(transportTypeHolder.workerGroup)
                .setKeepAlive(true)
                .build()
        );

        this.twitterWrapper = new TwitterWrapper();
        this.mailWrapper = new MailWrapper(mailProperties);
        this.gcmWrapper = new GCMWrapper(gcmProperties, asyncHttpClient);
        this.smsWrapper = new SMSWrapper(smsProperties, asyncHttpClient);

        this.eventorProcessor = new EventorProcessor(gcmWrapper, twitterWrapper, blockingIOProcessor, stats);
        this.dbManager = new DBManager(blockingIOProcessor);
        this.timerWorker = new TimerWorker(userDao, sessionDao, gcmWrapper);
        this.readingWidgetsWorker = new ReadingWidgetsWorker(sessionDao, userDao);
        this.limits = new Limits(props);
    }

    //for tests only
    public Holder(ServerProperties serverProperties, TwitterWrapper twitterWrapper, MailWrapper mailWrapper, GCMWrapper gcmWrapper, SMSWrapper smsWrapper) {
        this.props = serverProperties;

        this.region = "local";
        String netInterface = serverProperties.getProperty("net.interface", "eth");
        this.currentIp = serverProperties.getProperty("reset-pass.http.host", IPUtils.resolveHostIP(netInterface));
        this.redisClient = new RedisClient(new ServerProperties(RedisClient.REDIS_PROPERTIES), "real");

        String dataFolder = serverProperties.getProperty("data.folder");
        this.fileManager = new FileManager(dataFolder);
        this.sessionDao = new SessionDao();
        this.userDao = new UserDao(fileManager.deserialize(), this.region);
        this.blockingIOProcessor = new BlockingIOProcessor(
                serverProperties.getIntProperty("blocking.processor.thread.pool.limit", 5),
                serverProperties.getIntProperty("notifications.queue.limit", 10000),
                FileLoaderUtil.readFileAsString(BlockingIOProcessor.TOKEN_MAIL_BODY)
        );
        this.tokenManager = new TokenManager(this.userDao.users, blockingIOProcessor, redisClient, currentIp);
        this.stats = new GlobalStats();
        final String reportingFolder = getReportingFolder(dataFolder);
        this.reportingDao = new ReportingDao(reportingFolder, serverProperties);

        this.transportTypeHolder = new TransportTypeHolder(serverProperties);

        this.twitterWrapper = twitterWrapper;
        this.mailWrapper = mailWrapper;
        this.gcmWrapper = gcmWrapper;
        this.smsWrapper = smsWrapper;

        this.eventorProcessor = new EventorProcessor(gcmWrapper, twitterWrapper, blockingIOProcessor, stats);
        this.asyncHttpClient = new DefaultAsyncHttpClient(new DefaultAsyncHttpClientConfig.Builder()
                .setUserAgent(null)
                .setEventLoopGroup(transportTypeHolder.workerGroup)
                .setKeepAlive(false)
                .build()
        );

        this.dbManager = new DBManager(blockingIOProcessor);
        this.timerWorker = new TimerWorker(userDao, sessionDao, gcmWrapper);
        this.readingWidgetsWorker = new ReadingWidgetsWorker(sessionDao, userDao);
        this.limits = new Limits(props);
    }

    @Override
    public void close() {
        this.reportingDao.close();

        System.out.println("Stopping BlockingIOProcessor...");
        this.blockingIOProcessor.close();

        System.out.println("Stopping DBManager...");
        this.dbManager.close();

        System.out.println("Stopping Transport Holder...");
        transportTypeHolder.close();

        redisClient.close();
    }
}
