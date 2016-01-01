Like most modern programming languages, the C language has no input or output
capability within the language. All operations of this kind are provided by
functions from standard libraries.

##Input and Output Streams

the standard input/output functions in C have been designed to be
device-independent, so that the transfer of data to or from a specific device
isn’t a concern of the programmer.
The C library functions and the operating system make sure that operations with a
 specific device are executed correctly.

Each input source and output destination in C is called a stream.
An input stream is a source of
data that can be read into your program, and an output stream is a destination
for data that originates in your program.

A disk drive can have multiple input and output streams because it can contain
multiple files.

A stream can be associated with a specific file on the disk.
The stream that you associate with a particular file could be an input stream, so
 you could only read from the file;
it could be an output stream, in which case you could only write to the file;
or the stream might allow input and output so reading and writing
the file would both be possible.

You could also associate a
stream with a file on a CD-ROM drive. Because this device is typically read-only, the stream would, of necessity, be an input stream.

There are two further kinds of streams:
character streams, which are also referred to as text streams, and binary streams.

The main difference between these is that data transferred to or from
character streams is treated as a sequence of characters and may be modified by
the library routine concerned, according to a format specification.

Data that’s transferred to or from binary streams is just a sequence of bytes
that isn’t modified in any way.

C has three predefined standard streams that are automatically available in any
program. (If you include `<stdio.h>`)

These standard streams are stdin, stdout, and stderr.

No initialization or preparation is necessary to use these streams. You just have to apply the appropriate library function that sends data to them.

Stream |	Device
-------|--------------
stdin  |	Keyboard
stdout |	Display screen
stderr |	Display screen
stdprn | 	Printer
stdaux | 	Serial port

The stderr stream is simply the stream to which error messages from the C library
are sent, and you can direct your own error messages to stderr if you wish.

The main difference between stdout and stderr is that output to stdout is buffered
 in memory so the data that you write to it won’t necessarily be transferred
immediately to the device

whereas stderr is unbuffered so any data you write to it is
always transferred immediately to the device.

With a buffered stream your program transfers data to or from a buffer area in
memory, and the actual data transfer to or from the physical device can occur
asynchronously. This makes the input and output operations much more efficient.

The advantage of
using an unbuffered stream for error messages is that you can be sure that they
will actually be displayed but the output operations will be inefficient;

a buffered stream is efficient but may not get flushed
when a program fails for some reason, so the output may never be seen.

Both stdin and stdout can be reassigned to files, instead of the default of
keyboard and screen.
If you want to run your program several times with the same data, during testing
for example, you could prepare the data as a text file and redirect stdin to the
file.

This enables you to rerun the program with the same data without having to
re-enter it each time.

There are two forms of input from the keyboard on stdin:
formatted input, which is provided primarily by the scanf() function
and unformatted input, in which you receive the raw character data from a
function such as getchar().

Formatted Keyboard Input:
As you know, the function scanf() reads characters from the stream stdin and
converts them to one or more values according to the format specifiers in a
format control string.

The prototype of the scanf() function is as follows:

```c
int scanf(char* format, ... );
```

format control string usually appears as an explicit argument in the function call
, such as  `scanf("%lf", &variable);`
But there’s nothing to prevent you from writing this:

```c
char str[] = "%lf";
scanf(str, &variable);
```

The scanf() function makes use of the facility of handling a variable number of
arguments thatyou learned about.
The format control string is basically a coded description of how
scanf() should convert the incoming character stream to the values required.

The scanf() function reads from stdin until it comes to the end of the format
control string, or until an error condition stops the input process.

Note: scanf() returns a value that is the count of the number of input
values read.
This provides a way for you to detect when an error occurs by comparing the value
returned by scanf() with the number of input values you are expecting.

The wscanf() function provides exactly the same capabilities as scanf() except
that the first argument to the function, which is the format control string, must
be a wide character string of type `wchar_t *`.

Thus you could use wscanf() to read a floating-point value from the keyboard
like this: `wscanf(L"%lf", &variable);`

The format control string that you use with scanf() or wscanf() isn’t the same as
 that used with printf().

