##Workspace buttons

Specifies whether workspace buttons should be shown or not. This is useful if you want to display a statusline-only bar containing additional information.

The default is to show workspace buttons.

Syntax:

```
workspace_buttons yes|no
```

Example:

```
bar {
    workspace_buttons no
}
```

##Strip workspace numbers

Specifies whether workspace numbers should be displayed within the workspace buttons.
This is useful if you want to have a named workspace that stays in order on the bar according
to its number without displaying the number prefix.

When strip_workspace_numbers is set to yes, any workspace that has a name of the form
"[n]:[NAME]" will display only the name. You could use this, for instance, to display Roman numerals
rather than digits by naming your workspaces to "1:I", "2:II", "3:III", "4:IV", …

The default is to display the full name within the workspace button.

Syntax:

```
strip_workspace_numbers yes|no
```

Example:

```
bar {
    strip_workspace_numbers yes
}
```

##Colors

As with i3, colors are in HTML hex format (`#rrggbb`). The following colors can be configured
at the moment:

  * background
  Background color of the bar.

  * statusline
  Text color to be used for the statusline.

  * separator
  Text color to be used for the separator.

  * focused_workspace
  Border, background and text color for a workspace button when the workspace has focus.

  * active_workspace

  Border, background and text color for a workspace button when the workspace is active (visible) on some output, but the focus is on another one. You can only tell this apart from the focused workspace when you are using multiple monitors.

  * inactive_workspace

  Border, background and text color for a workspace button when the workspace does not have focus and is not active (visible) on any output. This will be the case for most workspaces.

  * urgent_workspace

  Border, background and text color for a workspace button when the workspace contains a window with the urgency hint set.

  * binding_mode

  Border, background and text color for the binding mode indicator. If not used, the colors will be taken from `urgent_workspace`.

Syntax:

```
colors {
    background <color>
    statusline <color>
    separator <color>

    <colorclass> <border> <background> <text>
}
```

Example (default colors):

```
bar {
    colors {
        background #000000
        statusline #ffffff
        separator #666666

        focused_workspace  #4c7899 #285577 #ffffff
        active_workspace   #333333 #5f676a #ffffff
        inactive_workspace #333333 #222222 #888888
        urgent_workspace   #2f343a #900000 #ffffff
        binding_mode       #2f343a #900000 #ffffff
    }
}
```
