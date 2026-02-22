<div align="center">
<img width="250" src="colorp_logo.png" alt="colorp logo">
</div>

# colorp

Simple CLI color picker for Linux made with Xlib (X11). 

Displays RGB and HEX color values (__the application must be run via terminal__)

![](./demo.gif)

## Controls

| Keys                                | Action                  |
|-------------------------------------|-------------------------|
| [ **Ctrl** ] + [ **Shift** ] + [ **C** ]  | **Copy** color value    |
| [ **Q** ] or [ **Esc** ]                | **Exit** the app        |
| [ **Ctrl** ] + [ **C** ]                | **Force exit** the app  |

## Installation

#### Dependencies

```console
$ sudo apt-get install libx11-dev libxext-dev
```

## Run

```console
$ cc -o colorp colorp.c -lX11
$ ./colorp
```
