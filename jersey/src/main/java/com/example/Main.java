package com.example;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.glassfish.grizzly.http.server.HttpServer;
import org.glassfish.jersey.grizzly2.httpserver.GrizzlyHttpServerFactory;
import org.glassfish.jersey.server.ResourceConfig;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.URI;
import java.util.Properties;

/**
 * Main class.
 *
 */
public class Main {
    // Base URI the Grizzly HTTP server will listen on
    public static final String BASE_URI = "http://localhost:8080/myapp/";

    private static final Logger log = LogManager.getLogger(Main.class.getName());

    /**
     * Starts Grizzly HTTP server exposing JAX-RS resources defined in this application.
     * @return Grizzly HTTP server.
     */
    public static HttpServer startServer() {
        // create a resource config that scans for JAX-RS resources and providers
        // in com.suninfo package
        final ResourceConfig rc = new ResourceConfig().packages("com.suninfo");

        // create and start a new instance of grizzly http server
        // exposing the Jersey application at BASE_URI
        return GrizzlyHttpServerFactory.createHttpServer(URI.create(BASE_URI), rc);
    }

    /**
     * Main method.
     * @param args
     * @throws IOException
     */
    public static void main(String[] args) throws IOException, InterruptedException {

        String log4jConfiguration = System.getProperty("log4j.configuration");
        PropertyConfigurator.configure(log4jConfiguration);

        String configRoot = System.getProperty("configRoot");
        MessageProducer mp = MessageProducer.getInstance();
        mp.init(configRoot + "/kafka.properties");

        final HttpServer server = startServer();
        log.info(String.format("Jersey app started with WADL available at %sapplication.wadl", BASE_URI));

        Runtime.getRuntime().addShutdownHook( new Thread(() -> {
            log.info("Shutting down Process.");
            server.shutdown();
            mp.close();
            log.info("Process stopped.");
        }));

        while (true) {
            Thread.sleep(1000);
        }
    }
}

