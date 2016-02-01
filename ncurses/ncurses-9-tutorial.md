#Windows, Windows Everywhere!

NCurses is a windowed environment. Nearly all NCurses text output 
functions are window oriented, either directly or via a macro defined in 
the NCURSES.H header file.

The standard screen is only one of many windows your programs can use 
in NCurses. So be prepared for some eye-opening looks.

#Ye Olde Standard Screen

When the `initscr()` function initializes NCurses, it does various 
things (performs duties, sets up memory, and so on). One of the many 
tasks is to create the default window for program output, the standard 
screen, named `stdscr`.

#Commands that Require a Window Argument

You've already experienced commands that use `stdscr` to refer to
the terminal window. Here's a sampling:

```c
getmaxyx(win,row,col)
getyx(win,y,x)
keypad(win,bf)
nodelay(win,bf)
```

Each of these commands requires a `win` or window argument, so that
NCurses knows which window on the screen to refer to. Previous chapters 
in this book use `stdscr` to represent the standard screen, the only 
window NCurses creates. But other window names can be used as well, the 
names of new windows you create yourself.

#The Pseudo Commands

Nearly all text-output functions in NCurses require a `win` argument, 
specifying which window to send the output. As a `win` argument, `stdscr` 
isn't used much, because many of the command text-output functions have 
been customized for use with `stdscr`; those text-output functions exist 
as special macros written in the NCURSES.H file.

For example, the `addch()` function, which adds a character to the 
standard screen, is really a macro. The actual function is named 
`waddch()` and here is its format:

```c
waddch(win,ch);
```

The `addch` macro, or pseudo function, looks like this in NCURSES.H:

```c
#define addch(ch)		waddch(stdscr,ch)
```

So anytime you've used `addch()` in a program, the compiler secretly 
swaps it out with the proper window-based function, `waddch()`, 
specifying `stdscr` as the named window.

Pseudo functions have been used for just about all of the character and 
textoutput functions introduced before.

The window (real) version of any pseudo function has the same name as 
the pseudo function, but the real function is prefixed by a `w`. Also, 
the function's first argument is the name of the window the function 
acts upon.

#The Other Prefix, mv

When you prefix a command with `mv`, the first two arguments are the 
row (y) and column (x) to position the cursor before outputting text. 
Therefore:

```c
addch(`Q');
```

The preceding command outputs a `Q` wherever the cursor currently 
happens to be.

```c
mvaddch(5,10,'Q');
```

The preceding command places a cursor at position 5,10 on the screen.
That's the sixth row, 11th column. Remember that the first row and 
column are numbered zero.

To direct `addch()` to a specific window *and* a specific location, this 
format is used:

```c
mvwaddch(stdscr,5,10,'Q');
```

The `mv` prefix always comes first; after all, the real command is 
`waddch()`, not `addch()`. *But*, inside the parenthesis, the window's 
name comes first, as shown above.

#Making Windows

Creating a new window is simple, thanks to the `newwin()` function:

```c
newwin(rows,cols,y_org,x_org)
```

The `newwin()` function is told the window's size in `rows` and `cols` 
(vertical by horizontal characters) and where the window is located on 
the terminal screen, given that the upper-left corner of the terminal 
is location 0,0.
The range of values for `rows`, `cols`, `y_org`, and `x_org` depends on 
your computer's memory and, of course, screen size. The smallest you can 
go is a one-character window.

When the call to `newwin()` is successful, a pointer to a window 
structure is returned. This structure is declared in your program using 
this format:

```c
WINDOW *name;
```

The `WINDOW` variable is defined in NCURSES.H; you supply the name, 
which is the name you'll use to refer to the new window. When the call 
fails, NULL is returned. (The call fails because of a lack of memory or 
because the window is too large for the screen or doesn't fit entirely 
on the screen.)
And now: the shortcut!

```c
newwin(0,0,0,0);
```

When you use the `newwin()` function with all zeros, a new window is 
created that is the exact same size and location as the standard screen. 
Ta-da!

#The Obligatory New Window Sample Program

The following source code directs NCurses to create two windows on the
screen.

*twowin1.c*

```c
#include <ncurses.h>

