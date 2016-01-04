#Configuring i3bar

The bar at the bottom of your monitor is drawn by a separate process called `i3bar`.
Having this part of "the i3 user interface" in a separate process has several advantages:

  1. It is a modular approach. If you don’t need a workspace bar at all, or if you prefer a
  different one (dzen2, xmobar, maybe even gnome-panel?), you can just remove the i3bar
  configuration and start your favorite bar instead.

  2. It follows the UNIX philosophy of "Make each program do one thing well". While i3 manages your windows well, i3bar is good at displaying a bar on each monitor (unless you configure it otherwise).

  3. It leads to two separate, clean codebases. If you want to understand i3, you don’t need to bother with the details of i3bar and vice versa.

That said, i3bar is configured in the same configuration file as i3. This is because it is tightly coupled with i3 (in contrary to i3lock or i3status which are useful for people using other window managers).

Configuring your workspace bar starts with opening a `bar` block. You can have multiple bar blocks to use different settings for different outputs (monitors):

Example:

```
bar {
    status_command i3status
}
```

##i3bar command

By default i3 will just pass `i3bar` and let your shell handle the execution, searching your `$PATH` for a correct version. If you have a different `i3bar` somewhere or the binary is not in your $PATH you can tell
i3 what to execute.

The specified command will be passed to `sh -c`, so you can use globbing and have to have correct quoting etc.

Syntax:

```
i3bar_command <command>
```

Example:

```
bar {
    i3bar_command /home/user/bin/i3bar
}
```

##Statusline command

i3bar can run a program and display every line of its `stdout` output on the right hand side of the bar.
This is useful to display system information like your current IP address, battery status or date/time.

The specified command will be passed to `sh -c`, so you can use globbing and have to have correct
quoting etc.

Syntax:

```
status_command <command>
```

Example:

```
bar {
    status_command i3status --config ~/.i3status.conf

    # For dash(1) users who want signal handling to work:
    status_command exec ~/.bin/my_status_command
}
```

##Display mode

You can either have i3bar be visible permanently at one edge of the screen (`dock` mode) or
make it show up when you press your modifier key (`hide` mode). It is also possible to force
i3bar to always stay hidden (`invisible` mode). The modifier key can be configured using the
modifier option.

The mode option can be changed during runtime through the `bar mode` command. On reload the mode will be reverted to its configured value.

The hide mode maximizes screen space that can be used for actual windows. Also, i3bar sends the `SIGSTOP`
and `SIGCONT` signals to the statusline process to save battery power.

In order to control whether i3bar is hidden or shown in hide mode, there exists the `hidden_state`
option, which has no effect in `dock` mode or `invisible` mode. It indicates the current `hidden_state` of
the bar: (1) The bar acts like in normal hide mode, it is hidden and is only unhidden in case of urgency hints or by pressing the modifier key (`hide` state), or (2) it is drawn on top of the currently visible workspace (`show` state).

Like the mode, the `hidden_state` can also be controlled through i3, this can be done by using the `bar hidden_state` command.

The default mode is dock mode; in hide mode, the default modifier is Mod4 (usually the windows key). The default value for the hidden_state is hide.

Syntax:

```
mode dock|hide|invisible
hidden_state hide|show
modifier <Modifier>
```

Example:

```
bar {
    mode hide
    hidden_state hide
    modifier Mod1
}
```
