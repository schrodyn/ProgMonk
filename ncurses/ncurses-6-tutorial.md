#More Text Manipulation

NCurses provides a wealth of text-manipulation functions, which are more
than just positioning the cursor. There are functions that can insert and
 delete characters or lines of text, causing other text on the screen 
to jump around and make room or to fill in the gaps. This chapter covers 
those amazing functions.

#Inserting and Deleting Functions

Writing a program that spits up text on the screen is a relatively easy 
task. What becomes a pain in the rear is when you have to *modify* 
the text you splash on the screen. That becomes the "Oh, no! I now have 
to rethink how all this is going to work" chore.

You can rely on the following functions in NCurses that insert and delete
text:

```c
insch(ch)
insertln()
delch()
deleteln()
```

The `ins` sisters are used to insert a single character or a complete 
line of text. The `insch()` inserts a character. The `insertln()` 
function scrolls in a blank line of text.

The `del` brothers have no arguments. They delete a single character or 
an entire line of text from the screen.

#Editing Shakespeare

The following source code serves as the core for the next several sets of
programs.

*hamlet1.c*

```c
#include <ncurses.h>

int main(void){

    char Ham1[] = "To be, or not to be: that is the question:\n";
    char Ham2[] = "Whether `tis nobler in the mind to suffer\n";
    char Ham3[] = "The slings and arrows of outrageous fortune,\n";
    char Ham4[] = "Or to take arms against a sea of troubles,\n";
    char Ham5[] = "And by opposing end them?\n";

    initscr();

    addstr(Ham1);
    addstr(Ham3);
    addstr(Ham5);
    refresh();
    getch();

    endwin();
    return 0;
}
```

#Inserting Some Lines

the `insertln()` function inserts a blank line all the way across 
the screen and scrolls the rest of the text beneath that line down
one notch. At that point, you merely need to add the string of text 
you need.

*hamlet2.c*

```c
#include <ncurses.h>

int main(void){

    char Ham1[] = "To be, or not to be: that is the question:\n";
    char Ham2[] = "Whether `tis nobler in the mind to suffer\n";
    char Ham3[] = "The slings and arrows of outrageous fortune,\n";
    char Ham4[] = "Or to take arms against a sea of troubles,\n";
    char Ham5[] = "And by opposing end them?\n";

    initscr();

    addstr(Ham1);
    addstr(Ham3);
    addstr(Ham5);
    refresh();
    getch();

    move(1,0);		/* position cursor */
    insertln();		/* insert a blank line, scroll text down */
    addstr(Ham2);	/* line to insert */
    refresh();
    getch();

    endwin();
    return 0;
}
```

the `insertln()` function creates a blank line on the same line where
the logical cursor is placed. It always blanks the entire line, 
even if the cursor is placed on the far left side of the screen.

Finally, the `addstr()` function inserts a new string of text at the 
cursor's position, which is still 1,0.

#Inserting One Character at a Time

The `insch()` function inserts only one character into a row of text,
 shoving all the characters to the left one space to the left. And like 
`insertln()`, any character that gets shoved off the left side of the 
screen is forgotten.

*marque1.c*

```c
#include <ncurses.h>
#include <string.h>

int main(void){
    char text[] = "Stock Market Swells! DOW tops 15,000!";
    char *t;
    int len;

    initscr();
    t = text;
    len = strlen(text);
    while(len){
        move(5,5);
        insch(*(t+len-1));
        refresh();
        napms(100);
        len--;
    }
    getch();

    endwin();
    return 0;
}
```

#Less of Hamlet

On the deleting side, NCurses offers the `delch()` and `deleteln()` 
functions.

First, the whole chunk of text is displayed. Good.

Press Enter to delete the text. Note how it scrolls up? Yes, 
the `deleteln()` function is nearly the opposite of the 
`insertln()` function, complete with logical reverse scrolling. 
And the vanished line goes off into bit-hell as well; don't look for 
it anywhere.

Do note that `deleteln()` does not affect the location of the cursor. 
After the last `refresh()` command above, the cursor is still at 
location 1,0.

*hamlet4.c*

```c
#include <ncurses.h>

int main(void){

    char Ham1[] = "To be, or not to be: that is the question:\n";
    char Ham2[] = "Whether `tis nobler in the mind to suffer\n";
    char Ham3[] = "The slings and arrows of outrageous fortune,\n";
    char Ham4[] = "Or to take arms against a sea of troubles,\n";
    char Ham5[] = "And by opposing end them?\n";

    initscr();

    addstr(Ham1);
    addstr(Ham2);
    addstr(Ham3);
    addstr(Ham4);
    addstr(Ham5);
    refresh();
    getch();

    move(1,0);
    deleteln();
    refresh();
    getch();

    endwin();
    return 0;
}
```


