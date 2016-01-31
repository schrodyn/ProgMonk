#Keyboard Madness!

The `getch()` function is one of the immediate blessings that NCurses 
offers struggling \*nix programmers. Unlike the C macro `getchar()`, which 
is a stream-reading command, `getch()` hops back right away with 
whatever key was pressed. Such a function helps make your programs more 
interactive, which can be a huge bonus.

#Reading from the Keyboard

The `getch()` function normally waits until a key is pressed on the 
keyboard. This is known as a *blocking* call; program execution is 
stopped until a key is pressed. But it's possible to change that behavior 
so that `getch()` does not pause the program. In NCurses, that's done 
with the `nodelay()` function.

```c
nodelay(sdtscr,TRUE)
```

The preceding function directs the `getch()` function to become 
nonblocking for the given window, the standard screen (`stdscr`). 
To restore `getch()` to its normal blocking mode, this version of 
`nodelay()` is used:

```c
nodelay(stdscr,FALSE)
```

When `getch()` is in nonblocking mode, it will not pause program 
execution to wait for a key press. Instead, when no key is pressed or a 
key isn't waiting to be read from the keyboard buffer, `getch()` returns 
the value `ERR` - and the program continues. Likewise, if a key is pressed
, `getch()` returns the key's value, as it usually does - and the 
program continues.

#Is a Character Waiting?

The following program demonstrates how `getch()`'s personality can be 
changed by using the `nodelay()` function.

*keywait1.c*

```c
#include <ncurses.h>

int main(void){
    int value = 0;

    initscr();

    addstr("Press any key to begin:\n");
    refresh();
    getch();

    nodelay(stdscr, TRUE);          /* turn off getch() wait */
    addstr("Press any key to stop the insane loop!\n");
    while(getch() == ERR){
        printw("%d\r", value++);
        refresh();
    }   

    endwin();
    return 0;
}
```

#How to Implement kbhit()

If you're an ancient DOS programmer or just familiar with the C language 
on the PC, you might remember (and reminisce about) the old `kbhit()` 
function. It was used to determine whether or not characters were waiting 
to be read from the keyboard buffer. `kbhit()` returned TRUE if characters
 were waiting, FALSE otherwise.

You can use `nodelay()` and `getch()` in NCurses to emulate this behavior
somewhat but not exactly. The problem is that `getch()` still fetches a 
key. So what you need is a way to take that key and stuff it back to the 
keyboard input queue. The function that handles that is called, 
logically, `ungetch()`. Here's the format:

```c
ungetch(ch)
```

The `ungetch()` function places the character ch back into the input 
buffer.

*kbhit.c*

```c
#include <ncurses.h>

int kbhit(void)
{
    int ch,r;

    /* turn off getch() blocking and echo */
    nodelay(stdscr,TRUE);
    noecho();

    /* check for input */
    ch = getch();
    if(ch == ERR)           /* no input */
        r = FALSE;
    else                    /* input */
    {
        r = TRUE;
        ungetch(ch);
    }

    /* restore block and echo */
    echo();
    nodelay(stdscr,FALSE);
    return r;
}


int main(void){
    initscr();

    addstr("Press any key to end this program:");
    while(!kbhit())
        ;

    endwin();
    return 0;
}
```

And when you press a key, the program ends. Voílà, there is your `kbhit()`
function equivalent.

#Flushing Input

Text typed at the computer keyboard is stored in a buffer. The various 
text-reading, or keyboard input, functions fetch characters from that 
buffer as the program needs them, which is a basic description of how 
keyboard input works.

The reason for the buffer is to allow for keyboard input while 
the program is doing something else. I can think of two benefits to 
this:

  * First, it allows you to type ahead while the computer can do 
	something (and the keyboard buffer is often called the *type-ahead 
	buffer*).

  * Second, it means that the program doesn't have to constantly scan the
	keyboard to determine whether you've pressed a key. 

There are times, however, when you want to clear or *flush* the keyboard
buffer. For example, suppose your program asks a very serious question 
and requires a Y or N key press. There may just be a rogue Y or N in the 
keyboard buffer, so when your program pauses to read the keyboard, it 
may read that older key press instead of the one corresponding to the 
current question. To prevent that from happening, you can flush input, 
clearing the keyboard buffer.

In NCurses, the `flushinp()` function carries out that task.

*flush1.c*

```c
#include <ncurses.h>

int main(void){
    char buffer[81];

    initscr();

    addstr("Type on the keyboard whilst I wait...\n");
    refresh();
    napms(5000);	/* 5 seconds */

    addstr("Here is what you typed:\n");
    getnstr(buffer, 80);
    refresh();

    endwin();
    return 0;
}
```

While that's on the screen, type something such as your name. Then, 
after a five-second pause, you'll see that text displayed.

The FLUSH1.C code demonstrates the type-ahead buffer in action. While
the `napms()` function is holding up program execution, the keyboard can 
still be used, and any characters you type are stored in the
keyboard's buffer. The `getnstr()` function later picks up, processes, 
and displays what you typed, what was waiting in the buffer. Pressing 
Enter ends `getnstr()`'s waiting, also ending the program.

The following modification to FLUSH1.C inserts the
`flushinp()` function before `getnstr()`. Because input is flushed, 
anything you type will be erased from the buffer and `getnstr()` will 
just have to wait!

*flush2.c*

```c
#include <ncurses.h>

int main(void){
    char buffer[81];

    initscr();

    addstr("Type on the keyboard whilst I wait...\n");
    refresh();
    napms(5000);	/* 5 seconds */

    addstr("Here is what you typed:\n");
    flushinp();
    getnstr(buffer, 80);
    refresh();

    endwin();
    return 0;
}
```

