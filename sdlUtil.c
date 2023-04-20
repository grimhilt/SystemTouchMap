
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <err.h>

void WritePixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
    {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *) p = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *) p = pixel;
            break;
    }
}

SDL_Surface *create_surface(int width, int height)
{
    return SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
}

void draw(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void event_loop(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_Event event;
    SDL_Texture *t = texture;

    draw(renderer, t);

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, t);
                }
                break;

                // Draw new picture
                draw(renderer, t);
                break;
        }
    }
}

void displaySurface(SDL_Surface *surface)
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // texture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    // SDL_RenderPresent(renderer);

    // Creates a window.
    SDL_Window *window = SDL_CreateWindow(
        "Image", 0, 0, 10, 10, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());



    SDL_Texture *texture =  SDL_CreateTextureFromSurface(renderer, surface);

    // Gets the width and the height of the texture.
    int w, h;
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window, w, h);
    
    // SDL_Surface *surface = load_image(argv[1]);
    // surface = binarization(surface);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    event_loop(renderer, texture);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Destroys the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}