Putting one or more whitespace characters — blank ' ', tab '\t', or newline '\n'—
in the format control string causes scanf() to read and ignore whitespace
characters up to the next nonwhitespace character in the input.

A single whitespace character in the format control string causes any number of
consecutive whitespace characters to be ignored.

The most general form of a format specifier is:
1) The % simply indicates the start of the format specifier. It must always be
present.

2) The next * is optional.
If you include it, it indicates that the next input value is to be ignored.
It does become useful when stdin has been reassigned to a file and you don’t want
 to process all the values that appear within the file in your program.

3) The field width is optional.
It’s an integer specifying the number of characters that scanf()
should assume makes up the current value being input.

4) The next character is also optional, and it can be `h`, `L`, `l` or `ll`.
If it’s `h`, it can only be included with an integer conversion specifier
(`d`, `I`, `o`, `u`, or `x`) and indicates that the input is to be converted as short.
If it’s `l`, it indicates `long` when preceding an `int`.
and `double` when preceding a float conversion specifier.

Prefixing the c specification with `l` specifies a wide character conversion

The prefix `L` applied to `e`, `E`, `f`, `g`, or `G` specifies the value is of type `long double`.

The `ll` prefix applies to integer conversions and specifies `long long`


Conversion Character  |   	Meaning
----------------------|---------------
d  |		Convert input to int.
i |		Convert input to int. \
		If preceded by 0, then assume octal digits input. \
		If preceded by 0x or 0X, then assume hexadecimal digits input.
o |		Convert input to int and assume all digits are octal.
u |		Convert input to unsigned int.
x |		Convert to int and assume all digits are hexadecimal.
c | 	Read the next character as char (including whitespace).
s | 	Input a string of successive nonwhitespace characters
e, f, or g | Convert input to type float.
n |		No input is read but the number characters that have been read
        from the input source up to this point are stored in the
		corresponding argument, which should be of type int*.


You can also read a string that consists of specific characters by placing all
the possible characters between square brackets in the specification.
For example:
the specification `%[0123456789.-]` -- will read a numerical value as a string

To read a string
consisting of the lowercase letters a to z, you could use the specification
`%[abcdefghijklmnopqrstuvwxyz]`.

you can specify the characters that are not in the string by
using the ^ character as the first character in the set.

Thus, the specification `%[^,]` will include everything in the string except
a comma

The `%n`
specifier does not extract data from the input stream; it just stores the number
of bytes that the scanf() function
has read from the input stream up to that point

It isn’t essential that you enter all the data on a single line.
If you key in the first two values and press Enter, the scanf() function will
wait for you to enter the next value on the next line.

for example:
the specification `%*d` causes the input to be read but not stored
the specification `%[^o]` accepts any character in the string except
the letter 'o'.


If you want to be able to recover from the situation in which invalid input is
entered, you can use the return value from scanf() as a measure of whether all
the necessary input has been processed correctly and include some code to
retrieve the situation when necessary.

You can include a sequence of one or more characters that isn’t a format
conversion specifier within the input format string.
If you do this, you’re indicating that you expect the same characters to appear
in the input and that the scanf() function
should read them but not store them.

These have to be matched
exactly, character for character, by the data in the input stream.

Any variation will terminate the input scanning process in scanf().

for example:
```c
int value_count = 0;
value_count = scanf("fp1 = %f i = %d %d", &fp1, &i , &j);
```

It doesn’t matter whether the blanks before and after the = are included in the
input—they’re whitespace characters and are therefore ignored.

The important thing is to include the same characters that appear in the format
control string in the correct sequence and at the correct place in the input.

Reading Hexadecimal and Octal Values
As you saw earlier, you can read hexadecimal values from the input stream using
the format specifier %x.
For octal values you use %o.

For example:
```c
scanf(" %d %x %o", &i , &j, &k );

printf("\ni = %d j = %d k = %d\n", i, j, k );
```

Input:
```
12 12 12
```

Output:
```
i = 12	j = 18	k = 10
```

Hexadecimal data entry can be useful when you want to enter bit patterns
(sequences of 1s and 0s), as they’re
easier to specify in hexadecimal than in decimal.

