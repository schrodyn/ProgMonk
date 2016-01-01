When confronted with a big task, the most sensible thing to do is break it up
into manageable chunks. You can then deal with each small chunk
fairly easily and be reasonably sure that you’ve done it properly.
If you design
the chunks of code carefully, you may be able to reuse some of them
in other programs.

One of the key ideas in the C language is that every program should be segmented
into functions that are relatively short.

##Program Structure

a C program consists of one or more functions, the most important
of which is the function main() where execution starts.

When a function is called, the code within the body of that function is executed,
and when the function has finished executing, control returns to
the point at which that function was called.

Execution of the program continues through the function
statements until it hits a return statement or reaches the closing brace marking
the end of the function body.

Variable Scope and Lifetime:

Variables exist only within the block in which they’re defined.
The variables declared at the beginning of an outer block also exist in the inner
block.

Variables that are created when they’re declared and destroyed at the end of a
block are called
automatic variables, because they’re automatically created and destroyed.

The extent within the
program code where a given variable is visible and can be referenced is called
the variable’s scope.

While your program is executing, a variable is created and memory is allocated
for it.

At some point, which for automatic variables is the end of the block in which the variable is declared, the memory that the variable occupies is returned back to
the system.

The time period during which a variable is in existence is referred to as the
lifetime of the variable.

