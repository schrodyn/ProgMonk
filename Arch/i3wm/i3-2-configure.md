##Keybindings for Most Common Activities and Applications

The following are some basic keybindings to help you get started. For a full map of default
keybindings see [here](https://i3wm.org/docs/userguide.html#_default_keybindings) or consult a very thoroughly commented `config` file located in `~/.config/i3`.

###Terminal

Pressing `Alt+Return` launches a terminal. If you want to change it, modify the following line:

```
bindsym $mod+Return exec i3-sensible-terminal
```

Specify a terminal of your choice:

```
bindsym $mod+Return exec /usr/bin/urxvt
```

###Close a Window

```
Alt+Shift+q
```

Go to a Given Workspace

```
Alt+2
```

In this instance we go to Workspace 2.

###Reload the Config

```
Alt+Shift+c
```

###Restart i3

```
Alt+Shift+r
```

###Quit i3

```
Alt+Shift+e
```

##Adding Your Own Keybindings

If you want to launch Firefox using `Alt+b`, add the following to `~/.config/i3/config`:

```
bindsym $mod+b exec /usr/bin/firefox
```

##Opening Other Applications

The `Alt+d` keybinding launches `dmenu` where you can type a program you want to run.

If you haven't installed it just type:

```
# pacman -S dmenu
```

Since `dmenu` can launch only GUI applications
Also you can use default wrapper for `dmenu` called `i3-dmenu-desktop`:

Comment this line in `~/.config/i3/config` file:

```
bindsym $mod+d exec dmenu_run
```

And un-comment this

```
# bindsym $mod+d exec --no-startup-id i3-dmenu-desktop
```

But it's recommended to use `j4-dmenu-desktop`, a replacement for `i3-dmenu-desktop`.
We'll install it later.
