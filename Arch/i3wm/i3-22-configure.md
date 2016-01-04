##Resizing containers/windows

If you want to resize containers/windows using your keyboard, you can use the `resize` command:

Syntax:

```
resize grow|shrink <direction> [<px> px [or <ppt> ppt]]
resize set <width> [px] <height> [px]
```

Direction can either be one of `up`, `down`, `left` or `right`. Or you can be less specific and use
`width` or `height`, in which case i3 will take/give space from all the other containers.
The optional pixel argument specifies by how many pixels a **floating container** should be grown or
shrunk (the default is 10 pixels). The ppt argument means percentage points and specifies by how many percentage points a **tiling container** should be grown or shrunk (the default is 10 percentage points). Note that `resize set` will only work for floating containers.

It's recommended to use the resize command inside a so called `mode`:

Example: Configuration file, defining a mode for resizing

```
mode "resize" {
        # These bindings trigger as soon as you enter the resize mode

        # Pressing left will shrink the window’s width.
        # Pressing right will grow the window’s width.
        # Pressing up will shrink the window’s height.
        # Pressing down will grow the window’s height.
        bindsym j           resize shrink width 10 px or 10 ppt
        bindsym k           resize grow height 10 px or 10 ppt
        bindsym l           resize shrink height 10 px or 10 ppt
        bindsym semicolon   resize grow width 10 px or 10 ppt

        # same bindings, but for the arrow keys
        bindsym Left        resize shrink width 10 px or 10 ppt
        bindsym Down        resize grow height 10 px or 10 ppt
        bindsym Up          resize shrink height 10 px or 10 ppt
        bindsym Right       resize grow width 10 px or 10 ppt

        # back to normal: Enter or Escape
        bindsym Return mode "default"
        bindsym Escape mode "default"
}

# Enter resize mode
bindsym $mod+r mode "resize"
```

Example 2 - setting urxvt size to 640x480:

```
for_window [class="urxvt"] resize set 640 480
```

##Jumping to specific windows

Often when in a multi-monitor environment, you want to quickly jump to a specific window. For example, while working on workspace 3 you may want to jump to your mail client to email your boss that you’ve achieved some important goal. Instead of figuring out how to navigate to your mail client, it would be more convenient to have a shortcut. You can use the `focus` command with criteria for that.

Syntax:

```
[class="class"] focus
[title="title"] focus
```

Examples:

```
# Get me to the next open VIM instance
bindsym $mod+a [class="urxvt" title="VIM"] focus
```

##Window title format

By default, i3 will simply print the X11 window title. Using `title_format`, this can be customized by setting the format to the desired output. This directive supports *Pango markup* and the following placeholders which will be replaced:

  * %title

  The X11 window title (`_NET_WM_NAME` or `WM_NAME` as fallback).

  * %class

  The X11 window class (second part of `WM_CLASS`). This corresponds to the `class` criterion.

  * %instance

  The X11 window instance (first part of `WM_CLASS`). This corresponds to the `instance` criterion.

Using the `for_window` directive, you can set the title format for any window based on `command_criteria`.

Syntax:

```
title_format <format>
```

Examples:

```
# give the focused window a prefix
bindsym $mod+p title_format "Important | %title"

# print all window titles bold
for_window [class=".*"] title_format "<b>%title</b>"

# print window titles of firefox windows red
for_window [class="(?i)firefox"] title_format "<span foreground='red'>%title</span>"
```
