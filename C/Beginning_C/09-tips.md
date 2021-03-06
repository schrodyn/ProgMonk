#Pointers to Functions

Up to now, you’ve considered pointers as an exceptionally useful device for
manipulating data and variables that contain data.

However, you can also use pointers to handle
functions at a distance.

Because a function has an address in memory where it starts execution (i.e.,
its starting address), the basic information to be stored in a pointer to a
function is going to be that address.

If a function is going to be called through a pointer,
information also has to be available about the number and type of the arguments
to be supplied, and the type of return value to be expected.
The compiler can’t deduce these just from the address of the function.

Just as a pointer holds an address and
must also define a type, a function pointer holds an address and must also define
a prototype.

The declaration for a pointer to a function looks a little strange and can be
confusing, so let’s start with a simple example:

```c
int (*pfunction) (int);
```

This declares a pointer to a function. It doesn’t point to anything—yet;
this statement just defines the pointer variable.

The name of the pointer is pfunction, and it’s intended to point to functions
that have one parameter of type int and that return a value of type int to the
calling program.

you can only use this particular pointer to point to functions with these
characteristics.

If you want a pointer to functions that accept a float argument and return float
values, you need to declare another pointer with the required characteristics.

##Declaration:

```
int			(*pfunction)	(int);
 ^		     	     ^		  ^
 |		     	     |		  |
The pointer will	The name of 	The pointer will
be assigned only	the pointer to	be assigned only
to functions with	a function	to functions that
this return type			have these types
					of arguments
```

Note:
If you omit the parentheses, you’ll have a declaration for a function called
pfunction() that returns a
value that’s a pointer to int, which isn’t what you want.

With “pointer to function” declarations, you must always put the * plus the
pointer name between parentheses. The second pair of parentheses just encloses
the parameter list in the same way as it
does with a standard function declaration.

Calling a Function Through a Function Pointer:
Suppose that you define a function that has the following prototype:

```c
int sum(int a, int b);	/* Calculates a+b */
```

This function has two parameters of type int and returns a value of type int so
you could store its address in a function pointer that you declare like this:

```c
int (*pfun)(int, int) = sum;
```

To supply an initial value you just use the name of a function that has
the required prototype.
You can now call sum() through the function pointer like this:

```c
int result = pfun(45, 55);
```

This statement calls the sum() function through the pfun pointer with argument
values of 45 and 55.
Note that you just use the function pointer name just like a function name to call
 the function that it points to; no dereference operator is required.

Suppose you define another function that has the following prototype:

```c
int product(int a, int b);	/* Calculates a*b */
```

You can store the address of product() in pfun with the following statement:
```c
pfun = product;
```

With pfun containing the address of product(), you can call product through the
pointer

```c
result = pfun(5, 12);
```

After executing this statement, result will contain the value 60.
Notice the way you assign a value to the pointer:

```c
pfun = sum;	/* Points to function sum() */
```

You just use a regular assignment statement that has the name of the function
on the right side. You don’t need to put in the parameter list or anything.
If you did, it would be wrong, because it would then
be a function call, not an address, and the compiler would complain.

A function is very much like an array in its usage
here. If you want the address of an array, you just use the name by itself, and
if you want the address of a function you also use the name by itself.

Arrays of Pointers to Functions:
Of course, a function pointer is a variable like any other. You can therefore
create an array of pointers to functions.

To declare an array of function pointers, you just put the array dimension after
the function pointer array name, for instance:

```c
int (*pfunctions[10]) (int);
```

This declares an array, pfunctions, with ten elements.
Each element in this array can store the
address of a function with a return type of int and two parameters of type int.

Apart from the function name on the right of the equal sign, this could be a
normal data array:

```c
int (*pfun[3])(int, int) = { sum, product, difference };
```

In fact, you could have left out the array dimension, too, and gotten it by
default:

```c
int (*pfun[])(int, int) = { sum, product, difference };
```

When it comes to calling a function that an array element points to,
you write it as follows:

```c
result = pfun[i](a, b);	/* Call the function through a pointer */
```

You can also pass a pointer to a function as an argument.
This allows a different function to be
called, depending on which function is addressed by the pointer that’s passed as
the argument.

For example:
The function that will accept a “pointer to a function” as an argument is
any_function(). The prototype for this function is the following:

```c
int any_function(int(*pfun)(int, int), int x, int y); /**/
```

