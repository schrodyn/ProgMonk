#i3 Tiling Window Manager

i3 is a very lightweight, flexible and customisable tiling window manager with excellent multi-monitor support. Tiling window managers aim at maximising the screen space by tiling all opened windows in a non-overlapping mode. They have a very minimalist look and are mostly keyboard-focused. If the concept of tiling window managers is still puzzling, please check the official i3 [screencast](http://i3wm.org/screenshots/).

i3 is a very keyboard centric tilling window manager which uses vim-like keybindings
to navigate around windows and workspaces. Learning to use such WM is quite fun and
could greatly improve your productivity.

##Installation

‘i3’ package group includes a window manager, screenlocker, and a status bar.

```
# pacman -S i3
```

If you have not yet installed a display manager or prefer to start X session manually, add ‘exec i3’ to `.xinitrc` file in the user’s home directory:

```
$ echo "exec i3" >> ~/.xinitrc
```

Then

```
$ startx
```

startx initialize an X session and loads the clients/applications specified in `.xinitrc`. When the last client exits, the session will exit.

##i3 Configuration

When you first start i3, you will be welcomed by `i3-config-wizard`:

```
You have not configured i3 yet.
Do you want me to generate ~/.config/i3/config?

<Enter> Yes, generate ~/.config/i3/config
<Esc>   No, I will use the defaults
```

Let the wizard generate the config file. You'll then face another dilemma:

```
Please choose either:

--> <Win>  Win as default modifier
    <Alt>  Alt as default modifier

Afterwards, press
```

Use the `Win` and `Alt` keys to switch between the modifiers and choose one of them.
In this tutorial I use `Alt` as the default modifier.

It would be good to know some basic shortcuts before starting X.

  * `modkey + return` to start a terminal.
  * `modkey + shift + q` to close current window.
  * `modkey + f` switches the active window to fullscreen view.
  * `modkey + d` for dmenu.
  * `modkey + shift + e` to kill i3 session.
  * `modkey + shift + r` to restart i3 session.
  * `modkey + shift + c` to reload the config

##Changing i3 Modifier Key(s)

It is easy to change or add i3 modifiers. Right at the top of the `~/.config/i3/config` file you'll see:

`Alt` as the default modifier:

```
set $mod Mod1
```

Or `Win` as the default modifier:

```
set $mod Mod4
```

You can also configure a secondary modifier assigning it to a variable (eg. $ms):

```
set $mod Mod1
set $ms Mod4
```
