int salary; -- variable declaration
printf("My salary is %d.", salary);

Argument 1 is a control string, so called because it controls how the output specified by the following argument (or arguments) is to be presented.

This is the character string between the double quotes. It is also
referred to as a format string because it specifies the format of the data that is
output.

Argument 2 is the name of the variable salary. How the value of this variable will
 be displayed is determined
by the first argument—the control string.

%d -- conversion specifier

Conversion specifiers always start with a % character.

to output a % character you must use the sequence %%

printf("%d brides for %d brothers", brides, brothers);

A variable name -- a sequence of one or more uppercase or lowercase letters,
digits, and underscore characters (_) that begins with a letter.

the underscore character counts as a letter.

so Hash! and Mary-Lou aren’t allowed as names.

variable names are case sensitive, which means that the names Democrat and 
democrat are distinct.

You must also declare a variable before you use it.

In assignment operations, the expression on the right side of the = sign is 
evaluated first, and the result is then stored in the variable 
on the left.

Basic Arithmetic Operators:

+ Addition
- Subtraction
* Multiplication
/ Division
% Modulus

printf("\nI have eaten %d cookies. There are %d cookies left",
							eaten, cookies);

Note that you cannot split the string that is the first argument in this way.

But you could write the previous statement as follows:
printf("\nI have eaten %d cookies. "
		   " There are %d cookies left",
							eaten, cookies);

Where there are two or more strings immediately following one another like this, 
the compiler will join them
together to form a single string.

With division, if the operands have different signs, the result will be negative.
For example:
expression –45 / 7 => -6
expression 45 / –7 => –6.

If the operands
in a division are of the same sign, positive or negative, the result is positive.

With the modulus operator, the sign of the result is always the same as the sign of the left operand.

Thus, 45 % –7 results in the value 3, whereas –45 % 7 results in the value –3.

Integer Variable Types:

Type Name | Number of Bytes | Range of Values
---------------------------------------------
signed char 	1 		128 to +127
short int 	2 		32,768 to +32,767
int 		4 		2,147,438,648 to +2,147,438,647
long int 	4 		2,147,438,648 to +2,147,438,647
long long int	8		 9,223,372,036,854,775,808 to +9,223,372,036,854,775,807

Unsigned Integer Types:
-----------------------

Type Name | Number of Bytes | Range of Values
---------------------------------------------
unsigned char 	1 		0 to 255
unsigned short 
int or unsigned 
short 		2 		0 to 65,535
unsigned int 	4 		0 to 4,294,967,295
unsigned long 
int or unsigned
 long 		4 		0 to 4,294,967,295
unsigned long
 long int or
unsigned long 
long 		8 		0 to +18,446,744,073,709,551,615


unsigned int count = 10;
unsigned long inchesPerMile = 63360UL;
int balance = -500;

Notice the L at the end of the value for the variable of type long. This identifies the constant as type long rather than type int; 
constants of type int have no suffix.

int type may usually occupy 4 bytes, but it could be, for example, 2 bytes with
another compiler

This variation may seem a little strange, but the int type is
intended to correspond to the size of integer that the computer has been designed
to deal with most efficiently.

with more recent C compilers on a PC, variables of type int occupy 4 bytes. This is because 
all modern processors move data around at least 4 bytes at a time.

Because you can have different kinds of integer variables, you might expect to have different kinds
of integer constants, and you do.

long Big_Number = 1287600L;

int decrease = -4;
long below_sea_level = -100000L;

long long really_big_number = 123456789LL;

if your compiler implementation uses 2 bytes to store type int values, 
the values 1000000 and 33000 will be of type long by default, because they won’t 
fit into 2 bytes.

unsigned int count = 100U;
unsigned long value = 999999999UL;

You can also write integer values in hexadecimal form—that is, to base 16.

hexadecimal numbers are written with the prefix 0x or 0X

Note: An integer constant that starts with a zero, such as 014 for example, will be interpreted by your compiler
as an octal number—a number to base 8. 
Thus 014 is the octal equivalent of the decimal value 12. If it is meant to
be the decimal value 14 it will be wrong, so don’t put a leading zero in your 
integers unless you really mean to specify an octal value.

