#Functions and Program Structure

Functions break large computing tasks into smaller ones, and enable 
people to build on what others have done instead of starting over from 
scratch.

C has been designed to make functions efficient and easy to use; 
C programs generally consist of many small functions rather than a 
few big ones. A program may reside in one or more source files. 
Source files may be compiled separately and loaded together, along with
previously compiled functions from libraries.

Each function definition has the form

```c
return-type function-name(argument declarations)
{
	declarations and statements
}
```

Various parts may be absent; a minimal function is

```c
dummy() {}
```

which does nothing and returns nothing. A do-nothing function like this
is sometimes useful as a place holder during program development. If 
the return type is omitted, `int` is assumed.

A program is just a set of definitions of variables and functions. 
Communication between the functions is by arguments and values returned
by the functions, and through external variables. The functions can 
occur in any order in the source file, and the source program can be 
split into multiple files, so long as no function is split.

The `return` statement is the mechanism for returning a value from the 
called function to its caller. Any expression can follow `return`:

```c
return expression;
```

The *expression* will be converted to the return type of the function 
if necessary. Parentheses are often used around the *expression*, but 
they are optional.

The calling function is free to ignore the returned value. Furthermore,
there need to be no expression after `return;`. In that case, no value 
is returned to the caller. Control also returns to the caller with no 
value when execution "falls off the end" of the function by reaching 
the closing right brace.

The mechanics of how to compile and load a C program that resides on 
multiple source files vary from one system to the next.
On the UNIX system, for example, the `cc` command does the job.
Suppose that the three functions are stored in three files called 
`main.c`, `getline.c`, and `strindex.c`. Then the command

```
cc main.c getline.c strindex.c
```

compiles the three files, placing the resulting object code in files 
`main.o`, `getline.o`, and `strindex.o`, then loads them all into an 
executable file called `a.out`.

If there is an error, say in `main.c`, the file can be recompiled by 
itself and the result loaded with the previous object files, with the 
command

```
cc main.c getline.o strindex.o
```

The `cc` command uses the ".c" versus ".o" naming convention to 
distinguish source files from object files.

#External Variables

A C program consists of a set of external objects, which are either 
variables or functions. The adjective "external" is used in contrast to
"internal", which describes the arguments and variables defined inside 
functions. 

External variables are defined outside of any function, and are thus 
potentionally available to many functions. Functions themselves are 
always external, because C does not allow functions to be defined 
inside other functions.

By default, external variables and functions have the property that 
all references to them by the same name, even from functions compiled 
separately, are references to the same thing. (The standard calls this 
property *external linkage*.)

Because external variables are globally accessible, they provide an 
alternative to function arguments and return values for communicating 
data between functions. Any function may access an external variable 
by referring to it by name, if the name has been declared somehow.

We will see later how to define external variables and functions that are visible only within a single source file.

External variables are also useful because of their greater scope and 
lifetime. Automatic variables are internal to a function; they come 
into existence when the function is entered, and disappear when it is 
left. External variables, on the other hand, are permanent, so they 
can retain values from one function invocation to the next. Thus if 
two functions must share some data, yet neither calls the other, it is
often most convenient if the shared data is kept in external variables
rather than being passed in and out via arguments.

##getch and ungetch 

What are `getch` and `ungetch`? It is often the case that a program 
cannot determine that it has read enough input until it has read too 
much. One instance is collecting characters that make up a number: 
until the first non-digit is seen, the number is not complete. 
But then the program has read one character too far, a character that 
it is not prepared for.

The problem would be solved if it were possible to "un-read" the 
unwanted character. Then, every time the program reads one character 
too many, it could push it back on the input, so the rest of the code 
could behave as if it had never been read. Fortunately, it's easy to 
simulate un-getting a character, by writing a pair of cooperating 
functions. `getch` delivers the next input character to be considered;`ungetch` will return them before reading new input.

