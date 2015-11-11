In this chapter you’ll extend your knowledge of
arrays by exploring how you can use arrays of characters.

You’ll frequently have a need to work with a text string as a single entity. 

As you’ll see, C doesn’t provide you with a string data type as some
other languages do. Instead, C uses an array of elements of type char to store a 
string.

A string constant is a
sequence of characters or symbols between a pair of double-quote characters.

printf("This is a string.");

a special character with the code value 0 is added to the end of each string
to mark where it ends. This character is known as the null character (not to be 
confused with NULL, 
which you’ll see later), and you write it as \0.

Note: 
Because a string in C is always terminated by a \0 character, the length of a 
string is always one greater
than the number of characters in the string.

printf("The character \0 is used to terminate a string.");

If you compile and run this program, you’ll get this output:
The character

The output ends after the first two words because the printf() function stops 
outputting the string when it reaches the first null character, \0.

You use an array of type char to hold strings. This is the simplest form of string
 variable. You could declare a char array variable as follows:

char saying[20];

The variable saying that you’ve declared in this statement can accommodate a 
string that has up to 19 characters, because you must allow one element for the 
termination character.

You could also initialize the preceding string variable like this:
char saying[] = "This is a string.";

Here you haven’t explicitly defined the array dimension. 
The compiler will assign a value to the dimension sufficient to hold the 
initializing string constant. 

In this case it will be 18, which corresponds to 17 elements for the characters in
 the string, plus an extra one for the terminating \0.

You could also initialize just part of an array of elements of type char:
char str[40] = "To be";

Here, the compiler will initialize the first five elements from str[0] to str[4]
and str[5] will contain the null value '\0'.

const char message[] = "The end of the world is nigh";

Because you’ve declared message as const, it’s protected from being modified 
explicitly within the program. Any attempt to do so will result in an error 
message from the compiler.

if you want to output the string stored in message using the printf() function, 
you could write this:
printf("\nThe message is: %s", message);

the printf() function will output successive characters from the message
array until it finds the '\0' character.

a while loop that determines the length of the first string:

while (str1[count] != '\0')	/* Increment count till we reach the string */
	count++;		/* terminating character.
				*/

this loop would typically be written like this:
while(str1[count])
	count++;

The ASCII code value for the '\0' character is zero which corresponds to the 
Boolean value false. 
All other ASCII code values are nonzero and therefore correspond to the Boolean 
value true.

Unfortunately you can’t use the assignment operator to copy a string in the way you do with int or double variables. 

To achieve the equivalent of an arithmetic assignment with strings, one string
has to be copied element by element to the other.

count2 = 0; /* Reset index for str2 to 0 */
while(str2[count2]) /* Copy up to null from str2 */
	str1[count1++] = str2[count2++];
str1[count1] = '\0';

/* Check that we have enough space for both strings */
if(sizeof str1 < count1 + count2 + 1)
	printf("\nYou can't put a quart into a pint pot.");

You should never append characters to a string without first checking that there 
is sufficient space in the array to accommodate them.

It’s essential that you do not try to place more characters in the array than it 
can hold, as this will overwrite some memory that may contain important data. 
This is likely to crash your program.

You could replace the three lines of code that did the copying with a more concise
 alternative:

while ((str1[count1++] = str2[count2++]));

Let’s consider what happens at each stage:

1. Assign the value of str2[count2] to str1[count1]. 
An assignment expression has a value that is the value that was stored in the 
left operand of the assignment operator. 
In this case it is the character that was copied into str1[count1].

2. Increment each of the counters by 1, using the postfix form of the ++ operator.

3. Check whether the value of the assignment expression - which will be the last character stored in str1 - is true or false.
The loop ends after the '\0' has been copied to str1, which will result in the 
value of the assignment being false.

Arrays of Strings:

char sayings[3][32] = {
			"Manners maketh man.",
			"Many hands make light work.",
			"Too many cooks spoil the broth."
			};


Although you must specify the last dimension in an array of strings, you can leave
 it to the compiler to figure out how many strings there are:

char sayings[][32] = {
			"Manners maketh man.",
			"Many hands make light work.",
			"Too many cooks spoil the broth."
			};

Because you have three initializing strings, the
compiler will make the first array dimension 3.


