#More Window Tricks

This chapters offers a lot of windows tricks, including copying windows, 
windows that scroll or not, and moving windows.

#Copying Window Contents

Copying text between two windows is possible by using one of NCurses 
window copying functions. They are

```c
overlay (swin,dwin)
overwrite(swin,dwin)
copywin(swin,dwin,srow,scol,drow,dcol,dxrow,dxcol,type)
dupwin(win)
```

##To overlay or to overwrite?

The `overlay()` and `overwrite()` functions work almost identically, 
taking the contents of source window, the source or `scrwin`, and 
plopping that text down into the destination window, or `destwin`. 
The difference between the two is subtle.

*doop1.c*

```c
#include <ncurses.h>

int main(void)
{
    WINDOW *alpha;
    char text1[] = "Lorem ipsum bla bla bla";
    char text2[] = "Four score and seven years we dance as cool disco guys";

    initscr();

    alpha = newwin(0,0,0,0);

    addstr(text1);
    refresh();
    getch();

    waddstr(alpha,text2);
    wrefresh(alpha);
    getch();

    overlay(stdscr,alpha);
    wrefresh(alpha);
    getch();

    endwin();
    return 0;
}
```

##The overwrite() difference

To see how `overwrite()` differs from `overlay()` is easy.
Change `overlay()` to this:

```c
overwrite(stdscr,alpha);
```

The `overwrite()` function is destructive, so no characters from the
original text appear.

##The magic of copywin()

The best of the three basic text-copying functions is `copywin()`, which 
allows you a great deal of control over which chunk of text is copied from
 the source window and where it ends up in the destination window. 
And unlike `overlay()` and `overwrite()`, `copywin()` seems to work on 
windows of any size. Here's the detailed format:

```c
copywin(swin,dwin,srow,scol,drow,dcol,dxrow,dxcol,type)
```

The `srcwin` and `destwin` are the source and destination windows, which
can be of any size or position on the screen.

`srow` and `scol` are the starting coordinates of the chunk of text to be copied, as offset within the `srcwin`.

`drow` and `dcol` set the starting coordinates of where the chunk will be
copied into the `destwin`. The size of the chunk copied is set by `dxrow` 
and `dxcol`, which are offsets with `destwin`.

Finally, type can be either TRUE or FALSE. If TRUE, then the text copied 
is non-destructive, as with `overlay()`. If FALSE, then the text block 
replaces the block in the `destwin`, just like `overwrite()`.

[here image]

#Plain old window duplication

The final window copying function is `dupwin()`, which copies an entire
window - size, text and all - to a new window, a duplicate. 
It's basically the `newwin()` function, but uses an existing window as 
a template to create the new window.

```c
newwin = dupwin(win)
```

The `dupwin()` function returns a WINDOW pointer, which is then used to 
reference and write to the new window

*dup.c*

```c
nclude <ncurses.h>

int main(void)
{
    WINDOW *fred, *barney;

    initscr();
    refresh();

    fred = newwin(0,0,0,0);
    waddstr(fred, "This is the original window, Fred.\n");
    wrefresh(fred);
    getch();

    barney = dupwin(fred);
    waddstr(barney, "This is the Barney copy of window Fred.\n");
    touchwin(barney);
    wrefresh(barney);
    getch();

    waddstr(fred, "Nice to see you again!\n");  
    touchwin(fred);
    wrefresh(fred);
    getch();

    waddstr(barney, "And Barney says 'Hi' as well.\n"); 
    touchwin(barney);
    wrefresh(barney);
    getch();

    endwin();
    return 0;
}
```

The new window `barney` is created based on `fred`. The new window 
inherits `fred`'s text. Then the program bounces back and forth between
the windows, displaying information.

#Scrolling Around

Scrolling text doesn't seem like a big deal, and it isn't - today! 
Thirty years ago it *was* a big deal - so much so that a scrolling screen 
of text was patented (probably by IBM).

The fact that text scrolls on the screen seems to be taken for granted. 
But it's not something that the terminal does automatically: scrolling 
must be programmed. After all, the screen is merely a matrix of text. 
Internally, the screen buffer is only as big as the screen. Whenever you 
see something larger than the screen displayed (and scrollable), it's 
programming magic that moves text from a separate buffer to the screen.

Fortunately, scrolling the screen is no big deal, even if you have to 
program the scroll yourself: a line of text is removed, the remaining 
text is moved, then a new line of text is added. It's not really that 
complicated, and you could do it yourself - but you don't have to! 
NCurses gladly handles any scrolling chores your programs require.

