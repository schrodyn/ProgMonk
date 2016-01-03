##Using workspaces

Workspaces are an easy way to group a set of windows. By default, you are on the first workspace,
as the bar on the bottom left indicates. To switch to another workspace, press `$mod+num` where
`num` is the number of the workspace you want to use. If the workspace does not exist yet,
it will be created.

A common paradigm is to put the web browser on one workspace, communication applications
(`mutt`, `irssi`, …) on another one, and the ones with which you work, on the third one.
Of course, there is no need to follow this approach.

If you have multiple screens, a workspace will be created on each screen at startup.

##Moving windows to workspaces

To move a window to another workspace, simply press `$mod+Shift+num` where `num` is
(like when switching workspaces) the number of the target workspace. Similarly to switching workspaces, the target workspace will be created if it does not yet exist.

##Resizing

The easiest way to resize a container is by using the mouse: Grab the border and move it to the wanted size.
Also you can resize windows using your keyboard. But I am not so geek maniac.

##Floating

Floating mode is the opposite of tiling mode. The position and size of a window are not managed
automatically by i3, but manually by you. Using this mode violates the tiling paradigm but can be
useful for some corner cases like "Save as" dialog windows, or toolbar windows
(GIMP or similar). Those windows usually set the appropriate hint and are opened in floating mode
by default.

You can toggle floating mode for a window by pressing `$mod+Shift+Space`. By dragging the window’s titlebar with your mouse you can move the window around. By grabbing the borders and moving them you can resize the window. Another way to resize floating windows using the mouse is to right-click on the titlebar and drag.

Floating windows are always on top of tiling windows.

##Floating Modifier

You can also move and resize windows by using the `floating_modifier`.
To move floating windows with your mouse, you can either grab their titlebar or configure the so
called floating modifier which you can then press and click anywhere in the window itself to move it.
The most common setup is to use the same key you use for managing windows (`Mod1` for example).
Then you can press `Mod1`, click into a window using your left mouse button, and drag it to the
position you want.

Syntax:

```
floating_modifier <Modifier>
```

Example:

```
floating_modifier Mod1
```

##How can we move focus between tiling and floating windows?

It is defined in i3 config file (by default) as:

```
# toggle tiling / floating
bindsym $mod+Shift+space floating toggle

# change focus between tiling / floating windows
bindsym $mod+space focus mode_toggle
```

So to move the focus from floating to tiled windows, use `$mod+Shift+space`!

You can move any window from floating to tiled with `floating disable` (or toggle between states with `floating toggle`).
