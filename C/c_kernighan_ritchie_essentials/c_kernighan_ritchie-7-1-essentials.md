#Input and Output

Input and output are not part of the C language itself. Nonetheless, 
programs interact with their environment in much more complicated ways 
than those we have shown before.

In this chapter we will describe the standard library, a set of 
functions that provide input and output, string handling, storage 
management, mathematical routines, and a variety of other services 
for C programs. 

#Standard Input and Output

The library implements a simple model of text input and output. A text 
stream consists of a sequence of lines; each line ends with a newline 
character. If the system doesn't operate that way, the library does 
whatever necessary to make it appear as if it does.

The simplest input mechanism is to read one character at a time from 
the *standard input*, normally the keyboard, with `getchar`:

```c
int getchar(void)
```

`getchar` returns the next input character each time it is called, or 
`EOF` when it encounters end of file. The symbolic constant `EOF` is 
defined in `<stdio.h>`. The value is typically -1, bus tests should be 
written in terms of `EOF` so as to be independent of the specific 
value.

The function

```c
int putchar(int)
```

is used for output: `putchar(c)` puts the character `c` on the standard
output, which is by default the screen. `putchar` returns the character
written, or `EOF` if an error occurs. 

Output produced by `printf` also finds its way to the standard output. 
Calls to `putchar` and `printf` may be interleaved - output happens in 
the order in which the calls are made.

Each source file that refers to an input/output library function must 
contain the line

```c
#include <stdio.h>
```

before the first reference. When the name is bracketed by < and > a 
search is made for the header in a standard set of places (for example,
on UNIX systems, typically in the directory `/usr/include`).

Many programs read only one input stream and write only one output 
stream; for such programs, input and output with `getchar`, `putchar`, 
and `printf` may be entirely adequate, and is certainly enough to get 
started.

For example, consider the program `lower`, which converts its input to 
lower case:

```c
#include <stdio.h>
#include <ctype.h>

main() /* lower: convert input to lower case*/
{
	int c;

	while ((c = getchar()) != EOF)
		putchar(tolower(c));
	return 0;
}
```

The function `tolower` is defined in `<ctype.h>`; it converts an upper 
case letter to lower case, and returns other characters untouched. As 
we mentioned earlier, "functions" like `getchar` and `putchar` in 
`<stdio.h>` and `tolower` in `<ctype.h>` are often macros, thus 
avoiding the overhead of a function call per character.

#Formatted Output - printf

The output function `printf` translates internal values to characters.

```c
int printf(char *format, arg1, arg2, ...);
```

`printf` converts, formats, and prints its arguments on the standard 
output under control of the format. It returns the number of characters
printed.

A width or precision may be specified as `*`, in which case the value 
is computed by converting the next argument (which must be an `int`). 
For example, to print at most `max` characters from a string `s`,

```c
 printf("%.*s", max, s);
 ```

The following table shows the effect of a variety of specifications in
printing "hello, world" (12 characters). We have put colons around 
each field so you can see it extent.

```c
:%s:          :hello, world:
:%10s:        :hello, world:
:%.10s:       :hello, wor:
:%-10s:       :hello, world:
:%.15s:       :hello, world:
:%-15s:       :hello, world   :
:%15.10s:     :     hello, wor:
:%-15.10s:    :hello, wor     :
```

A warning: `printf` uses its first argument to decide how many 
arguments follow and what their type is. It will get confused, and you
will get wrong answers, if there are not enough arguments of if they 
are the wrong type. You should also be aware of the difference between
these two calls:

```c
printf(s);         /* FAILS if s contains % */
printf("%s", s);   /* SAFE */
```

The function `sprintf` does the same conversions as `printf` does, but
stores the output in a string:

```c
int sprintf(char *string, char *format, arg1, arg2, ...);
```

`sprintf` formats the arguments in `arg1`, `arg2`, etc., according to 
format as before, but places the result in string instead of the 
standard output; `string` must be big enough to receive the result.

#Variable-length Argument Lists

This section contains an implementation of a minimal version of 
`printf`, to show how to write a function that processes a 
variable-length argument list in a portable way.

