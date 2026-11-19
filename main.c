#include "cache_config.h"
#include "cache_state.h"
#include "trace.h"

#include <stdio.h>
#include <stdlib.h>

// Main 
int main(int argc, char const *argv[])
{
    if (argc <= 2) {
        printf("missing arguments!");
        exit(EXIT_FAILURE);
        return 1;
    } else {
        mode = atoi(argv[1]);

        return read_text_file(argv[2]);
    }
}