#Silence, Please!

Aside from waiting for your keyboard input, another normal behavior for
`getch()` is to echo, or display, the character typed on the screen. 
It was popular for old C compilers to include a `getch()` function that 
did not echo the character typed; it was the companion `getche()` 
function that echoed the character typed to the display.

Those old functions are long gone.

*whoru.c*

```c
#include <ncurses.h>

int main(void){
    char name[46];
    char password[9];

    initscr();

    mvprintw(3,10,"Enter your name: ");
    refresh();
    getnstr(name,45);
    mvprintw(5,6,"Enter your password: ");
    refresh();

    getnstr(password,8);

    endwin();
    return 0;
}
```

The program asks for your name and password. Sadly, the password is 
displayed as it's typed, which is a security risk. It would be nice to 
turn off the echo of characters while the password is input, right?
But not if you insert the following:

```c
noecho();
```

After recompiling and running, the password is typed "in the dark." 
The `noecho()` function affected input by not echoing the characters 
typed. Yes, even though `getch()` isn't used, internally the `getnstr()` 
function does use `getch()`'s guts, so `noecho()` turns off the display 
for all input functions.

To turn on the display again (which isn't necessary in this sample 
program), use the `echo()` function.

Remember! Even with `noecho()`, text is being processed by the program.
The `noecho()` function affects only the display, not the keyboard.

*whoru2.c*

```c
#include <ncurses.h>

int main(void){
    char name[46];
    char password[9];

    initscr();

    mvprintw(3,10,"Enter your name: ");
    refresh();
    getnstr(name,45);
    noecho();
    mvprintw(5,6,"Enter your password: ");
    refresh();

    getnstr(password,8);

    mvprintw(7,6,"You entered: %s", password);
    getch();
    endwin();
    return 0;
}
```

#Keypad On!

All keys on the computer keyboard generate a code when that key is 
pressed. The code is returned by key-reading functions, either as the 
raw code itself or as some other special value as "cooked" by the 
operating system.

In the case of the alphanumeric keys, the key's ASCII character value is
returned when you press that key. For other keys, other values are 
returned, a 16-bit value, a pair of 8-bit values, or perhaps even an 
escape sequence. Knowing which type of value returned means you can 
determine exactly which key was pressed, even nonalphanumeric keys. 
The drawback, of course, is that not every terminal produces the same 
results or even has the same keys.

NCurses to the rescue! The `NCURSES.H` header file defines many of the 
extra keys found on terminal keyboards. To activate and use those 
definitions, the `keypad()` function is used:

```c
keypad(stdscr,TRUE)
```

The `keypad()` function indicates which window it affects, such as 
`stdscr` for the standard screen, followed by TRUE to turn the feature 
on or FALSE to turn it off. Once enabled, your program can use various 
defined keyboard constants to read those extra keyboard keys, as the 
following program.

*arrowkeys.c*

```c
nclude <ncurses.h>
int main(void){
    int ch;

    initscr();

    keypad(stdscr, TRUE);
    do
    {
        ch = getch();
        switch(ch)
        {
            case KEY_DOWN:
                addstr("Down\n");
                break;
            case KEY_UP:
                addstr("Up\n");
                break;
            case KEY_LEFT:
                addstr("Left\n");
                break;
            case KEY_RIGHT:
                addstr("Right\n");
            default:
                break;
        }
    }while(ch != '\n');

    endwin();
    return 0;
}
```

#What's Where on the Keyboard

When the `keypad()` function has activated the keyboard's extra keys, 
you can use the definitions in NCURSES.H to help your program read the 
keys.

[here table]

You might want to run some tests to assure that NCurses on your computer
can access the function keys. For example, to check for Function key 5, 
you would use:

```c
ch == KEY_F(5)
```

as the comparison. Remember that variable `ch` must be an `int`, not a 
`char` (that's a common mistake).

that's a common mistake).

Also be aware that your operating system may steal the function keys from
your program. If so, you can use the `raw()` function in NCurses to try to
get the function keys passed directly to your program.

#The Highlighted Menu Bar

*menubar.c*

```c
#include <ncurses.h>

#define MENUMAX 6

void drawmenu(int item)
{
    int c;
    char mainmenu[] = "Main Menu";
    char menu[MENUMAX][21] = {
        "Answer E-mail",
        "Off the Web",
        "Word Processing",
        "Financial management",
        "Maintenance",
        "Shutdown"
    };

    clear();
    addstr(mainmenu);
    for(c = 0; c < MENUMAX; c++)
    {
        if(c == item)
            attron(A_REVERSE);          /* highlight selection */
        mvaddstr(3+(c*2),20,menu[c]);
        attroff(A_REVERSE);             /* remove highlight */
    }
    mvaddstr(5+(MENUMAX*2),25, "Use arrow keys to move; Enter to select:");   
    refresh();
}

int main(void){
    int key,menuitem;

    menuitem = 0;

    initscr();

    drawmenu(menuitem);
    keypad(stdscr, TRUE);
    noecho();               /* Disable echo */
    do
    {
        key = getch();
        switch(key)
        {
            case KEY_DOWN:
                menuitem++;
                if(menuitem > MENUMAX-1) menuitem = 0;
                break;
            case KEY_UP:
                menuitem--;
                if(menuitem < 0) menuitem = MENUMAX-1;
                break;
            default:
                break;
        }
        drawmenu(menuitem);
    }while(key != '\n');

    echo();                 /* re-enable echo */

    endwin();
    return 0;
}
```
