#ifndef AQUARIUM_H
#define AQUARIUM_H

#define NB_MAX_VIEW 16


struct aquarium{
    int height;
    int width;
    int nb_view;
    struct view* tab_views[NB_MAX_VIEW];
};

struct view{
    char* id;
    int posx;
    int posy;
    int height;
    int width;
    int attributed;
};


/* Loading the aqurium from a file */
struct aquarium* load_aquarium(char* file_name);

/* Freeing the memory allocated during the loading of the aquarium.txt */
void free_aquarium(struct aquarium* aqua);

/* Printing the content of the aquarium currently loaded  */
void show_aquarium(struct aquarium aqua);

/* Adding a view to the aquarium */
void add_view(struct aquarium* aqua, char* id_new, int posx_new, int posy_new, int width_new, int height_new);

/* Deleting a view from the aquarium */
void del_view(struct aquarium* aqua, char* id_del);

/* Saving the views in a file */
void save_aquarium(struct aquarium aqua, char* file_name);

void prompt_aquarium();
#endif
