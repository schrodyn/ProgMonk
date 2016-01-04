##Border style for new windows

This option determines which border style new windows will have. The default is `normal`.
Note that `new_float` applies only to windows which are starting out as floating windows, e.g.,
dialog windows, but not windows that are floated later on.

Syntax:

```
new_window normal|none|pixel
new_window normal|pixel <px>
new_float normal|none|pixel
new_float normal|pixel <px>
```

Example:

```
new_window pixel
```

The "normal" and "pixel" border styles support an optional border width in pixels:

Example:

```
# The same as new_window none
new_window pixel 0

# A 3 px border
new_window pixel 3
```

##Hiding vertical borders

You can hide vertical borders adjacent to the screen edges using `hide_edge_borders`. This is useful
if you are using scrollbars, or do not want to waste even two pixels in displayspace.
Default is none.

Syntax:

```
hide_edge_borders none|vertical|horizontal|both
```

Example:

```
hide_edge_borders vertical
```

##Arbitrary commands for specific windows (for_window)

With the `for_window` command, you can let i3 execute any command when it encounters a specific window.
This can be used to set windows to floating or to change their border style, for example.

Syntax:

```
for_window <criteria> <command>
```

Examples:

```
# enable floating mode for all XTerm windows
for_window [class="XTerm"] floating enable

# Make all urxvts use a 1-pixel border:
for_window [class="urxvt"] border pixel 1

# A less useful, but rather funny example:
# makes the window floating as soon as I change
# directory to ~/work
for_window [title="x200: ~/work"] floating enable
```