You can enter hexadecimal values using A to F, or a to f, or even a mixture if
you’re so inclined.

There’s no difference between using "%x" and "%X" with scanf(), but they’ll have
a different effect when you use them with printf() for output.
You can demonstrate this by changing the last printf() statement to the
following:

```c
printf("\ni = %x j = %X k = %d\n", i, j, k );
```

Input:
```
26 AE 77
```

Output:
```
i = 26	j = AE	k = 63
```

So "%x" produces hexadecimal output using hexadecimal digits a to e, and "%X"
produces output using hexadecimal digits A to E.


Reading Characters Using scanf()
For a string of characters, you use either the specifier %s or the specifier %[],
or if you are storing the input as wide characters, %ls or %l[]

In this case, the string is stored as a null-terminated string with '\0' as the
last character.

With `%[]` or `%l[]` format specification, the string to be read must include only
the characters that appear between the square brackets, or if the first character
between the square brackets is ^, the string must contain
only characters that are not among those following the ^ characters.

Thus, `%[aeiou]` will read a string
that consists only of vowels. The first character that isn’t a vowel will signal
the end of the string.

The specification `%[^aeiou]` reads a string that contains any character that isn’t
a vowel. The first vowel will signal the end of the string.

Note that one interesting aspect of the %[] specification is it enables you to
read a string containing spaces, something that the %s specification can’t do.

You just need to include a space as one of the characters between
the square brackets.

If you don’t want a space to be accepted as the initial, you can fix this by
writing the input statement as follows:

```c
scanf(" %c", &initial );
```

Now the first character in the control string is a space, so scanf() will read
and ignore any number of spaces and read the first character that isn’t a space
into initial.

Pitfalls with scanf()
There are two very common mistakes people make when using scanf():

  1.  Don’t forget that the arguments must be pointers.
  2. When reading a string, remember to ensure that there’s enough space for the
  string to be read in, plus the terminating '\0'.
  If you don’t do this, you’ll overwrite something in memory,
  possibly even some of your program code. :)

##String Input from the Keyboard

Where you want to be sure that your array length will not be exceeded, you have
the option of using the fgets() function.

For string input, using fgets() is usually the preferred approach unless you want
 to control the content of the string, in which case you can use `%[]`.

The `%[]` specification is more convenient to use when the
nonstandard `%[a-z]` form is supported.

The C library function `int fflush(FILE *stream)` flushes the output buffer of a
stream.

```
fflush(stdout); -- flush stream
```
it's only used with output stream

The most portable solution for flushing stdin would probably be something along
the lines of the following:

```c
int c;
while ((c = getchar()) != '\n' && c != EOF);
```

##Unformatted Input from the Keyboard

The getchar()function reads one character at a time from stdin.
The getchar() function is defined in <stdio.h>, and its general syntax is as
follows:

```c
int getchar(void);
```

Note that this character is returned as int, and the character is displayed on
the screen as it is entered from the keyboard.

With many implementations of C, the nonstandard header file `<conio.h>` is often
included.
This provides additional functions for character input and output.
One of the most useful of these is getch(), which reads a character from the
keyboard without displaying it on the screen.

This is particularly useful when you need to prevent others from being able to see
 what’s being keyed in — for example, when a password is being entered.

The standard header `<stdio.h>` also declares the `ungetc()` function that enables
you to put a character that you have just read back into an input stream.
The function requires two arguments:
1) the first is the character to be pushed back onto the stream
2) and the second is the identifier for the stream

which would be stdin for the standard input stream.

The ungetc() returns a value of type int
that corresponds to the character pushed back onto the stream, or a special
character, EOF (end-of-file), if the operation fails.

the ungetc() function makes it possible
for you to read a succession of characters using getchar(), and when you find
you have read a character that is not a digit, you can return it to the stream.

Here’s a function that ignores spaces and tabs
from the standard input stream using the getchar() and ungetc() functions:

```
void eatspaces(void)
{
  char ch = 0;
  while(isspace(ch = getchar()));
  ungetc(ch, stdin);
}
```

The isspace() function that is declared in the <ctype.h> header file returns true
when the argument is a space character.

