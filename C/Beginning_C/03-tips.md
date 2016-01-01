```c
if(count)
	printf("The value of count is not zero.");
```

```
if(expression)
   Statement1;
else
   Statement2;

Next_statement;
```

example:

```c
if(quantity>10)
   printf("The price for %d is $%.2f\n", quantity, quantity*unit_price*0.95);
else
   printf("The price for %d is $%.2f\n", quantity, quantity*unit_price);
```

better:

```c
double discount = 0.0;
if(quantity>10)
  discount = 0.05;

printf("\nThe price for %d is $%.2f\n", quantity,
			quantity*unit_price*(1.0-discount));
```

```
if(expression)
{
StatementA1;
StatementA2;
...
}
else
{
StatementB1;
StatementB2;
...
}
Next_statement;
```

```c
long test = 0L;
....

if(test % 2L == 0L)
```

If you were to use 0 instead of `0L` here, your compiler may insert code to convert
 0, which is of type int, to a value of type long to allow the comparison
for equality to be made. Using the constant 0L of type long avoids this
unnecessary operation.

Here are a few examples of comparing
values of type char:

```c
'Z' >= 'A'	'Q' <= 'P'	'B' <= 'b'	'B' != 66
```

With the ASCII values of the characters in mind, the first expression is true,
because 'Z', which has the code value 90, comes after 'A',
which has the code value 65.

The third is true. This is because in ASCII code lowercase letters are 32
higher than their uppercase equivalents.

```c
letter = letter - 'A'+ 'a';	/* Convert from upper- to lowercase */
```

Note that although this works fine for ASCII, there are coding systems (such as
EBCDIC) in which this won’t work, because the letters don’t have a contiguous
sequence of codes.

If you want to be sure that the conversion works for any code, you can use the
standard library function `tolower()`.

To use this function, you need to include the header file `ctype.h` in your program.

This header file also declares the complementary function, `toupper()`, that will convert
lowercase letters to uppercase.

the `tolower()` and `toupper()` functions won’t work with wide characters. However,
the `<wctype.h>` header file defines the `towlower()` and `towupper()` functions that
will.

**AND Operator &&**

```
if(age > 12 && age < 20)
  printf("You are officially a teenager.");
```

**OR Operator ||**

```c
if((age > 12 && age < 20) || savings > 5000)
  printf ("Either you're a teenager, or you're rich, or possibly both.");

if(a < 10 || b > c || c > 50)
  printf("At least one of the conditions is true.");
```

**NOT Operator !**

Better avoid using this operator as far as possible; it tends to result in code
that becomes difficult to follow.

Conditional Operator -- this operator is also referred to as the ternary operator.

```
condition ? expression1 : expression2
```

```c
x = y > 7 ? 25 : 50;

total_price = unit_price*quantity*(quantity>10 ? 1.0 : 0.95);
```

if you want to display a message indicating the number of pets that a person has,
 and you want the message to change between singular and plural automatically, you
could write this:

```c
printf("You have %d pet%s.", pets, pets == 1 ? "" : "s" );
```

You can use this mechanism to vary an output message depending on the value of an
expression in many different ways: she instead of he, wrong instead of right, and
so on.

```
if(choice1)
/* Statement or block for choice 1 */
else if(choice2)
/* Statement or block for choice 2 */
else if(choice3)
/* Statement or block for choice 2 */
/* ... and so on ...
*/
else
/* Default statement or block
*/
```

```
switch Statement:

switch(ticket_number)
{
case 35:
printf("Congratulations! You win first prize!");
break;
case 122:
printf("You are in luck - second prize.");
break;
case 78:
printf("You are in luck - third prize.");
break;
default:
printf("Too bad, you lose.");
}
```

The effect of the break statement following the printf() is to skip over the other
 statements within that block and continue with whatever statement follows the
closing brace.

The case values must all be constant expressions, which are expres-
sions that can be evaluated at compile time.

```c
switch(tolower(ch))
{
case 'a': case 'e': case 'i': case 'o': case
printf("The character is a vowel.");
break;
case 'b': case 'c': case 'd': case 'f': case 'u':
case 'l': case 'm': case 'n': case 'p': case 'g': case 'h': case 'j': case 'k':
case 'v': case 'w': case 'x': case 'y': case 'q': case 'r': case 's': case 't':
case 'z':
printf("The character is a consonant.");
break;
default:
printf("The character is not a letter.");
break;
}
```

that’s declared in the `<ctype.h>` header.

The `isalpha()` function will return a nonzero integer (thus true) if the character
 that’s passed as the argument is an alphabetic character,
and it will `return 0 (false)` if the character isn’t an alphabetic character.

##Functions for Testing Characters

Function  |  Description
----------|-----------------
islower() |	Lowercase letter
isupper() |	Uppercase letter
isalnum() |	Uppercase or lowercase letter
iscntrl() |	Control character
isprint() |	Any printing character including space
isgraph() |	Any printing character except space
isdigit() |	Decimal digit('0' to '9')
isxdigit() |	Hexadecimal digit('0' to '9', 'A' to 'F', 'a' to 'f')
isblank() |	Standard blank characters(space, '\t')
isspace() |	Whitespace character(space, '\n', '\t', '\v', '\r', '\f')
ispunct() |	Printing character for whic isspace() and isalnum() return false

The case statements can be in any order in a switch, and default doesn’t
have to be the last.

##Bitwise Operators:

Operator | Description
---------|----------------
& |	Bitwise AND operator
\| |	Bitwise OR operator
^ |	Bitwise Exclusive OR (EOR) operator
~ |	Bitwise NOT operator
\>> | 	Bitwise shift right operator
<< |	Bitwise shift left operator

All of these only operate on integer types.
The `~` operator is a unary operator — it applies to one operand — and the others
are binary operators.

You can use all of the binary bitwise operators in the op= form of assignment.

The exception is the operator `~`, which is a unary operator

```
lhs op= rhs;
```

```
value <<= 4; <==> value = value << 4;
```

usecase:
You first define a value, usually called a mask, that you use to select the bit
or bits that you want.

```c
unsigned int male	= 0x1; /* Mask selecting first (rightmost) bit */
unsigned int french	= 0x2; /* Mask selecting second bit  */
unsigned int german	= 0x3; /* Mask selecting third bit  */
unsigned int italian	= 0x8; /* Mask selecting fourth bit  */
unsigned int payBracket	= 0x10;/* Mask selecting fifth bit  */
```

In each case, a 1 bit will indicate that the particular condition is true.
These masks in binary each pick out an individual bit, so you could have an
unsigned int variable, personal_data, which would store five items of information
 about a person.

to test bits: use &

```c
if(personal_data & german)
/* Do something because they speak German */
```

to set bits: use |

```c
personal_data |= french; /* Set second bit to 1 */
```

Because of the way the | operator works, you can set multiple bits in a single
statement:

```c
personal_data |= french|german|male;
```

What about resetting a bit?
Suppose you want to change the male bit to female. This amounts to
resetting a 1 bit to 0, and it requires the use of the ! operator with
the bitwise AND:

```c
personal_data &= !male; /* Reset male to female */
```
calculator:

```c
printf("\nEnter the calculation\n");
scanf("%llf %c %llf", &number1, &operation, &number2);
```
