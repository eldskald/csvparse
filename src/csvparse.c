#include "csvparse.h"
#include <stdio.h>

#define __CSV_MAX_COLUMNS_SIZE 32
#define __CSV_MAX_ROWS_SIZE 64

const char ***csvparse(const char *filepath, int *size) {
    printf("%s", filepath);
    return NULL;

    // FILE *file = fopen(filepath, "r");
    // if (file == NULL) {
    //     fclose(file);
    //     return NULL;
    // }
    //
    // static const char **result[__CSV_MAX_ROWS_SIZE];
    //
    // char *buffer = NULL;
    // while (fread(buffer, sizeof(char), 1, file) > 0) {
    //     printf("%s", buffer);
    // }
    //
    // fclose(file);
    // return result;
}
