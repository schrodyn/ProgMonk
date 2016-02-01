#A Mixture of Stuff

For most of us programmers, however, the basic smattering of NCurses 
functions presented in the previous chapters is enough to create more 
visually interesting programs than by just using the standard C I/O 
functions. This chapter contains a mélange of those leftover functions 
not covered earlier.

#Adios, Cursor

Sometimes the blinking (or not) cursor is a benefit, and sometimes it just
gets in the way. When it gets in the way, NCurses lets you turn it off, 
hiding the cursor from view. Here's the function:

```c
curs_set(n)
```

The `curs_set()` function is how NCursers controls the cursor. The value
of n can be 0, 1, or 2: 0 makes the cursor invisible; 1 sets the cursor to
 normal mode; 2 sets the cursor to a *very* visible mode that must be seento be believed.

Note that `curs_set()` works only on terminals that support 
cursor-visibility options. If your terminal can't reset the cursor, 
such as when the CURSSET.C program doesn't seem to do anything, check for 
the availability of the `curs_set()` function:

```c
if( curs_set(0) == ERR)
	something_bad();
```

#Line Drawing

NCurses has support for a variety of line-drawing and boxing functions,
allowing you to spruce up your plain-text screen with some 
almost-graphical lines.

The quality of the lines depends on the terminal. Sometimes the ASCII 
text characters such as | - and + are used for drawing. Sometimes the 
terminal may use extended ASCII or special line-drawing characters that 
look quite good on the screen. And sometimes you can specify which 
characters you want to use for drawing.

#Boxing Windows

One way to make a window stand out, especially when it's smaller than 
the screen, is to draw a box around it. NCurses has a handy `box()` 
function to help make that possible:

```c
box(win,v_char,h_char)
```

`win` is the window to box. `v_char` is the character to use when drawing
the vertical (up-down) lines around the window; `h_char` is the character 
for the horizontal (left-right) lines. If you put zero for `v_char` or 
`h_char` (or both), default characters are chosen as defined in the 
NCURSES.H header file.

*helpmenu3.c*

```c
#include <ncurses.h>

void showhelp(void);

WINDOW *help;

int main(void)
{
    int ch;

    initscr();
    refresh();

	/* build help menu */
    if( (help = newwin(10,30,4,26)) == NULL) {
        addstr("Unable to allocate window memory\n");
        endwin();
        return 1;
    }

    mvwaddstr(help, 1,7,"Help Menu Screen");
    mvwaddstr(help, 5,3,"Press the ~ key to quit");
    mvwaddstr(help, 8,4,"Press ENTER to go back");
    box(help,0,0);

	/* now start the program loop */
    addstr("Typer Program\n");
    addstr("Press + for help:\n\n");
    refresh();
    noecho();
    do {
        ch = getch();
        refresh();
        if(ch == '+')
            showhelp();
        else
            addch(ch);
    } while(ch != '~');
}

void showhelp(void)
{
    touchwin(help);			/* force update */
    wrefresh(help);		
    getch();			/* wait for key press */
    touchwin(stdscr);			/* forces character update */
    refresh();
}
```

#Building Better Boxes

It's possible to set the characters used to draw the box yourself.

*box.c*

```c
#include <ncurses.h>

int main(void)
{
    initscr();

    box(stdscr,'*','*');
    refresh();
    getch();

    endwin();
    return 0;
}
```

This program draws a box around the standard screen. The asterisk 
character `*` is used to draw the horizontal and vertical lines.

Asterisks are used to draw the box. Note that the corners of the box may 
still be + characters or perhaps specific corner graphics symbols.

If you want to replace the characters in the box's corner, you'll have to use the `border()` function instead:

```c
border(left,right,top,bot,uleft,uright,lleft,lright)
```

Note right away that `border()` is specific to the standard screen, 
`stdscr`. There is a related, `wborder()` function where the first 
argument is `win`, for the window to be boxed. The arguments in 
`border()` are characters to place in one of the eight positions of the 
box: `left`, `right`, `top`, and `bot` are the
sides of the box; `uleft`, `uright`, `lleft`, and `lright` are the 
corners.

#We Control the Horizontal and the Vertical

For plain-on straight lines - lines from here to there but not diagonal 
or weird angles - NCurses has the following functions:

```c
hline(ch,n)
vline(ch,n)
```

The `hline()` and `vline()` functions draw a horizontal or vertical line
from the cursor's current position (right or down, respectively). 
The line is drawn using character `ch` or using the standard line-
drawing character when `ch` is zero. n sets the length of the line in 
characters.

Remember: The line is drawn from the cursor's current position. 
And neither function, `hline()` nor `vline()`, changes the cursor's 
position. 

*steps.c*

```c
#include <ncurses.h>

int main(void)
{
    int y,x,maxy,maxx;

    initscr();

    getmaxyx(stdscr,maxy,maxx);

    for(y=x=0; y<maxy; y++,x+=2) {
        move(y,x);
        hline(0, maxx-x);
        vline(0, maxy-y);
    }
    refresh();
    getch();

    endwin();
    return 0;
}
```

As with many other screen-output functions, there are `mv` and `w` 
prefixes for the `hline()` and `vline()` function.
