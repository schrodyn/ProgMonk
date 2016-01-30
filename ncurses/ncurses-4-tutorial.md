#Formatting Text

NCurses not only puts text up on the screen; it also lets you put text 
on the screen with style. It is enough to add emphasis, fun, and perhaps 
a wee bit o' color to what would otherwise be boring terminal text.

There are three useful functions that control the tone of the text 
displayed on the screen:

```c
attrset(attr)
attron(attr)
attroff(attr)
```

The `attrset()` function sets text attributes.

The `attron()` and `attroff()` functions turn specific text attributes 
on or off, respectively.

There is some confusion regarding whether to use attrset() or
attron() to apply text attributes. You can use either one. For example:

```c
attrset(A_BOLD);
attron(A_BOLD);
```

Both these statements apply the bold text attribute to any text displayed
afterward. The difference is that `attrset()` *turns off* all other 
attributes previously applied, leaving only bold applied to the text, 
while `attron()` adds the bold attribute to any attributes already 
applied to the text.

When you start up NCurses, the text output is displayed using the normal
(`A_NORMAL`) text attribute. That equates to the standard white text 
on a black background or however you have your terminal configured.

But that's so boring!

[here table]

The attributes listed in table are used with the `attrset()`, `attron()`,
and `attroff()` functions to control the appearance of text on the screen.

`attron()` and `attroff()` are used to set and reset individual attributes. `attrset()` is used to override any previous text attributes and 
set a new attribute for all text.

##Testing Some Attributes

```c
#include <ncurses.h>

int main(void){
    initscr();

    attron(A_BOLD);
    addstr("Twinkle, twinkle star\n");
    attron(A_BLINK);
    addstr("How I wonder who you are.\n");
    attroff(A_BOLD);
    addstr("Up above the world so high,\n");
    addstr("Like a diamond in the sky.\n");
    attrset(A_NORMAL);
    addstr("Twinkle, twinkle little star\n");
    addstr("How I wonder what you are.\n");
    refresh();
    getch();
    endwin();
    return 0;
}
```

##Multiple-Attribute Mania

For example, if you want your text bold and blinking, you can use:

```c
attrset(A_BOLD | A_BLINK);
```

Here, both the bold and blink attributes are specified, meaning any text 
displayed after that statement will boldly blink.

Any number of attributes (including color) can be applied in a single
`attron()` or `attrset()` command and likewise turned off via `attroff()` 
or `standend()`. Simply specify the `|` (logical OR) between the attribute
 constants.

#Can It Do Color?

Of course, NCurses can do color! The question is whether or not the 
terminal can. Fortunately, the NCurses library comes with the tools to 
assist you in determining whether or not the terminal is color blind or 
rainbow ready.

```c
has_colors()
```

The `has_colors()` function returns a logical TRUE if the terminal is 
able to display colored text, FALSE otherwise.

After determining whether the terminal has colors, you need to start up
NCurses color abilities. Here's the function that does that, aptly named:

```c
start_color()
```

The `start_color()` function returns `OK` if the color functions are 
properly initialized. (And `OK` is defined in `NCURSES.H`.) So after 
successful completion of the `start_color()` command, you can use color 
attributes on text and to shade the background.

*colortest.c*

```c
#include <ncurses.h>
#include <stdlib.h>

void bomb(char *msg);

int main(void){
    initscr();

    if(!has_colors())
        bomb("Terminal cannot do colors\n");

    if(start_color() != OK)
        bomb("Unable to start colors.\n");

    printw("Colors have been properly initialized.\n");
    printw("Congratulations!\n");
    printw("NCurses reports that you can use %d colors,\n", COLORS);
    printw("and %d colors pairs.", COLOR_PAIRS);
    refresh();
    getch();

    endwin();
    return 0;
}

void bomb(char *msg){
    endwin();
    puts(msg);
    exit(1);
}
```

Note the use of the `COLORS` and `COLOR_PAIRS`. These constants are set
when `start_color()` checks to see how many colors are available to the 
terminal, as well as how much space is left for storing color information in the NCurses `attr_t` variable type. The next section explains the 
difference between `COLORS` and `COLOR_PAIRS`.

##Colors and Color Pairs

In NCurses, the `COLORS` and `COLOR_PAIRS` constants report how many 
colors the terminal can display and how many color combinations 
(foreground + background - a *color pair*) can be defined.

The `COLORS` value reflects the basic set of colors available to the 
terminal. The typical PC reports only eight colors available.

[here table]

A color pair is simply a combination of foreground and background color
attributes. Each combination is a color pair and assigned a number 
from 1 through the value of `COLOR_PAIRS` minus 1.

For example, the color pair `COLOR_YELLOW`, `COLOR_RED` indicates yellow
text on a red background. To assign those colors as a text attribute, you
 must first associate them with a color pair number. This is done with 
the `init_pair` function:

```c
init_pair(pair,f,b)
```

The `init_pair()` function assigns two colors to color pair number `pair`.
`f` is the foreground, or text, color; `b` is the background color.

```c
init_pair(1,COLOR_YELLOW,COLOR_RED);
```

The preceding statement defines color pair number 1 as yellow text on a 
red background.

The next step is to apply the color attribute to the text. This is done 
by using the `attrset()` or `attron()` function just as you would apply 
any other text attribute. The key, however, is to use the 
`COLOR_PAIR(n)` constant:

```c
attrset(COLOR_PAIR(1));
```

The preceding statement applies the color attributes assigned to color 
pair 1 to the text that follows.

You can use `attrset()` or `attron()` to apply that color pair combination
to your text.

Also If you're lucky you can use even more colors. That can be confirmed 
with another function, `can_change_color()`, which returns a
logical TRUE or FALSE depending on whether the terminal has the 
color-changing ability:

```c
if(!can_change_color())
	bomb("Unable to do color change.\n");
```

Now if you can get it to work, the `init_color()` function comes into 
play, wherein you can create your own unique text colors. That function 
looks something like this:

```c
init_color(color,r,g,b);
```

#Coloring a Window

In addition to coloring text, NCurses' color attributes can be applied to
windows as well, including the standard screen, the default window you 
see in NCurses. (The standard screen is the same size as the terminal 
screen.)

The function to fill the standard screen window with color (or any text
attribute) is:

```c
bkgd()
```

The `bkdg()` function, as its name suggests, sets the background 
attributes for the standard screen. Every screen location is filled 
with the attribute(s) specified, whether they're plain text attributes 
or color.



```c
#include <ncurses.h>

int main(void){
    initscr();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    bkgd(COLOR_PAIR(1));
    refresh();
    getch();

    endwin();
    return 0;
}
```

The result should be a solid blue screen, top to bottom. Any text added 
at this point appears with the color attribute applied automatically, 
in this case with white text on a blue background.


