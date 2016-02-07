#Character Input and Output

The model of input and output supported by the standard library is very simple. Text input or output, regardless of where it originates or where it goes to, is dealt with as streams of characters. A text stream is a sequence of characters divided into lines; each line consists of zero or more characters followed by a newline character. It is the responsibility of the library to make each input or output stream confirm this model.

The standard library provides several functions for reading or writing 
one character at a time, of which getchar and putchar are the simplest.
Each time it is called, getchar reads the next input character from a 
text stream and returns that as its value. That is, after

```
c = getchar();
```

the variable `c` contains the next character of input. The characters 
normally come from the keyboard.

The function `putchar` prints a character each time it is called:

```
putchar(c);
```

prints the contents of the integer variable `c` as a character, 
usually on the screen.

```
int c;
while((c = getchar()) != EOF) {
  ...
```

The problem is distinguishing the end of input from valid data. 
The solution is that `getchar` returns a distinctive value when there 
is no more input, a value that cannot be confused with any real 
character. This value is called `EOF`, for "end of file". We must 
declare `c` to be a type big enough to hold any value that `getchar` 
returns. We can't use char since `c` must be big enough to hold `EOF` 
in addition to any possible char. Therefore we use `int`.

`EOF` is an integer defined in `<stdio.h>`, but the specific numeric 
value doesn't matter as long as it is not the same as any char value. 
By using the symbolic constant, we are assured that nothing in the 
program depends on the specific numeric value.

#Functions

Function definitions can appear in any order, and in one source file 
or several, although no function can be split between files.

The function declaration declares the parameter types and names, and 
the type of the result that the function returns.

The function declaration:

```
int power(int base, int n);
```

says that `power` is a function that expects two `int` arguments and 
returns an `int`. This declaration, which is called a *function 
prototype*, has to agree with the definition and uses of `power`. It is 
an error if the definition of a function or any uses of it do not agreewith its prototype.

parameter names need not agree. Indeed, parameter names are optional in
a function prototype, so for the prototype we could have written

```
int power(int, int);
```

Well-chosen names are good documentation however.

#Arguments - Call by Value

In C, all function arguments are passed "by value". This means that the
called function is given the values of its arguments in temporary 
variables rather than the originals.

When necessary, it is possible to arrange for a function to modify a 
variable in a calling routine. The caller must provide the *address* of
the variable to be set (technically a *pointer* to the variable), and 
the called function must declare the parameter to be a pointer and 
access the variable indirectly through it.

The story is different for arrays. When the name of an array is used as
an argument, the value passed to the function is the location or 
address of the beginning of the array - there is no copying of array 
elements.

#Character Arrays

the character '\0' (the null character, whose value is zero) marks the 
end of the string of characters. This conversion is also used by the 
C language: when a string constant like

```
"hello\n"
```

appears in a C program, it is stored as an array of characters 
containing the characters in the string and terminated with a 
'\0' to mark the end.

```
hello\n\0
```

The `%s` format specification in `printf` expects the corresponding 
argument to be a string represented in this form. 

#External Variables and Scope

Each local variable in a function comes into existence only when the 
function is called, and disappears when the function is exited. This 
is why such variables are usually known as *automatic* variables, 
following terminology in other languages.

Because automatic variables come and go with function invocation, they
do not retain their values from one call to the next, and must be 
explicitly set upon each entry. If they are not set, they will contain
garbage.

As an alternative to automatic variables, it is possible to define 
variables that are external to all functions, that is, variables that 
can be accessed by name by any function. 

Because external variables are globally accessible, they can be used 
instead of argument lists to communicate data between functions. 
Furthermore, because external variables remain in existence 
permanently, rather than appearing and disappearing as functions are 
called and exited, they retain their values even after the functions 
that set them have returned.

An external variable must be *defined*, exactly once, outside of any 
function; this sets aside storage for it. The variable must also be 
*declared* in each function that wants to access it;
The declaration may be an explicit `extern` statement or may be 
implicit from context.

From stackoverflow =) :

"extern" changes the linkage. With the keyword, 
the function / variable is assumed to be available somewhere else and 
the resolving is deferred to the linker.

There's a difference between "extern" on functions and on variables: 
on variables it doesn't instantiate the variable itself, i.e. doesn't 
allocate any memory. This needs to be done somewhere else. Thus it's 
important if you want to import the variable from somewhere else. For 
functions, this only tells the compiler that linkage is extern. 
As this is the default (you use the keyword "static" to indicate that 
a function is not bound using extern linkage) you don't need to use it
explicitly.

```
int max;                /* maximum length seen so far */
char line[MAXLINE];     /* current input line */
char longest[MAXLINE];  /* longest line saved here */

int getline(void);
void copy(void);

int main() {
 ...
}

/* copy: specialized version */
void copy(void)
{
	int i;
	extern char line[], longest[];
	
	i = 0;
	while ((longest[i] = line[i]) != '\0')
		++i;
}
```

Syntactically, external definitions are just like definitions of local
variables, but since they occur outside of functions, the variables 
are external. Before a function can use an external variable, the name
of the variable must be made known to the function; the declaration is
the same as before except for the added keyword `extern`.

In certain circumstances, the `extern` declaration can be omitted. If 
the definition of the external variable occurs in the source file 
before its use in a particular function, then there is no need for an 
`extern` declaration in the function.

In fact, common practice is to place definitions of all external 
variables at the beginning of the source file, and then omit all 
extern declarations.

If the program is in several source files, and a variable is defined 
in *file1* and used in *file2* and *file3*, then `extern` declarations
are needed in `file2` and `file3` to connect the occurrences of the 
variable.

The usual practice is to collect `extern` declarations of variables 
and functions in a separate file, historically called a `header`, that
is included by `#include` at the front of each source file.
The suffix `.h` is conventional for header names.
The functions of the standard library, for example, are declared in 
headers like `<stdio.h>`.

You should note that we are using the words *definition* and 
*declaration* carefully when we refer to external variables in this 
section. "Definition" refers to the place where the variable is 
created or assigned storage; "declaration" refers to places where the 
nature of the variable is stated but no storage is allocated.
