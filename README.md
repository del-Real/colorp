<div align="center">
<img width="250" src="colorp_logo.png" alt="colorp logo">
</div>

# colorp

![](./demo.gif)

Simple CLI color picker for Linux made with Xlib (X11)

- Displays RGB and HEX color values (the application must be run via terminal)
- To **copy** a color value, select it from the terminal and press **[Ctrl] + [Shift] + [C]**
- Press **[Q]** or **[Esc]** in the terminal to **exit** the application
- Press **[Ctrl] + [C]** in the terminal to **force exit** the application

## Manual installation

### Dependencies (Debian / Ubuntu)

```console
$ sudo apt-get install libx11-dev libxext-dev
```

## Run the application

```console
$ cc -o colorp colorp.c -lX11
$ ./colorp
```
