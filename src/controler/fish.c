#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aquarium.h"
#include "fish.h"

#define TICK_RATE 1

struct fish stop_fish = {"stop", 0, 0, 0, 0, 0, 0, NULL, NULL};

llist *llist_create()
{
    struct node *new_node;

    llist *new_list = (llist *)malloc(sizeof (llist));
    *new_list = (struct node *)malloc(sizeof (struct node));

    new_node = *new_list;
    new_node->data = &stop_fish;
    new_node->next = NULL;
    return new_list;
}

void llist_free(llist *list)
{
    struct node *curr = *list;
    struct node *next;

    while (curr != NULL) {
        next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }

    free(list);
}



void llist_push(llist *list, struct fish data)
{
    struct fish* new_fish = malloc(sizeof(struct fish));
    *new_fish = data;

    struct node *head;
    struct node *new_node;
    if (list == NULL || *list == NULL) {
        fprintf(stderr, "llist_add_inorder: list is null\n");
    }

    head = *list;

    // Head is not empty, add new node to front

    new_node = malloc(sizeof (struct node));
    new_node->data = new_fish;
    new_node->next = head;
    *list = new_node;
}


int is_last_element(struct node n){
    return(n.next == NULL);
}

int llist_delete(llist *list, char* name)
{
    struct node * head = *list;
    struct node * tmp;
    if(strcmp(head->data->name, name) == 0){
        tmp = head;
        *list = head->next;
        free(tmp->data);
        free(tmp);
        return EXIT_SUCCESS;
    }
    struct node* prev = head;
    head = head->next;
    while(!is_last_element(*head)){
        if (strcmp(head->data->name, name) == 0){
            tmp = head;
            prev->next = head->next;
            free(tmp->data);
            free(tmp);
            return EXIT_SUCCESS;
        }
        prev = head;
        head = head->next;
    }
    return EXIT_FAILURE;
}

void llist_print(llist *list)
{
    struct node *curr = *list;
    while (curr != NULL) {
        printf("%s", curr->data->name);
        printf(" ");
        curr = curr->next;
    }
    printf("\n");
}

struct fish* search_fish(llist* list, char* name){
    struct node * element = *list;
    while(!is_last_element(*element)){
        if(!strcmp(element->data->name, name)){
            return element->data;
        }
        element = element->next;
    }
    return NULL;
}

int random_way_point(struct fish* poisson, struct aquarium aqua){
    struct random_way_point_args* arguments = (struct random_way_point_args*) poisson->args;
    int width = aqua.width;
    int height = aqua.height;
    if ((struct random_way_point_args*) poisson->status)
      if(arguments->time <= 0){
          poisson->posx = arguments->posx;
          poisson->posy = arguments->posy;
          arguments->posx = rand() % width;
          arguments->posy = rand() % height;
          arguments->time = 1 + rand() % 10;
          poisson->updated = 0;
      }
      else{
          arguments->time -= TICK_RATE;
      }
    return EXIT_SUCCESS;
}



void* fish_routine(void* args){
    struct  aquarium* aqua = ((struct args_fish_routine*) args)->aqua;
    llist* list = ((struct args_fish_routine*) args)->list_fish;
    struct node* fish_node;
    struct fish* fish;
    while(1){

          // printf("list_fish: %p\n", (*list)->data);
          // printf("list_fish: %p\n", (*list)->data->args);

        fish_node = *list;
        while(!is_last_element(*fish_node)){
            fish = fish_node->data;
            fish->movement(fish, *aqua);

#ifdef DEBUG
            printf("%s, pos: %dx%d, dest : %dx%d, time : %f\n",
                    fish->name,
                    fish->posx, fish->posy,
                    ((struct random_way_point_args*) fish->args)->posx,
                    ((struct random_way_point_args*) fish->args)->posy,
                    ((struct random_way_point_args*) fish->args)->time);
#endif
            fish_node = fish_node->next;
        }

        sleep(TICK_RATE);
    }
}
