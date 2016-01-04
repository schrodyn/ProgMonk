##Popups during fullscreen mode

When you are in fullscreen mode, some applications still open popup windows
(take Xpdf for example). This is because these applications may not be aware that they
are in fullscreen mode (they do not check the corresponding hint). There are three things
which are possible to do in this situation:

  1. Display the popup if it belongs to the fullscreen application only. This is the default and should be reasonable behavior for most users.

  2. Just ignore the popup (don’t map it). This won’t interrupt you while you are in fullscreen. However, some apps might react badly to this (deadlock until you go out of fullscreen).

  3. Leave fullscreen mode.

Syntax:

```
popup_during_fullscreen smart|ignore|leave_fullscreen
```

Example:

```
popup_during_fullscreen smart
```

##Automatic back-and-forth when switching to the current workspace

Assume you are on workspace "1: www" and switch to "2: IM" using mod+2 because somebody sent you a
message. You don’t need to remember where you came from now, you can just press `$mod+2` again to
switch back to "1: www".

Syntax:

```
workspace_auto_back_and_forth yes|no
```

Example:

```
workspace_auto_back_and_forth yes
```

##Focus on window activation

If a window is activated, e.g., via `google-chrome www.google.com`, it may request to take focus.
Since this may not preferable, different reactions can be configured.

Syntax:

```
focus_on_window_activation smart|urgent|focus|none
```

The different modes will act as follows:

  * smart
  This is the default behavior. If the window requesting focus is on an active workspace, it will receive the focus. Otherwise, the urgency hint will be set.

  * urgent
  The window will always be marked urgent, but the focus will not be stolen.

  * focus
  The window will always be focused and not be marked urgent.

  * none
  The window will neither be focused, nor be marked urgent.

##Line continuation

Config files support line continuation, meaning when you end a line in a backslash character (\\), the line-break will be ignored by the parser. This feature can be used to create more readable configuration files.

Examples:

```
bindsym Mod1+f \
fullscreen toggle
```