Floating-Point Values

Floating-point numbers are often expressed as a decimal value multiplied by some 
power of 10.

Value 	With an Exponent 	Can Also Be Written in C As
-----------------------------------------------------------
1.6 	 0.16×10^1 		0.16E1
0.00008  0.8×10^-4 		0.8E-4
7655.899 0.7655899×10^4 	0.7655899E4

Floating-Point Variables

Keyword | Number of Bytes | Range of Values
-------------------------------------------
float 		4 		±3.4E38 (6 decimal digits precision)
double 		8 		±1.7E308 (15 decimal digits precision)
long double 	12 		±1.19E4932 (18 decimal digits precision)

Like the integer types, the memory occupied and the range of values are dependent
on the machine and the compiler.

The type long double is sometimes exactly the same as type double
with some compilers.

Note that the number of decimal digits of precision is only an approximation
because floating-point values will be stored internally in binary form, and a 
decimal floating-point value does not always have an exact representation 
in binary.

Any number that you write containing a decimal
point is of type double unless you append the F to make it type float.

For instance, 1E3f is of type float and 3E8 is of type double.

To specify a long double constant:
long double huge = 1234567.89123L;

format specifier for values of type float in the printf() statement:
printf("A plank %f feet long can be cut into %f pieces %f feet long.",
				plank_length, piece_count, piece_length);


if you output a value of type float with the specifier %d
that’s intended for use with integer values, you’ll get rubbish. This is because the float value will be interpreted as an integer, which it isn’t.
Similarly, if you use %f with a value of an integer type, you’ll also get rubbish as output.

You can specify the number of places that you want to see
after the decimal point in the format specifier.

To obtain the output to two decimal places, you
would write the format specifier as %.2f. 
To get three decimal places, you would write %.3f.

printf("A plank %.2f feet long can be cut into %.0f pieces %.2f feet long.",
					plank_length, piece_count, piece_length);


A more general form of the format specifier for floating-point values 
can be written like this:

%[width][.precision][modifier]f

width -- integer specifying the total number of characters in the output
precision -- integer specifying the number of decimal places that are to appear
after the decimal point.

When you specify the field width, the value will be right-aligned by default. 
If you want the value to be left-aligned in the field, just put a minus sign 
following the %. For instance, the specification %-10.4f will output a floating-point value left-aligned in a field width of 10 characters with 4 digits following
the decimal point.

Note that you can specify a field width and the alignment in the field with a specification for outputting an integer value. For example, %-15d specifies an integer value will be presented left-aligned in a field width of 15 characters.

scanf("%f", &diameter); /* Read the diameter from the keyboard */

The scanf() function is another function that requires the <stdio.h> header file to be included. This function handles input from the keyboard. 
In effect it takes what you enter through the keyboard and interprets it as spec-
ified by the first argument, which is a control string between double quotes.
In this case the control string is “%f” because you’re reading a value of type 
float.

Format Specifiers for Reading Data:
-----------------------------------

Action 				Required Control String
To read a value of type short	%hd
To read a value of type int	%d
To read a value of type long	%ld
To read a value of type float	%f or %e
To read a value of type double	%lf or %le


#define PI 3.14159f	/* Definition of the symbol PI */

This is a preprocessing directive.
You’ve now defined PI as a symbol that is to be replaced in the code by 3.14159f.

it’s a common convention in C to write identifiers that appear in a #define 
statement in capital letters.

The second possibility is to define Pi as a variable, but to tell the compiler 
that its value is fixed and must not be changed:
const float Pi = 3.14159f;	/* Defines the value of Pi as fixed */

The value that you pass to a function can be the result of evaluating an 
expression rather than the value of a particular variable. 

The compiler will create a temporary variable to hold the value and that will
be passed to the function.

The header file <limits.h> defines symbols
representing values for the limits for each type.

Symbols Representing Range Limits for Integer Types:

Type 		Lower Limit 	Upper Limit
-------------------------------------------
char		CHAR_MIN	CHAR_MAX
short		SHRT_MIN	SHRT_MAX
int		INT_MIN		INT_MAX
long		LONG_MIN	LONG_MAX
long long	LLONG_MIN	LLONG_MAX

