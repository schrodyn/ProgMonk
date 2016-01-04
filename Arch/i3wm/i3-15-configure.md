##Changing colors

You can change all colors which i3 uses to draw the window decorations.

Syntax:

```
<colorclass> <border> <background> <text> <indicator>
```

Where colorclass can be one of:

  * **client.focused**
  A client which currently has the focus.

  * **client.focused_inactive**
  A client which is the focused one of its container, but it does not have the focus at the moment.

  * **client.unfocused**
  A client which is not the focused one of its container.

  * **client.urgent**
  A client which has its urgency hint activated.

  * **client.placeholder**
  Background and text color are used to draw placeholder window contents (when restoring layouts).
  Border and indicator are ignored.

  * **client.background**
  Background color which will be used to paint the background of the client window on top of which
  the client will be rendered. Only clients which do not cover the whole area of this window expose the color. Note that this colorclass only takes a single color.

Colors are in HTML hex format (#rrggbb), see the following example:

Examples (default colors):

```
# class                 border  backgr. text    indicator
client.focused          #4c7899 #285577 #ffffff #2e9ef4
client.focused_inactive #333333 #5f676a #ffffff #484e50
client.unfocused        #333333 #222222 #888888 #292d2e
client.urgent           #2f343a #900000 #ffffff #900000
client.placeholder      #000000 #0c0c0c #ffffff #000000

client.background       #ffffff
```

Note that for the window decorations, the color around the child window is the background color, and the border color is only the two thin lines at the top of the window.

##Focus follows mouse

By default, window focus follows your mouse movements. However, if you have a setup where your
mouse usually is in your way (like a touchpad on your laptop which you do not want to disable
completely), you might want to disable `focus follows mouse` and control focus only by using
your keyboard. The mouse will still be useful inside the currently active window
(for example to click on links in your browser window).

Syntax:

```
focus_follows_mouse yes|no
```

Example:

```
focus_follows_mouse no
```
