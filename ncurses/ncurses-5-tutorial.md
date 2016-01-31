#Around the Window

One of the things that make NCurses appeal to programmers is the control 
it offers, not only over the text you display but over the location of 
that text as well. The key is moving the cursor. Secondary to that, of 
course, is knowing how big the terminal screen (or, in NCurses, 
the standard screen window) can be, so that you keep all your text 
visible.

#Measuring the Standard Screen

it's an especially bad thing to assume that all terminals follow the 
dimensions of the standard PC text screen's 25 rows by 80 columns.

In NCurses the terminal screen isn't part of the equation. Instead, a 
window is used called the *standard screen*, which coincidentally is 
just the same size and dimensions as the terminal's screen.

#The Size of the Window Is Y by X

To discover the size of any window in NCurses, the following function 
can be used:

```c
getmaxyx(win,y,x)
```

The `getmaxyx()` function returns the dimensions of the window `win` 
in rows and columns, where y is the row and x is the column. That the 
function is named `getmaxyx()` should help you remember that Y, or rows, 
comes first.

The `win` argument is the name of a specific window in NCurses. For the 
terminal screen, you use the standard screen window, which is named 
`stdscr`.

The y and x arguments are int variables (not pointers) that will hold
the maximum number of rows and columns in the named window.

*screensize.c*

```c
#include <ncurses.h>

int main(void){
    int x,y;

    initscr();

    getmaxyx(stdscr,y,x);
    printw("Window size is %d rows and %d columns.\n", y, x);
    refresh();
    getch();

    endwin();
    return 0;
}
```

The point here is to remember that it's a *bad thing* to guess the 
terminal's size. Write your code so that your program uses `getmaxyx()` 
to determine the screen size and save those values in variables; do not 
use constants!

The `getmaxyx()` function can read the dimensions of any NCurses window,
but because the standard screen is special, there are two NCurses 
constants you can use as well:

```
LINES
COLS
```

The value of the LINES constant is equal to the number of rows on the 
standard screen window and, therefore, the terminal screen.

The value of COLS is equal to the number of columns on the standard screen
window, as well as the terminal.

#Moving the Cursor Around

In NCurses, the cursor - that thing that tells curses where to put the 
next character on the screen - is moved around via the `move()` function 
but also by most commands that put text to the screen.

To provide basic cursor control, there is only one command:

```c
move(y,x)
```

There are three more interesting commands that combine moving the cursor 
with slapping down text:

```c
mvaddch(y,x,ch)
mvaddstr(y,x,*str)
mvprintw(y,x,format,arg[...])
```

*moveput.c*

```c
#include <ncurses.h>
#include <string.h>


int main(void){
    initscr();
    char *str = "Hello, I am centered!";
    mvaddstr(LINES/2, (COLS-strlen(str))/2, str);
    refresh();
    getch();

    endwin();
    return 0;
}
```

*corners1.c*

```c
#include <ncurses.h>

int main(void){
    int rows,cols;

    initscr();
    getmaxyx(stdscr, rows,cols);
    rows--;
    cols--;

    move(0,0);      /* UL corner */
    addch('*');
    refresh();
    napms(1000);    /* pause a sec. */

    move(0, cols);  /* UR corner */
    addch('*');
    refresh();
    napms(1000);

    move(rows, 0);  /* LL corner */
    addch('*');
    refresh();
    napms(1000);

    move(rows, cols); /* LR corner */
    addch('*');
    refresh();
    getch();

    endwin();
    return 0;
}
```

Or even shorter

*corners2.c*

```c
#include <ncurses.h>

int main(void){
    int rows,cols;

    initscr();
    getmaxyx(stdscr, rows,cols);
    rows--;
    cols--;

    mvaddch(0, 0, '*');
    refresh();
    napms(1000);    /* pause a sec. */

    mvaddch(0, cols, '*'); /* UR corner */
    refresh();
    napms(1000);

    mvaddch(rows, 0, '*'); /* LL corner */
    refresh();
    napms(1000);

    mvaddch(rows, cols, '*'); /* LR corner */
    refresh();
    getch();

    endwin();
    return 0;
}
```

The output is the same, but you've saved a lot of typing and redundancy.
Note the format for the `mvaddch()` function:

```c
mvaddch(y,x,ch)
```

The row and column come first, and the character to punch in that spot 
goes last. Again, this is just like combining the two functions, 
`move()` and `addch()`.

##Some Fun with mvprintw()

Why do the math when the compiler can do it for you? One gem with
`printf()`, and then, of course, NCurses' `printw()`, is that it can 
justify strings in its output. For example:

```c
printf("%40s", "This is right-justified");
```

The `%s` normally tells `printf()` to replace the `%s` with a string. 
The `%40s`, however, tells `printf()` to set aside 40 spaces in which to 
stick the string. As with numbers, the string is right-justified:

```
________________This is right-justified
```

Now if you prefix the size of the field with a minus sign, you get 
left-justified output:

```c
printf("%-40s", "This is left-justified");
```

The `-40` means to set aside 40 spaces and left-justify the given string.
To wit:

```
This is left-justified_________________
```

*mvdata.c*

```c
#include <ncurses.h>

#define COL1 0
#define COL2 40

int main(void){
    initscr();

    mvprintw(5, COL1, "%30s", "Your name:");
    mvprintw(5, COL2, "%-30s", "Art Grockmeister");

    mvprintw(7, COL1, "%30s", "Your company:");
    mvprintw(7, COL2, "%-30s", "Sterling/Worbletyme");

    mvprintw(9, COL1, "%30s", "Position:");
    mvprintw(9, COL2, "%-30s", "Grand Duke of Finance");

    mvprintw(11, COL1, "%30s", "Date hired:");
    mvprintw(11, COL2, "%-30s", "October 19, 1993");
    refresh();
    getch();

    endwin();
    return 0;
}
```

Here's what the output generally looks like:

```
Your name: 			Art Grockmeister
Your company: 		Sterling/Worbletyme
Position: 			Grand Duke of Finance
Date hired: 		October 19, 1993
```

#Where the Cursor?

Knowing where the cursor is doesn't seem as popular or necessary a 
function as being able to move the cursor. But consider the case 
where a program involves full-screen user input. In that example, 
it helps to have a function that tracks down where the booger is.

The `getyx()` function reads the logical cursor location from the window
*win* (*stdscr* for the standard screen) and puts its row and column 
positions into the variables y and x. As with the move() function, 
the home location 0,0 is the upper-left corner of the screen.

*whereami.c*

```c
#include <ncurses.h>

int main(void){
    char ch='\0';
    int row,col;

    initscr();

    addstr("Type some text; '~' to end:\n");
    refresh();

    while( (ch=getch()) != '~')
            ;

    getyx(stdscr,row,col);
    printw("\n\nThe cursor was at row %d, column %d\n", row, col);
    printw("when you stopped typing.");
    refresh();
    getch();

    endwin();
    return 0;
}
```

