
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Iterator;
import client.*;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.time.Clock;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import javafx.scene.layout.GridPane;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.image.Image ;
import javafx.scene.image.ImageView;
import javafx.application.Platform;
import java.lang.Thread;
import java.lang.Math;

public class Client extends Application{
  public ArrayList<Poisson> poissons;
  public ArrayList<Poisson> poissonsAttente;
  private Dimensions coordinates;
  private Dimensions dimensions;
  private Stage primary;

  public Client(){
	}

//Fonction recuperer un perametre du fichier de configuration '../affichage.cfg'
 String getConfig(String configName, String parameter){
    try {
      String relative_path = "../";
      BufferedReader reader = fileAccess(relative_path + configName);
      String str = "";
      ConfigElement conf = new ConfigElement(parameter);
      for (int i = 0; i < conf.getLine(); i++){
        str = reader.readLine();
      }
      //System.out.println(str);
      String info = str.substring(conf.getInterval()[0]);
      //System.out.println(info);
      return info;
    } catch (IOException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
      return "";
    }
  }

//Fonction ouvrire un buffer de lecture du fichier correspondant au nom entre.
  BufferedReader fileAccess(String name){
	  try {
      File myfile = new File(name);
	    FileReader myReader = new FileReader(myfile);
	    BufferedReader reader = new BufferedReader(myReader);
      return reader;
    } catch (FileNotFoundException e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
      return null;
    }
  }

// Focntion tenter d'etablir une connexion avec un serveur dont l'adresse ip est indiquee
// dans le fichieer de configuration.
	public Socket connexion(){
    try {
    String ipAddr = getConfig("affichage.cfg","addr");
    System.out.println(ipAddr);
    String[] ipStr = ipAddr.split("\\.");
    System.out.println(ipStr.length);
    int[] ipInt = new int[ipStr.length];
    byte[] ipByte = new byte[ipStr.length];
    for (int i = 0; i < ipStr.length; i++){
      ipByte[i] = (byte) Integer.parseInt(ipStr[i]);
      System.out.println(ipByte[i]);
    }
    InetAddress netAddr = InetAddress.getByAddress(ipByte);
    netAddr = InetAddress.getLocalHost();
    System.out.println(netAddr);

    Socket socket = new Socket(netAddr, 8080);
    System.out.println("SOCKET = " + socket);

    return socket;
    /*plec.close();
    pred.close();
    socket.close();*/
    } catch ( UnknownHostException e) {
      System.out.println("UnknownHostException");
      return null;
    } catch ( IOException e) {
      System.out.println("IOException");
      return null;
    }
  }

//Fonction mise a jour des poissons.
  void updateFishes(int timeReducer){
    int[] toRemove = new int[this.poissons.size()];
    for (int i = 0; i < toRemove.length; i++){
      toRemove[i] = -1;
    }

    int idx = 0;
    for (Poisson fish: this.poissons){
      //fish.updateTime(timeReducer);
       //System.out.println(fish.getName() + " time : " + fish.objectifs.get(fish.objectifs.size() - 1).getTime());
      //fish.updateCoord();
      fish.updateState(timeReducer);
      if ((fish.getCoord().getx() < 0 || fish.getCoord().getx() > 100) || (fish.getCoord().gety() < 0 || fish.getCoord().gety() > 100)){
        toRemove[idx] = idx;
      }
      idx++;
      // System.out.println(fish.getName() + " Coordonnées : \n"+ fish.getCoord().toString());
      // System.out.println("Objectifs : \n" + fish.objectifs.toString() + "\n");
      // System.out.println();
    }

    for (int i = 0; i < toRemove.length; i++){
      if (toRemove[i] >= 0){
        this.poissonsAttente.add(this.poissons.get(i));
        this.poissons.remove(toRemove[i]);
        for (int j = 0; j < toRemove.length; j++){
          toRemove[j]--;
        }
      }
    }

    toRemove = new int[this.poissonsAttente.size()];
    for (int i = 0; i < toRemove.length; i++){
      toRemove[i] = -1;
    }
    idx = 0;
    for (Poisson fish: this.poissonsAttente){
      fish.updateWaiting(timeReducer);
      // System.out.println(fish.getName() + " Coordonnées : \n"+ fish.getCoord().toString());
      // System.out.println("Objectifs : \n" + fish.objectifs.toString() + "\n");
      // System.out.println();
      if ((fish.getCoord().getx() >= 0 && fish.getCoord().getx() <= 100) && (fish.getCoord().gety() >= 0 && fish.getCoord().gety() <= 100)){
        this.poissons.add(fish);
        toRemove[idx] = idx;
      }
      if (fish.objectifs.isEmpty()){
        toRemove[idx] = idx;
      }
      idx++;
    }

    for (int i = 0; i < toRemove.length; i++){
      if (toRemove[i] >= 0){
        this.poissonsAttente.remove(toRemove[i]);
        for (int j = 0; j < toRemove.length; j++){
          toRemove[j]--;
        }
      }
    }

    Client client = this;
    Platform.runLater(new Runnable() {
            @Override public void run() {
                client.displayFishes(client.poissons);
            }
        });
  }

//Mis a jour des objectifs de deplacements des poissons.
  private void readGetFishes(String in){
    ArrayList<String> str = new ArrayList<String>();
    int beg = 0;
    try{
      for (int i = 0; i < in.length(); i++){
        if (in.charAt(i) == '[')
          beg = i;
        if (in.charAt(i) == ']')
          str.add(in.substring(beg+1, i));
      }
      for (int i = 0; i < str.size(); readFish(str.get(i++)));
    }
  catch (Exception e) {e.printStackTrace();}
}

//Mis a jour des objectifs d'un poissons, grace aux donnees indiquee dans le resultat de la requete getFishes etc..
  private void readFish(String in){
    int i = 0;
    for (; i < in.length(); i++){
      if (in.charAt(i) == ' ' && in.charAt(i+3) == ' '){
        i +=3;
        break;
      }
		}
    String name = in.substring(0, i-3);

		int x, y, h, w, t;

    if (in.charAt(i + 2) == 'x'){
      x = Integer.parseInt(in.substring(i+1, i+2));
      i += 2;
    }
    else{
      x = Integer.parseInt(in.substring(i+1, i+3));
      i += 3;
    }

    if (in.charAt(i + 2) == ','){
      y = Integer.parseInt(in.substring(i+1, i+2));
			i += 2;
    }
    else{
      y = Integer.parseInt(in.substring(i+1, i+3));
			i += 3;
    }


    if (in.charAt(i + 2) == 'x'){
      h = Integer.parseInt(in.substring(i+1, i+2));
      i += 2;
    }
    else{
      h = Integer.parseInt(in.substring(i+1, i+3));
      i += 3;
    }

    if (in.charAt(i + 2) == ','){
      w = Integer.parseInt(in.substring(i+1, i+2));
			i += 2;
    }
    else{
      w = Integer.parseInt(in.substring(i+1, i+3));
			i += 3;
    }
    t = Math.round(Float.parseFloat(in.substring(i+1, in.length())));

    boolean found = false;
    for(Poisson p: this.poissons){
      if (p.name.equals(name)){
        p.objectifs.add(new Objectif(new Dimensions(x, y), t));
        found = true;
        break;
      }
    }
    if (!found){
      this.poissonsAttente.add(new Poisson(name, new Dimensions(-1, -1), new Dimensions(h, w) /* à changer*/, new Objectif(new Dimensions(x, y), t)));
    }
  }


