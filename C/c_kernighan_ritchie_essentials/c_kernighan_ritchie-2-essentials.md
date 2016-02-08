#Variable Names

Names are made up of letters and digits; the first character must be a 
letter. The underscore `_` counts as a letter; it is sometimes useful 
for improving the readability of long variable names. Don't begin 
variable names with underscore, however, since library routines often 
use such names. Upper and lower case letters are distinct, so `x` and 
`X` are two different names. Traditional C practice is to use lower 
case for variable names, and all upper case for symbolic constants.

It's wise to choose variable names that are related to the purpose of 
the variable, and that are unlikely to get mixed up typographically. 
We tend to use short names for local variables, especially loop 
indices, and longer names for external variables.

#Data Types and Sizes

There are only a few basic data types in C:

  * `char` a single byte, capable of holding one character in the local  character set
  * `int` an integer, typically reflecting the natural size of integers  on the host machine
  * `float` single-precision floating point
  * `double` double-precision floating point

In addition, there are a number of qualifiers that can be applied to 
these basic types. `short` and `long` apply to integers:

```
short int sh;
long int counter;
```

The word `int` can be omitted in such declarations, and typically 
it is.

The intent is that `short` and `long` should provide different lengths 
of integers where practical; `int` will normally be the natural size 
for a particular machine. `short` is often 16 bits long, and `int` 
either 16 or 32 bits. Each compiler is free to choose appropriate sizes
for its own hardware, subject only to the the restriction that `short`s
and `int`s are at least 16 bits, `long`s are at least 32 bits, and 
`short` is no longer than `int`, which is no longer than `long`.

The qualifier `signed` or `unsigned` may be applied to `char` or any 
integer. `unsigned` numbers are always positive or zero, and obey the 
laws of arithmetic modulo 2^n^, where n is the number of bits in the 
type. So, for instance, if `char`s are 8 bits, `unsigned char` 
variables have values between 0 and 255, while `signed `char`s have 
values between -128 and 127 (in a two's complement machine.) Whether 
plain `char`s are signed or unsigned is machine-dependent, but 
printable characters are always positive.

The type `long double` specifies extended-precision floating point. As 
with integers, the sizes of floating-point objects are 
implementation-defined; `float`, `double` and `long double` could 
represent one, two or three distinct sizes.

The standard headers `<limits.h>` and `<float.h>` contain symbolic 
constants for all of these sizes, along with other properties of the 
machine and compiler.

#Constants

An integer constant like 1234 is an `int`. A `long` constant is written
with a terminal `l` or `L`, as in `123456789L`. Unsigned constants are 
written with a terminal `u` or `U`, and the suffix `ul` or `UL` 
indicates `unsigned long`.

Floating-point constants contain a decimal point (`123.4`) or an 
exponent (`1e-2`) or both; their type is `double`, unless suffixed. 
The suffixes `f` or `F` indicate a `float` constant; `l` or `L` 
indicate a `long double`.

we might write octal constants

```
#define VTAB '\013'   /* ASCII vertical tab */
#define BELL '\007'   /* ASCII bell character */
```

or, in hexadecimal

```
#define VTAB '\xb'   /* ASCII vertical tab */
#define BELL '\x7'   /* ASCII bell character */
```

There is one other kind of constant, the *enumeration constant*. 
An enumeration is a list of constant integer values, as in

```
enum boolean { NO, YES };
```

The first name in an `enum` has value 0, the next 1, and so on, unless 
explicit values are specified. If not all values are specified, 
unspecified values continue the progression from the last specified 
value, as the second of these examples:

```
enum escapes { BELL = '\a', BACKSPACE = '\b', TAB = '\t',
               NEWLINE = '\n', VTAB = '\v', RETURN = '\r' };

enum months { JAN = 1, FEB, MAR, APR, MAY, JUN,
              JUL, AUG, SEP, OCT, NOV, DEC };
					/* FEB = 2, MAR = 3, etc. */
