#include <stddef.h>
#include <X11/Xlib.h>

void getScreenSize(int *screenWidth, int *screenHeight) {
    Display *display = XOpenDisplay(NULL);
    int screen_num = DefaultScreen(display);
    Screen *screen = ScreenOfDisplay(display, screen_num);
    *screenWidth = screen->width;
    *screenHeight = screen->height;
    XCloseDisplay(display);
}