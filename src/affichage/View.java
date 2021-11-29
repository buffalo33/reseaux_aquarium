import client.*;
import java.util.ArrayList;
import java.util.Iterator;
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

//Cette classe sert à tester l'affichage en javafx. Son affichage fonctionne comme celui de Client
public class View extends Application {
    static private Stage primary;

    public View(){
    }

    public static void main(String[] args) {
        launch(args);
    }

  public void displayFishes(ArrayList<Poisson> pisces){
    try {
      launch("");
    } catch (IllegalStateException e) {}

    Stage primaryStage = primary;
    primaryStage.setTitle("View");

    Button btn = new Button();
    btn.setText("Say 'BLOUP BLOUP'");
    btn.setOnAction(new EventHandler<ActionEvent>() {

        @Override
        public void handle(ActionEvent event) {
            System.out.println("BLOUP BLOUP");
        }
    });

    GridPane grid = new GridPane();
    grid.setAlignment(Pos.TOP_LEFT);
    grid.setHgap(6.75);
    grid.setVgap(4);
    grid.setPadding(new Insets(5, 5, 5, 5));
    grid.add(btn, 0, 0);

    ArrayList<String> bank = new ArrayList<String>();
    bank.add("file:client/fishes/poisson_balkany.png");
    bank.add("file:client/fishes/poisson_davril.png");
    bank.add("file:client/fishes/poisson_gdb.png");
    bank.add("file:client/fishes/poisson_tortoise.png");
    bank.add("file:client/fishes/poisson_clipart.png");

    System.out.println(pisces.size());

    ArrayList<Image> fishes = new ArrayList<Image>();
    ArrayList<ImageView> views = new ArrayList<ImageView>();
    for (int i = 0; i < pisces.size(); i++){
        fishes.add(new Image(bank.get(i)));
        views.add(new ImageView(fishes.get(i)));
        views.get(i).setFitWidth(pisces.get(i).size.getx()*10);
        views.get(i).setFitHeight(pisces.get(i).size.gety()*10);
        grid.add(views.get(i), pisces.get(i).coordinates.getx(), pisces.get(i).coordinates.gety());
    }

    Scene scene = new Scene(grid, 1350, 900);
    primaryStage.setScene(scene);
    scene.getStylesheets().add(View.class.getResource("View.css").toExternalForm());

    primaryStage.show();
  }

  @Override
  public void init() throws Exception {}

  @Override
  public void start(Stage primaryStage) {
      primaryStage.setTitle("View");
      primary = primaryStage;

      GridPane grid = new GridPane();

      Scene scene = new Scene(grid, 1350, 900);
      primaryStage.setScene(scene);
      scene.getStylesheets().add(View.class.getResource("View.css").toExternalForm());
      primaryStage.show();

      ArrayList<Poisson> p = new ArrayList<Poisson>();

      p.add(new Poisson("test", new Dimensions(10, 10), new Dimensions(10, 10), null));
      p.add(new Poisson("test", new Dimensions(50, 50), new Dimensions(10, 10), null));
      p.add(new Poisson("test", new Dimensions(100, 100), new Dimensions(10, 10), null));
      p.add(new Poisson("test", new Dimensions(80, 30), new Dimensions(20, 10), null));

      displayFishes(p);
  }
}
