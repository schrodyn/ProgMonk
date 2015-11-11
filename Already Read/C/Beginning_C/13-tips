Preprocessing

preprocessing of your source code occurs before it’s compiled to
machine instructions.

The preprocessing phase can execute a range of service operations specified
by preprocessing directives, which are identified by 
the # symbol as the first character of each preprocessor directive.

Once the preprocessing phase is complete, all such preprocessing directives will 
no longer appear in the source code that results.

The compiler begins the compile phase proper, which generates
the machine code equivalent of your program.

Including Header Files
A header file is any external file whose contents are included into your program 
by use of the #include preprocessor directive.

#include <standard_library_file_name>

If you include a header file that you don’t use, the only effect, apart from 
slightly confusing anyone reading the program, is to extend the compilation time.

You can include your own source files into your program with a slightly different #include statement.

#include "myfile.h"

The contents of any file can be included into your program by this means.
You can also give the file whatever name you like within the constraints of the 
operating system.

In theory you don’t have to use the extension .h, although it’s a convention 
commonly adhered to by most programmers in C

------------------------------
External Variables and Functions

With a program that’s made up of several source files, you’ll frequently find 
that you want to use a global variable that’s defined in another file.

You can do this by declaring the variable as external to the current file using 
the "extern" keyword.

For example, if you have variables defined in another file
as global (which means outside of any of the functions) using the statements

int number = 0;
double in_to_mm = 2.54;

then in a function in which you want to access these, you can specify that these 
variable names are external by using these statements:

extern int number;
extern double in_to_mm;

These statements don’t create these variables—they just identify to the compiler 
that these names are defined elsewhere, and this assumption about these names 
should apply to the rest of this source file.

Note
----
Only one declaration of each global variable is allowed in a file. Of course, 
the global variables may be declared as external in as many files as necessary.

--------------------------
Substitutions in Your Program Source Code

There are preprocessor directives for replacing symbols in your source code 
before it is compiled.

#define PI 3.14159265

The general form of this sort of preprocessor directive is the following:

#define identifier sequence_of_characters

#define MAXLEN 256
char *buffer[MAXLEN];
char *str[MAXLEN];

Of course, you can also define a value such as MAXLEN as a const variable:

const size_t MAXLEN = 256;

The difference between this approach and using the #define directive is that 
MAXLEN here is no longer a token but is a variable of a specific type with the 
name MAXLEN.

The MAXLEN in the #define
directive does not exist once the source file has been preprocessed because all 
occurrences of MAXLEN in the code will be replaced by 256.

You could, for example, write the following:

#define Black White

The sequence 
of characters that is to replace the token identifier can be anything at all.

----------------------
Macro Substitutions

This not only involves substitution of a fixed sequence of characters for a token identifier,
but also allows parameters to be specified, which may themselves be replaced by 
argument values

#define Print(My_var) printf("%d", My_var)

You could, for example, write the following:
Print(ival);

This will be converted during preprocessing to this statement:
printf("%d", ival);

--------------------
Macros That Look Like Functions

#define macro_name( list_of_identifiers ) substitution_string

To illustrate how you use this, you can define a macro for producing a maximum 
of two values with the following directive:

#define max(x, y) x>y ? x : y

You can then put the statement:
result = max(myval, 99);

This will be expanded during preprocessing to produce the following code:
result = myval>99 ? myval : 99;

It’s important to be conscious of the substitution that is taking place and not 
to assume that this is a function.
For example, the following modest extension of the last
example can produce an erroneous result:

result = max(myval++, 99);

The substitution process will generate this statement:

result = myval++>99 ? myval++ : 99;

You need to be very cautious if you’re writing macros that generate expressions 
of any kind. A simple example will illustrate this.

#define product(m, n) m*n

result = product(x, y + 1); ====> result = x*y + 1;

The solution is very simple. If you use macros to generate
expressions, put parentheses around everything.

#define product(m, n) ((m)*(n))


A preprocessor directive must be a single logical line, but this doesn’t prevent 
you from using the statement continuation character, \.

You could write the following:

#define min(x, y) \
	       ((x)<(y) ? (x) : (y))

----------------------------
Strings As Macro Arguments

#define MYSTR "This string"

Suppose you now write the statement
printf("%s", MYSTR);    ======>    printf("%s", "This string");

Let's assume

#define MYSTR This string

you could specify a macro to display a string using the function
printf() as follows:

#define PrintStr(arg) printf("%s", #arg)

The # character indicates that the argument is to be surrounded by double quotes 
when the substitution is generated.

If you write
PrintStr(Output); ========> printf("%s", "Output");