int main(void){
    WINDOW *two;

    initscr();

    addstr("This is the original window, stdscr.\n");
    refresh();
    getch();

    two = newwin(0,0,0,0);
    if(two == NULL)
    {
        addstr("Unable to allocate memory for new window.");
        endwin();
        return 1;
    }
    waddstr(two, "This is the new window created!\n");

    getch();
    endwin();
    return 0;
}
```

When the program runs, you'll see the text:

```
This is the original window, stdscr
```

That's the standard screen. Press Enter to see the new window and its 
text. And... *nothing*!

Press Enter to end the program.

The program compiled properly, so there must be some other reason why
the new window didn't display.

Hmmm. You can't see the text. If you recall from your basic knowledge of
NCurses, what is required so that you can see text output?

Yep! It's the `refresh()` function. But for a specific window, you need 
the `wrefresh()` function. Here's the format:

```c
wrefresh(win)
```

The `wrefresh()` function is required to update and display the content 
of a specific window, `win`. (In fact, the `refresh()` function itself 
is merely a macro defined as `wrefresh(stdscr)`.) So all the program is 
missing is a `wrefresh()` to update the new window. The following needs to
 be added:

```c
wrefresh(two);
```

updated source

```c
#include <ncurses.h>

int main(void){
    WINDOW *two;

    initscr();

    addstr("This is the original window, stdscr.\n");
    refresh();
    getch();

    two = newwin(0,0,0,0);
    if(two == NULL)
    {
        addstr("Unable to allocate memory for new window.");
        endwin();
        return 1;
    }
    waddstr(two, "This is the new window created!\n");
    wrefresh(two);

    getch();
    endwin();
    return 0;
}
```

The program now runs as expected, and when you press the Enter key,
you'll see the window two and the text displayed:

```
This is the new window created!
```

What you're seeing above and on the screen is text belonging to the 
window `two`. Though you cannot see the standard screen window's text, 
it's still there, in memory somewhere. What's visible on the screen now 
is only the contents of the new window, `two`.

#Switching between Windows

There is no advantage of having multiple windows unless you can switch
between them. For example, you can have one window (or a set of windows)
detailing help information. You can then display those windows when a user
hits the Help key or some other key on the keyboard.

*helpmenu1.c*

```c
#include <ncurses.h>

void showhelp(void);

WINDOW *help;

int main(void)
{
    int ch;

    initscr();

    /* build help menu */
    if((help = newwin(0,0,0,0)) == NULL){
        addstr("Unable to allocate window memory\n");
        endwin();
        return 1;
    }   

    mvwaddstr(help,6,32,"Help menu Screen");
    mvwaddstr(help,9,28,"Press the ~ key to quit");
    mvwaddstr(help,12,28,"Press ENTER to go back");

    /* now start the program loop */

    addstr("Typer Program\n");
    addstr("Press + for help:\n\n");
    refresh();
    noecho();
    do{
        ch = getch();
        refresh();
        if(ch == '+')
            showhelp();
        else
            addch(ch);
    }while(ch != '~');

    endwin();
    return 0;
}

