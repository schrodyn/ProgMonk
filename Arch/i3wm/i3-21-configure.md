#Commands

Commands are what you bind to specific keypresses. You can also issue commands at runtime
without pressing a key by using the IPC interface. An easy way to do this is to use
the `i3-msg` utility:

Example:

```
# execute this on your shell to make the current container borderless
i3-msg border none
```

Commands can be chained by using `;` (a semicolon). So, to move a window to a specific workspace and immediately switch to that workspace, you can configure the following keybinding:

Example:

```
bindsym $mod+x move container to workspace 3; workspace 3
```

Furthermore, you can change the scope of a command - that is, which containers should be affected
by that command, by using various criteria. The criteria are specified before any command
in a pair of square brackets and are separated by space.

When using multiple commands, separate them by using a `,` (a comma) instead of a semicolon.
Criteria apply only until the next semicolon, so if you use a semicolon to separate commands,
only the first one will be executed for the matched window(s).

Example:

```
# if you want to kill all windows which have the class Firefox, use:
bindsym $mod+x [class="Firefox"] kill

# same thing, but case-insensitive
bindsym $mod+x [class="(?i)firefox"] kill

# kill only the About dialog from Firefox
bindsym $mod+x [class="Firefox" window_role="About"] kill

# enable floating mode and move container to workspace 4
for_window [class="^evil-app$"] floating enable, move container to workspace 4
```

The criteria which are currently implemented are:

  * class

  Compares the window class (the second part of WM_CLASS). Use the special value `focused` to match all windows having the same window class as the currently focused window.

  * instance

  Compares the window instance (the first part of WM_CLASS). Use the special value `focused` to match all windows having the same window instance as the currently focused window.

  * window_role

  Compares the window role (WM_WINDOW_ROLE). Use the special value focused to match all windows having the same window role as the currently focused window.

  * window_type

  Compare the window type (`_NET_WM_WINDOW_TYPE`). Possible values are `normal`, `dialog`, `utility`,
  `toolbar`, `splash`, `menu`, `dropdown_menu`, `popup_menu` and `tooltip`.

  * id

  Compares the X11 window ID, which you can get via `xwininfo` for example.

  * title

  Compares the X11 window title (`_NET_WM_NAME` or `WM_NAME` as fallback). Use the special value `focused`
  to match all windows having the same window title as the currently focused window.

  * workspace

  Compares the workspace name of the workspace the window belongs to. Use the special value `focused` to
  match all windows in the currently focused workspace.

The criteria `class`, `instance`, `role`, `title`, `workspace` are actually regular expressions (PCRE). See `pcresyntax(3)` or `perldoc perlre` for information on how to use them.

##Sticky floating windows

If you want a window to stick to the glass, i.e., have it stay on screen even if you switch to another workspace, you can use the `sticky` command. For example, this can be useful for notepads,
a media player or a video chat window.

Note that while any window can be made sticky through this command, it will only take effect if the window is floating.

Syntax:

```
sticky enable|disable|toggle
```
Examples:

```
# make a terminal sticky that was started as a notepad
for_window [instance=notepad] sticky enable
```

##Changing (named) workspaces/moving to workspaces

To change to a specific workspace, use the `workspace` command, followed by the number or name of the workspace. To move containers to specific workspaces, use `move container to workspace`.

Syntax:

```
workspace next|prev|next_on_output|prev_on_output
workspace back_and_forth
workspace <name>
workspace number <name>

move [window|container] [to] workspace <name>
move [window|container] [to] workspace number <name>
move [window|container] [to] workspace prev|next|current
```

Examples:

```
bindsym $mod+1 workspace 1
bindsym $mod+2 workspace 2
bindsym $mod+3 workspace 3:<span foreground="red">vim</span>
...

bindsym $mod+Shift+1 move container to workspace 1
bindsym $mod+Shift+2 move container to workspace 2
...

# switch between the current and the previously focused one
bindsym $mod+b workspace back_and_forth
bindsym $mod+Shift+b move container to workspace back_and_forth

# move the whole workspace to the next output
bindsym $mod+x move workspace to output right

# move firefox to current workspace
bindsym $mod+F1 [class="Firefox"] move workspace current
```

##Named workspaces

Workspaces are identified by their name. So, instead of using numbers in the workspace command,
you can use an arbitrary name:

Example:

```
bindsym $mod+1 workspace mail
...
```

If you want the workspace to have a number **and** a name, just prefix the number, like this:

Example:

```
bindsym $mod+1 workspace 1: mail
bindsym $mod+2 workspace 2: www
...
```

Note that the workspace will really be named "1: mail". i3 treats workspace names beginning with
a number in a slightly special way. Normally, named workspaces are ordered the way they appeared.
When they start with a number, i3 will order them numerically. Also, you will be able to use
`workspace number 1` to switch to the workspace which begins with number 1,
regardless of which name it has.

##Renaming workspaces

You can rename workspaces. This might be useful to start with the default numbered workspaces,
do your work, and rename the workspaces afterwards to reflect what’s actually on them. You can also
omit the old name to rename the currently focused workspace.

Syntax:

```
rename workspace <old_name> to <new_name>
rename workspace to <new_name>
```

Examples:

```
i3-msg 'rename workspace 5 to 6'
i3-msg 'rename workspace 1 to "1: www"'
i3-msg 'rename workspace "1: www" to "10: www"'
i3-msg 'rename workspace to "2: mail"'
bindsym $mod+r exec i3-input -F 'rename workspace to "%s"' -P 'New name: '
```
