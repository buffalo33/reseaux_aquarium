#include "fish.h"
#include "aquarium.h"
#include <stdlib.h>

int main(){
    srand(1);
    char * file_name = "tst/aquarium.txt";
    struct aquarium * test_aqua = load_aquarium(file_name);
    llist* fish_list = llist_create();
    struct random_way_point_args args_f1 = {10, 10, 2};
    struct fish f1 = {"fish1", 5, 5, 2, 1, 1, 0, random_way_point, (void*) &args_f1};
    struct random_way_point_args args_f2 = {4, 7, 3};
    struct fish f2 = {"fish2", 9, 3, 1, 3, 1, 0, random_way_point, (void*) &args_f2};
    llist_push(fish_list, f1);
    llist_push(fish_list, f2);
    struct args_fish_routine args_fish= {test_aqua, fish_list};
    fish_routine(&args_fish);
    llist_free(fish_list);
}