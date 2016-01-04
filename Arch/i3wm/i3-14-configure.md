##Variables

If you want to save yourself some typing and be able to change the modifier you use later,
variables can be handy.

Syntax:

```
set $<name> <value>
```

Example:

```
set $m Mod1
bindsym $m+Shift+r restart
```

Variables are directly replaced in the file when parsing. Variables expansion is not recursive
so it is not possible to define a variable with a value containing another variable.

If you need a more dynamic configuration you should create a little script which generates a configuration file and run it before starting i3 (for example in your `~/.xsession` file).

##Automatically putting clients on specific workspaces

To automatically make a specific window show up on a specific workspace, you can use an assignment. You can match windows by using any criteria.

It is recommended that you match on **window classes** (and instances, when appropriate)
instead of **window titles** whenever possible because some applications first create their window,
and then worry about setting the correct title.

Assignments are processed by i3 in the order in which they appear in the config file. The first one which matches the window wins and later assignments are not considered.

Syntax:

```
assign <criteria> [→] [workspace] <workspace>
```

Examples:

```
# Assign URxvt terminals to workspace 2
assign [class="URxvt"] 2

# Same thing, but more precise (exact match instead of substring)
assign [class="^URxvt$"] 2

# Same thing, but with a beautiful arrow :)
assign [class="^URxvt$"] → 2

# Assignment to a named workspace
assign [class="^URxvt$"] → work

# Start urxvt -name irssi
assign [class="^URxvt$" instance="^irssi$"] → 3
```

Note that the arrow is not required, it just looks good :-). If you decide to use it, it has to be a
UTF-8 encoded arrow, not `->` or something like that.

To get the class and instance, you can use `xprop`. After clicking on the window, you will see the
following output:

xprop:

```
WM_CLASS(STRING) = "irssi", "URxvt"
```

The first part of the WM_CLASS is the instance ("irssi" in this example), the second part is the class ("URxvt" in this example).

Start iceweasel on workspace 3 (once):

```
# Start iceweasel on workspace 3, then switch back to workspace 1
# (Being a command-line utility, i3-msg does not support startup notifications,
#  hence the exec --no-startup-id.)
# (Starting iceweasel with i3’s exec command is important in order to make i3
#  create a startup notification context, without which the iceweasel window(s)
#  cannot be matched onto the workspace on which the command was started.)
exec --no-startup-id i3-msg 'workspace 3; exec iceweasel; workspace 1'
```

##Automatically starting applications on i3 startup

By using the `exec` keyword outside a keybinding, you can configure which commands will be performed
by i3 on initial startup. `exec` commands will not run when restarting i3, if you need a command to
run also when restarting i3 you should use the `exec_always` keyword. These commands will be run in order.

Syntax:

```
exec [--no-startup-id] <command>
exec_always [--no-startup-id] <command>
```

Examples:

```
exec chromium
exec_always ~/my_script.sh

# Execute the terminal emulator urxvt, which is not yet startup-notification aware.
exec --no-startup-id urxvt
```

The `--no-startup-id` parameter disables startup-notification support for this
particular `exec` command. With startup-notification, i3 can make sure that a window appears
on the workspace on which you used the `exec` command. Also, it will change the X11 cursor to watch
(a clock) while the application is launching. So, if an application is not startup-notification aware
(most GTK and Qt using applications seem to be, though), you will end up with a
watch cursor for 60 seconds.
