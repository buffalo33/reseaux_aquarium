package client;

//import java.io.*;
//import java.net.*;

public class Dimensions{
    private int x;
    private int y;

    public Dimensions(int x, int y){
        this.x = x;
        this.y = y;
    }

    public Dimensions(Dimensions dim){
        //new Dimensions(dim.getx(),dim.gety());
        //return copy;
        this.x = dim.getx();
        this.y = dim.gety();
    }

    public int getx(){
        return this.x;
    }

    public int gety(){
        return this.y;
    }

    public String toString(){
        String state = "x: " + this.x + "\ny: " + this.y;
        return state;
    }
}
