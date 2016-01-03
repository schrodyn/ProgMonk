##Background image

The background image configuration does not depend on the window manager.
i3 will not touch the X11 root window (which is the window on which the background is set),
so you need to set a background with an external program.

Many people use feh. [feh](http://feh.finalrewind.org/) is a lightweight and powerful image viewer
that can also be used to manage the desktop wallpaper for standalone window managers lacking such features.

To install it type

```
# pacman -S feh
```

you could set your background when X starts adding a line in your `~/.xinitrc` file:

```
feh --bg-scale /path/to/image
```

Or you could add this line in i3 config file:

```
exec --no-startup-id feh --bg-scale /path/to/image
```

To set a static color (not an image), use

```
xsetroot -solid "#333333"
```

You can place the `feh` or `xsetroot` call either in your `~/.xsession` before `exec`ing i3, or
(in case you don’t use `~/.xsession`) in your i3 configuration file `~/.config/i3/config`:

```
exec --no-startup-id xsetroot -solid "#333333"
```
