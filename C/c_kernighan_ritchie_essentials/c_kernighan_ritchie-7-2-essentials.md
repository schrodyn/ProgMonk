#File Access

The rules are simple. Before it can be read or written, a file has to 
be *opened* by the library function `fopen`. `fopen` takes an external 
name like `x.c` or `y.c`, does some housekeeping and negotiation with 
the operating system (details of which needn't concern us), and returns
a pointer to be used in subsequent reads or writes of the file.

This pointer, called the *file pointer*, points to a structure that 
contains information about the file, such as the location of a buffer, 
the current character position in the buffer, whether the file is being
read or written, and whether errors or end of file have occurred.

Users don't need to know the details, because the definitions obtained from `<stdio.h>` include a structure declaration called `FILE`. 
The only declaration needed for a file pointer is exemplified by

```c
FILE *fp;
FILE *fopen(char *name, char *mode);
```

This says that `fp` is a pointer to a `FILE`, and `fopen` returns a 
pointer to a `FILE`. Notice that `FILE` is a type name, like `int`, not
a structure tag; it is defined with a `typedef`. 

The call to `fopen` in a program is

```c
fp = fopen(name, mode);
```

The first argument of `fopen` is a character string containing the name
of the file. The second argument is the *mode*, also a character 
string, which indicates how one intends to use the file. 

Allowable modes include read ("r"), write ("w"), and append ("a"). Some
systems distinguish between text and binary files; for the latter, 
a "b" must be appended to the mode string.

If a file that does not exist is opened for *writing* or *appending*, 
it is created if possible. Opening an existing file for writing causes 
the old contents to be discarded, while opening for appending 
preserves them.

Trying to read a file that does not exist is an error, and there may be
other causes of error as well, like trying to read a file when you 
don't have permission. If there is any error, `fopen` will return 
`NULL`.

The next thing needed is a way to read or write the file once it is 
open. `getc` returns the next character from a file; it needs the file 
pointer to tell it which file.

```c
int getc(FILE *fp)
```

`getc` returns the next character from the stream referred to by `fp`; 
it returns `EOF` for end of file or error.

`putc` is an output function:

```c
int putc(int c, FILE *fp)
```

`putc` writes the character `c` to the file `fp` and returns the 
character written, or `EOF` if an error occurs. Like `getchar` and 
`putchar`, `getc` and `putc` may be macros instead of functions.

When a C program is started, the operating system environment is 
responsible for opening three files and providing pointers for them. 
These files are the standard input, the standard output, and the 
standard error; the corresponding file pointers are called `stdin`, 
`stdout`, and `stderr`, and are declared in `<stdio.h>`. Normally 
`stdin` is connected to the keyboard and `stdout` and `stderr` are 
connected to the screen, but `stdin` and `stdout` may be redirected to 
files or pipes.

`getchar` and `putchar` can be defined in terms of `getc`, `putc`, 
`stdin`, and `stdout` as follows:

```c
#define getchar()    getc(stdin)
#define putchar(c)   putc((c), stdout)
```

For formatted input or output of files, the functions `fscanf` and 
`fprintf` may be used. These are identical to `scanf` and `printf`, 
except that the first argument is a file pointer that specifies the 
file to be read or written; the format string is the second argument.

```c
int fscanf(FILE *fp, char *format, ...)
int fprintf(FILE *fp, char *format, ...)
```

The file pointers `stdin` and `stdout` are objects of type `FILE *`. 
They are constants, however, *not* variables, so it is not possible to
assign to them.

The function

```c
int fclose(FILE *fp)
```

is the inverse of `fopen`, it breaks the connection between the file 
pointer and the external name that was established by `fopen`, freeing
the file pointer for another file. 

Since most operating systems have some limit on the number of files 
that a program may have open simultaneously, it's a good idea to free 
the file pointers when they are no longer needed

There is also another reason for `fclose` on an output file - it 
flushes the buffer in which `putc` is collecting output. `fclose` is 
called automatically for each open file when a program terminates 
normally. (You can close `stdin` and `stdout` if they are not needed. 
They can also be reassigned by the library function `freopen`.)

#Error Handling - Stderr and Exit

To handle error output, a second output stream, called `stderr`, is 
assigned to a program in the same way that `stdin` and `stdout` are. 
Output written on `stderr` normally appears on the screen even if the 
standard output is redirected.

The program signals errors in two ways. First, the diagnostic output 
produced by `fprintf` goes to `stderr`, so it finds its way to the 
screen instead of disappearing down a pipeline or into an output file.

Second, the program uses the standard library function `exit`, which 
terminates program execution when it is called. The argument of `exit`
is available to whatever process called this one, so the success or 
failure of the program can be tested by another program that uses this
one as a sub-process. Conventionally, a return value of 0 signals that
all is well; non-zero values usually signal abnormal situations. 
`exit` calls `fclose` for each open output file, to flush out any 
buffered output.

Within `main`, `return` *expr* is equivalent to `exit(expr)`. `exit` 
has the advantage that it can be called from other functions.

The function `ferror` returns non-zero if an error occurred on the 
stream `fp`.

```c
int ferror(FILE *fp)
```

The function `feof(FILE *)` is analogous to `ferror`; it returns 
non-zero if end of file has occurred on the specified file.

```c
int feof(FILE *fp)
```

#Line Input and Output

The standard library provides an input and output routine `fgets`

```c
char *fgets(char *line, int maxline, FILE *fp)
```

`fgets` reads the next input line (including the newline) from file 
`fp` into the character array line; at most `maxline-1` characters 
will be read. The resulting line is terminated with `'\0'`.

Normally `fgets` returns `line`; on end of file or error it returns 
`NULL`.

For output, the function `fputs` writes a string (which need not 
contain a newline) to a file:

```c
int fputs(char *line, FILE *fp)
```

It returns `EOF` if an error occurs, and non-negative otherwise.

The library functions `gets` and `puts` are similar to `fgets` and 
`fputs`, but operate on `stdin` and `stdout`. Confusingly, `gets` 
deletes the terminating '\n', and `puts` adds it.

#Miscellaneous Functions

The standard library provides a wide variety of functions. This 
section is a brief synopsis of the most useful.

##String Operations

We have already mentioned the string functions `strlen`, `strcpy`, 
`strcat`, and `strcmp`, found in `<string.h>`. 


`strcat(s,t)` -- concatenate `t` to end of `s`

`strncat(s,t,n)` -- concatenate n characters of t to end of s

`strcmp(s,t)` -- return negative, zero, or positive for 
`s < t`, `s == t`, `s > t`

`strncmp(s,t,n)` -- same as `strcmp` but only in first n characters

`strcpy(s,t)` -- copy t to s

`strncpy(s,t,n)` -- copy at most n characters of t to s

`strlen(s)` -- return length of s

`strchr(s,c)` -- return pointer to first c in s, or NULL if not 
present

`strrchr(s,c)` -- return pointer to last c in s, or NULL if not 
present

##Character Class Testing and Conversion

Several functions from `<ctype.h>` perform character tests and 
conversions. The function returns `int`.

`isalpha(c)` -- non-zero if c is alphabetic, 0 if not

`isupper(c)` -- non-zero if c is upper case, 0 if not

`islower(c)` -- non-zero if c is lower case, 0 if not

`isdigit(c)` -- non-zero if c is digit, 0 if not

`isalnum(c)` -- non-zero if `isalpha(c)` or `isdigit(c)`, 0 if not

`isspace(c)` -- non-zero if c is blank, tab, newline, return, 
formfeed, vertical tab

`toupper(c)` -- return c converted to upper case

`tolower(c)` -- return c converted to lower case

##Ungetc

The standard library provides a function that pushes the character 
`c` back onto file `fp`, and returns either `c`, or `EOF` for an 
error.

```c
int ungetc(int c, FILE *fp)
```

`ungetc` may be used with any of the input functions like `scanf`, 
`getc`, or `getchar`.

##Command Execution

The function `system(char *s)` executes the command contained in the 
character string `s`, then resumes execution of the current program.

The contents of `s` depend strongly on the local operating system. As 
a trivial example, on UNIX systems, the statement

```c
system("date");
```

causes the program `date` to be run; it prints the date and time of 
day on the standard output. 

`system` returns a system-dependent integer status from the command 
executed. In the UNIX system, the status return is the value returned 
by `exit`.

##Storage Management

The functions `malloc` and `calloc` obtain blocks of memory 
dynamically.

```c
void *malloc(size_t n)
```

returns a pointer to `n` bytes of uninitialized storage, or NULL if 
the request cannot be satisfied.

```c
void *calloc(size_t n, size_t size)
```

returns a pointer to enough free space for an array of `n` objects of 
the specified size, or NULL if the request cannot be satisfied. The 
storage is initialized to zero.

The pointer returned by `malloc` or `calloc` has the proper alignment 
for the object in question, but it must be cast into the appropriate 
type, as in

```c
int *ip;

ip = (int *) calloc(n, sizeof(int));
```

`free(p)` frees the space pointed to by `p`, where `p` was originally 
obtained by a call to `malloc` or `calloc`. There are no restrictions 
on the order in which space is freed, but it is a ghastly error to 
free something not obtained by calling `malloc` or `calloc`.

It is also an error to use something *after* it has been freed. 
A typical but incorrect piece of code is this loop that frees items 
from a list:

```c
for (p = head; p != NULL; p = p->next) /* WRONG */
	free(p);
```

The right way is to save whatever is needed before freeing:

```c
for (p = head; p != NULL; p = q) {
	q = p->next;
	free(p);
}
```

##Mathematical Functions

There are more than twenty mathematical functions declared in 
`<math.h>`; here are some of the more frequently used.

`sin(x)` -- sine of x, x in radians

`cos(x)` -- cosine of x, x in radians

`atan2(y,x)` -- arctangent of y/x, in radians

`exp(x)` -- exponential function ex

`log(x)` -- natural (base e) logarithm of x (x>0)

`log10(x)` -- common (base 10) logarithm of x (x>0)

`pow(x,y)` -- x^y^

`sqrt(x)` -- square root of x (x>0)

`fabs(x)` -- absolute value of x

##Random Number generation

The function `rand()` computes a sequence of pseudo-random integers in
the range zero to `RAND_MAX`, which is defined in `<stdlib.h>`. One 
way to produce random floating-point numbers greater than or equal to 
zero but less than one is

```c
#define frand() ((double) rand() / (RAND_MAX+1.0))
```

The function `srand(unsigned)` sets the seed for `rand`.


