#ifndef FISH_H
#define FISH_H

#include <stdlib.h>
#include "aquarium.h"


#define NAME_SIZE 64
#define NOT_STARTED 0
#define STARTED 1

struct fish{
    char name[NAME_SIZE];
    int posx;
    int posy;
    int width;
    int height;
    int status;
    int updated;
    int (*movement)(struct fish* poisson, struct aquarium aqua);
    void* args;

};



struct node{
    struct fish* data;
    struct node* next;
};



typedef struct node * llist;

/* llist_create: Create a linked list */
llist *llist_create();

/* llist_free: Free a linked list */
void llist_free(llist *list);

/* llist_push: Add to head of list */
void llist_push(llist *list,struct fish data);

/* llist_pop: remove and return head of linked list */
int llist_delete(llist *list, char* name);

/* llist_print: print linked list */
void llist_print(llist *list);

struct fish* search_fish(llist* list, char* name);

int is_last_element(struct node n);


int random_way_point(struct fish* poisson, struct aquarium aqua);

struct random_way_point_args{
    int posx;
    int posy;
    float time;
};


void * fish_routine(void* args);

struct args_fish_routine{
    struct aquarium* aqua;
    llist* list_fish;
};

#endif