If You have two variables with one name (let's assume called count):
inside the inner block the local variable will “hide” the version of count
that exists at the outer level. The compiler will
assume that when you use the name count, you mean the one that was declared in the current block.

it isn’t a good idea to use the same variable name for two different variables in
a function, even though it’s legal. At best, it’s most confusing.

The body of every function
is a block (which may contain other blocks, of course). As a result, the automatic
variables that you declare within a function are local to the function and don’t
exist elsewhere.

You’ve seen how you can receive information back from a function in two ways.
The first way is through one of the function arguments in parentheses. You provide an address of a variable through an argument to a function,
and the function places a value in that  variable. When you use scanf() to read
data from the keyboard, for instance, the input is stored in an address that you
supply as an argument.

The second way is that you can receive information back from a function as
a return value.

When you create a function, you need to specify the function header as the first
line of the function definition, followed by the executable code for the function
enclosed between braces.
The block of
code between braces following the function header is called the function body.

The general form of a function is essentially the same as you’ve been using for
main(), and it looks like this:

```
Return_type Function_name( Parameters - separated by commas )
{
   Statements;
}
```

A function that does not return a value must also have the return type specified
as void.
Conversely, for a function that does not have a void return type, every return
statement in the function body must return a value of the specified return type.

The parameters between parentheses are placeholders for the argument values that
you must specify when you call a function.
The term parameter refers to a placeholder in the function definition that
specifies the type of value that should be passed to the function when it is
called.

The term argument refers to the value that you supply corresponding to a parameter
when you call a function.

The general form for calling a function is the following expression:
Function_name(List of Arguments - separated by commas)

The name of a function can be any legal name in C that isn’t a reserved word
(such as int, double, sizeof, and so on) and isn’t the same as the name of another
function in your program.

A legal name has the same form as that of a variable: a sequence of letters and
digits, the first of which must be a letter.

As with variable names, the underline character counts as a letter.

You’ll often want to define function names (and variable names, too) that consist
of more than one word. There are two common approaches you can adopt:

  1. Separate each of the words in a function name with an underline character.
  2. Capitalize the first letter of each word.

Both approaches work well. Which one you choose is up to you, but it’s a good idea
 to pick an approach and stick to it.

If there are no parameters to a function, you can specify the parameter list as
void, as you have been doing in the case of the main() function.

The return type can also be type `void *`, which is a “pointer to void.” The value
returned in this case is an address value but with no specified type.
This type is used when you want the flexibility to
be able to return a pointer that may be used for a variety of purposes, as in the case of the malloc() function for allocating memory.

A return type can also be an enumeration type or a pointer to an enumeration type.
If a function has its return type specified as other than void, it must return a
value. This is achieved by executing a return statement to return the value.

The return statement provides the means of exiting from a function and resuming
execution of the calling function at the point from which the call occurred.

In its simplest form:
```c
return;
```

However, the more general form of the return statement is this:
```c
return expression;
```

The return expression can be any expression, but it should result in a value
that’s the same type as that declared for the return value in the function header.

If it isn’t of the same type, the compiler
will insert a conversion from the type of the return expression to the one
required where this is possible. The compiler will produce an error message if the
 conversion isn’t possible.

The Pass-By-Value Mechanism:
The important point is this: copies of the values of value1 and value2 are
transferred to the function as arguments, not the variables themselves. This means
that the function can’t change the values stored in value1 or value2.

For instance, if you input the values 4.0 and 6.0 for the two variables, the
compiler will create separate copies of these two values on the stack, and the
average() function will have access to these copies when it’s called.

This mechanism is how all argument values are passed
to functions in C, and it’s termed the pass-by-value mechanism.

The only way that a called function can change a variable belonging to the
calling function is by receiving an argument value that’s the address of the
variable.

When you pass an address as an argument to a function, it’s still only a copy of
the address that’s actually passed to the function, not the original. However, the
 copy is still the address of the original variable.

The average() function is defined by the following statements:
```c
float average(float x, float y)
{
   return (x + y)/2.0f;
}
```

Note that without the f on the constant 2.0, this number would be of type double
and the whole expression would be evaluated as type double. Because the compiler
would then arrange to cast this to type float for the return value, you would get
 a warning message during compilation because data could be lost when the value is
 converted from type double to type float before it is returned.

Let's consider:
```c
printf("\nThe average is: %f", average(4.0, 6.0));
```

In this case if the literal 4.0 and 6.0 don't already exist, the compiler would
typically create a memory location to store each of the constant arguments to
average(), and then supply copies of those values to the function. A copy of the
result returned from the function average() would then be passed to printf().

##Function Declarations

Let's see an example, you could define the function main() first and then
the function average():

```c
#include <stdio.h>
int main(void)
{
 /* Code in main() ... */*
}
float average(float x, float y)
{
 return (x + y)/2.0f;
}
```

As it stands this won’t compile. When the compiler comes across the call to the
average() function, it will have no idea what to do with it, because at that point
 the average() function has not been defined.
For this to compile you must add something before the definition of main() that
tells the compiler about the average() function.

A function declaration is a statement that defines the essential characteristics
of a function. It defines its name, its return value type, and the type of each of
 its parameters. You can actually write it exactly the same as the function header
 and just add a semicolon at the end if you want.

A function declaration is also called a function prototype, because it provides
all the external specifications for the function.

A function prototype enables the compiler to generate the appropriate instructions
 at each point where you use the function and to check that you use it correctly
in each case.

When you include a header file in a program, the header file adds the function
prototypes for library functions to the program. For example, the header file
<stdio.h> contains function prototypes for printf() and scanf(), among others.

To get this program to compile, you just need to add the function prototype for
average() before the definition of main():

```c
#include <stdio.h>
float average(float, float);	/* Function prototype */

int main(void)
{
/* Code in main() ... */*
}

float average(float x, float y)
{
return (x + y)/2.0f;
}
```

Now the compiler can compile the call to average() in main() because it knows all
 its characteristics, its name, its parameter types, and its return type.

You can qualify a function parameter using the const keyword, which indicates that the function will treat the argument that is passed for this parameter as a
constant. Because arguments are passed by value, this is only useful when the
parameter is a pointer.

Typically you apply the const keyword to a parameter that is a pointer to specify
that a function will not change the value pointed to.

Here’s an example of a function with a const parameter:

```c
bool SendMessage(const char* pmessage)
{
	/* Code to send the message */*
	return true;
}
```

The compiler will verify that the code in the body of the function does not use
the pmessage pointer to modify the message text.

You could specify the pointer itself as const too, but this makes
little sense because the address is passed by value so you cannot change the
original pointer in the calling function.

```c
const int *pvalue = &value1;  /* pointer to constant */
int const *cpvalue = &value1; /* Constant pointer */

pvalue = &value2;	/* OK: pointer is not constant */
*pvalue = 77;		/* Illegal: data is constant */

cpvalue = &value2;	/* Illegal: pointer is constant */
*cpvalue = 77;		/* OK: data is not constant */
```

sorting strings:

```c
void str_sort(const char *p[], int n)
{
	char *pTemp = NULL;	/* Temporary pointer
	bool sorted = false;	/* Strings sorted indicator
	while(!sorted)		/* Loop until there are no swaps */
	{
		sorted = true;		/* Initialize to indicate no swaps
		for(int i = 0 ; i<n-1 ; i++ )
		if(strcmp(p[i], p[i + 1]) > 0)
		{
			sorted = false;	/* indicate we are out of order
			swap(&p[i], &p[i+1]);	/* Swap the pointers
		}
	}
}
```

You aren’t obliged to put the parameter names in a function prototype, but it’s
usually better if you do. I omitted them in this example to demonstrate that you
can leave them out but I recommend that you include them in your programs.

```c
bool str_in(const char **);
void str_sort(const char *[], int);
void swap( void **p1, void **p2);
void str_out(char *[], int);
```
Of course, type `const char**` is the same as type `const char*[]`, which is an array
of elements of type
`const char*`. You could use either type specification here.

`exit(1)` -- function that terminates program with code 1.

Note that there is no return statement in the definition of the str_sort()
function. Coming to the end of the function body during execution is equivalent of
executing a return statement without a return expression.

The swap() function called by sort() is a short utility function that swaps two
pointers:

```c
void swap( void **p1, void **p2)
{
 void *pt = *p1;
 *p1 = *p2;
 *p2 = pt;
}
```

As you know, pointers are passed by value, just like any other type of argument so
 to be able to change a pointer, you must pass a pointer to a pointer as the
function argument.

The parameters here are of type `void**`, which is “pointer to void*”. Any pointer
of the form `type*` will convert to type `void*` so this function can swap two
pointers of any given type.

You can also return a pointer from a function.

**Cast-Iron Rule**
Never return the address of a local variable in a function.

If you want the new pay value to be stored in another location, the IncomePlus()
function could conceivably allocate space for it using malloc() and then return
the address of this memory. However, you should be cautious about doing this
because responsibility for freeing the memory would then be left to the calling
function.

It would be
better to pass two arguments to the function, one being the address of the initial
 pay and the other being the address of the location in which the new pay is to be
 stored. That way, the calling function has control of the memory.

Separating the allocation of memory at runtime from the freeing of the memory is
sometimes a recipe for something called a memory leak.

As far as possible, you should make the function that allocates memory
responsible for releasing it.

Note, however, when you need to allocate memory that has to be released somewhere
else in the program it’s best to code the function that will release the memory in
 tandem with coding the function that allocates it. That way,
there’s less risk of a memory leak.

So tips:
  1. Before you use a function in your source file, you’d either define the function
  or declare the function with a function prototype.

  2. Specifying a pointer parameter as const indicates to the compiler that the
  function does not modify the data pointed to by the function.

  3. Arguments to a function should be of a type that’s compatible with the
  corresponding parameters specified in its header. If you pass a value of type
  double to a function that expects an integer argument, the value will be truncated,
  removing the fractional part.

  4. Copies of the argument values are transferred to a function, not the original
  values in the calling function. This is referred to as the pass-by-value mechanism
  for transferring data to a function.

  5. If you want a function to modify a variable that’s declared in its calling
  function, the address of the variable needs to be transferred as an argument.


```c
void strrev(char *p) -- reverse string
{
  char *q = p;
  while(q && *q) ++q; -- find eos
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}
```