char str[][40] = {
		"To be or not to be" ,
		", that is the question"
		};

int count[] = {0, 0};	/* Lengths of strings */

The string lengths are now stored as elements in the count array. 
With count as an array we are able to find 
the lengths of both strings in the same loop:

/* find the lengths of the strings */
for(int i = 0 ; i<2 ; i++)
  while (str[i][count[i]])
	count[i]++;



String Library Functions:
-------------------------

The string functions are declared in the <string.h> header file, so you’ll need to put

#include <string.h>

at the beginning of your program if you want to use them

copying strings:
strcpy(string1, string2);

The arguments to the strcpy() function are char array names.

What the function actually does
is copy the string specified by the second argument to the string specified by the first argument, so in the preceding example string2 will be copied to string1, 
replacing what was previously stored in string1. 
The copy operation will include the terminating '\0'. It’s your responsibility to 
ensure that the array string1 has sufficient space to accommodate string2.

Obviously, the sizeof operator
is important because you’ll most likely check that everything is as it should be:

if(sizeof(string2) <= sizeof (string1))
	strcpy(string1, string2);


You have another function available, strncpy(), that will copy the 
first n characters of one string to another. 
The first argument is the destination string, the second argument is the source 
string, and the third argument is an integer of type size_t that specifies the 
number of characters to be copied.

char destination[] = "This string will be replaced";
char source[] = "This string will be copied in part";
size_t n = 26; /* Number of characters to be copied */
strncpy(destination, source, n);

A '\0' character will be
appended after the last character copied.

If source has fewer than 26 characters, the function will 
add '\0' characters to make up the count to 26.


To find out the length of a string you have the function strlen(), which returns 
the length of a string 
as an integer of type size_t.

count2 = strlen(str2);

It also returns the value as size_t 
which corresponds to an unsigned integer type

type size_t is a type that is defined in the standard library header file
<stddef.h>. 
This is also the type returned by the operator sizeof. The type size_t will be 
defined to be one of the unsigned integer types you have seen, typically unsigned int.

The reason for implementing things this way is code portability.
The type returned by sizeof and the strlen() function, among others, can vary from one C implementation to another. It’s up to 
the compiler writer to decide what it should be.

As long as you define count2 in the preceding example as type size_t, you have 
code that will work in every standard C implementation, even though the definition of size_t may vary from one implementation to another.

So for the most portable code, you should write the following:

size_t count2 = 0;
count2 = strlen(str2);

Note that it returns the length of the string
excluding the '\0', which is generally the most convenient result.

Joining Strings Using a Library Function:
You could use a function that joins one string to the end of another.

strcat(str1, str2);	/* Copy str2 to the end of str1 */

The strcat() function is so called because it performs string catenation; 
in other words it joins one string onto the end of another.
As well as appending str2 to str1, the strcat() function also returns str1.

to append part of the source string to the destination string, you can use the
strncat() function.

strncat(str1, str2, 5);	/* Copy 1st 5 characters of str2 to the end of str1 */

As with all the operations that involve copying one string to another, it’s up to 
you to ensure that the destination array is sufficiently large to accommodate what
’s being copied to it.

All these string functions return the destination string.
This allows you to use the value returned in another string operation, for example

size_t length = 0;
length = strlen(strncat(str1, str2, 5));

if(STR_LENGTH > strlen(str1) + strlen(str2)) /* Enough space ? */
	printf("\n%s\n", strcat(str1, str2)); /* yes, so display joined string */
else
	printf("\nYou can't put a quart into a pint pot.");


Note that the comparison uses the > operator—this is because the
array length must be at least one greater than the sum of the two string lengths 
to allow for the terminating '\0' character.

The function strcmp(str1, str2) compares two strings. It returns a value of type 
int that is less than, equal to, or greater than 0, corresponding to whether str1 is less than, equal to, or greater than str2.

char str1[] = "The quick brown fox";
char str2[] = "The quick black fox";
if(strcmp(str1, str2) < 0)
	printf("str1 is less than str2");


The strncmp() function compares up to n characters of the two strings. The first 
two arguments are the same as for the strcmp() function and the number of 
characters to be compared is specified by a third argument that’s an integer of 
type size_t.


