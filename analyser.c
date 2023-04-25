#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include "analyser.h"
#include "utils/scaleColor.h"
#include "utils/screenSize.h"
#include "utils/sdl.h"

void printMap(int col, int row, int *map)
{
    for (int y = 0; y < row; y++)
    {
        for (int x = 0; x < col; x++)
        {
            printf("%i ", *map);
            map++;
        }
        printf("\n");
    }
}

void parseArgument(struct arg_config *config)
{
    // setup screen size if not defined by user
    if (config->screenWidth == 0 && config->screenHeight == 0)
    {
        getScreenSize(&(config->screenWidth), &(config->screenHeight));
    }

    // setup default size of the image
    if (config->width == 0 && config->height == 0)
    {
        config->width = config->screenWidth / 2;
        config->height = config->screenHeight / 2;
    }
    else if (config->width == 0 && config->height != 0)
    {
        config->width =
            config->height * config->screenWidth / config->screenHeight;
    }
    else if (config->width != 0 && config->height == 0)
    {
        config->height =
            config->width * config->screenHeight / config->screenWidth;
    }

    if (config->width > config->screenWidth ||
        config->height > config->screenHeight)
    {
        printf("Cannot have an image size larger than the screen size\n");
        exit(EXIT_FAILURE);
    }

    if (config->row == 0 && config->column == 0)
    {
        config->row = 10;
        config->column = 10;
    }
    else if (config->row == 0 && config->column != 0)
    {
        config->row = config->column;
    }
    else if (config->width != 0 && config->height == 0)
    {
        config->column = config->row;
    }
}

void analyser(struct arg_config config)
{
    parseArgument(&config);
    FILE *f;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    f = fopen(config.file, "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    int sizeRow = config.screenHeight / config.row;
    int sizeCol = config.screenWidth / config.column;

    int val1;
    int val2;
    int second;

    int *map = calloc(config.row * config.column, sizeof(int));
    int max = 0;
    while ((read = getline(&line, &len, f)) != -1)
    {
        second = val1 = val2 = 0;
        // extract values from line
        for (char *c = line; *c != '\n'; c++)
        {
            if (second == 1)
            {
                val2 *= 10;
                val2 += *c - 48;
            }
            else if (*c == ',')
            {
                second = 1;
            }
            else
            {
                val1 *= 10;
                val1 += *c - 48;
            }
        }

        // save value in the map
        int colPlace = val1 / sizeCol;
        int rowPlace = val2 / sizeRow;
        if (colPlace >= config.column)
        {
            colPlace = config.column - 1;
        }
        if (rowPlace >= config.row)
        {
            rowPlace = config.row - 1;
        }

        int index = rowPlace + colPlace * config.row;
        if (index >= 0 && index < config.row * config.column)
        {
            map[index]++;
            if (map[index] > max)
            {
                max = map[index];
            }
        }
        else
        {
            printf("Error: out of bounds access at index %d\n", index);
        }
    }

    printf("max click in same spot: %i\n", max);

    // construct image based on the map
    int sizeRowImg = config.height / config.row;
    int sizeColImg = config.width / config.column;
    printf("c:%i, r:%i, w:%i, h:%i, szC:%i, szR:%i, szW:%i, szH:%i\n",
           config.column, config.row, config.width, config.height, sizeColImg,
           sizeRowImg, sizeColImg * config.column, sizeRowImg * config.row);

    SDL_Surface *surface =
        create_surface(sizeColImg * config.column, sizeRowImg * config.row);
    Uint32 color = 0xFFFF0000;
    for (int y = 0; y < config.row; y++)
    {
        for (int x = 0; x < config.column; x++)
        {
            color = scaleColor(0, max, map[x + y * config.row], 0xFFFFFFFF,
                               0xFF0000FF);
            for (int yy = sizeRowImg * y; yy < sizeRowImg * (y + 1); yy++)
            {
                for (int xx = sizeColImg * x; xx < sizeColImg * (x + 1); xx++)
                {
                    WritePixel(surface, xx, yy, color);
                }
            }
        }
    }

    if (config.save != NULL)
    {
        IMG_SavePNG(surface, config.save);
    }
    displaySurface(surface);
    fclose(f);
    free(map);
}