void showhelp(void)
{
    wrefresh(help);
    getch();            /* wait for key press */

    refresh();
}
```

This example is yet another typing program. Note, however, that
echo is off and, therefore, an `addch()` function is required to display
input. But anyway, when you press the `+` key, a second window help 
appears displaying the help screen. Pressing Enter then returns you to 
the standard screen.

The `showhelp()` function is where the swapping takes place. The function
used to display the help screen is `wrefresh()` function.

Likewise, a `refresh()` function is used to switch back to the standard 
screen. That makes sense, so why doesn't it work?

Well, it does work! The `wrefresh()` function writes only changed text to
the screen. Only new text written to the screen or text
changed - what NCurses refers to as *touched* text - is written to the 
screen when you `wrefresh()`. By working in that manner, NCurses is very 
efficient. But when you want to display an entire window's contents, 
that behavior isn't very helpful.

The solution is to force NCurses to display the entire window. Without 
manually going through and rewriting the thing, you can use the 
`touchwin()` function:

```c
touchwin(win)
```

The `touchwin()` function leads NCurses to believe that every character
location in the window win has been touched or updated since the last 
refresh. Therefore, on the next `refresh()` or `wrefresh()` call, the 
entire window will be written to the screen.

To fix the code, add the following:

```c
touchwin(help);
```

And add the following

```c
touchwin(stdscr);
```

After compiling, you can deftly switch between the windows (using + and
Enter), with everything working as expected.

```c
void showhelp(void)
{
    touchwin(help);
    wrefresh(help);
    getch();            /* wait for key press */

    touchwin(stdscr);
    refresh();
}
```

#Windows of a Smaller Size

There is no reason for the help menu window to be as large as it is. 
Using the `newwin()` function, you can set the size and location of the 
new window to be as large as the screen (with all zeros;) or as tiny as 
one character. Here is the full format for the `newwin()` function again:

```c
newwin(rows,cols,y_org,x_org)
```

Picture below shows a graphical representation of how those arguments 
work.

[here picture]

The values `rows` and `cols` set the size of the new window. The window's
position is relative to the screen, `y_org` and `x_org`, where the *home* 
position is 0,0. Remember that the `newwin()` function fails either when 
not enough memory is available or part of the window hangs off the visible
 screen. I didn't do it in the sample program, but consider using the 
`getmaxyx()` function to determine screen width and height before you set
the new window's size.

#Removing a Window

To remove a single window, such as a new window you created, the 
`delwin()` function is used:

```c
delwin(win)
```

The `delwin()` function removes the window `win`, a window you created
by using the `newwin()` function sometime earlier in your code. 
`delwin()` removes the window's internal structure and memory used by the 
window, but it does not erase the window's screen image. To do that, 
you'll have to `wrefresh()`, and possibly `touchwin()`, another window 
on the screen.

*twowin2.c*

```c
#include <ncurses.h>

int main(void)
{
    WINDOW *two;

    initscr();

    addstr("This is the original window, stdscr.\n");
    refresh();
    getch();

    two = newwin(0,0,0,0);

    if(two == NULL){
        addstr("Unable to allocate memory for new window.");
        endwin();
        return 1;
    }   
    waddstr(two, "This is the new window created!\n");  
    wrefresh(two);
    getch();

    delwin(two);
    addstr("The second window was removed.\n"); 
    refresh();
    getch();

    endwin();
    return 0;
}
```

On some terminals you may need to force NCurses to update the *standard
screen*. That's because the `delwin()` function does not repaint the 
screen after deleting a window. So to update the screen you need another 
refresh; insert a new line before the `refresh()` function:

```c
touchwin(stdscr);
```

That fixes the updating problem.

#Dueling Windows

The following series of programs shows you what fun you can have with 
windows. Each program is based on the code shown below, which produces 
four separate windows on the screen.

*quad1.c*

```c
#include <ncurses.h>
#include <stdlib.h>

void bomb(void);

int main(void)
{   
    WINDOW *a, *b, *c, *d;
    int maxx,maxy,halfx,halfy;

    initscr();
    refresh();

    getmaxyx(stdscr,maxy,maxx);
    halfx = maxx >> 1;
    halfy = maxy >> 1;
        
    
    if( (a = newwin(halfy,halfx,0,0)) == NULL) bomb();
    if( (b = newwin(halfy,halfx,0,halfx)) == NULL) bomb();
    if( (c = newwin(halfy,halfx,halfy,0)) == NULL) bomb();
    if( (d = newwin(halfy,halfx,halfy,halfx)) == NULL) bomb();
    
    mvwaddstr(a,0,0,"This is window A\n");
    wrefresh(a);
    mvwaddstr(b,0,0,"This is window B\n");
    wrefresh(b);
    mvwaddstr(c,0,0,"This is window C\n");
    wrefresh(c);
    mvwaddstr(d,0,0,"This is window D\n");
    wrefresh(d);
    getch();

    endwin();
    return 0;
}