How they work together is simple. `ungetch` puts the pushed-back 
characters into a shared buffer - a character array. `getch` reads 
from the buffer if there is anything else, and calls `getchar` if the 
buffer is empty. There must also be an index variable that records the
position of the current character in the buffer.

Since the buffer and the index are shared by `getch` and `ungetch` and
must retain their values between calls, they must be external to both 
routines.

The standard library includes a function `ungetch` that provides one 
character of pushback; we will discuss it in Chapter 7.

#Scope Rules

The functions and external variables that make up a C program need not
all be compiled at the same time; the source text of the program may 
be kept in several files, and previously compiled routines may be 
loaded from libraries.

The *scope* of a name is the part of the program within which the name
can be used. For an automatic variable declared at the beginning of a 
function, the scope is the function in which the name is declared. 
Local variables of the same name in different functions are unrelated.
The same is true of the parameters of the function, which are in 
effect local variables.

The scope of an external variable or a function lasts from the point 
at which it is declared to the end of the file being compiled.

For example, if `main`, `sp`, `val`, `push`, and `pop` are defined in 
one file, in the order shown below, that is,

```c
main() { ... }

int sp = 0;
double val[MAXVAL];

void push(double f) { ... }

double pop(void) { ... }
```

then the variables `sp` and `val` may be used in `push` and `pop` 
simply by naming them; no further declarations are needed. But these 
names are not visible in `main`, nor are `push` and `pop` themselves.

On the other hand, if an external variable is to be referred to before
it is defined, or if it is defined in a different source file from the
one where it is being used, then an `extern` declaration is mandatory.

It is important to distinguish between the *declaration* of an 
external variable and its *definition*. A declaration announces the 
properties of a variable (primarily its type); a definition also 
causes storage to be set aside. If the lines

```c
int sp;
double val[MAXVAL];
```

appear outside of any function, they *define* the external variables 
`sp` and `val`, cause storage to be set aside, and also serve as the 
declarations for the rest of that source file. On the other hand, 
the lines

```c
extern int sp;
extern double val[];
```

*declare* for the rest of the source file that `sp` is an `int` and 
that `val` is a double array (whose size is determined elsewhere), but
they do not create the variables or reserve storage for them.

There must be only one *definition* of an external variable among all 
the files that make up the source program; other files may contain 
*extern* declarations to access it. (There may also be extern 
declarations in the file containing the definition.) Array sizes must 
be specified with the definition, but are optional with an extern 
declaration.

Initialization of an external variable goes only with the definition.

#Static Variables

The `static` declaration, applied to an external variable or function,
limits the scope of that object to the rest of the source file being 
compiled.

External static thus provides a way to hide names, which must be 
external so they can be shared, yet which should not be visible to 
users of other files.

Static storage is specified by prefixing the normal declaration with 
the word `static`.

If the two routines and the two variables are compiled in one file, as
in

```c
static char buf[BUFSIZE];  /* buffer for ungetch */
static int bufp = 0;       /* next free position in buf */

int getch(void) { ... }

void ungetch(int c) { ... }
```

then no other routine will be able to access `buf` and `bufp`, and 
those names will not conflict with the same names in other files of 
the same program.

The external `static` declaration is most often used for variables, 
but it can be applied to functions as well. Normally, function names 
are global, visible to any part of the entire program. If a function 
is declared `static`, however, its name is invisible outside of the 
file in which it is declared.

The `static` declaration can also be applied to internal variables. 
Internal static variables are local to a particular function just as 
automatic variables are, but unlike automatics, they remain in 
existence rather than coming and going each time the function is 
activated. This means that internal static variables provide private, 
permanent storage within a single function.

#Register Variables

A `register` declaration advises the compiler that the variable in 
question will be heavily used. The idea is that register variables 
are to be placed in machine registers, which may result in smaller and
faster programs. But compilers are free to ignore the advice.

The `register` declaration looks like

```c
register int  x;
register char c;
```

and so on. The `register` declaration can only be applied to automatic
variables and to the formal parameters of a function. In this later 
case, it looks like

