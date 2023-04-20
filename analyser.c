#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include "sdlUtil.h"

#include <stdint.h>

uint32_t scaleColor(int min, int max, int value, uint32_t min_color, uint32_t max_color) {
    // Calculate the range of the input values
    float range_size = (float)(max - min);
    
    // Calculate the distance between the value and the minimum of the range
    float distance_from_min = (float)(value - min);
    
    // Calculate the interpolation factor between the two colors
    float factor = distance_from_min / range_size;
    
    // Extract the red, green, and blue components of the minimum color
    uint8_t min_red = (min_color >> 16) & 0xFF;
    uint8_t min_green = (min_color >> 8) & 0xFF;
    uint8_t min_blue = min_color & 0xFF;
    
    // Extract the red, green, and blue components of the maximum color
    uint8_t max_red = (max_color >> 16) & 0xFF;
    uint8_t max_green = (max_color >> 8) & 0xFF;
    uint8_t max_blue = max_color & 0xFF;
    
    // Interpolate between the two colors using the factor
    uint8_t red = (uint8_t)(min_red + factor * (max_red - min_red));
    uint8_t green = (uint8_t)(min_green + factor * (max_green - min_green));
    uint8_t blue = (uint8_t)(min_blue + factor * (max_blue - min_blue));
    
    uint32_t color = (red << 16) | (green << 8) | blue | 0xFF000000;
    return color;
}

void analyser(char *file, int row, int column, int width, int height) {
    // todo without value
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

    // init value size
    int map[row][column];
    int sizeRow = height / column;
    int sizeCol = width / row;

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

    SDL_Surface *surface = create_surface(width, height);
    Uint32 color;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            color = scaleColor(0, 1000, i, 0xFFFFFFFF, 0xFF0000FF);
            WritePixel(surface, i, j, color);
        }
    }
    IMG_SavePNG(surface, "./grid.png");
    displaySurface(surface);
    printf("%s, %i, %i\n", line, val1, val2);

    fclose(f);
}