A common use of this mechanism is for converting a variable name to a string, 
such as in this directive:

#define show(var) printf("\n%s = %d", #var, var);

If you now write
show(number);	==========> printf("\n%s = %d", "number", number);

PrintStr("Output"); ==========> printf("%s", "\"Output\"");

This is possible because the preprocessing phase is clever enough.

------------------------------
Joining Two Results of a Macro Expansion

#define join(a, b) a##b

strlen(join(var, 123)); ============> strlen(var123);

This might be applied to synthesizing a variable name for some reason, or 
generating a format control string from two or more macro parameters.

-----------------------
Logical Preprocessor Directives

Conditional Compilation
#if defined identifier

If the specified identifier is defined, statements that follow the #if are 
included in the program code until the directive #endif is reached.

If the identifier isn’t defined, the statements between the
#if and the #endif will be skipped.

You can also test for the absence of an identifier.
The general form of this directive is:

#if !defined identifier

Here, the statements following the #if down to the #endif will be included if the
 identifier hasn’t previously been defined.

This provides you with a method of avoiding duplicating functions,
or other blocks of code and directives, in a program consisting of several files

The mechanism is simply as follows:

#if !defined block1
  #define block1
  /* Block of code you do not */
  /* want to be repeated. */
#endif


You aren’t limited to testing just one value with the #if preprocessor directive. You can use logical operators to test if multiple identifiers have been defined.
For example

#if defined block1 && defined block2

You can also undefine identifier. This is achieved using a directive such as

#undef block1

Now, if block1 was previously defined, it is no longer defined after this 
directive.

There are alternative ways of writing these directives that are slightly more 
concise. You can use whichever of the following forms you prefer.

The directive #ifdef block is the same as #if defined block. 
And the directive #ifndef block is the same as #if !defined block.

----------------------------
Directives Testing for Specific Values
 
#if constant_expression

#if CPU == Pentium4
  printf("\nPerformance should be good." );
#endif

The printf() statement will be included in the program here only if the identifier
 CPU has been defined as Pentium4 in a previous #define directive.

Multiple-Choice Selections

#if CPU == Pentium4
  printf("\nPerformance should be good." );
#else
  printf("\nPerformance may not be so good." );
#endif

The preprocessing phase also supports #elif directive
An example of using this would be as follows:

#define US 0
#define UK 1
#define Australia 2
#define Country US
#if Country == US
  #define Greeting "Howdy, stranger."
#elif Country == UK
  #define Greeting "Wotcher, mate."
#elif Country == Australia
  #define Greeting "G'day, sport."
#endif
printf("\n%s", Greeting );

-------------------
Standard Preprocessing Macros

The __DATE__ macro provides a string representation of the date in the form 
Mmm dd yyyy when it’s invoked in your program.
Here Mmm is the month in characters, such as Jan, Feb, and so on.

A similar macro, __TIME__, provides a string containing the value of the time 
when it’s invoked, in the form hh:mm:ss

You could use this macro to record when your program was last compiled with a 
statement such as this:

printf("\nProgram last compiled at %s on %s", __TIME__, __DATE__ );

Note that both __DATE__ and __TIME__ have two underscore characters at the 
beginning and the end.

--------------------------
The Preprocessor in Debugging

#define iterations 6
#define test		/* Select testing output 	*/
#define testf		/* Select function call trace	*/
#define repeatable	/* Select repeatable execution	*/


...

/* Conditional code for repeatable execution */
#ifdef repeatable
  srand(1);
#else
  srand((unsigned int)time(NULL)); /* Seed random number generation */
#endif

...

#ifdef test
  printf("\nRandom index = %d", funsel);
#endif

...

#ifdef testf
  printf("\nFunction sum called args %d and %d.", x, y);
#endif

-------------------------
assert() Macro

The assert() macro is defined in the standard library header file <assert.h>.

This macro enables you to insert tests of arbitrary expressions in your program 
that will cause the program to be terminated with a diagnostic message if a 
specified expression is false (that is, evaluates to 0)

for example, assert(a == b);

The expression will be true (nonzero) if a is equal to b. If a and b are unequal,
the argument to the macro will be false and the program will be terminated with 
a message relating to the assertion.

Termination is achieved by calling abort(), so it’s an abnormal end to the 
program. When abort() is called, the program terminates immediately.


The assertion facility allowing assertions to occur can be switched off by 
defining the symbol NDEBUG before the #include directive for assert.h, like this:

#define NDEBUG			/* Switch off assertions */
#include <assert.h>

With some systems, assertions are disabled by default, in which case you can 
enable them by undefining NDEBUG:

#undef NDEBUG			/* Switch on assertions */
#include <assert.h>


