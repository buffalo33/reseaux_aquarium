Projet réseaux RE203: Simulation d'un aquarium de poissons
=====================

Introduction
------------

Le projet consiste a simuler un aquarium géré par un serveur. Il offrira des vues de l'aquarium à des clients et leur fournira des poissons dont l'affichage sera géré par le client. Le projet a été réalisé par:
* Augustin Gauchet
* Vincent Ridacker
* Eloi Magon de la Villehuchet 
* Clément Préaut

Installation
-------------
* Pour l'affichage, il faut téléchager le dossier SDK de javaFX adapté au système d'exploitation disponible sur ce [lien](https://gluonhq.com/products/javafx/). Il suffit de placer le dossier décompréssé dans `src/affichage/`.
* `make controler` : Compile le serveur.
* `./controler tst/aquarium.txt` : Lance le serveur.
* Dans le fichier `src/affichage.cfg`, à la section `controller-address:`, entrer l'adresse ip du serveur (par exemple l'adresse locale si les tests se font sur la même machine).
* `make client` : lance un client.

Script client proposé
---------
* `hello in as N1` : Demande une vue disponible au serveur.
* `addFish PoissonNain at 61x5,10x10 RandomWayPoint` : Ajoute un premier poisson.
* `addFish PoissonBeau at 61x5,10x10, RandomWayPoint` : Ajoute un deuxième poisson.
* `startFish PoissonNain` : Démarre le premier poisson. 
* `startFish PoissonBeau` : Démarre le deuxième poisson.
* `getFishesContinuously` : Met à jour en continu les coordonnées des poissons