```c
f(register unsigned m, register long n)
{
	register int i;
	...
}
```

In practice, there are restrictions on register variables, reflecting the realities of underlying hardware. Only a few variables in each 
function may be kept in registers, and only certain types are allowed.
And it is not possible to take the address of a register variable.
The specific restrictions on number and types of register variables 
vary from machine to machine.

#Block Structure

In C functions may not be defined within other functions. On the other
hand, variables can be defined in a block-structured fashion within a 
function.

Declarations of variables (including initializations) may follow the 
left brace not just the one that begins a function.
Variables declared in this way hide any identically named variables in
outer blocks, and remain in existence until the matching right brace.

Automatic variables, including formal parameters, also hide external 
variables and functions of the same name. Given the declarations

```c
int x;
int y;

f(double x)
{
	double y;
}
```

then within the function `f`, occurrences of `x` refer to the 
parameter, which is a `double`; outside `f`, they refer to the 
external `int`. The same is true of the variable `y`.

As a matter of style, it's best to avoid variable names that conceal 
names in an outer scope; the potential for confusion and error is too 
great.

#Initialization

In the absence of explicit initialization, external and static 
variables are guaranteed to be initialized to zero; automatic and 
register variables have undefined (i.e., garbage) initial values.

For external and static variables, the initializer must be a 
constant expression; the initialization is done once, conceptionally 
before the program begins execution.

For automatic and register variables, the initializer is not 
restricted to being a constant: it may be any expression involving 
previously defined values, even function calls.

An array may be initialized by following its declaration with a list 
of initializers enclosed in braces and separated by commas. 
For example, to initialize an array `days` with the number of days in 
each month:

```c
int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
```

When the size of the array is omitted, the compiler will compute the 
length by counting the initializers, of which there are 12 in 
this case.

If there are fewer initializers for an array than the specified size, 
the others will be zero for external, static and automatic variables. It is an error to have too many initializers. 

There is no way to specify repetition of an initializer, nor to 
initialize an element in the middle of an array without supplying all 
the preceding values as well.

Character arrays are a special case of initialization; a string may be
used instead of the braces and commas notation:

```c
char pattern = "ould";
```

is a shorthand for the longer but equivalent

```c
char pattern[] = { 'o', 'u', 'l', 'd', '\0' };
```

In this case, the array size is five (four characters plus the 
terminating `'\0'`).

#Recursion

C functions may be used recursively; that is, a function may call 
itself either directly or indirectly.

When a function calls itself recursively, each invocation gets a fresh
set of all the automatic variables, independent of the previous set.

#The C Preprocessor

C provides certain language facilities by means of a preprocessor, 
which is conceptionally a separate first step in compilation. The two 
most frequently used features are `#include`, to include the contents 
of a file during compilation, and `#define`, to replace a token by an 
arbitrary sequence of characters. Other features described in this 
section include conditional compilation and macros with arguments.

##File Inclusion

File inclusion makes it easy to handle collections of `#defines` and 
declarations (among other things). Any source line of the form

```c
#include "filename"
```

or

```c
#include <filename>
```

is replaced by the contents of the file filename. If the filename is 
quoted, searching for the file typically begins where the source 
program was found; if it is not found there, or if the name is 
enclosed in `<` and `>`, searching follows an implementation-defined 
rule to find the file. An included file may itself contain 
`#include` lines.

There are often several `#include` lines at the beginning of a source 
file, to include common `#define` statements and extern declarations, 
or to access the function prototype declarations for library functions
from headers like `<stdio.h>`.

`#include` is the preferred way to tie the declarations together for 
a large program. It guarantees that all the source files will be 
supplied with the same definitions and variable declarations, and thus
eliminates a particularly nasty kind of bug. 

Naturally, when an included file is changed, all files that depend on 
it must be recompiled.

#Macro Substitution

A definition has the form

```c
#define name replacement text
```

It calls for a macro substitution of the simplest kind - subsequent 
occurrences of the token `name` will be replaced by the `replacement 
text`.

