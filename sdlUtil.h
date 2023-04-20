
#ifndef SDLUTIL_H
#define SDLUTIL_H

void displaySurface(SDL_Surface *surface);
void WritePixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface *create_surface(int width, int height);

#endif
