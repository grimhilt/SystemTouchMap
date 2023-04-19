#include <stdio.h>
#include <stdlib.h>

void analyser(char *file, int row, int column, int width, int height) {

    printf("analyser\n");
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    f = fopen(file, "r");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    int map[row][column];
    int val1;
    int val2;
    int second;
    while ((read = getline(&line, &len, f)) != -1) {
        second = val1 = val2 = 0;
        for (char *c = line; *c != '\n'; c++) {
            if (second == 1) {
                val2 *= 10;
                val2 += *c - 48;
            } else if (*c == ',') {
                second = 1;
            } else {
                val1 *= 10;
                val1 += *c - 48;
            }
        }
        printf("%s, %i, %i\n", line, val1, val2);
    }

    fclose(f);
   
}