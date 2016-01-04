##Mouse button commands

Specifies a command to run when a button was pressed on i3bar to override the default behavior.
This is useful, e.g., for disabling the scroll wheel action or running scripts that implement
custom behavior for these buttons.

A button is always named `button<n>`, where 1 to 5 are default buttons as follows and higher numbers can be special buttons on devices offering more buttons:

  * button1
  Left mouse button.

  * button2
  Middle mouse button.

  * button3
  Right mouse button.

  * button4
  Scroll wheel up.

  * button5
  Scroll wheel down.

Syntax:

```
bindsym button<n> <command>
```

Example:

```
bar {
    # disable clicking on workspace buttons
    bindsym button1 nop
    # execute custom script when scrolling downwards
    bindsym button5 exec ~/.i3/scripts/custom_wheel_down
}
```

##Bar ID

Specifies the bar ID for the configured bar instance. If this option is missing, the ID is set to `bar-x`, where x corresponds to the position of the embedding bar block in the config file (`bar-0`, `bar-1`, …).

Syntax:

```
id <bar_id>
```

Example:

```
bar {
    id bar-1
}
```

##Position

This option determines in which edge of the screen i3bar should show up.

The default is bottom.

Syntax:

```
position top|bottom
```

Example:

```
bar {
    position top
}
```
