<div align="center">
<img width="250" src="colorp_logo.png" alt="colorp logo">
</div>

# colorp

![](./demo.gif)

Simple color picker for Linux made with Xlib (X11)

- This application must be run via terminal
- To **copy** a color code, select it and press **[Ctrl] + [Shift] + [C]**
- Press **[Ctrl] + [C]** to **exit** the application

## Dependencies

### Debian / Ubuntu

```console
$ sudo apt-get install libx11-dev libxext-dev
```

## Run the application

```console
$ cc -o colorp colorp.c -lX11
$ ./colorp
```