##Can it scroll?

In NCurses, scrolling text is a window attribute, just like the window's 
size, location, cursor location, and other attributes. This attribute is 
normally turned off, meaning that windows in NCurses doesn't scroll a 
window by default.

*scroller.c*

```c
nclude <ncurses.h>

#define FILENAME "somedummy_file"

int main(void)
{
    FILE *text;
    WINDOW *lister;
    int maxy,maxx,ch;

    initscr();
    refresh();

    getmaxyx(stdscr,maxy,maxx);

    /* create window lister */
    lister = newwin(maxy,maxx/2,0,maxx/4);
    if(lister == NULL)
    {
        addstr("unable to create window\n");
        refresh();
        getch();
        endwin();
        return 1;
    }

    /* open the file */
    text = fopen(FILENAME,"r");
    if(text == NULL)
    {
        addstr("unable to open file\n");
        refresh();
        getch();
        endwin();
        return 2;
    }

    /* display the file's contents */
    do{
        ch = fgetc(text);
        waddch(lister,ch);
        wrefresh(lister);
    } while(ch != EOF);
    fclose(text);
    getch();

    endwin();
    return 0;
}
```

This program creates a new window, `lister`. The window is half the 
width of the standard screen, but just as tall. A file is opened on disk 
and displayed, character by character, in the window. The notion here is 
to display enough text that the window needs to scroll to display the 
whole thing.

And... it doesn't scroll. Instead, after the window is filled, remaining 
text in the file continues to be plugged in to the bottom-most, right 
character position in window `lister`.

##Scroll Away

Enabling scrolling in NCurses is easy, thanks to the `scrollok()` 
function:

```c
scrollok(win,TRUE);
```

The `scrollok()` function sets or resets the ability of a window to 
scroll. The window is specified as `win`, and the second argument is 
either TRUE or FALSE to turn scrolling on or off (respectively) for that 
window.

Add the following:

```c
scrollok(lister,TRUE);
```

This time the text continues to display as the window scrolls its 
contents up, allowing for the new text. If your text file isn't long 
enough to scroll the window, then choose another file.

##The old manual scroll

Whether a window has its scrolling attribute set or not, you can still 
manually scroll the text in a window by using the `scroll()` function:

```c
scroll(win)
```

The `scroll()` function scrolls text in the window win up one line. This
works only if the window has its scrolling attribute set. When in doubt, 
set scrolling on with `scrollok(win,TRUE)`, otherwise the `scroll()` 
function returns ERR.

##Scrolling by leaps and bounds

Say you need to scroll the screen up two lines instead of three? It can 
happen. One way to make it happen is to just use `scroll()` thrice.

```c
scroll(stdscr); scroll(stdscr); scroll(stdscr);
```

Did it work?

```c
scroll(stdscr);
refresh();
scroll(stdscr);
refresh();
scroll(stdscr);
refresh();
```

Regardless of how you get it to work, you're wasting code. Obviously there
must be a better solution than awkwardly re-issuing `scroll()` functions.
And that solution is this function:

```c
scrl(n)
```

The `scrl()` function scrolls the standard screen n number of lines. So to
scroll up three lines, `scrl(3)` would be used. To make `scrl()` imitate
`scroll()`, `scrl(1)` would be used.

The window version of the `scroll()` function is `wscrl()`. The format is

```c
wscrl(win,n)
```

`win` is the name of the window being scrolled and `n` indicates the 
number of lines to scroll.

Both `wscrl()` and `scrl()` return OK upon success and ERR when there is
problem. The most common problem is that the window does not have its
scrolling attribute set.

##Negative scrolling

The `scrl()` function is a true scrolling function, one that doesn't 
assume things always have to scroll up. In fact, when you specify a 
negative value for n in `scrl(n)`, text on the screen scrolls down.

```c
scrl(-3)
```

Scrolling down is merely the opposite of scrolling up: lines move down 
on the screen. New, blank lines appear at the top. Text at the bottom of 
the screen is scrolled into oblivion. The cursor's position does not 
change.


#The Moving Experience

When NCurses creates a window it doesn't really bolt it down on the 
screen. Just as you can change text within a window, text attributes, 
the cursor location in the window, you can also change the window's 
location on the screen. This is thanks to the handy `mvwin()` function:

```c
mvwin(win,row,col);
```

The `mvwin()` function moves window `win` to new location `row`, `col`, 
relative to the standard screen. (So 0,0 is the upper-left corner.) 
As long as the whole window remains on the screen, you can move it 
anywhere.