char word1[20];
scanf("%19s", word1);

The function scanf() will limit the number of characters read if you specify 
a width with the format specification.

While this ensures the array limit will not be exceeded, any characters in excess of the width you specify will be left in the input stream and will be read by the
 next input operation for the stream.

The width specification of 19 characters ensures that the array size of 20 
elements will not be exceeded. 
Notice how in this example you haven’t used an & operator before the variables in
 the arguments to the scanf() function.
This is because the name of an array by itself is an address.
It corresponds to the address of the first element in the array.

You could write this explicitly using the & operator like this:
scanf("%s", &word1[0]);

C provides a remarkably useful type of variable called a pointer. 
A pointer is a variable that contains an address—that is, it contains a reference
 to another location in memory that can contain a value.

You already used an address when you used the function scanf().

A pointer with the name pNumber is defined by the second of the following two 
statements:

int Number = 25;
int *pNumber = &Number;

You declare a variable, Number, with the value 25, 
and a pointer, pNumber, which contains the address of Number.

You can now use the variable pNumber in the expression *pNumber to obtain the
value contained in Number. 
The * is the dereference operator and its effect is to access the data stored
at the address specified by a pointer.

The strchr() function searches a given string for a specified character.

The first argument to the function is the string to be searched (which will be the
 address of a char array), and the second argument is the character that you’re 
looking for.

The function will search the string starting at the beginning and
return a pointer to the first position in the string where the character is found.

This is the address of this position in memory and is of type char* 
described as “pointer to char.”

If the character isn’t found, the function will return a special value NULL, 
which is the equivalent of 0 for a pointer and
represents a pointer that doesn’t point to anything.

char str[] = "The quick brown fox";	/* The string to be searched */
char c = 'q';				/* The character we are looking for */
char *pGot_char = NULL;			/* Pointer initialized to zero */
pGot_char = strchr(str, c);		/* Stores address where c is found */

Because the strchr()
function expects the second argument to be of type int, the compiler will convert
 the value of c to this type before passing it to the function.

You could just as well define c as type int like this:
int c = 'q';	/* Initialize with character code for q */

Functions are often implemented so that a character is passed as an argument of 
type int because it’s simpler to work with type int than type char.


If you do try to dereference a NULL pointer, your program will crash. This is very easy to avoid with an if statement, like this:
if(pGot_char != NULL)
	printf("Character found was %c.", *pGot_char);


The strrchr() function is very similar in operation to the strchr() function, 
except that it searches for the character starting from the end of the string. 

Thus, it will return the address of the last occurrence of the character in the 
string, or NULL if the character isn’t found.


Searching a String for a Substring:

The strstr() function searches one string for the first occurrence of a substring 
and returns a pointer to the position in the first string where the substring 
is found.
If it doesn’t find a match, it returns NULL.

The first argument to the function is the string that is to be
searched, and the second argument is the substring you’re looking for.

char text[] = "Every dog has his day";
char word[] = "dog";
char *pFound = NULL;
pFound = strstr(text, word);

The search is case sensitive, so if you search the text string for "Dog", it won’t be found.

If you need to examine the internal contents of a string, you can use the set of 
standard library functions that are declared the <ctype.h> header file

Function	Tests For
-------------------------
islower()	Lowercase letter
isupper()	Uppercase letter
isalpha()	Uppercase or lowercase letter
isalnum()	Uppercase or lowercase letter or a digit
iscntrl()	Control character
isprint()	Any printing character including space
isgraph()	Any printing character except space
isdigit()	Decimal digit ('0' to '9')
isxdigit()	Hexadecimal digit ('0' to '9', 'A' to 'F', 'a' to 'f')
isblank()	Standard blank characters (space, '\t')
isspace()	Whitespace character (space, '\n', '\t', '\v', '\r', '\f')
ispunct()	Printing character for which isspace() and isalnum() return false

The argument to a function is the character to be tested. All these functions 
return a nonzero value of type int if the character is within the set that’s 
being tested for; otherwise, they return 0.

char buffer[80];
printf("\nEnter an interesting string of less than 80 characters:\n");
gets(buffer);	/* Read a string into buffer */

gets is deprecated because it can cause buffer overflows!

use fgets instead:
fgets(buffer, sizeof(buffer), stdin);