The name in a `#define` has the same form as a variable name; the 
replacement text is arbitrary. Normally the replacement text is the 
rest of the line, but a long definition may be continued onto 
several lines by placing a `\` at the end of each line to be 
continued.

The scope of a name defined with `#define` is from its point of 
definition to the end of the source file being compiled.

A definition may use previous definitions.

Substitutions are made only for tokens, and do not take place within 
quoted strings. For example, if `YES` is a defined name, there would 
be no substitution in `printf("YES")`

It is also possible to define macros with arguments, so the 
replacement text can be different for different calls of the macro. 
As an example, define a macro called `max`:

```c
#define  max(A, B)  ((A) > (B) ? (A) : (B))
```

Although it looks like a function call, a use of `max` expands into 
in-line code. Each occurrence of a formal parameter (here `A` or `B`) 
will be replaced by the corresponding actual argument.

Nonetheless, macros are valuable. One practical example comes from 
`<stdio.h>`, in which `getchar` and `putchar` are often defined as 
macros to avoid the run-time overhead of a function call per character
processed. The functions in `<ctype.h>` are also usually implemented 
as macros.

Names may be undefined with `#undef`, usually to ensure that a routine
is really a function, not a macro:

```c
#undef getchar

int getchar(void) { ... }
```

Formal parameters are not replaced within quoted strings. If, however,
a parameter name is preceded by a `#` in the replacement text, the 
combination will be expanded into a quoted string with the parameter 
replaced by the actual argument.

for example

```c
#define  dprint(expr)   printf(#expr " = %llf\n", expr)
```

When this is invoked, as in

```
dprint(x/y)
```

the macro is expanded into

```
printf("x/y" " = %llf\n", x/y);
```

and the strings are concatenated, so the effect is

```
printf("x/y = %llf\n", x/y);
```

Within the actual argument, each `"` is replaced by `\"` and each 
`\` by `\\`, so the result is a legal string constant.

The preprocessor operator `##` provides a way to concatenate actual 
arguments during macro expansion. If a parameter in the replacement 
text is adjacent to a `##`, the parameter is replaced by the actual 
argument, the `##` and surrounding white space are removed, and the 
result is re-scanned. For example, the macro paste concatenates its 
two arguments:

```c
#define  paste(front, back)  front ## back
```

so `paste(name, 1)` creates the token `name1`.

##Conditional Inclusion

It is possible to control preprocessing itself with conditional 
statements that are evaluated during preprocessing.

The `#if` line evaluates a constant integer expression (which may not 
include `sizeof`, casts, or `enum` constants).

If the expression is non-zero, subsequent lines until an `#endif` or 
`#elif` or `#else` are included.(The preprocessor statement `#elif` is
like else-if.) 

The expression `defined(name)` in a `#if` is 1 if the `name` has been 
defined, and 0 otherwise.

For example, to make sure that the contents of a file `hdr.h` are 
included only once, the contents of the file are surrounded with a 
conditional like this:

```c
#if !defined(HDR)
#define HDR

/* contents of hdr.h go here */

#endif
```

The first inclusion of `hdr.h` defines the name `HDR`; subsequent 
inclusions will find the name defined and skip down to the `#endif`. 
A similar style can be used to avoid including files multiple times. 
If this style is used consistently, then each header can itself 
include any other headers on which it depends, without the user of the
header having to deal with the interdependence.

This sequence tests the name `SYSTEM` to decide which version of a 
header to include:

```c
#if SYSTEM == SYSV
	#define HDR "sysv.h"
#elif SYSTEM == BSD
	#define HDR "bsd.h"
#elif SYSTEM == MSDOS
	#define HDR "msdos.h"
#else
	#define HDR "default.h"
#endif
#include HDR
```

The `#ifdef` and `#ifndef` lines are specialized forms that test 
whether a name is defined. The first example of `#if` above could have
been written

```
#ifndef HDR
#define HDR

/* contents of hdr.h go here */

#endif
```