```c
/* Function to check for newline */
bool isnewline(void)
{
  char ch = 0;
  if((ch = getchar()) == '\n')
    return true;
  ungetc(ch, stdin);
  return false;
}
```

Output to the Screen
Writing data to the command line on the screen is much easier than reading input
from the keyboard.
The primary function for formatted output to the stdout stream is printf().

The printf() function is defined in the header file <stdio.h>, and its general
form is the following:

```c
int printf(char* format, ...);
```

The first parameter is the format control string.

There must be at least as many arguments as there are format specifiers.
If not, the results are unpredictable. If there are more arguments than specifiers
, the excess is ignored.

The <stdio.h> header also declares the wprintf() function.
the wprintf() function works in exactly the same way as printf() except that it
expects the first argument to be a wide character string.

The general form of an output format specifier is:

  1. The % sign indicates the start of the specifier, as it does for output.
  2. The optional flag characters are +, -, #, and space.
  These affect the output, as shown in table

Character |	Use
----------|-----------------------------------------
+ 	|	Ensures that, for signed output values, there’s always a sign \
		preceding the output value—either a plus or a minus sign. \
		By default, only negative values have a sign.
- 	|	Specifies that the output value is left-justified in the output \
		field and padded with blanks on the right. The default positioning \
        of the output is right-justified.
0 	|	Prefixes a field_width value to specify that the value should be \
		padded with zeros to fill out the field width to the left.
\# 	|	Specifies that 0 is to precede an octal output value, 0x; or 0X \
		is to precede a hexadecimal output value;

  3. The optional field_width specifies the minimum number of characters for the
  output value. If the value requires more characters, the field is simply expanded.

  4. The precision specifier is also optional and is generally used with
  floating-point output values.

A specifier of `.n` indicates that n decimal places are to be output. If the value
to be output has more than n significant digits, it’s rounded or truncated.

  5. You prefix the appropriate type conversion character with the h, l
  (lowercase letter L), ll, or L modifier to specify that the output conversion
  is being applied to short, long, long long, or long double values, respectively.

The `l` modifier applied to the c type specification specifies `wchar_t`

  6. The conversion character that you use defines how the output is to be converted
  for a particular type of value.


##Applicable to integers

Conversion Character |	Output Produced
---------------------|-----------------
d	 |		Signed decimal integer value
o	 |		Unsigned octal integer value
u	 |		Unsigned decimal integer value
x	 |		Unsigned hexadecimal integer value with lowercase \
			hexadecimal digits a,b,c,d,e,f
X	 |		As x but with uppercase hexadecimal digits A,B,C,D,E,F

##Applicable to floating-point

Conversion Character |	Output Produced
---------------------|-----------------
f	 |		Signed decimal value
e	 |		Signed decimal value with exponent
E	 |		As e but with E for exponent instead of e
g	 |		As e or f dependig on size of value and precision
G	 |		As g but with E for exponent values

##Applicable to characters

Conversion Character |	Output Produced
---------------------|-----------------
c	 |		Single character
s	 |		All characters until '\0' is reached or precision \
			characters have been output

##Escape Sequences

You can include whitespace characters in the format control string for printf()
and wprintf().
Some of these are represented by escape sequences that begin with \.

You use the escape sequence \\ when you want to output the backslash character \.
To write a % character to stdout, use %%.

Escape Sequence	 |	Description
-----------------|-----------------
\b 		|	Backspace
\f 		|	Form-feed or page eject
\n 		|	Newline
\r 		|	Carriage return (for printers) or move to the beginning \
			of the current line
\t 		|	Horizontal tab

**Tip** \
When you’re outputting multiple rows of values on the screen, using a width
specification —possibly
with tabs—will enable you to line them up in columns.

**Hack**: if you want to output number in binary format:
Here is a quick hack to demonstrate techniques to do what you want.

```c
const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}
```

A few changes: strcat is an inefficient method of adding a single char to the
string on each pass of the loop.
Instead, add a `char *p = b;` and replace the inner loop with
`*p++ = (x & z) ? '1' : '0'`.

So