Since we are mainly interested in the argument processing, `minprintf`
will process the format string and arguments but will call the real 
`printf` to do the format conversions.

The proper declaration for `printf` is

```c
int printf(char *fmt, ...)
```

where the declaration `...` means that the number and types of these 
arguments may vary. The declaration `...` can only appear at the end 
of an argument list. Our `minprintf` is declared as

```c
void minprintf(char *fmt, ...)
```

The tricky bit is how `minprintf` walks along the argument list when 
the list doesn't even have a name. The standard header `<stdarg.h>` 
contains a set of macro definitions that define how to step through an
argument list. The implementation of this header will vary from 
machine to machine, but the interface it presents is uniform.

The type `va_list` is used to declare a variable that will refer to 
each argument in turn; in `minprintf`, this variable is called `ap`, 
for "argument pointer". The macro `va_start` initializes `ap` to point
to the first unnamed argument. It must be called once before `ap` is 
used. There must be **at least one** named argument; the final named 
argument is used by `va_start` to get started.

Each call of `va_arg` returns one argument and steps `ap` to the next;`va_arg` uses a type name to determine what type to return and how big
a step to take. Finally, `va_end` does whatever cleanup is necessary. 
It must be called before the program returns.

These properties form the basis of our simplified `printf`:

```c
#include <stdarg.h>

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...)
{
	va_list ap; /* points to each unnamed arg in turn */
	char *p, *sval;
	int ival;
	double dval;

	va_start(ap, fmt); /* make ap point to 1st unnamed arg */
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		switch (*++p) {
			case 'd':
				ival = va_arg(ap, int);
				printf("%d", ival);
				break;
			case 'f':
				dval = va_arg(ap, double);
				printf("%f", dval);
				break;
			case 's':
				for (sval = va_arg(ap, char *); *sval; sval++)
					putchar(*sval);
				break;
			default:
				putchar(*p);
				break;
		}
	}
	va_end(ap); /* clean up when done */
}
```

#Formatted Input - Scanf

The function `scanf` is the input analog of `printf`, providing many 
of the same conversion facilities in the opposite direction.

```c
int scanf(char *format, ...)
```

`scanf` reads characters from the standard input, interprets them 
according to the specification in format, and stores the results 
through the remaining arguments.

Other arguments, each of which must be a **pointer**, indicate where 
the corresponding converted input should be stored.

`scanf` stops when it exhausts its format string, or when some input 
fails to match the control specification. It returns as its value the 
number of successfully matched and assigned input items. This can be 
used to decide how many items were found. On the end of file, `EOF` is
returned;

There is also a function `sscanf` that reads from a string instead of 
the standard input:

```c
int sscanf(char *string, char *format, arg1, arg2, ...)
```

It scans the string according to the format in format and stores the 
resulting values through `arg1`, `arg2`, etc. These arguments must be 
pointers.

The conversion character indicates the interpretation of the input 
field. The corresponding argument must be a pointer, as required by 
the call-by-value semantics of C.

Suppose we want to read input lines that contain dates of the form

```
25 Dec 1988
```

The `scanf` statement is

```c
int day, year;
char monthname[20];

scanf("%d %s %d", &day, monthname, &year);
```

No `&` is used with `monthname`, since an array name is a pointer.

`scanf` ignores blanks and tabs in its format string. Furthermore, it 
skips over white space (blanks, tabs, newlines, etc.) as it looks for 
input values.

Suppose we want to read lines that might contain a date in either of 
the forms. Then we could write

```c
while (getline(line, sizeof(line)) > 0) {
	if (sscanf(line, "%d %s %d", &day, monthname, &year) == 3)
		printf("valid: %s\n", line); /* 25 Dec 1988 form */
	else if (sscanf(line, "%d/%d/%d", &month, &day, &year) == 3)
		printf("valid: %s\n", line); /* mm/dd/yy form */
	else
		printf("invalid: %s\n", line); /* invalid form */
}
```

A final warning: the arguments to `scanf` and `sscanf` must be 
pointers. By far the most common error is writing

```c
scanf("%d", n);
```

instead of

```c
scanf("%d", &n);
```