```

Names in different enumerations must be distinct. Values need not be 
distinct in the same enumeration.

#Declarations

All variables must be declared before use, although certain 
declarations can be made implicitly by content. A declaration 
specifies a type, and contains a list of one or more variables of 
that type

```
int  lower, upper, step;
char c, line[1000];
```

A variable may also be initialized in its declaration. If the name is 
followed by an equals sign and an expression, the expression serves as
an initializer

```
char  esc = '\\';
int   i = 0;
int   limit = MAXLINE+1;
float eps = 1.0e-5;
```

If the variable in question is not automatic, the initialization is 
done once only, conceptionally before the program starts executing, 
and the initializer must be a constant expression. 

An explicitly initialized automatic variable is initialized each time 
the function or block it is in is entered; the initializer may be any 
expression. 

External and static variables are initialized to zero by default. 
Automatic variables for which is no explicit initializer have 
undefined (i.e., garbage) values.

The qualifier `const` can be applied to the declaration of any 
variable to specify that its value will not be changed. For an array, 
the const qualifier says that the elements will not be altered.

#Type Conversions

When an operator has operands of different types, they are converted 
to a common type according to a small number of rules.

In general, the only automatic conversions are those that convert a 
"narrower" operand into a "wider" one without losing information, 
such as converting an integer into floating point in an expression 
like `f + i`.

Expressions that might lose information, like assigning a longer 
integer type to a shorter, or a floating-point type to an integer, 
may draw a warning, but they are not illegal.

A `char` is just a small integer, so chars may be freely used in 
arithmetic expressions. 

Relational expressions like `i > j` and logical expressions connected 
by `&&` and `||` are defined to have value 1 if true, and 0 if false. 
Thus the assignment

```
d = c >= '0' && c <= '9'
```

sets `d` to 1 if `c` is a digit, and 0 if not.
However, functions like `isdigit` may return any non-zero value for 
true. In the test part of `if`, `while`, `for`, etc., "true" just 
means "non-zero", so this makes no difference.

Implicit arithmetic conversions work much as expected. In general, if 
an operator like `+` or `*` that takes two operands 
(a binary operator) has operands of different types, the "lower" type 
is promoted to the "higher" type before the operation proceeds.

Conversions take place across assignments; the value of the right side
is converted to the type of the left, which is the type of the result.

Longer integers are converted to shorter ones or to `char`s by 
dropping the excess high-order bits.

If `x` is `float` and `i` is `int`, then `x = i` and `i = x` both 
cause conversions; `float` to `int` causes truncation of any 
fractional part. When a `double` is converted to `float`, whether the 
value is rounded or truncated is implementation dependent.

Since an argument of a function call is an expression, 
type conversion also takes place when arguments are passed to 
functions. In the absence of a function prototype, `char` and `short`
become `int`, and `float` becomes `double`. This is why we have 
declared function arguments to be `int` and `double` even when the 
function is called with `char` and `float`.

Finally, explicit type conversions can be forced in any expression, 
with a unary operator called a cast. In the construction

```
(type name) expression
```

the *expression* is converted to the named type by the conversion 
rules above.

Let's see

```
(double) n
```

Note that the cast produces the value of `n` in the proper type; 
`n` itself is not altered.

#Increment and Decrement Operators

C provides two unusual operators for incrementing and decrementing 
variables. The increment operator `++` adds 1 to its operand, while 
the decrement operator `--` subtracts 1.

The unusual aspect is that `++` and `--` may be used either as prefix 
operators (before the variable, as in `++n`), or postfix operators 
(after the variable: `n++`). In both cases, the effect is to 
increment `n`. But the expression `++n` increments `n` *before* its 
value is used, while `n++` increments `n` *after* its value has been 
used.

This means that in a context where the value is being used, not just 
the effect, `++n` and `n++` are different.

The increment and decrement operators can only be applied to 
variables; an expression like `(i+j)++` is illegal.

#Bitwise Operators

C provides six operators for bit manipulation; these may only be 
applied to integral operands, that is, `char`, `short`, `int`, and 
`long`, whether signed or unsigned.

Operator | Description
---------|-------------
`&`		| bitwise AND
`|`		| bitwise inclusive OR
`^`		| bitwise exclusive OR
`<<`	| left shift
`>>`	| right shift
`~`		| one's complement (unary)

Right shifting an `unsigned` quantity always fits the vacated bits 
with zero. Right shifting a signed quantity will fill with bit signs 
("arithmetic shift") on some machines and with 0-bits 
("logical shift") on others.

#Assignment Operators and Expressions

Most binary operators (operators like `+` that have a left and right 
operand) have a corresponding assignment operator `op=`, where `op` is
one of

```
 +   -   *   /   %   <<   >>   &   ^   |
```

If expr1 and expr2 are expressions, then

```
expr1 op= expr2
```

is equivalent to

```
expr1 = (expr1) op (expr2)
```

except that expr1 is computed only once. 

Notice the parentheses around expr2:

```
x *= y + 1
```

means

```
x = x * (y + 1)
```

rather than

```
x = x * y + 1
```

Declaring the argument x to be an `unsigned` ensures that when it is 
right-shifted, vacated bits will be filled with zeros, not sign bits, 
regardless of the machine the program is run on.

We have already seen that the assignment statement has a value and 
can occur in expressions; the most common example is

```
while ((c = getchar()) != EOF)
	...
```

The other assignment operators (`+=`, `-=`, etc.) can also occur in 
expressions, although this is less frequent.

#Precedence and Order of Evaluation

Note that the precedence of the bitwise operators `&`, `^`, and `|` 
falls below `==` and `!=`. This implies that bit-testing expressions 
like

```
if ((x & MASK) == 0) ...
```
must be fully parenthesized to give proper results.

C, like most languages, does not specify the order in which the 
operands of an operator are evaluated. (The exceptions are `&&`, `||`,
`?:`, and `,`.) For example, in a statement like

```
x = f() + g()
```

`f` may be evaluated before `g` or vice versa; thus if either `f` or 
`g` alters a variable on which the other depends, `x` can depend on 
the order of evaluation. Intermediate results can be stored in 
temporary variables to ensure a particular sequence.

Similarly, the order in which function arguments are evaluated is not 
specified, so the statement

```
printf("%d %d\n", ++n, power(2, n));   /* WRONG */
```

can produce different results with different compilers, depending on 
whether `n` is incremented before power is called. The solution, of 
course, is to write

```
++n;
printf("%d %d\n", n, power(2, n));
```