The lower limits for the unsigned integer types are all 0 so there are no symbols 
for these.

The symbols corresponding to the upper limits for the unsigned integer types are 
UCHAR_MAX, USHRT_MAX, UINT_MAX, ULONG_MAX, and ULLONG_MAX.

The <float.h> header file defines symbols that characterize floating-point values.

Symbols Representing Range Limits for Floating-Point Types:

Type 		Lower Limit 	Upper Limit
-------------------------------------------
float		FLT_MIN		FLT_MAX
double		DBL_MIN		DBL_MAX
long double	LDBL_MIN	LDBL_MAX

You can use the %e specifier for the floating-point limits, which presents the values in exponential form.

The L modifier is necessary when
the value being displayed by the printf() function is type long double.
Remember, this has to be a capital letter L; a small letter l won’t do here.

printf("\nThe size of the largest value of type long double is %.3Le\n",
LDBL_MAX);

sizeof operator -- find how many bytes are occupied by a given type
sizeof(int)

result is an integer of type size_t.

size_t is defined in the standard header file <stddef.h>

casting -- a process of converting a value of one type to another type.

float RevQuarter = (float)QuarterSold/150.0f*Revenue_Per_150;

Binary arithmetic operations (add, subtract, multiply, divide, and remainder) can only be executed
by your computer when both operands are of the same type.

When you use operands in a binary operation that are of different types, 
the compiler arranges for the value that is of a type with a more limited range 
to be converted to the type of the other. 

This is referred to as an implicit conversion.

example:
QuarterSold / 150 * Revenue_Per_150

it evaluated as 64400 (int) / 150 (int), which equals 429 (int). Then 429 (int converted to float) is multiplied by 4.5 (float), giving 1930.5 (float).

As a signed type, a variable of type char can store values from –128 to +127.

both ranges correspond to the same set of bit patterns: 
from 0000 0000 to 1111 1111.

With unsigned values, so 0000 0000 corresponds to 0, and 
1111 1111 corresponds to 255.

With signed values, the leftmost bit is a sign bit, so –128 is the binary value 
1000 0000, 0 is 0000 0000, and 127 is 0111 1111. The value 1111 1111 as a signed 
binary value is the decimal value –1.

char character = 74; /* ASCII code for the letter J */

char letter = 'C'; /* letter contains the decimal code value 67 */
letter = letter + 3; /* letter now contains 70, which is 'F' */

You can read a single character from the keyboard:

char ch = 0;
scanf("%c", &ch); /* Read one character */

To write a single character to the command line:
printf("The character is %c", ch);

you can output the numeric value of a character, too:
printf("The character is %c and the code value is %d", ch, ch);

to convert from uppercase
to lowercase, you simply add the result of ‘a’-’A’ (which is 32 for ASCII) to the
uppercase character.

Wide Character Type:

A variable of type wchar_t stores a multibyte character code and typically 
occupies 2 bytes.

use type wchar_t when you are working with Unicode characters, for example.

Type wchar_t is defined in the <stddef.h> standard header file

You can define a wide character constant by preceding what would otherwise be a 
character constant of type char with the modifier L:

wchar_t w_ch = L'A';

To read a character from the keyboard into a variable of type wchar_t, use the %lc
format specification:
wchar_t wch = 0;
scanf("%lc", &wch);
printf("You entered %lc", wch);

Enumeration:

enum Weekday {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};

The name of the new type, Weekday in this instance, is referred to as the tag of 
the enumeration.
Variables of type Weekday are called enumerators or enumeration constants.

An enumeration is an integer type because the enumerators that you specify 
will correspond to integer values that by default will start from zero 
with each successive enumerator having a value of one more than the 
previous enumerator.

Thus, in this example, 
the values Monday through Sunday will map to values 0 through 6.

enum Weekday today = Wednesday; /* declare a new variable of type Weekday */

Here’s how you could define the Weekday type so that the enumerator values 
start from 1:
enum Weekday {Monday=1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};

Now the enumerators Monday through Sunday will correspond to values 1 through 7.

