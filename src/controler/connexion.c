#include <stdio.h>
#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "connexion.h"
#include "utils.h"
#include "aquarium.h"
#include "fish.h"

#define BUFFER_SIZE 512
#define NB_CLIENT 10
#define TICK_RATE 1

struct args_exchange{
    int sockfd;
    struct aquarium* aqua;
    llist* list;
};

void* exchange_with_client(void* data){
    int sockfd = (*(struct args_exchange*) data).sockfd;
    struct aquarium* aqua = (*(struct args_exchange*) data).aqua;
    llist* fish_list = (*(struct args_exchange*) data).list;

    char buffer[BUFFER_SIZE];
    char buffer_send[BUFFER_SIZE];
    char send_message[BUFFER_SIZE];
    int ret;
    int log = 1;

    char cmd[10], arg1[32], arg2[32], arg3[32], arg4[32];
    struct view* client_view = malloc(sizeof(struct view));
    int i;
    struct view* curr_view = malloc(sizeof(struct view*));
    time_t t_deb = time(NULL);
    time_t t_fin;
    time_t t_diff;
    int posx, posy, width, height;
    int (*mv_func) (struct fish poisson, struct aquarium aqua);
    void* args;
    struct fish* fish;
    struct node* element;

    while(log){
        bzero(buffer, BUFFER_SIZE);
        ret = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (ret < 0) exit_error("ERROR reading from socket\n");
        sscanf(buffer, "%s %s %s %s %s", cmd, arg1, arg2, arg3, arg4);
        // printf("entrée: %s/ %s/ %s/ %s/ %s\n", cmd, arg1, arg2, arg3, arg4);
        if (strcmp(cmd, "hello") == 0){
            t_deb = time(NULL);
            if(strcmp(arg1, "in") == 0){
                i = 0;
                while(i < aqua->nb_view
                        && (!strcmp(aqua->tab_views[i]->id, arg3) != 0
                        || curr_view->attributed == 1)){
                    i++;
                }
                if(i < aqua->nb_view){
                    curr_view = aqua->tab_views[i];
                    curr_view->attributed = 1;
                    client_view = curr_view;

                    sprintf(send_message, "greeting %s\n", curr_view->id);
                    ret = send(sockfd, send_message, strlen(send_message), 0);
                    if(ret < 0) exit_error("ERROR writing to socket\n");
                }
                else{
                    i = 0;
                    while (i < aqua->nb_view && curr_view->attributed == 1){
                        i++;
                    }
                    if(i < aqua->nb_view){
                        curr_view = aqua->tab_views[i];
                        curr_view->attributed = 1;
                        client_view = curr_view;

                        sprintf(send_message, "greeting %s\n", curr_view->id);
                        ret = send(sockfd, send_message, strlen(send_message), 0);
                        if(ret < 0) exit_error("ERROR writing to socket\n");
                    }
                    else{
                        ret = send(sockfd, "no greeting", strlen("no greeting"), 0);
                        if(ret < 0) exit_error("ERROR writing to socket\n");
                    }
                }
            }
            t_fin = time(NULL);
            t_diff = difftime(t_deb, t_fin);
        }
        else if(!strcmp(cmd, "log")){
            t_deb = time(NULL);
            client_view->attributed = 0;
            client_view = NULL;
            ret = send(sockfd, "bye\n", 3, 0);
            if(ret < 0) exit_error("ERROR writing to socket\n");
            t_fin = time(NULL);
            t_diff = difftime(t_deb, t_fin);
        }

        else if(!strcmp(cmd, "ping")){
            t_deb = time(NULL);
            // send_message = strcat("pong \0", arg1);//SEGSIGV
            ret = send(sockfd, "pong\n", strlen("pong\n") , 0);
            if(ret < 0) exit_error("ERROR writing to socket\n");
            t_fin = time(NULL);
            t_diff = difftime(t_deb, t_fin);
        }

        else if(strcmp(cmd, "addFish") == 0){
            t_deb = time(NULL);
            char* name = arg1;
            if(search_fish(fish_list, name)!= NULL){
                ret = send(sockfd, "NOK\n", 4, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
            else{
                posx = atoi(strtok(arg3, "x"));
                posy = atoi(strtok(NULL, ","));
                width = atoi(strtok(NULL, "x"));
                height = atoi(strtok(NULL, ","));


                struct fish fish = {"Placeholder", posx, posy, width, height, NOT_STARTED, 1, random_way_point, NULL};
                memcpy(fish.name, arg1, strlen(arg1));
                llist_push(fish_list, fish);
                ret = send(sockfd, "OK\n", 3, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
            t_fin = time(NULL);
            t_diff = difftime(t_deb, t_fin);
        }
        else if(!strcmp(cmd, "startFish")){
            llist_print(fish_list);
            fish = search_fish(fish_list, arg1);
            // printf("fish: %p\n", fish);
            // printf("mvt: %p == %p\n", fish->movement, random_way_point);

            if(fish == NULL){
                ret = send(sockfd, "NOK not found\n", 4, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
            if (fish->movement == random_way_point){
                mv_func = random_way_point;
                args = malloc(sizeof(struct random_way_point_args));
                ((struct random_way_point_args*) args)->posx = rand()%aqua->width;
                ((struct random_way_point_args*) args)->posy = rand()%aqua->height;
                ((struct random_way_point_args*) args)->time = rand()%10;
                fish->args = args;
                fish->status = STARTED;
                fish->updated = 0;
                ret = send(sockfd, "OK\n", 3, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
            else{
                ret = send(sockfd, "NOK\n", 4, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
        }

        else if(!strcmp(cmd, "delFish")){
            fish = search_fish(fish_list, arg1);
            if(fish == NULL){
                ret = send(sockfd, "NOK\n", 4, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
            else{
                llist_delete(fish_list, arg1);
                ret = send(sockfd, "OK\n", 3, 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
            }
        }

        else if(!strcmp(cmd, "getFishes")){
            element = *fish_list;
            bzero(buffer_send, BUFFER_SIZE);
            sprintf(buffer_send, "list ");
            for(;!is_last_element(*element); element = element->next){
                bzero(buffer, BUFFER_SIZE);
                sprintf(buffer, "[%s at %dx%d,%dx%d,%f]",
                        element->data->name,
                        ((struct random_way_point_args*) element->data->args)->posx%100,
                        ((struct random_way_point_args*) element->data->args)->posy%100,
                        element->data->width,
                        element->data->height,
                        ((struct random_way_point_args*) element->data->args)->time);
                sprintf(buffer_send, "%s ", strcat(buffer_send, buffer));
            }
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "\n");
            sprintf(buffer_send, "%s", strcat(buffer_send, buffer));

            ret = send(sockfd, buffer_send, strlen(buffer_send), 0);
            if(ret < 0) exit_error("ERROR writing to socket\n");
        }

        else if(!strcmp(cmd, "getFishesContinuously")){
            while(1){
                element = *fish_list;
                bzero(buffer_send, BUFFER_SIZE);
                sprintf(buffer_send, "list ");
                for(;!is_last_element(*element); element = element->next){
                    if(element->data->updated == 0) {
                        bzero(buffer, BUFFER_SIZE);
                        sprintf(buffer, "[%s at %dx%d,%dx%d,%f]",
                                element->data->name,
                                ((struct random_way_point_args *) element->data->args)->posx%100,
                                ((struct random_way_point_args *) element->data->args)->posy%100,
                                element->data->width,
                                element->data->height,
                                ((struct random_way_point_args *) element->data->args)->time);
                        sprintf(buffer_send, "%s ", strcat(buffer_send, buffer));
                        element->data->updated = 1;
                    }
                }
                bzero(buffer, BUFFER_SIZE);
                  sprintf(buffer, "\n");
                sprintf(buffer_send, "%s", strcat(buffer_send, buffer));

                ret = send(sockfd, buffer_send, strlen(buffer_send), 0);
                if(ret < 0) exit_error("ERROR writing to socket\n");
                sleep(TICK_RATE);
            }
        }

        // ret = send(sockfd, "message recieved\n" , strlen("message recieved\n"),0);
        if(ret < 0) exit_error("ERROR writing to socket\n");
        for (int i = 0; i < 5; cmd[i++] = 6);
    }
    return NULL;
}


void* connexion_with_client(void* param){
    struct args_connexion args = *((struct args_connexion*) param);
    int port = args.port;
    struct aquarium* aqua = args.aqua;
    llist* fish_list = args.list;

    int sockfd, newsockfd, portno, ret;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int yes=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (sockfd < 0) exit_error("ERROR opening socket\n");
    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = port;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr= INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        exit_error("ERROR on binding\n");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    pthread_t thread_client[NB_CLIENT];
    int id_client = -1;
    struct args_exchange* args_exchange_tab[NB_CLIENT];
    for (int i = 0; i < NB_CLIENT; args_exchange_tab[i++] = malloc(sizeof(struct args_exchange)));
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) exit_error("ERROR on accept");
        id_client ++;
        if(id_client > NB_CLIENT -1) perror("too much client");
        printf("Client connected\n addr client : %d\n", cli_addr.sin_addr.s_addr);
        printf("id : %d\n", id_client);

        args_exchange_tab[id_client]->sockfd = newsockfd;
        args_exchange_tab[id_client]->aqua = aqua;
        args_exchange_tab[id_client]->list = fish_list;
        ret = pthread_create(&thread_client[id_client], NULL, exchange_with_client, (void *) args_exchange_tab[id_client]);
        if (ret) perror("Client thread error\n");
    }
    return 0;
}

int getnum(char* buf, int len){
    int res = 0;
    int mult = 1;
    for (int i = len -1; i>=0; i--){
        if(buf[i] >= '0' && buf[i] <= '9'){
            res += (buf[i] - '0') *mult;
            mult *= 10;
        }
    }
    return res;
}

int get_num_port(char* confing_path){
    FILE* config_file = fopen(confing_path, "r");
    if (config_file == 0) exit_error("error opening config\n");
    int ret;
    char* buf = NULL;
    size_t len = 0;
    ret = getdelim(&buf, &len, '=', config_file);
    buf = NULL;
    if (ret < 0) exit_error("config file reading error");
    ret = getdelim(&buf, &len, '\n', config_file);
    if (ret < 0) exit_error("config file reading error");
    printf("buf : %s\n", buf);
    return getnum(buf, len);
}