The function named any_function() has three parameters. The first parameter type
is a pointer to a function that
accepts two integer arguments and returns an integer.
The last two parameters are integers that will be used in the call of the function
 specified by the first parameter.

```c
int any_function(int(*pfun)(int, int), int x, int y)
{
  return pfun(x, y);
}
```

Notice how you initialize the function pointer pf:

```c
int (*pf)(int, int) = sum;	/* Pointer to function */
```

you can initialize function pointers to the addresses of specific functions just
by putting the function name as an initializing value.

```c
result = any_function(pf, a, b);
```

You can also explicitly enter the name of a function, product, as the first
argument:

```c
result = any_function(product,a, b);
```

In this case, you’re effectively persuading the
compiler to create an internal pointer to the function product and passing it to
any_function().

##Static Variables: Keeping Track Within a Function

So far, all the variables you’ve used have gone out of scope at the end of the
block in which they were defined, and their memory on the stack then becomes free
for use by another function.

These are called automatic variables because they’re automatically created at the
point where they’re declared, and they’re automatically destroyed when program
execution leaves the block in which they were declared.

However, there are some circumstances in which you might want to retain
information from one function call to the next within a program.
You may wish to maintain a count of something
within a function, such as the number of times the function has been called or
the number of lines of output that have been written.

C does provide you with a way to do this with static variables. You could declare
 a static variable count, for example, with this declaration:

```c
static int count = 0;
```

First of all, despite the fact that it may be defined
within the scope of a function,
this static variable doesn’t get destroyed when execution leaves the function.

Second, whereas an automatic variable is initialized each time its scope is
entered, the initialization of a variable declared as static occurs only once,
right at the beginning of the program.

Although a static variable is visible only within the function that contains its
declaration

**Note:** \
All static variables are initialized to 0 unless you initialize them with some
other value.

Because the variable has been declared as static, the compiler arranges things
so that the variable will be initialized only once. Because initialization occurs
before program startup, you can always
be sure a static variable has been initialized when you use it.

Although a static variable will persist for as long as the program is running,
it will be visible only within the
scope in which it is declared, and it can’t be referenced outside of that original
 scope.

You also have a way of sharing variables between all your functions.
These are called global variables because they’re accessible anywhere.
Global variables are declared in the normal way;

global variable will be initialized by default to 0 if you don’t initialize it.
It’s potentially accessible in any function from the point where
it’s declared to the end of the file, so it’s accessible in any of the functions
here.

Global variables can replace the need for function arguments and return values.
However, you should use global variables sparingly.
They can be a major asset in simplifying and shortening some programs,
but using them excessively will make your programs prone to errors.

Caution:
As a rule, it’s unwise to use the same names in C for local and global variables.

Recursion
It’s possible for a function to call itself. This is termed recursion.

```c
/* Our recursive factorial function */
unsigned long factorial(unsigned long n)
{
	if(n < 2L)
		return n;
	return n*factorial(n - 1);
}
```

For each call, a copy of
the argument is created, and the location to be returned to is stored. This can
get expensive as far as memory is concerned if there are many levels of recursion.

A loop to do the same thing would be cheaper and faster.

Functions with a Variable Number of Arguments:
It can’t have escaped your notice that some functions in the standard libraries
accept a variable number of arguments.
The functions printf() and scanf() are obvious examples.

You may come up with a need to do this yourself from time to time, so the standard
 library `<stdarg.h>` provides you  
with routines to write some of your own.

The immediately obvious problem with writing a function with a variable number of
parameters is how to specify its prototype.

The prototype would be written as follows:
```c
double average(double v1, double v2, ...);
```

The ellipsis(three dots) indicates that a variable number of arguments may follow
the first two fixed arguments.
You must have at least one fixed argument.

The second problem is how you reference the arguments when writing the function.
Because you don’t know how many there are, you can’t possibly give them names.

The `<stdarg.h>` library header provides you with routines that are usually
implemented as macros to
help with this, but they look and operate like functions.

You need to use three of these when implementing your own function with a variable
 number of arguments.

They are called `va_start()`, `va_arg()`, and `va_end()`. The first of these has the
following form:

```c
void va_start(va_list parg, last_fixed_arg);
```

The name, `va_start`, is obtained from variable argument start.
This function accepts two arguments:
a pointer parg of type va_list, and the name of the last fixed parameter that you
 specified for the function you’re writing.