You use the fgets() function here because it will read in any string from the 
keyboard, including spaces, the
input being terminated when the Enter key is pressed.

There is no check that buffer has sufficient space to store the string so there’s another opportunity to crash the program. 
To avoid this you could use the fgets() function, which allows you to specify the maximum length of the input string. This is a function that is used for any kind 
of input stream, as opposed to gets() which only reads from
the standard input stream stdin; so you also have to specify a third argument to 
fgets() indicating the stream that is to be read. Here’s how you could use fgets() to read a string from the keyboard:

if(fgets(buffer, sizeof(buffer), stdin) == NULL)
{
printf("Error reading input.");
return 1;
/* End the program */
}

If an error of some kind prevents the fgets() function
from reading the input successfully, it will return NULL (normally, it returns the
 address passed as the argument— buffer, in this case).

The fgets() function reads a maximum of one less than the number of characters 
specified by the second argument. It then appends a \0 character to the end of 
the string in memory, so the second argument in this case is
sizeof(buffer).

Note that there is another important difference between fgets() and gets(). 
For both functions, reading a newline character ends the input process, but 
fgets() stores a '\n' character when a newline is entered, 
whereas gets() does not. This means that if you are reading strings from the 
keyboard, strings read by fgets() will be one character longer than strings read 
by gets(). It also means that just pressing the Enter key as the input will
result in an empty string "\0" with gets(), but will result in the string "\n\0" 
with fgets().


You can therefore convert a string to uppercase using this statement:

for(int i = 0 ; (buffer[i] = toupper(buffer[i])) != '\0' ; i++);

This loop will convert the entire string to uppercase by stepping through the 
string one character at a time, converting lowercase to uppercase and leaving 
uppercase characters unchanged.
The loop stops when it reaches the string termination character '\0'.

This sort of pattern in which
everything is done inside the loop control expressions is quite common in C.

You’ll recall that fgets() stores the newline character that ends the input 
process. This doesn’t matter particularly for the first string but it matters a 
lot for the second string you are searching for. For example, if the string you
want to find is "dogs", the fgets() function will actually store "dogs\n", which 
is not the same at all. You therefore remove the newline from each string by 
overwriting it with a '\0' character:

text[strlen(text)-1] = '\0';
substring[strlen(substring)-1] = '\0';

The conversion of both strings to uppercase is accomplished using the 
following statements:

for(int i = 0 ; (text[i] = toupper(text[i])) ; i++);
for(int i = 0 ; (substring[i] = toupper(substring[i])) ; i++);

you can test for the occurrence of substring in text, regardless of the case
of the original strings:

printf("\nThe second string %s found in the first.",
	((strstr(text, substring) == NULL) ? "was not" : "was"));


Converting Strings to Numerical Values:

The <stdlib.h> header file declares functions that you can use to convert a string to a numerical value.

Functions That Convert Strings to Numerical Values
Function	Returns
-----------------------
atof()		A value of type double that is produced from the string argument
atoi()		A value of type int that is produced from the string argument
atol()		A value of type long that is produced from the string argument
atoll()		A value of type long long that is produced from the string 
		argument

ATOF stands for ASCII to Float, etc

These functions are very easy to use, for example

char value_str[] = "98.4";
double value = 0;
value = atof(value_str);	/* Convert string to floating-point */

You pass the
array name as the argument to the atof() function to convert it to type double.

Working with Wide Character Strings:

Working with wide character strings is just as easy as working with the strings 
you have been using up to now.

You store a wide character string in an array of elements of type wchar_t and a wide character string constant just needs the L modifier in front of it. 
Thus you can declare and initialize a wide character string like this:

wchar_t proverb[] = L"A nod is as good as a wink to a blind horse.";

As you saw back, a wchar_t character occupies 2 bytes. The proverb string contains
44 characters plus the terminating null, so the string will occupy 90 bytes.

If you wanted to write the proverb string to the screen using printf() you must 
use the %S format specifier rather than %s that you use for ASCII string. 
If you use %s, the printf() function will assume
the string consists of single-byte characters so the output will not be correct. 
Thus the following statement will output the wide character string correctly:

printf("The proverb is:\n%S", proverb);