  public static void main(String args[]) throws IOException{
    launch(args);
  }

  @Override
  public void init() throws Exception {} //useless

  @Override
  public void start(Stage primaryStage) {
    poissons = new ArrayList<Poisson>();
    poissonsAttente = new ArrayList<Poisson>();
		try{

    //Test creation de poissons avec objsctifs.
    // Poisson p1 = new Poisson("p1",
    //                         new Dimensions(10, 10),
    //                         new Dimensions(5, 5),
    //                         new Objectif(new Dimensions(80, 80), 2));
    // //p1.objectifs.add(new Objectif(new Dimensions(60, 70), 2));
    // p1.objectifs.add(new Objectif(new Dimensions(-1, 70), 2));
    // p1.objectifs.add(new Objectif(new Dimensions(40, 70), 3));
    // this.poissons.add(p1);
    // Poisson p2 = new Poisson("p2",
    //                         new Dimensions(34, 44),
    //                         new Dimensions(7, 7),
    //                         new Objectif(new Dimensions(12, 34), 3));
    // p2.objectifs.add(new Objectif(new Dimensions(60, 70), 5));
    // p2.objectifs.add(new Objectif(new Dimensions(3, 4), 3));
    // this.poissons.add(p2);
    //this.updateFishes(3);

    //Connexion au serveur avec validation de l'execution par affichage de l'objet Socket.
    Socket socket = this.connexion();
    System.out.println("SOCKET = " + socket);

    //Buffer de ce qu'envoie le serveur
    BufferedReader sockRead = new BufferedReader(new InputStreamReader(socket.getInputStream()));

    //Entree ecriture de la socket
    PrintWriter sockWrite = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);

    //initialisation de la fenêtre
    primaryStage.setTitle("View");
    primary = primaryStage;
    GridPane grid = new GridPane();
    Scene scene = new Scene(grid, 1350, 900);
    primaryStage.setScene(scene);
    scene.getStylesheets().add(Client.class.getResource("View.css").toExternalForm());
    primaryStage.show();

		//code pour les instructions en ligne de commande
    // bufferreader pour le terminal
    BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

    //Initialisation echange entre cleint et serveur. (toujours necessaire ?)
    // sockWrite.println("hey");
    // System.out.println(sockRead.readLine());

    /*long timeForUpdate0 =  Clock.systemUTC().millis();
    // readGetFishes("list [PoissonRouge at 92x4,10x4,5] [PoissonClown at 22x80,12x6,3]", client);
    for (;;){
      long newTimeForUpdate = Clock.systemUTC().millis();
      if (newTimeForUpdate - timeForUpdate0 >= 1000){
        //System.out.println(newTimeForUpdate - timeForUpdate0);
        timeForUpdate0 = newTimeForUpdate;
        client.updateFishes(1);
      }
      String in = "";
      String sockIn = "";
      if (reader.ready()){
        in = reader.readLine();
        if (!in.equals("")){
          //System.out.println(in);
          sockWrite.println(in);
        }*/

    //A thread must run the infinite loop, otherwise javafx won't work
    new Thread(new Runnable (){
      private Client client;

      public Runnable init(Client client) {
        this.client = client;
        return this;
      }

      @Override
      public void run() {
        try{
          // ci-dessous: pour tester
          // client.poissons.add(new Poisson("test", new Dimensions(10, 10), new Dimensions(20, 20), new Objectif(new Dimensions(10, 10), 3)));

          long timeForUpdate0 =  Clock.systemUTC().millis();
          for(;;){
          //Calcul temps ecoule depuis derniere MAJ du temps
          long newTimeForUpdate = Clock.systemUTC().millis();
          if (newTimeForUpdate - timeForUpdate0 >= 1000){
            //MAJ de l'etat des poissons toutes les secondes.
            timeForUpdate0 = newTimeForUpdate;
            client.updateFishes(1);
          }
            String in = "";
            String sockIn = "";
            //On regarde s'il y a qqchose a lire sur entree standard, sinon on passe.
            if (reader.ready()){
              in = reader.readLine();
              if (!in.equals("")){
                //On entre ligne ecrite par client dans entree standard dans l'entree de la Socket.
                sockWrite.println(in);
              }
            }
            //On regarde s'il y a qqchose a lire depuis la socket, sinon on passe.
            if (sockRead.ready()){
              sockIn = sockRead.readLine();
              if (!sockIn.equals("")){
                //On affiche à l'ecran les commandes du serveur entree en Socket.
                System.out.println(sockIn);
                if (sockIn.length() >= 4 && sockIn.substring(0,4).equals("list")){
                  //Lecture des nouveaux objectifs des poissons indiques par serveur
                  client.readGetFishes(sockIn);
                }
                //sockWrite.println(sockIn);
              }
            }
          }
        } catch(Exception e) {e.printStackTrace();}
      }
    }.init(this)).start();

     }catch(Exception e){e.printStackTrace();}
  }


