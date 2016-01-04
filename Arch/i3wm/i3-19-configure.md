##Tray output

i3bar by default provides a system tray area where programs such as NetworkManager, VLC, Pidgin, etc. can place little icons.

You can configure on which output (monitor) the icons should be displayed or you can turn off the functionality entirely.

Syntax:

```
tray_output none|primary|<output>
```

Example:

```
# disable system tray
bar {
    tray_output none
}

# show tray icons on the primary monitor
bar {
    tray_output primary
}

# show tray icons on the big monitor
bar {
    tray_output HDMI2
}
```

Note that when you use multiple bar configuration blocks, either specify `tray_output` primary in all
of them or explicitly specify `tray_output none` in bars which should not display the tray, otherwise the different instances might race each other in trying to display tray icons.

##Tray padding

The tray is shown on the right-hand side of the bar. By default, a padding of 2 pixels is used for the upper, lower and right-hand side of the tray area and between the individual icons.

Syntax:

```
tray_padding <px> [px]
```

Example:

```
# Obey Fitts's law
tray_padding 0
```

##Font

Specifies the font to be used in the bar.

Syntax:

```
font <font>
```

Example:

```
bar {
    font -misc-fixed-medium-r-normal--13-120-75-75-C-70-iso10646-1
    font pango:DejaVu Sans Mono 10
}
```

##Custom separator symbol

Specifies a custom symbol to be used for the separator as opposed to the vertical,
one pixel thick separator.

Syntax:

```
separator_symbol <symbol>
```

Example:

```
bar {
    separator_symbol ":|:"
}
```
