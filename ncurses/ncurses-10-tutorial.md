#Subwindows

Subwindows are strange and interesting creatures in NCurses. In fact, much
of the documentation on the Internet claims that subwindows are buggy and
should be avoided. That isn't exactly correct. With proper understanding,
subwindows can be time-saving gizmos in NCurses, not to be avoided at all.

#The Thing with Subwindows

Subwindows are like real windows in NCurses in that they share the same
data structure. You use the same WINDOW variable to create a subwindow as
you do a full window. NCurses functions that control or manipulate 
a window also control and manipulate subwindows (with a few exceptions). 
But a subwindow is not the same thing as a real window.

The main difference between a subwindow and a real window is that 
subwindows share memory with a parent window. So when you put a character 
to a subwindow, you're also placing that character into the parent window 
as well.

If you were to change the text in the subwindow, you would also be 
changing or adding text to the parent. Likewise, the parent window can 
change text in the subwindow. This can be confusing and frustrating but 
only when you assume that a subwindow is like a regular window. It is not.

The best way to think of a subwindow is merely as a convenient way to 
reference a specific portion of the parent.

#Making Subwindows

Subwindows have all of the basic attributes of real windows: a name, 
size, location, unique cursor coordinates, and so on. Because they share 
memory with the parent, the subwindow must reside completely within the 
parent window. And, naturally, subwindows are created by using their own 
unique functions:

```c
subwin(win,rows,cols,y,x)
derwin(win,rows,cols,y,x)
```

The only difference between these functions has to do with the final two
arguments. With `subwin()`, y and x are coordinates relative to the 
screen; in `derwin()`, y and x are relative to the parent window.

After creation, the subwindow can be addressed like any other window,
named in a window-oriented command, deleted, or affected in the same was
as regular windows. Subwindows can even have subwindows of their own.

#Your First Subwindow

Subwindows exist totally within a parent window. In a way, it helps to 
think of them more as a *region* of the parent window, though the 
subwindow still maintains its own cursor and text, and attributes can be 
written directly to the subwindow just as with other windows in NCurses.

```c
subwin(win,rows,cols,y,x)
```

The `subwin()` function creates the subwindow. `win` refers to the parent
window, which can be `stdscr`, the standard screen. `rows` and `cols` 
gives the size of the subwindow in character rows and columns. Finally, 
the x and y represent the upper-left location of the subwindow relative tothe *screen*, not the parent window org.

When the `subwin()` function is successful, a new window is created,
returned as a WINDOW pointer by `subwin()`. When the function fails, 
such as when no memory is available or the subwindow does not reside 
completely within the parent, NULL is returned.

*kid1.c*

```c
#include <ncurses.h>

int main(void)
{
    WINDOW *sony;

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_YELLOW);

    sony = subwin(stdscr,30,20,10,30);
    if(sony == NULL)
    {
        addstr("Unable to create subwindow\n");
        endwin();
        return 1;
    }

    bkgd(COLOR_PAIR(1));
    addstr("Hello, son.");

    wbkgd(sony, COLOR_PAIR(2));
    waddstr(sony, "Hello, Dad.");
    refresh();
    getch();

    endwin();
    return 0;
}
```

Note that the `start_color()` function is used in this code without error 
checking; remember to do error checking, as well as use the `has_colors()` function for any code you plan on releasing publicly.

#Sub-subwindows

Because a subwindow is considered a real window in nearly every sense, 
it's quite possible for a subwindow to have a subwindow of its own. 
You simply name the subwindow's variable as the `win` in the `subwin()` 
function.

*kid2.c*

```c
nclude <ncurses.h>

int main(void)
{
    WINDOW *grandpa, *father, *boy;
    int maxx,maxy;

    initscr();
    refresh();

    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    init_pair(2,COLOR_RED,COLOR_YELLOW);
    init_pair(3,COLOR_CYAN,COLOR_GREEN);

    getmaxyx(stdscr, maxy, maxx);

    grandpa = newwin(maxy-4,maxx-10,2,5);
    father = subwin(grandpa,maxy-8,maxx-20,4,10);
    boy = subwin(father,maxy-16,maxx-40,8,20);

    wbkgd(grandpa,COLOR_PAIR(1));
    waddstr(grandpa,"Grandpa");
    wbkgd(father,COLOR_PAIR(2));
    waddstr(father,"Father");
    wbkgd(boy,COLOR_PAIR(3));
    waddstr(boy,"Boy");
    wrefresh(grandpa);


    getch();
    endwin();
    return 0;
}
```

The program creates three windows. First comes window `grandpa`, which
is a "real" window. Then `father` is created as a subwindow of `grandpa`.
Finally, `boy` is created as a subwindow of `father`.

#Removing a Subwindow

Subwindows are killed off just like regular windows, using the same 
`delwin()` function. Deleting a subwindow removes the subwindow's 
internal structure (the thing WINDOW points at) but not the window's 
data, because that information is shared with the parent window. 
Also, to visually remove the subwindow, the parent window must be touched 
and refreshed.

This is important: You should delete a window's subwindows before
you can delete the main window. If you don't, the memory used by the
subwindow will not be released in your program, and other, various ugly 
and unpredictable errors may result.

#Subwindows Versus Windows

Subwindows work like real windows in many ways:

  * Subwindows use a separate WINDOW data structure in memory.
  * Subwindows sport their own cursor, separate from the parent
	window's cursor.
  * Subwindows can have their own color and text attributes.
  * Subwindows can be manipulated by all the same functions that 
	manipulate regular windows.

Internally, a subwindow knows that it's a subwindow. It can reference 
the parent window, thanks to a pointer stored inside the subwindow's 
WINDOW structure. Sadly, that doesn't work the other way around: 
Parent windows have no way of knowing whether they have subwindows. 
This is the main reason you're not supposed to remove a window unless 
you first remove its subwindows.

Finally, remember the shared memory deal. Anything you write to a 
subwindow is also written to the parent. Likewise, the parent has no 
respect for its subwindow and can effortlessly write text over the 
subwindow's text. In fact, text written over the subwindow's text becomes 
part of the subwindow, thanks to the shared memory.