//Affichage des poissons.
  public void displayFishes(ArrayList<Poisson> pisces){

    Stage primaryStage = primary;
    primaryStage.setTitle("View");

    GridPane grid = new GridPane();
    grid.setAlignment(Pos.TOP_LEFT);
    grid.setHgap(6.75);
    grid.setVgap(4);
    grid.setPadding(new Insets(5, 5, 5, 5));

    //pour afficher des poissons différents
    ArrayList<String> img_bank = new ArrayList<String>();
    String img_path = "file:client/fishes/poisson_";
    img_bank.add(img_path + "balkany.png");
    img_bank.add(img_path + "davril.png");
    img_bank.add(img_path + "gdb.png");
    img_bank.add(img_path + "tortoise.png");
    img_bank.add(img_path + "clipart.png");


    ArrayList<Image> fishes = new ArrayList<Image>();
    ArrayList<ImageView> views = new ArrayList<ImageView>();
    for (int i = 0; i < pisces.size(); i++){
      if (pisces.get(i).coordinates.getx() > 0 && pisces.get(i).coordinates.gety() > 0 && pisces.get(i).coordinates.gety() < 100 && pisces.get(i).coordinates.getx() < 100){
          if (i < 5)
            fishes.add(new Image(img_bank.get(i)));
          else
            fishes.add(new Image(img_bank.get(0)));
          views.add(new ImageView(fishes.get(i)));
          views.get(i).setFitWidth(pisces.get(i).size.getx()*10);
          views.get(i).setFitHeight(pisces.get(i).size.gety()*10);
          grid.add(views.get(i), pisces.get(i).coordinates.getx(), pisces.get(i).coordinates.gety());
        }
    }


    /*long timeForUpdate0 =  Clock.systemUTC().millis();
    for(;;){
      long newTimeForUpdate = Clock.systemUTC().millis();
      if (newTimeForUpdate - timeForUpdate0 >= 1000){
        //System.out.println(newTimeForUpdate - timeForUpdate0);
        timeForUpdate0 = newTimeForUpdate;
        client.updateFishes(1);
      }
    }*/


    Scene scene = new Scene(grid, 1350, 900);
    primaryStage.setScene(scene);
    scene.getStylesheets().add(Client.class.getResource("View.css").toExternalForm());

    primaryStage.show();
  }

}
