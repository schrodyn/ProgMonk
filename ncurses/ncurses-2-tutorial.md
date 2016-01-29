#The Setup

`NCurses` is about programming the terminal screen, so you'll need access to
 a terminal screen or window to run the programs.

You can either use one of the virtual terminals (which you can access on
most PCs by pressing Alt+F1, Alt+F2, Alt+F3, and so on) or open a terminal
window in the X Window System environment or in Mac OS X using the 
Terminal program.

Note that the terminal you choose can affect what NCurses does. Not all 
terminal types can, for example, do color or draw lines on the screen.

most of the time you'll be cycling through three sets of commands. 

First comes the editing:

```sh
vim goodbye.c
```

Then comes the compiling:

```sh
gcc -lncurses goodbye.c
```

Then comes the running:
```sh
./a.out
```

#Know Thy Compiler

The standard C compiler in the UNIX environment is `gcc`, the GNU C 
compiler. To compile, you will type something at the shell prompt, 
perhaps like this:

```c
gcc goodbye.c -lncurses
```

That's the gcc command, your compiler.

The first option is the name of the source code file, the text file you created. In this case, it's named goodbye.c. The single, lowercase c denotes a standard C source code file, not C++.

Finally comes `-lncurses`, which tells the compiler to `-l` "link in" the
NCurses library. This is very important! NCurses is not just a header file; it's also a library. And you must link in the library to have those 
NCurses functions

```c
#include <ncurses.h>
int main(void)
{
	initscr();
	addstr("Goodbye, cruel C programming!");
	refresh();
	getch();
	
	endwin();
	return 0;
}
```

Only by using `refresh()` is the NCurses
"window" updated and any text written to the screen displayed.

Even with the `refresh()` function in the code, it's still possible 
that you won't see any program output. The problem isn't the program 
or even NCurses; it's your terminal.

Many terminals, such as xterm, support a feature known as `rmcup`. It
restores the screen to what it looked like before a program was run. 
The situation also occurs with any full-screen terminal program, 
such as man or less; the program's text disappears after you quit the 
program, and the prompt "window" is restored.

The quick solution is to use the `getch()` function in your code. By 
inserting a line with `getch()` before the `endwin()` function, you can 
pause output and see what NCurses does before the program quits.


