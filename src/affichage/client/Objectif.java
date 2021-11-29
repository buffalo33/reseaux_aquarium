package client;

//import java.io.*;
//import java.net.*;
//import client.Dimensions;

public class Objectif{
    private Dimensions coordinates;
    private int time;

    public Objectif(Dimensions dim, int t){
        this.coordinates = dim;
        this.time = t;
    }

    public Objectif(Objectif obj){
        this.coordinates = obj.getCoord();
        this.time = obj.getTime();
    }

    public Dimensions getCoord(){
        return new Dimensions(this.coordinates);
    }

    public int getTime(){
        return this.time;
    }

    public void setCoord(Dimensions myCoordinates){
        this.coordinates = myCoordinates;
    }

    public void setTime(int myTime){
        this.time = myTime;
    }

    public String toString(){
        String state = "objectif Coord: \n" + this.coordinates.toString() + "\nTime: " + this.time;
        return state;
    }
}
