#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include "utils/sdl.h"
#include "utils/scaleColor.h"
#include "analyser.h"

void analyser(struct arg_config config) {
    // todo without value
    printf("w: %i, h: %i, sw: %i, sh: %i, r: %i, c: %i\n", config.width, config.height, config.screenWidth, config.screenHeight, config.row, config.column);
    printf("analyser\n");
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    f = fopen(config.file, "r");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    // init value size
    int map[config.row][config.column];
    for (int y = 0; y < config.row; y++) {
        for (int x = 0; x < config.column; x++) {
            map[y][x] = 0;
        }
    }

    int sizeRow = config.screenHeight / config.row;
    int sizeCol = config.screenWidth / config.column;

    int val1;
    int val2;
    int second;

    int max = 0;
    while ((read = getline(&line, &len, f)) != -1) {
        second = val1 = val2 = 0;
        // extract values from line
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
        
        // save value in the map
        int colPlace = val1 / sizeCol;
        int rowPlace = val2 / sizeRow;
        int *v = &map[rowPlace][colPlace];
        if (!*v) {
            *v = 1;
        } else {
            ++*v;
            if (*v > max) {
                max = *v;
            }
        }
    }

    // for (int x = 0; x < config.column; x++) {
    //     for (int y = 0; y < config.row; y++) {
    //         printf("%i ", map[x][y]);
    //     }
    //     printf("\n");
    // }
    // printf("max: %i\n", max);
    
    // construct image based on the map
    int sizeRowImg = config.height / config.row;
    int sizeColImg = config.width / config.column;
    printf("w:%i, h:%i, szC:%i, szR:%i, szW:%i, szH:%i\n", config.width, config.height, sizeColImg, sizeRowImg, sizeColImg*config.column, sizeRowImg*config.row);
    SDL_Surface *surface = create_surface(sizeColImg*config.column, sizeRowImg*config.row);
    Uint32 color = 0xFFFF0000;
    for (int y = 0; y < config.row; y++) {
        for (int x = 0; x < config.column; x++) {
            color = scaleColor(0, max, map[x][y], 0xFFFFFFFF, 0xFF0000FF);
            for (int yy = sizeRowImg * y; yy < sizeRowImg * (y + 1); yy++) {
                for (int xx = sizeColImg * x; xx < sizeColImg * (x + 1); xx++) {
                    WritePixel(surface, xx, yy, color);
                }
            }
        }
    }

    IMG_SavePNG(surface, "./save.png");
    displaySurface(surface);
    fclose(f);
}