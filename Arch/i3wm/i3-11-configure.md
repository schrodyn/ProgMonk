
##Keyboard bindings

A keyboard binding makes i3 execute a command (see below) upon pressing a specific key.
i3 allows you to bind either on keycodes or on keysyms.

  * A keysym (key symbol) is a description for a specific symbol, like "a" or "b", but also more
  strange ones like "underscore" instead of "\_". To get the current mapping of your keys, use
  `xmodmap -pke`. To interactively enter a key and see what keysym it is configured to, use `xev`.

  * Keycodes do not need to have a symbol assigned (handy for custom vendor hotkeys on some notebooks)
  and they will not change their meaning as you switch to a different keyboard layout (when using `xmodmap`).

Syntax:

```
bindsym [--release] <keysym> command
bindcode [--release] <keycode> command
```

Examples:

```
# Fullscreen
bindsym $mod+f fullscreen toggle

# Restart
bindsym $mod+Shift+r restart

# Notebook-specific hotkeys
bindcode 214 exec --no-startup-id /home/michael/toggle_beamer.sh
```

If the `--release` flag is given, it will run when the key button is released.

##Mouse bindings

A mouse binding makes i3 execute a command upon pressing a specific mouse button in the scope of
the clicked container. You can configure mouse bindings in a similar way to key bindings.

Syntax:

```
bindsym [--release] [--border] [--whole-window] [<Modifiers>+]button<n> command
```

By default, the binding will only run when you click on the titlebar of the window. If the `--release`
flag is given, it will run when the mouse button is released.

If the `--whole-window` flag is given, the binding will also run when any part of the window is clicked,
with the exception of the border. To have a bind run when the border is clicked, specify
the `--border` flag.

Examples:

```
# The middle button over a titlebar kills the window
bindsym --release button2 kill

# The middle button and a modifer over any part of the window kills the window
bindsym --whole-window $mod+button2 kill

# The right button toggles floating
bindsym button3 floating toggle
bindsym $mod+button3 floating toggle
```