```c
const char *byte_to_binary(int x)
{
    static char b[9];   
    b[8] = '\0';
    char *p = b;

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        *p++ = (x & z) ? '1' : '0';
    }

    return b;
}
```

##Character Output

There are basically four flavors of output specifications you can use with
printf() and wprintf() for character data:

`%c` and `%s` for single characters and strings,
and `%lc` and `%ls` for single wide characters and wide character strings,
respectively.

cool technique to print every 32 characters on one line:

```c
int count = 0;
char ch = 0;
for(int code = 0 ; code <= CHAR_MAX ; code++)
{
  ch = (char)code;
  if(isprint(ch))
  {
    if(count++ % 32 == 0)
	printf("\n");
     printf("%c", ch);
  }
}
```

Per the C standard, each FILE stream has associated with it an "orientation"
(either "byte" or "wide) which is set by the first operation performed on it, and
which can be inspected with the fwide function. Calling any function whose
orientation conflicts with the orientation of the stream results in undefined
behavior.

The "#" sign in printf and wprintf specification means "0x" in output.
for example:

```c
wprintf(L" %lc %x %lc %#x", L'a', (long)L'a', towupper(L'a'), (long)towupper(L'a'))
```

output the following:
```
a 61 A 0x41
```

##Other Output Functions

In addition to the string output capabilities of printf() and wprintf(), you have
the puts() function that is also declared in <stdio.h>, and which complements
the gets() function.

The name of the function derives from its purpose: put string.
The general form of puts() is as follows:

```c
int puts(const char *string);
```

The puts() function accepts a pointer to a string as an argument and writes the
string to the standard output stream, stdout.
The string must be terminated by '\0'.

The parameter to puts()
is `const` so the function does not modify the string you pass to it.

The function returns a negative
integer value if any errors occur on output, and a nonnegative value otherwise.

The puts() function
is very useful for outputting single-line messages, for example:

```c
puts("Is there no end to input and output?");
```

This will output the string that is passed as the argument and then move the
cursor to the next line.

Also included in <stdio.h>, and complementing the function getchar(), is the
function putchar().
This has the following general form:

```c
int putchar(int c);
```

The putchar() function outputs a single character, c, to stdout and returns the
character that was displayed.
This gives you control over whether or not you output particular characters.

Formatted Output to an Array
You can write formatted data to an array of elements of type char in memory
using the sprintf() function that is declared in the <stdio.h> header file.

This function has the prototype:

```c
int sprintf(char *str, const char *format, . . .);
```

The function writes the data specified by the third and subsequent arguments
formatted according to the format string second argument.

This works identically to printf() except that the
data is written to the string specified by the first argument to the function.

The integer returned is a
count of the number of characters written to str, excluding the terminating null
character.

For example:

```c
char result[20];	/* Output from sprintf */
int count = 4;
int nchars = sprintf(result, "A dog has %d legs.", count);
```

The effect of this is to write the value of count to result using the format
string that is the second argument.
The effect is that result will contain the string "A dog has 4 legs.".

The variable nchars will contain the value 17, which is the same as the return
value you would get from strlen(result) after the sprintf() call has executed.

The sprintf() function will return a negative integer if an
encoding error occurs during the operation.

One use for the sprintf() function is to create format strings programmatically.

Formatted Input from an Array
The sscanf() function complements the sprintf() function because it enables you
to read data under the control of a format string from an array of elements of
type char.
The prototype looks like this:

```c
int sscanf(const char *str, const char *format, . . .);
```

Data will be read from str into variables that are specified by the third and
subsequent arguments according to the format string format.

The function returns a count of the number of items actually read, or EOF if a
failure occurs before any data values are read and stored.

The end of the string is recognized as the end-of-file condition, so reaching the
end of the str string before any values are converted will cause EOF to be
returned

For example:

```c
char *source = "Fred 94";
char name[10];
int age = 0;
int items = sscanf(source, " %s %d", name, age);
```

The result of executing this fragment is that name will contain the string "Fred"
and age will contain the value 94.

One use for the sscanf() function is to try various ways of reading the same data:

You can always read an input line into an array as a string.
You can then use sscanf() to reread the same input line from the array with
different format strings as many times as you like.
