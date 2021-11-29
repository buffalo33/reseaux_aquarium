#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void exit_error(char* message){
    printf("%s",message);
    exit(EXIT_FAILURE);
}