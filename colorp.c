#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SQUARE_SIZE 100
#define REFRESH_INTERVAL 50000

int main() {
    Display *display;
    Window root, win;
    Window dummy_win;
    XEvent event;
    Atom wmStateAbove, wmState;
    XEvent xev;

    int screen;
    int x, y;
    int dummy_int;
    unsigned int mask;
    bool shouldClose = true;
    unsigned long pixel = 0;
    unsigned long prevPixel = 1;

    XImage *image = NULL;
    XColor color;
    GC gc;

    // Open connection to the X server
    if (!(display = XOpenDisplay(NULL))) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    // Get the root window
    screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    // Create window
    win = XCreateSimpleWindow(display, root, 0, 0, SQUARE_SIZE, SQUARE_SIZE, 1,
                              BlackPixel(display, screen), WhitePixel(display, screen));

    // Set window name
    XStoreName(display, win, "colorp");

    // Get WM_STATE atom
    wmState = XInternAtom(display, "_NET_WM_STATE", False);
    wmStateAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

    // Set _NET_WM_STATE_ABOVE property to make window always on top
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = win;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;  // _NET_WM_STATE_ADD
    xev.xclient.data.l[1] = wmStateAbove;
    xev.xclient.data.l[2] = 0;

    XSelectInput(display, win, ExposureMask | KeyPressMask);                                    // Select input events for the window
    XMapWindow(display, win);                                                                   // Map/show the window
    XSendEvent(display, root, False, SubstructureNotifyMask | SubstructureRedirectMask, &xev);  // Send the ClientMessage
    gc = XCreateGC(display, win, 0, NULL);                                                      // Create graphics context

    // Print instructions
    printf("\033[1;36m╔═══════════════════════════════════════╗\033[0m\n");
    printf("\033[1;36m║ \033[0m  \033[1;35m● \033[1;31m● \033[1;33m● \033[1;32m● \033[1;36m● \033[1;34m●\033[0m  \033[1;37mcolorp\033[0m  \033[1;34m● \033[1;36m● \033[1;32m● \033[1;33m● \033[1;31m● \033[1;35m●\033[0m  \033[1;36m  ║\033[0m\n");
    printf("\033[1;36m║\033[0m  \033[1;37mA simple CLI Color Picker for Linux \033[0m \033[1;36m║\033[0m\n");
    printf("\033[1;36m╟───────────────────────────────────────╢\033[0m\n");
    printf("\033[1;36m║\033[0m \033[1;33m➤\033[0m Move mouse to sample colors         \033[1;36m║\033[0m\n");
    printf("\033[1;36m║\033[0m \033[1;33m➤\033[0m Press \033[1;37m[Q]\033[0m or \033[1;37m[Esc]\033[0m to exit          \033[1;36m║\033[0m\n");
    printf("\033[1;36m╚═══════════════════════════════════════╝\033[0m\n");
    printf("\n");

    // =========
    // Main loop
    // =========
    while (shouldClose) {
        // Handle pending events first for better responsiveness
        while (XPending(display)) {
            XNextEvent(display, &event);
            if (event.type == Expose) {
                // Repaint the window with the current color
                XFillRectangle(display, win, gc, 0, 0, SQUARE_SIZE, SQUARE_SIZE);
            } else if (event.type == KeyPress) {
                KeySym keysym = XLookupKeysym(&event.xkey, 0);
                if (keysym == XK_q || keysym == XK_Escape) {
                    shouldClose = false;
                }
            }
        }

        // Query the pointer/mouse cursor
        if (XQueryPointer(display, root, &dummy_win, &dummy_win, &x, &y, &dummy_int, &dummy_int, &mask)) {
            // Only get the image if it is a new position
            if (image != NULL) {
                XDestroyImage(image);
            }

            image = XGetImage(display, root, x, y, 1, 1, AllPlanes, ZPixmap);
            if (image != NULL) {
                pixel = XGetPixel(image, 0, 0);

                // Only update if the color has changed
                if (pixel != prevPixel) {
                    prevPixel = pixel;

                    // Update the color info
                    color.pixel = pixel;
                    XQueryColor(display, DefaultColormap(display, screen), &color);

                    // Set the foreground color and fill the window
                    XSetForeground(display, gc, pixel);
                    XFillRectangle(display, win, gc, 0, 0, SQUARE_SIZE, SQUARE_SIZE);

                    // Print color HEX/RGB info
                    int r = color.red >> 8;
                    int g = color.green >> 8;
                    int b = color.blue >> 8;

                    printf("\033[38;2;%d;%d;%dm██████████\033[0m", r, g, b);
                    printf("    HEX | #%02x%02x%02x", r, g, b);
                    printf("\tRGB | %d %d %d\n", r, g, b);
                }
            }
        }

        XFlush(display);           // Flush pending requests to improve responsiveness
        usleep(REFRESH_INTERVAL);  // Sleep to avoid flooding CPU
    }

    // Clean up
    if (image != NULL) {
        XDestroyImage(image);
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}