The va_list type is a type that is also defined in <stdarg.h>

So using the function average() as an illustration, you can start to write the
function as follows:

```c
double average(double v1, double v2,...)
{
	va_list parg;	/* Pointer for variable argument list */
	/* More code to go here... */
	va_start( parg, v2);
	/* More code to go her. . . */*
}
```

The effect of the call to
`va_start()` is to set the variable parg to point to the first variable argument
that is passed to the function when it is called.

You still don’t know what type of value this represents, and the standard library
is no further help in this context.

You now need to know how to access the value of each of the variable arguments, so
 let’s see how this is done by completing the function average():

```c
/* Function to calculate the average of a variable number of arguments */
double average( double v1, double v2,...)
{
	va_list parg;		/* Pointer for variable argument list */
	double sum = v1+v2;	/* Accumulate sum of the arguments */
	double value = 0;	/* Argument value */
	int count = 2;		/* Count of number of arguments */

	va_start(parg,v2);	/* Initialize argument pointer */
	while((value = va_arg(parg, double)) != 0.0)
	{
		sum += value;
		count++;
	}
	va_end(parg);		/* End variable argument process */*
	return sum/count;
}
```

Look at the loop condition:

```c
while((value = va_arg(parg, double)) != 0.0)
```

The loop condition calls another function from `stdarg.h`, va_arg().
The first argument to va_arg() is the variable parg you initialized through the call to va_start().
The second argument is
a specification of the type of the argument you expect to find.

The function va_arg() returns the
value of the current argument specified by parg, and this is stored in value.
It also updates the pointer
parg to point to the next argument in the list, based on the type you specified in
 the call.

Another assumption you’re making is that all the arguments will be nonzero except
for the last. This is reflected in the condition for continuing the
loop, being that value isn’t equal to 0.

When an argument value obtained is 0, the loop ends and you execute the statement
va_end(parg);	/* End variable argument process */*

The call to va_end() is essential to tidy up loose ends left by the process.
It resets the parg point to NULL. If you omit this call, your program may not work
 properly.

Once the tidying up is complete,
you can return the required result with the statement
```c
return sum/count;
```

You might be wondering how printf() manages to handle a mix of types.
Well, remember the first argument
is a control string with format specifiers.
This supplies the information necessary to determine the types of the arguments
that follow, as well as how many there are.

The number of arguments following the first must match the number of format
specifiers in the control string.
The type of each argument after the first must match the type implied by the
corresponding format specifier.

You’ve seen how things don’t work out right if you specify the wrong
format for the type of variable you want to output.

Copying a va_list:
It is possible that you may need to process a variable argument list more than
once.
The <stdarg.h>
header file defines a routine for copying an existing va_list for this purpose.

Suppose you have created and initialized a va_list object, parg, within a function
 by using va_start().
You can now make a copy of parg like this:

```c
va_list parg_copy;
copy(parg_copy, parg);
```

You can then process parg and parg_copy independently to extract
argument values using va_arg() and va_end().

Note that the copy() routine copies the va_list object in whatever state it’s in
so if you have already executed va_arg() with parg to extract argument values from
 the list, parg_copy will be in an identical state to
 parg with some argument values already extracted.

Note also that you must not use the va_list object parg_copy as the destination
for another copy operation before you have executed pa_end() for parg_copy.

**In Total:** \
The second argument to va_arg() that specifies the type of the argument value to
be retrieved must be such that the pointer to the type can be specified by
appending * to the type name.
Check the documentation for your compiler for other restrictions that may apply.

You must call va_end() before you exit a function with a variable number of
arguments. If you fail to do so, the function won’t work properly.

The main() Function:
You already know that the main() function is where execution starts.
main() can have a parameter list so that you can pass arguments to main() when you
execute a program from the command line.

When you write main() with parameters, the first parameter is of type int and
represents a count of the number of arguments that appear in the command that is
used to execute main(), including the name of the program itself.

The second parameter to main() is an array of pointers to strings.
The argument that will be passed when you
write two arguments following the name of the program at the command line will be an array of three pointers.

```c
/* Program 9.8 A program to list the command line arguments */
#include <stdio.h>
int main(int argc, char *argv[])
{
  printf("Program name: %s\n", argv[0]);
  for(int i = 1 ; i<argc ; i++)
   printf("\nArgument %d: %s", i, argv[i]);
  return 0;
}
```

