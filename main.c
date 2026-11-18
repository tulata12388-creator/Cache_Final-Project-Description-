#include "cache_config.h"
#include "cache_state.h"
#include "trace.h"

#include <stdio.h>
#include <stdlib.h>

// Main 
int main(int argc, char const *argv[])
{
    if (argc <= 1) {
        mode = 0;

        return read_text_file(TEXT_FILE_NAME);

        // printf("thiếu argument!");
        // exit(EXIT_FAILURE);
        // return 1;
    } else {
        mode = atoi(argv[1]);

        return read_text_file(TEXT_FILE_NAME);
    }
}