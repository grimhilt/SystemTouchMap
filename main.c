#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/input.h>
#include <X11/Xlib.h>

FILE *f;
Display *display;
int fd;

void sigint_handler(int sig) {
    printf("Received SIGINT signal. Cleaning up...\n");
    
    close(fd);
    XCloseDisplay(display);
    fclose(f);

    exit(0);
}

void saveValue(FILE *f, int x, int y) {
    fprintf(f, "%i,%i\n", x, y);
}

int main() {
    // set up the signal handler function for SIGINT.
    signal(SIGINT, sigint_handler);

    f = fopen("file.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    display = XOpenDisplay(NULL);
    int screen_num = DefaultScreen(display);
    Screen *screen = ScreenOfDisplay(display, screen_num);
    printf("Screen size: %dx%d\n", screen->width, screen->height);

    Window root_window = RootWindow(display, screen_num);
    Window child_window;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;

    
    struct input_event ie;

    // change "event18" to the appropriate device for your system
    fd = open("/dev/input/event18", O_RDONLY);
    if (fd == -1) {
        perror("Could not open input device");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int numberClicks = 0;

    int xDown = 0;
    int yDown = 0;

    bool isTouch = false;
    bool isClick = false;

    while (read(fd, &ie, sizeof(struct input_event))) {
        isTouch = (ie.type == EV_KEY && ie.code == BTN_TOUCH);
        isClick = (ie.type == EV_KEY && (ie.code == BTN_LEFT || ie.code == BTN_RIGHT) && ie.value == 1);
        if (isTouch || isClick) {
            if (ie.value == 1) { // finger down
                XQueryPointer(display, root_window, &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);
                xDown = root_x;
                yDown = root_y;

                count++;
            } else { // finger up
                XQueryPointer(display, root_window, &root_window, &child_window, &root_x, &root_y, &win_x, &win_y, &mask);
                // todo get timer between events to avoid touch click release to be count as two clicks
                if (xDown == root_x && yDown == root_y) {
                    numberClicks++;

                    // reopen the stream to save the buffer
                    if (numberClicks % 500 == 0) {
                        fclose(f);
                        f = fopen("file.txt", "a");
                    }
                    saveValue(f, root_x, root_y);
                    printf("Mouse position: %d,%d, %i\n", root_x, root_y, numberClicks);
                }
                count--;
            }
        } 
    }



    return 0;
}

