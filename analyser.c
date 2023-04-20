#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include "utils/sdl.h"
#include "utils/scaleColor.h"

struct arg_config
{
    int log;
    char *file;
    int analyse;
    char *save;
    int row;
    int column;
    int width;
    int height;
    int screenWidth;
    int screenHeight;
};

void analyser(struct arg_config *config) {
    // todo without value
    printf("analyser\n");
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    f = fopen(config->file, "r");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    // init value size
    int map[config->row][config->column];
    for (int y = 0; y < config->row; y++) {
        for (int x = 0; x < config->column; x++) {
            map[x][y] = 0;
        }
    }

    printf("%i / %i\n", config->screenHeight, config->column);
    int sizeRow = config->screenHeight / config->column;
    int sizeCol = config->screenWidth / config->row;

    int val1;
    int val2;
    int second;

    int max = 0;
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

        int colPlace = val1 / sizeCol;
        int rowPlace = val2 / sizeRow;
        if (!map[rowPlace][colPlace]) {
            map[rowPlace][colPlace] = 1;
        } else {
            map[rowPlace][colPlace]++;
            if (map[rowPlace][colPlace] > max) {
                max = map[rowPlace][colPlace];
            }
        }
    }
        for (int x = 0; x < config->column; x++) {
    for (int y = 0; y < config->row; y++) {
            printf("%i ", map[x][y]);
        }
        printf("\n");
    }
    printf("max: %i\n");
    
    int sizeRowImg = config->height / config->column;
    int sizeColImg = config->width / config->row;
    SDL_Surface *surface = create_surface(config->width, config->height);
    Uint32 color;
    for (int y = 0; y < config->row; y++) {
        for (int x = 0; x < config->column; x++) {
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