The <wchar.h> header file declares a range of functions for operating on wide 
character strings that
parallel the functions you have been working with that apply to ordinary strings.

wcslen(const wchar_t* ws)
wcscpy(wchar_t* destination, const wchar_t source)
wcsncpy(wchar_t* destination, const wchar_t source, size_t n)
...

all functions work in essentially the same way as the
string functions you have already seen.

Where the const keyword appears in the specification of the type of argument you 
can supply to a function, it implies that the argument will not be modified by
the function. This forces the compiler to check that the function does not attempt
to change such arguments.

The <wchar.h> header also declares the fgetws() function that reads a 
wide character string from a stream such as stdin, which by default corresponds to
 the keyboard.

You must supply three arguments to the fgetws() function, just like the fgets() 
function you use for reading for single-byte strings:

1. The first argument is a pointer to an array of wchar_t elements that is to 
store the string.

2. The second argument is a value n of type size_t that is the maximum number of 
characters that can be stored in the array.

3. The third argument is the stream from which the data is to be read, which will be stdin when you are reading a string from the keyboard.

The function returns a pointer to the array containing the string.

The function reads up to n-1 characters from the stream and stores them in the 
array with an L'\0' appended.

The function fgetws() also add \n character to the end of string

Testing and Converting Wide Characters

The <wchar.h> header also declares functions to test for specific subsets of wide
 characters, analogous to the functions you have seen for characters of type char.

Function	Tests For
-------------------------
iswlower()	Lowercase letter
iswupper()	Uppercase letter
iswalnum()	Uppercase or lowercase letter
iswcntrl()	Control character
iswprint()	Any printing character including space
iswgraph()	Any printing character except space
iswdigit()	Decimal digit (L'0' to L'9')
iswxdigit()	Hexadecimal digit (L'0' to L'9', L'A' to L'F', L'a' to L'f')
iswblank()	Standard blank characters (space, L'\t')
iswspace()	Whitespace character (space, L'\n', L'\t', L'\v', L'\r', L'\f')
iswpunct()	Printing character for which iswspace() and iswalnum() return 
		false

You also have the case-conversion functions, towlower() and towupper(), 
that return  the lowercase or uppercase equivalent of the wchar_t argument.

towlower() and towupper() are declared in <wctype.h> header file

#define TEXTLEN 10000	/* Maximum length of text */
#define BUFFERSIZE 100	/* Input buffer size */

char text[TEXTLEN+1];
char buffer[BUFFERSIZE];
char endstr[] = "*\n";	/* Signals end of input */


/* A string containing an asterisk followed by newline */
/* signals end of input
*/
if(!strcmp(fgets(buffer, BUFFERSIZE, stdin), endstr))
	break;

The fgets() function reads a maximum of BUFFERSIZE-1 characters from stdin. 
If the user enters a line longer than this, it won’t really matter. The characters that are in excess of BUFFERSIZE-1 will be left in the input stream and will be 
read on the next loop iteration.

You can check that this works by
setting BUFFERSIZE at 10, say, and entering lines longer than ten characters.

Because the fgets() function returns a pointer to the string that you pass as the first argument,
you can use fgets() as the argument to the strcmp() function to compare the string that was read with endstr. Thus, the if statement not only reads a line of input, it also checks whether the end of the input has been signaled by the user.

To append the new line, just
use the strcat() library function to concatenate the string stored in buffer with the existing string in text.

strcat(text, buffer);

char word[WORDLEN+1];	// stores a single word

/* Check for word already stored */
bool isnew = true;
for(int i = 0 ; i< wordcount ; i++)
	if(strcmp(word, words[i]) == 0)
	{
		++nword[i];
		isnew = false;
		break;
	}

using the strcmp() library function that compares two strings.
The function returns 0 if the strings are identical; as soon as this occurs you 
set isnew to false, increment the corresponding element in the nword array, and 
exit the for loop.

/* Output the words and frequencies */
for(int i = 0 ; i<wordcount ; i++)
{
	if( !(i%3) )	/* Three words to a line */
	printf("\n");
	printf(" %-15s%5d", words[i], nword[i]);
}

The expression i%3 will be zero when i is a multiple of 3, and this
value maps to the bool value false, so the expression !(i%3) will be true.




