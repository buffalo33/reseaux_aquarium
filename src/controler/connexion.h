#ifndef CONNEXION_H
#define CONNEXION_H
#include "fish.h"
struct args_connexion{
    int port;
    struct aquarium* aqua;
    llist* list;
    
};

void* connexion_with_client(void* param);

int get_num_port(char* confing_path);

#endif