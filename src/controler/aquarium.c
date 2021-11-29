#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aquarium.h"
#include "utils.h"


#define BUF_SIZE 64

struct aquarium* load_aquarium(char* file_name){
    FILE* file = fopen(file_name, "r+");
    size_t read;
    size_t len = 0;
    char tmp[20];
    //char buf [BUF_SIZE];
    char * buf;

    fopen(file_name, "r+");
    struct aquarium * aqua = malloc(sizeof(struct aquarium));

    if (file == NULL){
        exit_error("Aquarium file doesn't exist\n");
    }

    //parsing the Aquarium format
    read = getdelim(&buf, &len, 'x', file);
    if (read <= 0 ){
        exit_error("Aquarium file empty");
    }
    aqua->width = atoi(buf);

    read = getdelim(&buf, &len, '\n', file);
    aqua->height = atoi(buf);

    //parsing view lines in the aquarium.txt file
    int i_views = 0;
    while(read != -1){
        struct view* v = malloc(sizeof(struct view));
        v->id = malloc(2*sizeof(char));

        getdelim(&buf, &len, ' ', file);
        sscanf(buf, "%s", tmp);
        strcpy(v->id,tmp);

        getdelim(&buf, &len, 'x', file);
        v->posx = atoi(buf);

        getdelim(&buf, &len, '+', file);
        v->posy = atoi(buf);

        getdelim(&buf, &len, '+', file);
        v->width = atoi(buf);

        read = getdelim(&buf, &len, '\n', file);
        v->height = atoi(buf);

        v -> attributed = 0;

        aqua->tab_views[i_views] = v;

        if(read != -1) i_views ++;

    }
    aqua->nb_view = i_views;
    if(buf) free(buf);

    printf("-> aquarium.txt loaded (%d display view) !\n", i_views);
    fclose(file);

    return aqua;
}

void free_aquarium(struct aquarium* aqua){
    for (size_t i = 0; i < aqua->nb_view; i++){
        free(aqua->tab_views[i]->id);
        free(aqua->tab_views[i]);

    }
    free(aqua);
}

void show_aquarium(struct aquarium aqua){
    printf("%dx%d\n", aqua.width, aqua.height);
    for (int i = 0; i < aqua.nb_view; i++){
        printf("%s %dx%d+%d+%d\n", aqua.tab_views[i]->id,
                                    aqua.tab_views[i]->posx,
                                    aqua.tab_views[i]->posy,
                                    aqua.tab_views[i]->width,
                                    aqua.tab_views[i]->height);
    }
}

void add_view(struct aquarium* aqua, char* id_new, int posx_new, int posy_new, int width_new, int height_new){
    struct view* v = malloc(sizeof(struct view));
    v->id = malloc(2*sizeof(char));
    strcpy(v->id, id_new);
    v -> posx = posx_new;
    v -> posy = posy_new;
    v -> width = width_new;
    v -> height = height_new;
    v -> attributed = 0;
    aqua -> tab_views[aqua -> nb_view] = v;
    aqua -> nb_view ++;
}

void del_view(struct aquarium* aqua, char* id_del){
    int i = 0;
    while(strcmp(aqua->tab_views[i]->id, id_del) != 0){
        i++;
    }
    free(aqua -> tab_views[i]->id);
    free(aqua -> tab_views[i]);


    aqua -> tab_views[i] = NULL;
    while(i < aqua->nb_view-1){
        aqua -> tab_views[i] = aqua -> tab_views[i+1];
        aqua -> tab_views[i+1] = NULL;
        i++;
    }
    aqua->nb_view--;
}

void save_aquarium(struct aquarium aqua, char* file_name){
    FILE* file = fopen(file_name, "w");
    //size_t wrote;
    //size_t len = 0;
    char buf[BUF_SIZE];
    if ( file == NULL ) {
        exit_error("Aquarium file doesn't exist\n");
    }

    sprintf( buf, "%dx%d\n", aqua.width, aqua.height);
    if ( EOF != fputs(buf, file) ){ //     fwrite( buf, BUF_SIZE, 1, file ) ) {
        fprintf( stderr, "Cannot write block in file\n" );
    }

    for(int i = 0; i < aqua.nb_view; i++){
        sprintf( buf, "%s %dx%d+%d+%d\n", aqua.tab_views[i]->id,
                                        aqua.tab_views[i]->posx,
                                        aqua.tab_views[i]->posy,
                                        aqua.tab_views[i]->width,
                                        aqua.tab_views[i]->height);


        if (EOF != fputs(buf, file) ){ //fwrite( buf, BUF_SIZE, 1, file ) ) {
            fprintf( stderr, "Cannot write block in file\n" );
        }
    }
    fclose(file);
}



void prompt_aquarium(){
    int log = 1;
    char * buffer = malloc(BUF_SIZE*sizeof(char));
    struct aquarium * aquarium1;
    int isAquariumLoaded = 0;
    char cmd[5], arg1[20], arg2[20], arg3[20], arg4[20], arg5[20], arg6[20];
    printf("interface de gestion de l'aquarium!\n");
    while(log){
        printf("$>");
        fgets(buffer, BUF_SIZE, stdin);
        sscanf(buffer, "%s %s %s %s %s %s %s", cmd, arg1, arg2, arg3, arg4, arg5, arg6);
        //printf("%s %s %s %s %s %s %s\n", cmd, arg1, arg2, arg3, arg4, arg5, arg6);
        if (strcmp(cmd, "load") == 0){
            aquarium1 = load_aquarium(arg1);
            isAquariumLoaded = 1;
        }
        else if (strcmp(cmd, "save") == 0){;

            if (isAquariumLoaded){
                save_aquarium(*aquarium1, arg1);
            }
            else {
                printf("Please load an aquarium before trying to save it\n");
            }
        }
        else if (strcmp(cmd, "show") == 0){

            if (isAquariumLoaded){
                show_aquarium(*aquarium1);
            }
            else{
                printf("please load an aquarium before trying to show it\n");
            }

        }
        else if (strcmp(cmd, "del") == 0){
            if (isAquariumLoaded) {
                del_view(aquarium1, arg2);
                printf("\t -> view %s deleted\n", arg2);
            }
            else{
                printf("Please load an aquarium before trying to delete a view\n");
            }
        }
        else if (strcmp(cmd, "add") == 0){
            if (isAquariumLoaded) {
                add_view(aquarium1, arg2, atoi(arg3), atoi(arg4), atoi(arg5), atoi(arg6));
                printf("\t -> view added\n");
            }
            else{
                printf("please load an aquarium before trying to add a view\n");
            }
        }
        else if (strcmp(cmd, "END") == 0){
            log = 0;
        }
    }
    free(buffer);
    if (isAquariumLoaded) {
        free_aquarium(aquarium1);
    }
}