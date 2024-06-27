# colorp

![](./demo.gif)

Simple color picker for Linux made with Xlib (X11)

- This application must be run via terminal
- Press [Q] or [Esc] to exit the application

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
