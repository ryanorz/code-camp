package com.example;

/**
 * Created by shiy on 7/21/17.
 */
public class Device {
    private String id;
    private String os = "Android";
    private String brand = "None";

    public Device() {
    }

    public Device(String id) {
        this.id = id;
    }

    public Device(String id, String os, String brand) {
        this.id = id;
        this.os = os;
        this.brand = brand;
    }

    public boolean infoValidation() {
        return true;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getOs() {
        return os;
    }

    public void setOs(String os) {
        this.os = os;
    }

    public String getBrand() {
        return brand;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }

    public void print() {
        System.out.println("\tid: " + id);
        System.out.println("\tos: " + os);
        System.out.println("\tbd: " + brand);
    }
}
