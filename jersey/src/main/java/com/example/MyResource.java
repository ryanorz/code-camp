package com.example;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

/**
 * Created by shiy on 7/21/17.
 */
@Path("myresource")
@AccessLogger
public class MyResource {

    @GET
    @Produces(MediaType.TEXT_PLAIN)
    public String getResouce() {
        return "Got it!";
    }
}
