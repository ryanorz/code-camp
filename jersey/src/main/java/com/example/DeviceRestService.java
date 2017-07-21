package com.example;

import javax.ws.rs.*;
import javax.ws.rs.core.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by shiy on 7/21/17.
 */
@Path("/device")
public class DeviceRestService {

    @Context
    UriInfo uriInfo;

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public List<Device> fetchAll(
            @QueryParam("page") @DefaultValue("0") int page,
            @QueryParam("size") @DefaultValue("30") int size
    ) {
        System.out.println("> Get list of devices, page = " + page + ", size = " + size);
        List<Device> devices = new ArrayList<>();

        Device d1 = new Device();
        d1.setId("0001");
        d1.setOs("Android 5.0");
        d1.setBrand("Samsung");
        devices.add(d1);

        Device d2 = new Device();
        d2.setId("0002");
        d2.setOs("Android 6.0");
        d2.setBrand("Huawei");
        devices.add(d2);

        return devices;
    }

    @Path("/{id: [0-9]+}")
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response fetchBy(@PathParam("id") String id) {
        Device d1 = new Device();
        d1.setId("0001");
        d1.setOs("Android 5.0");
        d1.setBrand("Samsung");

        Device d2 = new Device();
        d2.setId("0002");
        d2.setOs("Android 6.0");
        d2.setBrand("Huawei");

        if (id.equals("0001")) {
            return Response.ok(d1).build();
        } else if (id.equals("0002")) {
            return Response.ok(d2).build();
        } else {
            return Response.status(Response.Status.NOT_FOUND).entity("device not found for id: " + id).build();
        }
    }

    @Path("/{id: [0-9]+}")
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    public Response create(Device device) {
        System.out.println("> Create device:");
        device.print();
        return Response.ok().build();
    }

    @Path("/{id: [0-9]+}")
    @PUT
    @Consumes(MediaType.APPLICATION_JSON)
    public Response update(Device device) {
        System.out.println("> Update device:");
        device.print();
        return Response.ok().build();
    }

    @Path("/{id: [0-9]+}")
    @DELETE
    public Response delete(@PathParam("id") String id) {
        System.out.println("> Delete device id: " + id);
        if (id.equals("0001") || id.equals("0002")) {
            return Response.ok().build();
        } else {
            return Response.status(Response.Status.NOT_FOUND).build();
        }
    }

}
