#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../src/controler/aquarium.h"
#include "../src/controler/utils.h"

#define BUF_SIZE 64

void test_load_aquarium(struct aquarium * test_aqua){
  //testing the dimension of the aquarium.txt are correct
  assert(test_aqua->height == 1000);
  assert(test_aqua->width == 1000);
  assert(test_aqua->nb_view == 4);

  //testing the position and the dimension of the first view
  assert(!strcmp(test_aqua->tab_views[0]->id, "N1"));
  assert(test_aqua->tab_views[0]->posx == 0);
  assert(test_aqua->tab_views[0]->posy == 0);
  assert(test_aqua->tab_views[0]->height == 500);
  assert(test_aqua->tab_views[0]->width == 500);


  //testing the position and the dimension of the second view
  assert(!strcmp(test_aqua->tab_views[1]->id, "N2"));
  assert(test_aqua->tab_views[1]->posx == 500);
  assert(test_aqua->tab_views[1]->posy == 0);
  assert(test_aqua->tab_views[1]->height == 500);
  assert(test_aqua->tab_views[1]->width == 500);
  printf("test_load_aquarium successful\n\n");
}

void test_add_view(struct aquarium * test_aqua){
  add_view(test_aqua, "N5", 250, 250, 50, 50);
  assert(!strcmp(test_aqua->tab_views[4]->id, "N5"));
  assert(test_aqua->tab_views[4]->posx == 250);
  assert(test_aqua->tab_views[4]->posy == 250);
  assert(test_aqua->tab_views[4]->height == 50);
  assert(test_aqua->tab_views[4]->width == 50);
  printf("test_add_view successful\n\n");
}

void test_del_view(struct aquarium * test_aqua){
  del_view(test_aqua, "N2");
  assert(!strcmp(test_aqua->tab_views[1]->id, "N3"));
  assert(test_aqua->tab_views[1]->posx == 0);
  assert(test_aqua->tab_views[1]->posy == 500);
  assert(test_aqua->tab_views[1]->height == 500);
  assert(test_aqua->tab_views[1]->width == 500);
  printf("test_del_view successful\n\n");
}

void test_save_aquarium(struct aquarium * test_aqua) {
    int err;
    size_t len = 0;
    save_aquarium(*test_aqua, "aquarium.txt");
    FILE *file = fopen("aquarium.txt", "r+");
    //size_t read;
    //size_t len = 0;
    char *buf = "\n";
    if (file == NULL) {
        exit_error("Aquarium file doesn't exist\n");
    }

    err = 0;
    while (err != -1) {
        printf("%s", buf);
        err = getline(&buf, &len, file);

    }
  //free(buf);

  fclose(file);
    printf("\n\ntest_save_aquarium successful\n");
}



int main(int argc, char * argv[]){
  char * file_name = "tst/aquarium.txt";
  struct aquarium * test_aqua = load_aquarium(file_name);
  test_load_aquarium(test_aqua);
  show_aquarium(*test_aqua);
  printf("\n");
  test_add_view(test_aqua);
  test_del_view(test_aqua);
  test_save_aquarium(test_aqua);
  free_aquarium(test_aqua);
  prompt_aquarium();
  
  
  return 0;

}