enum Suit{clubs = 10, diamonds, hearts, spades);
enum Suit card_suit = diamonds;

Unnamed Enumeration Types:
enum {red, orange, yellow, green, blue, indigo, violet} shirt_color;

You can assign a value to shirt_color in the normal way:
shirt_color = blue;

Because you don’t have a type name,
there’s no way to define additional variables of this type later in the code.

Boolean Values:
The value of a variable of type _Bool can be either 0 or 1
_Bool is regarded as an integer type.

_Bool valid = 1; /* Boolean variable initialized to true */

you can use bool as the type name; you just need to add an #include directive
for the standard header file <stdbool.h> to any source file that uses it

the <stdbool.h> header file also defines the symbols true and false to corre-
spond to 1 and 0 respectively.

You can cast between Boolean values and other numeric types.
A nonzero numeric value will result in 1 (true) when cast to type _Bool, 
and 0 will cast to 0 (false).

You have three types that store complex numbers:
float _Complex with real and imaginary parts of type float
double _Complex with real and imaginary parts of type double
long double _Complex with real and imaginary parts of type long double

double _Complex z1; /* Real and imaginary parts are type double */

the <complex.h> header defines complex as being equivalent to _Complex, so you 
could declare the variable z1 like this:
double complex z1; /* Real and imaginary parts are type double */

sqrt(-1) is represented by the keyword _Complex_I

The <complex.h> header defines I to be the equivalent of _Complex_I

double complex z1 = 2.0 + 3.0*I; /* Real and imaginary parts are type double */

creal() -- returns real part of a value of type double complex
cimag() -- returns imaginary part

double real_part = creal(z1); /* Get the real part of z1 */
double imag_part = cimag(z1); /* Get the imaginary part of z1 */

crealf() and cimagf() -- working with float complex
creall() and cimagl() -- working with long double complex

conj() -- returns the complex conjugate of its double complex argument
also, conjf(), conjl()

_Imaginary keyword to define variables that store purely imaginary numbers

The <complex.h> header defines imaginary as a more readable equivalent of 
_Imaginary:
double imaginary ix = 2.4*I;

you can use arithmetic expressions involving complex and imaginary values using 
the arithmetic operators +,-, *, and /.

double complex cx = 1.0 + 3.0*I;
double complex cy = 1.0 - 4.0*I;

double complex product = cx*cy;
printf("\n\nThe product cx * cy = %.2f%+.2fi",
                           creal(product), cimag(product));

The output specification used for the imaginary part of each complex value 
is %+.2f. The + following the % specifies that the sign should always be output. 

If the + was omitted you would only get the sign in the output when
the value is negative.

The op= Form of Assignment:
shortcuts:
number = number + 10; => number += 10;
The op in op= can be any of the arithmetic operators:
+ - * / %
also few other operators:
<< >> & ^ |

lhs op= rhs; equal to lhs = lhs op (rhs);

so the statement a /= b+1; is the same as a = a/(b+1);

The math.h header file includes declarations for a wide range of mathematical 
functions.
All the functions return a value of type double.

Function    |	Operation
-------------------------
floor(x)	Returns the largest integer that isn't greater than x as type 
		double

ceil(x)		Returns the smalles integer that isn't less than x as type double

fabs(x)		Returns the absolute value of x

log(x)		Returns the natural algorithm (base e) of x

log10(x)	Returns the logarithm to base 10 of x

exp(x)		Returns the value of e^x

sqrt(x)		Returns the square root of x

pow(x, y)	Returns the value x^y

Functions for Trigonometry:
Function    |	Operation
-------------------------
sin(x)		Sine of x expressed in radians
cos(x)		Cosine of x
tan(x)		Tangent of x

example:
double angle = 45.0;	/* Angle in degrees */
double pi = 3.14159;
double sine = 0.0;
double cosine = 0.0;
sine = sin(pi*angle/180.0);	/*Angle converted to radians */
cosine = sin(pi*angle/180.0);	/*Angle converted to radians */

You also have the inverse trigonometric functions available: 
asin(), acos(), and atan(), 
as well as the hyperbolic functions sinh(), cosh(), and tanh().




