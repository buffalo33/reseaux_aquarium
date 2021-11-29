package client;

import java.io.*;
import java.net.*;
import client.Objectif;
import client.Dimensions;
import java.util.ArrayList;
import java.lang.Math;

public class Poisson{
   public String name;
   public Dimensions coordinates;
   public Dimensions size;
   public ArrayList<Objectif> objectifs;

   public Poisson(String name, Dimensions coord, Dimensions size, Objectif obj){
     this.name = name;
     this.coordinates = coord;
     this.objectifs = new ArrayList<Objectif>();
     this.objectifs.add(obj);
     this.size = size;
   }

  public void updateState(int timeReducer){
    if (this.objectifs.isEmpty()){
      return;
    }
    Objectif obj = this.objectifs.get(0);
    if (obj.getTime() <= 0){
      this.coordinates = obj.getCoord();
      this.objectifs.remove(0);
      return;
    }

    int validTime = timeReducer;
    if (validTime > obj.getTime()){
      validTime = obj.getTime();
    }
    int wayPointX = ((obj.getCoord().getx() - this.coordinates.getx())/(obj.getTime())) * timeReducer + this.coordinates.getx();
    int wayPointY = ((obj.getCoord().gety() - this.coordinates.gety())/(obj.getTime())) * timeReducer + this.coordinates.gety();
    this.coordinates = new Dimensions(wayPointX,wayPointY);

    obj.setTime(obj.getTime() - timeReducer);
    if (obj.getTime() <= 0){
      this.objectifs.remove(0);
    }
  }

  public void updateWaiting(int timeReducer){
    if (this.objectifs.isEmpty()){
      return;
    }
    Objectif obj = this.objectifs.get(0);
     obj.setTime(obj.getTime() - timeReducer);
     //System.out.println(obj.getTime());
     if (obj.getTime() <= 0){
        this.coordinates = obj.getCoord();
        this.objectifs.remove(0);
      }
  }

  public void updateTime(int timeReducer){
     Objectif obj = this.objectifs.get(0);
     obj.setTime(obj.getTime() - timeReducer);
     /*for (Objectif obj: this.objectifs){
       obj.setTime(obj.getTime() - timeReducer);
     }*/
   }

  public void updateCoord(){
    //System.out.println(this.name);
    //System.out.println(this.coordinates.toString());

    Objectif obj = this.objectifs.get(0);
    if (obj.getTime() <= 0){
        this.coordinates = obj.getCoord();
        this.objectifs.remove(0);
      }

    /*int[] toRemove = new int[this.objectifs.size()];
    for (int i = 0; i < toRemove.length; i++){
      toRemove[i] = -1;
    }


    int idx = 0;
    for (Objectif obj: this.objectifs){
      if (obj.getTime() <= 0){
        this.coordinates = obj.getCoord();
        toRemove[idx] = idx;
      }
      idx = idx + 1;
    }
    for (int i = 0; i < toRemove.length; i++){
      if (toRemove[i] >= 0){
        this.objectifs.remove(toRemove[i]);
        for (int j = 0; j < toRemove.length; j++){
          toRemove[j]--;
        }
      }
    }*/
    //System.out.println(this.coordinates.toString());
  }

   public String getName(){
    return new String(this.name);
   }

   public Dimensions getCoord(){
    return new Dimensions(this.coordinates);
   }

   public Dimensions getSize(){
     return new Dimensions(this.size);
   }

   public void setName(String myName){
     this.name = myName;
   }

   public void setCoord(Dimensions myCoordinates){
     this.coordinates = myCoordinates;
   }

   public void setSize(Dimensions mySize){
     this.size = mySize;
   }


}
