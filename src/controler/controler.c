#include <pthread.h>
#include <stdio.h>
#include "controler.h"
#include "aquarium.h"
#include "connexion.h"
#include "fish.h"
#include <time.h>

struct aquarium* aqua;
llist* list_fish;


int main(int argc, char* argv[]){
    srand(time(NULL));
    if(argc < 2){
        printf("USAGE : ./contoler <aquarium_path>\n");
        exit(EXIT_FAILURE);
    }
    aqua = load_aquarium(argv[1]);
    list_fish = llist_create();

    //thread qui gère la connection
    int ret;
    pthread_t thread_connect;
    int port = get_num_port("src/controler/controler.cfg");

    struct args_connexion args_co = {port, aqua, list_fish};
    ret = pthread_create(&thread_connect, NULL, connexion_with_client, (void *) &args_co);
    if (ret) perror("Client thread error\n");

    //thread qui gère le déplacement des poissons
    pthread_t thread_fish;
    struct args_fish_routine args_fish = {aqua, list_fish};
    pthread_create(&thread_fish, NULL, fish_routine, (void*) &args_fish);

    prompt_aquarium();
    return 0;
}
