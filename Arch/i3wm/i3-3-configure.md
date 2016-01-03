##VIM-like keyboard

If you are a vim user, you may want to edit `~/.config/i3/config`
to use VI’s default `hjkl` keys for changing
focus instead of i3’s default `jkl;`

To accomplish this task comment lines:

```
bindsym $mod+j focus left
bindsym $mod+k focus down
bindsym $mod+l focus up
bindsym $mod+semicolon focus right
```

And add this:

```
bindsym $mod+h focus left
bindsym $mod+j focus down
bindsym $mod+k focus up
bindsym $mod+l focus right
```
