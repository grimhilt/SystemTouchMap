#ifndef ANALYSER_H
#define ANALYSER_H

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
void analyser(struct arg_config config);

#endif