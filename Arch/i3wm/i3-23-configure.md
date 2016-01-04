##Changing border style

To change the border of the current client, you can use `border normal` to use the normal border (including window title), `border pixel 1` to use a 1-pixel border (no window title) and `border none`
to make the client borderless.

There is also `border toggle` which will toggle the different border styles.

Syntax:

```
border normal|pixel [<n>]
border none|toggle

# legacy syntax, equivalent to "border pixel 1"
border 1pixel
```

Examples:

```
# use window title, but no border
bindsym $mod+t border normal 0
# use no window title and a thick border
bindsym $mod+y border pixel 3
# use neither window title nor border
bindsym $mod+u border none
```

##Scratchpad

There are two commands to use any existing window as scratchpad window.
`move scratchpad` will move a window to the scratchpad workspace. This will make it invisible
until you show it again. There is no way to open that workspace. Instead, when using `scratchpad show`,
the window will be shown again, as a floating window, centered on your current workspace (using
`scratchpad show` on a visible scratchpad window will make it hidden again, so you can have a keybinding
to toggle). Note that this is just a normal floating window, so if you want to "remove it
from scratchpad", you can simple make it tiling again (`floating toggle`).

As the name indicates, this is useful for having a window with your favorite editor always at hand.
However, you can also use this for other permanently running applications which you don’t want
to see all the time: Your music player, alsamixer, maybe even your mail client…?

Syntax:

```
move scratchpad

scratchpad show
```

Examples:

```
# Make the currently focused window a scratchpad
bindsym $mod+Shift+minus move scratchpad

# Show the first scratchpad window
bindsym $mod+minus scratchpad show

# Show the sup-mail scratchpad window, if any.
bindsym mod4+s [title="^Sup ::"] scratchpad show
```

##Nop

There is a no operation command `nop` which allows you to override default behavior.
This can be useful for, e.g., disabling a focus change on clicks with the middle mouse button.

The optional `comment` argument is ignored, but will be printed to the log file for debugging purposes.

Syntax:

```
nop [<comment>]
```

Example:

```
# Disable focus change for clicks on titlebars
# with the middle mouse button
bindsym button2 nop
```

##i3bar control

There are two options in the configuration of each i3bar instance that can be changed during
runtime by invoking a command through i3. The commands `bar hidden_state` and `bar mode` allow setting
the current `hidden_state` respectively mode option of each bar. It is also possible to toggle
between hide state and show state as well as between dock mode and hide mode. Each i3bar instance
can be controlled individually by specifying a bar_id, if none is given, the command is executed for
all bar instances.

Syntax:

```
bar hidden_state hide|show|toggle [<bar_id>]

bar mode dock|hide|invisible|toggle [<bar_id>]
```

Examples:

```
# Toggle between hide state and show state
bindsym $mod+m bar hidden_state toggle

# Toggle between dock mode and hide mode
bindsym $mod+n bar mode toggle

# Set the bar instance with id 'bar-1' to switch to hide mode
bindsym $mod+b bar mode hide bar-1

# Set the bar instance with id 'bar-1' to always stay hidden
bindsym $mod+Shift+b bar mode invisible bar-1
```
