// colorp | A simple color picker for Linux
// Copyright (c) 2024 Alberto del Real
// This code is licensed under the MIT License.

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <unistd.h>

#define SQUARE_SIZE 100

int main() {
    Display *display;
    Window root, win;
    Window dummy_win;
    XEvent event;

    int screen;
    int x, y;
    int dummy_int;
    unsigned int mask;
    int shouldClose = 1; // Flag to control the loop
    unsigned long pixel;

    XImage *image;
    XColor color;
    XColor prevColor;
    GC gc;

    // Open connection to the X server
    display = XOpenDisplay(NULL);

    // Get the root window
    screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    // Create window
    win = XCreateSimpleWindow(display, root, 10, 10, SQUARE_SIZE, SQUARE_SIZE, 1, BlackPixel(display, screen),
                              WhitePixel(display, screen));

    // Select input events for the window
    XSelectInput(display, win, ExposureMask | KeyPressMask);

    // Map/show the window
    XMapWindow(display, win);

    // Create graphics context
    gc = XCreateGC(display, win, 0, NULL);

    printf("==============================================\n");
    printf("    colorp by Alberto del Real (RedSantar)\n");
    printf("----------------------------------------------\n");
    printf(" Press [Q] or [Esc] to exit the application\n");
    printf("==============================================\n\n");

    // Main loop
    while (shouldClose) {
        // Query the pointer/mouse cursor
        if (XQueryPointer(display, root, &dummy_win, &dummy_win, &x, &y, &dummy_int, &dummy_int, &mask)) {
            // Get the image at the pointer's coordinates
            image = XGetImage(display, root, x, y, 1, 1, AllPlanes, ZPixmap);
            if (image != NULL) {
                pixel = XGetPixel(image, 0, 0);
                color.pixel = pixel;
                XQueryColor(display, DefaultColormap(display, screen), &color);

                // Set the foreground color of the graphics context
                XSetForeground(display, gc, pixel);

                // Fill the window with the color
                XFillRectangle(display, win, gc, 0, 0, SQUARE_SIZE, SQUARE_SIZE);

                if (color.pixel != prevColor.pixel) {
                    // Print color and Hex code
                    printf("\033[38;2;%d;%d;%dm██████████\033[0m", color.red >> 8, color.green >> 8, color.blue >> 8);
                    printf(" HEX | #%02x%02x%02x\n", color.red >> 8, color.green >> 8, color.blue >> 8);
                }
                prevColor.pixel = pixel;
                XDestroyImage(image);
            }
        }

        // Event handler
        while (XPending(display)) {
            XNextEvent(display, &event);
            if (event.type == Expose) {
                // Repaint the window with the current color
                XFillRectangle(display, win, gc, 0, 0, SQUARE_SIZE, SQUARE_SIZE);
            }

            // Close app is Q_KEY or ESC is pressed
            else if (event.type == KeyPress) {
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                if (keysym == XK_q || keysym == XK_Escape) {

                    shouldClose = 0;
                }
            }
        }

        // Sleep for a short duration to avoid flooding the output (100 milliseconds)
        usleep(100000);
    }

    // Close the connection to the X server
    XCloseDisplay(display);

    return 0;
}
