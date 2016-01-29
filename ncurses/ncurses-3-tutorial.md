#Basic I/O, the NCurses Way

NCurses allows you full control over terminal screen (or window) but only if you heed its rules! You must use NCurses' own I/O functions to display 
text as well as read input from the keyboard.

The majority of NCurses programs have the same basic skeleton, which looks
something like this:

```
#include <ncurses.h>
int main(void)
{
	initscr(); /* Initialize ncurses */

	/* i/o and other programming done here */

	endwin(); /* Properly close ncurses */
	return 0; /* cough up return value for the shell */
}
```

You must include the NCURSES.H header file so that the compiler doesn't
choke on your NCurses functions.

Please note that including the NCURSES.H header file does not
automatically link in the NCurses library. No, you must do that with the
`-lncurses` switch when you compile.

The NCurses header file does a few nifty tricks. First, it automatically
includes the following other header files:

```
stdio.h
unctrl.h
stdarg.h
stddef.h
```

Therefore, there is no need to re-include these header files in your 
source code. In fact, if you do, you may end up slowing things down and 
creating files much larger than they need to be. So if you're tempted 
to do this:

much larger than they need to be. So if you're tempted to do this:

```c
#include <stdio.h>
#include <ncurses.h>
```

Do only this instead:

```
#include <ncurses.h>
```

If you have the time, peruse the header file, which can be found at 
`/USR/INCLUDE/NCURSES.H`.

#The initscr() Function

The `initscr()` function initializes NCurses. It does not clear the 
terminal screen. Instead, it sets up internal memory structures and 
interfaces between the NCurses functions and your computer's terminal 
I/O thingy.

Two important items `initscr()` creates are called the *standard screen* and the current screen. Both of these are internal structures used by 
NCurses to efficiently display information on the terminal screen.

The standard screen, or `stdscr`, is the default output window for NCurses. The `initscr()` function creates the standard screen
and uses the variable `stdscr` to reference it.

The standard screen, however, is not the same as the terminal window, and
stuff you write to the standard screen doesn't appear on the terminal 
window.

The `refresh()` command is required to update text on the terminal 
window, letting you see what NCurses has done. What `refresh()` does is 
to check for new text has been output by NCurses and update that text 
on the current screen, or `curscr`.

The current screen is NCurses's internal representation of what is 
believed to be on the terminal screen, or what the user sees. 
The `refresh()` function is responsible for updating the current screen, 
which then updates what is shown to the user.

In addition to the standard screen and current screen, there is something
called the *virtual screen*. The virtual screen exists for efficiency's 
sake. It contains updated information, only those items changed or 
*touched* in a certain window and which are waiting to be updated on 
the current screen.

Internally, the `refresh()` function consists of two commands. 
The first is `wnoutrefresh()`, which updates only the changed portions 
of a window or the standard screen to the virtual screen. 
The second half of the  `refresh()` function is `doupdate()`, which makes
the current screen match the virtual screen's updates. 
Again, this is done to keep text output efficient.

It's merely the internal mechanisms by which NCurses works.

Note that the standard C output functions - `putc()`, `puts()`, 
`printf()`,
and so on - do output text when NCurses is active, though by doing so
NCurses would be confused about what's on the display. Therefore, it's 
better not to use such output functions while NCurses is actively 
outputting text.

#Writing Text

Here are three popular NCurses text output functions:

```c
addch(ch);
addstr(*str);
printw(format,var[,var...]);
```

The `addch()` function places (or adds) a single character to the
display. 

`printw()` is the NCurses version of the `printf()` function. It outputs 
a formatted string to the display.

#Pausing for a Side-trip

NCurses also features a variable pausing function, `napms()`. Such a cute
name!

```c
napms(ms)
```

The `napms()` function pauses program execution for ms milliseconds. So
the statement:

```c
napms(1000);
```

causes program execution to pause for one whole second.

##Blurping Text

```c
#include <ncurses.h>

int main(void){
    char text1[] = "Oh give me a clone!\n";
    char text2[] = "Yes a clone of my own!";

    initscr();
    addstr(text1);  /* add the first string */
    addstr(text2);  /* add the second string */
    move(2,0);      /* cursor to row 3, column 1 */
    addstr("With the Y position changed to the X");
    refresh();      /* display the result */
    getch();

    endwin();
    return 0;
}
```

##The move() Function

There are many cursors in NCurses. For example, each window has its own
cursor and that cursor location stays the same in each window regardless 
of what happens to the cursor in other windows.

the `move()` function moves the cursor's location. Here
is the format of the `move()` function:

```c
move(y,x)
```

y is a row value, going up and down the screen.
x is a column value, going left to right across the screen.

The upper-left corner of the screen is coordinate 0,0. And, of course, the total number of rows and columns depends on your terminal configuration, 
though there is a way to discover it, as I'll show you later.

It's important to remember that the `move()` function puts the row
first, or Y, X (if you're used to Cartesian coordinates). Think row, 
column as opposed to X, Y.

##The Old Formatted Text Trick

In NCurses, the `printw()` function can be used just like `printf()` to 
displaystrings of text, variables, formatted text, and all that sort of 
junk. If you know `printf()`, you also know `printw()` - but remember 
that in NCurses the `printw()` function is the one you want to use.

```c
#include <ncurses.h>

int main(void){
    int yoda = 874;
    int ss = 55;

    initscr();
    printw("Yoda is %d years old\n", yoda);
    printw("He has collected %d years\n", yoda-ss);
    printw("of Social Security.");
    refresh();
    getch();

    endwin();
    return 0;
}
```

#Reading Text

the really cool stuff - especially if you're weary of a lack of 
single-character input functions in C - comes with 
reading text in from the keyboard.

Here are some NCurses console input functions:

```c
getch()
getstr(*str)
getnstr(*str,length)
scanw(format,var[,var...])
```

The `getch()` function returns a single character from the console.

The `getstr()` and `getnstr()` functions read in a string of text from 
the console. Of the two, use `getnstr()`, which measures input and is 
therefore more secure than the straight `getstr()` function.
Finally, `scanw()` works just like the standard I/O function `scanf()`. 

##The Silly Typewriter Program

```c
#include <ncurses.h>

int main(void){
    char ch;

    initscr();
    addstr("Type a few lines of text\n");
    addstr("Press ~ to quit\n");
    refresh();

    while( (ch = getch()) != '~');

    endwin();
    return 0;
}
```

Note the while loop here. Basically, it's saying to read all character 
input until the `~` character is received; then bail.

Note, however, that `getch()` in its natural state does display the text 
you've input; there is no need to use `refresh()` when `getch()` is 
reading text.

The `getch()` function doesn't always display text as it's typed. It's 
possible in NCurses to turn off echo on input. 

##Consuming a String Whole

```c
#include <ncurses.h>

int main(void)
{
	char first[24];
	char last[32];
	
	initscr();
	addstr("What is your first name? ");
	refresh();
	getstr(first);

	addstr("What is your last name? ");
	refresh();
	getstr(last);

	printw("Pleased to meet you, %s %s!",first,last);
	refresh();
	getch();

	endwin();
	return 0;
}
```

for security reasons, in your real programs, please use the `getnstr()` 
function instead.


