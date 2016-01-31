#Clearing and Zapping

There is a difference between clearing a chunk of the screen and 
deleting text in NCurses. That difference is that clearing merely 
erases, replacing the text with blank characters. Deleting, on the other 
hand, removes the text and tightens up the hole, which is demonstrated 
with the `delch()` and `deleteln()`.

#Commands to Erase Chunks of the Screen

NCurses sports four functions to clear or erase any chunk of the screen:

```
erase()
clear()
clrtobot()
clrtoeol()
```

The `erase()` and `clear()` functions do pretty much the same thing: 
clear the screen. Between the two, the `clear()` function is more 
thorough, though it has more internal overhead.

The `clrtobot()` function clears from the cursor's current position 
to the bottom of the screen. And `clrtoeol()` clears from the cursor's 
position to the end of the current line. Simple.

There are no NCurses commands to clear the top part of the screen, so 
no counterpart to `clrtobot()` exists. Likewise, no command exists to 
erase text from the cursor's position to the start of a line. Even so, 
it's possible to code these commands yourself, should you need them.


