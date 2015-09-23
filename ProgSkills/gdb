GDB, short for GNU Debugger, is the most popular debugger for UNIX systems 
to debug C and C++ programs.

How GDB Debugs?
GDB allows you to run the program up to a certain point, then stop and print out 
the values of certain variables at that point, or step through the program one 
line at a time and print out the values of each variable after executing each line

A Debugging Symbol Table maps instructions in the compiled binary program to 
their corresponding variable, function, or line in the source code. 

This mapping could be something like:
Program instruction => item name, item type, original file, line number defined.

Symbol tables may be embedded into the program or stored as a separate file. 
So if you plan to debug your program, then it is required to create a symbol table
which will have the required information to debug the program.

We can infer the following facts about symbol tables:

1) A symbol table works for a particular version of the program – if the program 
changes, a new table must be created.

2) Debug builds are often larger and slower than retail (non-debug) builds; 
debug builds contain the symbol table and other ancillary information.

3) If you wish to debug a binary program you did not compile yourself, you must 
get the symbol tables from the author. 

To let GDB be able to read all that information line by line from the symbol table, we need to compile it a bit differently. Normally we compile our programs as:

gcc hello.cc -o hello

Instead of doing this, we need to compile with the -g flag as shown below:

gcc -g hello.cc -o hello 

GDB offers a big list of commands, however the following commands are the ones 
used most frequently:

b main -- Puts a breakpoint at the beginning of the program
b -- Puts a breakpoint at the current line
b N - Puts a breakpoint at line N
b +N - Puts a breakpoint N lines down from the current line
b fn - Puts a breakpoint at the beginning of function "fn"
d N - Deletes breakpoint number N
info break - list breakpoints
r - Runs the program until a breakpoint or error
c - Continues running the program until the next breakpoint or error
f - Runs until the current function is finished
s - Runs the next line of the program
s N - Runs the next N lines of the program
n - Like s, but it does not step into functions
p var - Prints the current value of the variable "var"
bt - Prints a stack trace
u - Goes up a level in the stack
d - Goes down a level in the stack
q - Quits gdb

Debugging Programs:

gcc -g myprogram.c

Compiles myprogram.c with the debugging option (-g). You still get an a.out, but 
it contains debugging information that lets you use variables and function names 
inside GDB, rather than raw memory locations (not fun).

gdb a.out

Opens GDB with file a.out, but does not run the program. You’ll see a prompt (gdb) - all examples are from this prompt.

r
r arg1 arg2
r < file1

Three ways to run “a.out”, loaded previously. You can run it directly (r), pass 
arguments (r arg1 arg2), or feed in a file. You will usually set breakpoints 
before running.

Stepping through Code:

l
l 50
l myfunction

Lists 10 lines of source code for current line (l), a specific line (l 50), or 
for a function (l myfunction).

next

Runs the program until next line, then pauses. If the current line is a function,
it executes the entire function, then pauses. next is good for walking through 
your code quickly.

step

Runs the next instruction, not line. If the current instruction is setting a 
variable, it is the same as next. If it’s a function, it will jump into the 
function, execute the first statement, then pause. step is good for diving into 
the details of your code.

finish

Finishes executing the current function, then pause (also called step out). 
Useful if you accidentally stepped into a function.

Breakpoints:

Breakpoints play an important role in debugging. They pause (break) a program when it reaches a certain point.

break 45
break myfunction

Sets a breakpoint at line 45, or at myfunction. The program will pause when it 
reaches the breakpoint.

watch x == 3

Sets a watchpoint, which pauses the program when a condition changes (when x == 3 changes). Watchpoints are great for certain inputs (myPtr != NULL)

continue

Resumes execution after being paused by a breakpoint/watchpoint. The program will continue until it hits the next breakpoint/watchpoint.

delete N
Deletes breakpoint N (breakpoints are numbered when created).

Viewing and changing variables at runtime is a critical part of debugging:
print x
Prints current value of variable x. 

set x = 3
set x = y

Sets x to a set value (3) or to another variable (y)

call myfunction()
call myotherfunction(x)
call strlen(mystring)

Calls user-defined or system functions. This is extremely useful, but beware of 
calling buggy functions.

display x
Constantly displays the value of variable x, which is shown after every step or 
pause. Useful if you are constantly checking for a certain value.

undisplay x
Removes the constant display of a variable displayed by display command.

Backtrace and Changing Frames:
A stack is a list of the current function calls - it shows you where you are in 
the program. A frame stores the details of a single function call, such as the 
arguments.

bt
Backtraces or prints the current function stack to show where you are in the current program. If main calls function a(), which calls b(), which calls c(), the 
backtrace is

c <= current location 
b 
a 
main 

----------

up
down

Move to the next frame up or down in the function stack. If you are in c, you can move to b or a to examine local variables.

return
Returns from current function.



