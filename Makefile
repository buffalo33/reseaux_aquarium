CC = gcc
CFLAGS = -std=gnu99 -Wall -g
SRC=src
TST=tst
AFFICHAGE=${SRC}/affichage
CONTROL = ${SRC}/controler
CLIENT = ${AFFICHAGE}/client
PATH_TO_FX = ./javafx-sdk-11.0.2/lib

all:

install:
	javac --module-path ${AFFICHAGE}/${PATH_TO_FX} --add-modules javafx.controls ${CLIENT}/ConfigElement.java ${CLIENT}/Dimensions.java ${CLIENT}/Objectif.java ${CLIENT}/Poisson.java ${AFFICHAGE}/Client.java ${AFFICHAGE}/Serveur.java

client: install
	cd ${AFFICHAGE} && java --module-path ${PATH_TO_FX} --add-modules javafx.controls Client && cd ../..

client_connexion: install
	gnome-terminal -- "./server.sh" && cd ${AFFICHAGE} && java --module-path ${PATH_TO_FX} --add-modules javafx.controls Client && cd ../../

utils.o: ${CONTROL}
	${CC} ${CFLAGS} -c ${CONTROL}/utils.c

aquarium.o: ${CONTROL}/aquarium.c ${CONTROL}/aquarium.h
	${CC} ${CFLAGS} -c ${CONTROL}/aquarium.c

fish.o: ${CONTROL}/fish.c ${CONTROL}/fish.h
	${CC} ${CFLAGS} -c ${CONTROL}/fish.c

fish_debug.o: ${CONTROL}/fish.c ${CONTROL}/fish.h
	${CC} ${CFLAGS} -D DEBUG -c ${CONTROL}/fish.c -o fish_debug.o

connexion.o: ${CONTROL}/connexion.c ${CONTROL}/connexion.h
	${CC} ${CFLAGS} -c ${CONTROL}/connexion.c

controler: ${CONTROL}/controler.c ${CONTROL}/controler.h utils.o fish.o aquarium.o connexion.o
	${CC} ${CFLAGS} utils.o fish.o aquarium.o connexion.o  ${CONTROL}/controler.c -o $@ -pthread -I ${CONTROL}

test_aquarium: aquarium.o utils.o ${TST}/test_aquarium.c
	${CC} ${CFLAGS} -I ${CONTROL} -o test_aquarium aquarium.o utils.o ${TST}/test_aquarium.c

test_fish: fish_debug.o utils.o ${TST}/test_fish.c aquarium.o
	${CC} ${CFLAGS} -I ${CONTROL} -o test_fish fish_debug.o utils.o aquarium.o ${TST}/test_fish.c

alltests: test_aquarium test_fish
	./test_controler

affichage:
	javac --module-path ${AFFICHAGE}/${PATH_TO_FX} --add-modules javafx.controls ${CLIENT}/ConfigElement.java ${CLIENT}/Dimensions.java ${CLIENT}/Objectif.java ${CLIENT}/Poisson.java ${AFFICHAGE}/Client.java ${AFFICHAGE}/Serveur.java ${AFFICHAGE}/View.java
	cd ${AFFICHAGE} && java --module-path ${PATH_TO_FX} --add-modules javafx.controls View &


clean:
	rm *.o test_aquarium fish_test