The value of argc must be at least 1 because you can’t execute a program without
entering the program name.

Note the use of double quotes to enclose an argument that includes spaces.
This is because spaces are normally treated as delimiters. You can always enclose
an argument between double quotes to ensure it will be treated as a single
argument.

All command-line arguments will be read as strings, so when numerical values are
entered at the command line, you’ll need to convert the string containing the
value to the appropriate numerical type.
You can use one of the functions that are declared in <stdlib.h> to do this.
Function |	Description
---------|--------------------
atof() |	Converts the string passed as an argument to type double
atoi() |	Converts the string passed as an argument to type int
atol() |	Converts the string passed as an argument to type long

##Ending a Program

There are several ways of ending a program.
Falling off the end of the body of main() is equivalent to
executing a return statement in main().

There are two standard library functions that you can call to end a program, both
of which are declared in the `<stdlib.h>` header.

Calling the abort() function terminates the program immediately and represents an
 abnormal end to program operations, so you shouldn’t use this for a normal end
to a program.

For example:
```c
abort();	/* Abnormal program termination */
```

This statement can appear anywhere in your program.

Calling the exit() function results in a normal end to a program.
The function requires an integer argument that will be returned to the operating
system.
A value of 0 usually represents a completely normal program end.

As you have seen, you can also end a program by executing a return statement with an integer value in main(), for example:
return 1;

The return statement has a special meaning in main() (and only in main(), not in
 any other function), and it’s the equivalent of calling exit() with the value
specified in the return statement as the argument.
Thus, the value in the return statement will be passed back to the operating
system.

Libraries of Functions: Header Files
your compiler comes with a wide range of standard functions that are
declared in header files, sometimes called include files.

Header File	Functions | Description
-------------------------------------
<stdio.h> |	Input/output functions
<stdarg.h> |	Macros supporting a variable number of arguments to a function
<math.h> | 	Mathematical floating-point functions
<stdlib.h> | 	Memory allocation functions
<string.h> | 	String-handling functions
<stdbool.h> | 	bool type and Boolean values true and false
<complex.h>  |	Complex number support
<ctype.h> | 	Character classification functions
<wctype.h>  |	Wide character conversion functions

I strongly recommend that you invest some time in becoming familiar with the
contents of these header files and the libraries that are supplied with
your compiler.

This familiarity will greatly
increase the ease with which you can develop applications in C.

##Enhancing Performance

You have two facilities that are intended to provide cues to your compiler
to generate code with better performance:
One relates to how short function calls are compiled and the other is concerned
with the use of pointers.

Declaring Functions inline:
With very short functions it is possible to
improve execution performance by replacing each function call of a short function
with inline code that implements the effects of the function.

You can indicate that you would like this technique to be
applied by the compiler by specifying a short function as inline.
For example:
```c
inline double bmi(double kg_wt, double m_height)
{
  return kg_wt/(m_height*m_height);
}
```

This is specified by the inline keyword in the function header.
There’s no guarantee in general that the compiler will take note of a function
being declared as inline though.

##Using the restrict Keyword:

Sophisticated C compilers have the capability to optimize the performance of the
object code
and this can involve changing the sequence in which calculations occur compared to
 the sequence in which you specify operations in the code.

For such code optimization to be possible, the compiler
must be sure that such resequencing of operations will not affect the result of
the calculations.

To allow optimization of code involving pointers, the compiler has to be certain
that the pointers are not aliased — in other words the data
item that each pointer references is not referenced by some other
means in a given scope.

The restrict keyword provides a way for you to tell the compiler when this is
the case and thus allows code optimization to be applied.

Here’s an example of a function that is declared in <string.h>:

```c
char *strcpy(char * restrict s1, char * restrict s2)
{
 /* Implementation of the function to copy s2 to s1 */
}
```

This function copies s2 to s1. The restrict keyword is applied to both parameters thus indicating that the strings referenced by s1 and s2 are only referenced
through those pointers in the body of the function,
so the compiler can optimize the code generated for the function.

Most of the time you won’t need to use the restrict keyword. Only if your code is very computationally intensive will it have any appreciable effect, and even then
it depends on your compiler.

A functional structure is inherent to the C language, and you should keep your
functions short with a well-defined purpose.
This is the essence of good C code.
