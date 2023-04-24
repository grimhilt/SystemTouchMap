#include <X11/Xlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

FILE *f;
Display *display;
int fd;

int find_tracking_device(char *mouse_device)
{
    FILE *f_devices;
    char *line;
    size_t len = 0;
    size_t read;

    f_devices = fopen("/proc/bus/input/devices", "r");
    if (f_devices == NULL)
    {
        printf("Error opening /proc/bus/input/devices file!\n");
        exit(EXIT_FAILURE);
    }

    char waitForChar = 'N';
    while ((read = getline(&line, &len, f_devices)) != -1)
    {
        if (*line == waitForChar)
        {
            if (*line == 'N')
            {
                // line of Name
                if (strstr(line, "Touchpad") != NULL)
                {
                    waitForChar = 'H';
                }
            }
            else
            {
                // line of handler
                line += 12;
                for (char *c = line; *c != ' ' && *c != '\0' && *c != '\n'; ++c)
                {
                    *mouse_device = *c;
                    mouse_device++;
                }
                return 0;
            }
        }
    }
    return 1;
}

void sigint_handler(int sig)
{
    printf("Received SIGINT (%i) signal. Cleaning up...\n", sig);

    close(fd);
    XCloseDisplay(display);
    fclose(f);

    exit(0);
}

void saveValue(FILE *f, int x, int y)
{
    fprintf(f, "%i,%i\n", x, y);
}

void clicked(int *numberClicks, int root_x, int root_y, FILE *f, char *file)
{
    ++*numberClicks;

    // reopen the stream to save the buffer
    if (*numberClicks % 500 == 0)
    {
        fclose(f);
        f = fopen(file, "a");
    }
    saveValue(f, root_x, root_y);
    printf("Mouse position: %d,%d, %i\n", root_x, root_y, *numberClicks);
}

void logger(char *file)
{
    // set up the signal handler function for SIGINT.
    signal(SIGINT, sigint_handler);

    f = fopen(file, "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
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

    // open input event
    char mouse_device[256];
    if (find_tracking_device(mouse_device) > 0)
    {
        printf("Could not find tracking device.\n");
        exit(EXIT_FAILURE);
    }

    char fileInput[256] = "/dev/input/";
    strcat(fileInput, mouse_device);
    printf("Opening %s\n", fileInput);
    fd = open(fileInput, O_RDONLY);
    if (fd == -1)
    {
        perror("Could not open input device");
        exit(EXIT_FAILURE);
    }

    // listen for events
    int numberClicks = 0;
    int xDown = 0;
    int yDown = 0;

    bool isTouch = false;
    bool isClick = false;

    while (read(fd, &ie, sizeof(struct input_event)))
    {
        isTouch = (ie.type == EV_KEY && ie.code == BTN_TOUCH);
        isClick =
            (ie.type == EV_KEY &&
             (ie.code == BTN_LEFT || ie.code == BTN_RIGHT) && ie.value == 1);
        if (isClick)
        {
            XQueryPointer(display, root_window, &root_window, &child_window,
                          &root_x, &root_y, &win_x, &win_y, &mask);
            clicked(&numberClicks, root_x, root_y, f, file);
            xDown = -1;
            yDown = -1;
        }
        else if (isTouch)
        {
            if (ie.value == 1)
            { // finger down
                XQueryPointer(display, root_window, &root_window, &child_window,
                              &root_x, &root_y, &win_x, &win_y, &mask);
                xDown = root_x;
                yDown = root_y;
            }
            else
            { // finger up
                XQueryPointer(display, root_window, &root_window, &child_window,
                              &root_x, &root_y, &win_x, &win_y, &mask);
                if (xDown == root_x && yDown == root_y)
                {
                    clicked(&numberClicks, root_x, root_y, f, file);
                }
            }
        }
    }
}
