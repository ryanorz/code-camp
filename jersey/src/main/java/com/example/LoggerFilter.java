package com.example;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import javax.ws.rs.container.ContainerRequestContext;
import javax.ws.rs.container.ContainerRequestFilter;
import javax.ws.rs.container.ContainerResponseContext;
import javax.ws.rs.container.ContainerResponseFilter;
import javax.ws.rs.ext.Provider;
import java.io.IOException;

@Provider
@AccessLogger
public class LoggerFilter implements ContainerRequestFilter, ContainerResponseFilter {

    private static Logger logger = LogManager.getLogger(LoggerFilter.class.getName());

    @Override
    public void filter(ContainerRequestContext containerRequestContext) throws IOException {
        logger.info("Log Request");
    }

    @Override
    public void filter(ContainerRequestContext containerRequestContext, ContainerResponseContext containerResponseContext) throws IOException {
        logger.info("Log Response");
        logger.info(String.format("%s %s %d %s",
                containerRequestContext.getMethod(),
                containerRequestContext.getUriInfo().getPath(),
                containerResponseContext.getStatus(),
                containerResponseContext.getStatusInfo().getReasonPhrase()));
    }
}