void bomb(void)
{
    addstr("Unable to allocate memory for new window.\n");
    refresh();
    endwin();
    exit(1);
}
```

Is it a window or a hole? In fact, the window itself is really defined 
by the space around it, the frame and the panes of glass. When I work 
with windows, I want to see that frame. The best way I think that can 
happen is with color, as the following improvement to the QUAD1.C program demonstrates.

*quad2.c*

```c
#include <ncurses.h>
#include <stdlib.h>

void bomb(void);

int main(void)
{
    WINDOW *a, *b, *c, *d;
    int maxx,maxy,halfx,halfy;

    initscr();
    refresh();
    
    start_color();
    
    init_pair(1, COLOR_BLACK, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    
    getmaxyx(stdscr,maxy,maxx);
    halfx = maxx >> 1;
    halfy = maxy >> 1;
    
    
    if( (a = newwin(halfy,halfx,0,0)) == NULL) bomb();
    if( (b = newwin(halfy,halfx,0,halfx)) == NULL) bomb();
    if( (c = newwin(halfy,halfx,halfy,0)) == NULL) bomb();
    if( (d = newwin(halfy,halfx,halfy,halfx)) == NULL) bomb();

    mvwaddstr(a,0,0,"This is window A\n");
    wbkgd(a, COLOR_PAIR(1));
    wrefresh(a);
    mvwaddstr(b,0,0,"This is window B\n");
    wbkgd(b, COLOR_PAIR(2));
    wrefresh(b);
    mvwaddstr(c,0,0,"This is window C\n");
    wbkgd(c, COLOR_PAIR(3));
    wrefresh(c);
    mvwaddstr(d,0,0,"This is window D\n");
    wbkgd(d, COLOR_PAIR(4));
    wrefresh(d);
    getch();

    endwin();
    return 0;
}

void bomb(void)
{
    addstr("Unable to allocate memory for new window.\n");
    refresh();
    endwin();
    exit(1);
}
```

The `start_color()` function initializes color but without error
checking: I'm assuming that your terminal can do color and further that
`start_color()` will not fail. Remember in your "real" programs to always
use `has_colors()` and test to see if `start_color()` returns OK before
moving on in color.

The program defines four colors pairs, each of which is assigned to a 
specific window via the `wbkgd()` function.

#Rot 13 Cycle

Now we write a program that creates two side-by-side windows that fill 
the screen. As you type in the first window, text also appears in the 
second window but with the rot13 filter applied. (Rot13 is a simple 
cipher where A and N are swapped, B and O, C and P, and so on for the 
26 letters of the alphabet.)

*rot13.c*

```c
#include <ncurses.h>

int rot(int ch){
    return ('a'+((ch-'a')+13)%26);
}

int main(void)
{
    WINDOW *one, *two;
    int maxy, maxx, halfy, ch;   

    initscr();
    refresh();

    getmaxyx(stdscr, maxy, maxx);
    halfy = maxy >> 1;

    one = newwin(halfy, maxx, 0, 0);
    two = newwin(halfy, maxx, halfy, 0);

    waddstr(one, "Window 1\n");
    wrefresh(one);
    waddstr(two, "Window 2\n");
    wrefresh(two);

    noecho();
    do{
        ch = getch();
        waddch(one, ch);
        wrefresh(one);
        waddch(two, rot(ch));
        wrefresh(two);
    }while(ch != '~');

    endwin();
    return 0;
}
```

