I talked about how your computer allocates an
area of memory when you declare a variable. You refer to this area in memory 
using the variable name in your program, but once your program is compiled and 
running, your computer references
it by the address of the memory location. This is the number that the computer 
uses to refer to the “box” in which the value of the variable is stored.

Variables that can store addresses are called pointers, and the address that’s stored in a pointer
is usually that of another variable.

You, and more importantly, the compiler, must know the type of data stored in the variable to which it points. Without this information it’s virtually impossible to know how to handle the
contents of the memory to which it points. A pointer to a value of type char is pointing to a value
occupying 1 byte, whereas a pointer to a value of type long is usually pointing
to the first byte of a value occupying 4 bytes.

The type name void means absence of any type, so a pointer of type void * can 
contain the address of a data item of any type. Type void * is often used as an argument type or return value type
with functions that deal with data in a type-independent way. 
Any kind of pointer can be passed
around as a value of type void * and then cast to the appropriate type when you 
come to use it.

When
you want to access the integer value at the address stored in the void * pointer, you must first cast
the pointer to type int *. You’ll meet the malloc() library function later in 
this chapter that returns a pointer of type void *.

You can declare a pointer to a variable of type int with the following 
statement:

int *pointer;

This statement just creates the pointer but doesn’t initialize it.

You can initialize
pointer so that it doesn’t point to anything by rewriting the declaration like 
this:

int *pointer = NULL;

NULL is a constant that’s defined in the standard library and is the equivalent
of zero for a pointer.
NULL is a value that’s guaranteed not to point to any location in memory.

NULL is defined in the header files <stddef.h>, <stdlib.h>, <stdio.h>, 
<string.h>, <time.h>, <wchar.h>, and <locale.h>, and you must have at least one
 of these headers included in your source
file for NULL to be recognized by the compiler.

If you want to initialize your variable pointer with the address of a variable 
that you’ve already declared, you use the address of operator &:

int number = 10;
int *pointer = &number;

Note that the declaration
of number must precede the declaration of the pointer. 
The compiler needs to have already allocated space and thus an address for 
number to use it to initialize the pointer variable.

You can declare regular variables and pointers in the same statement:

double value, *pVal, fnum;

This statement declares two double precision floating-point variables, value and fnum, and a
variable, pVal of type “pointer to double.” With this statement it is obvious that only the second variable,
pVal, is a pointer, but consider this statement:

int *p, q;

This declares a pointer, p, and a variable, q, that is of type int. It is a 
common mistake to think that both p and q are pointers.

Accessing a Value:

You use the indirection operator, *, to access the value of the variable pointed
 to by a pointer. This operator is also referred to as the dereference operator 
because you use it to “dereference” a pointer.

int number = 15;
int *pointer = &number;
int result = 0;

The pointer variable contains the address of the variable number, so you can use
 this in an expression to calculate a new value for total, like this:

result = *pointer + 5;

number = 10;
printf("\nnumber's address: %p", &number);	/* Output the address */

To output the address of the variable called number, you use the output format 
specifier %p. This outputs the value as a memory address in hexadecimal form.

Remember, a pointer itself has an address, just like any other variable.

Because you can access the contents of number through the pointer pointer, you 
can use a dereferenced pointer in arithmetic statements:

*pointer += 25;

you can change the variable that pointer points to by a statement such as this:

pointer = &another_number;

a pointer can
contain the address of any variable of the same type, so you can use one 
pointer variable to change the values of many other variables, as long as 
they’re of the same type as the pointer.

Caution: You should always initialize your pointers when you declare them. 
Using a pointer that isn’t initialized to store an item of data is dangerous. 
Who knows what you might overwrite when you use the
pointer to store a value?

++*pnum;

You can see that the expression ++*pnum increments the value pointed to by pnum
without any problem
However, if you want to use the postfix form, you have to write (*pnum)++.


The parentheses are essential —
assuming that you want to increment the value rather than the address

This is because the operators ++ and unary * (and unary &, for that matter) 
share the same precedence level and are evaluated right to left.

The compiler would apply the ++ to pnum first, incrementing the
address, and only then dereference it to get the value.

Until now, when you’ve used scanf() to input values, you’ve used the & operator 
to obtain the address to be transferred to the function. When you have a pointer 
that already contains an address, you simply need to use the pointer
name as a parameter:

int value = 0;
int *pvalue = NULL;

pvalue = &value;
scanf(" %d", pvalue);

When you assign 0 to a pointer, it’s the equivalent of setting it to NULL, so you
 could write the following:
int *pvalue = 0;

Because NULL is the equivalent of zero, if you want to test whether the pointer 
pvalue is NULL, you can write this:

if(!pvalue)
{
...
}

When pvalue is NULL, !pvalue will be true, so the block of statement will be 
executed only if pvalue is NULL. Alternatively you can write the test as follows:

if(pvalue == NULL)
{
...
}

Pointers to Constants:

long value = 9999L;
const long *pvalue = &value;	/* Defines a pointer to a constant */

The following statement will now result in an error message from the compiler:

*pvalue = 8888L;	/* Error - attempt to change const location */

You are quite free to do what you want with value:
value = 7777L;

The value pointed to has changed but you did not use the pointer to make the 
change. Of course, the pointer itself is not constant, so you can still change 
what it points to:

long number = 8888L;
pvalue = &number;	/* OK - changing the address in pvalue */

This will change the address stored in pvalue to point to number. You still cannot
 use the pointer to change the value that is stored though. You can change the 
address stored in the pointer as much as you like but using the pointer to change
the value pointed to is not allowed.

Constant Pointers:

you might also want to ensure that the address stored in a pointer cannot be 
changed. Here’s how you could ensure that a pointer always points to the same 
thing:

int count = 43;
int *const pcount = &count;	/* Defines a constant */

The second statement declares and initializes pcount and indicates that the 
address stored must not be changed.

The compiler will therefore check that you do not inadvertently attempt to
change what the pointer points to elsewhere in your code.

so the following statements will result in an error message when you compile:

int item = 34;
pcount = &item;	/* Error - attempt to change a constant pointer */

You can still change the value that pcount points to using pcount though:

*pcount = 345;	/* OK - changes the value of count */

You can create a constant pointer that points to a value that is also constant:
int item = 25;
const int *const pitem = &item;

pitem is a constant pointer to a constant so everything is fixed. 
You cannot change the address stored in pitem and you cannot use pitem to modify 
what it points to.

Naming Pointers:
it’s quite a good idea to use names beginning with p for use as pointer names. If
you follow this method religiously, you stand a reasonable chance of knowing which
variables are pointers.

A string is just an array of elements of type char. 
If you want to input a single character with scanf(), you could use this:

char single;
scanf("%c", &single);

Here you need the address of operator for scanf() to work because scanf() needs 
the address of the location where the input data is to be stored.

However, if you’re reading in a string, you can write this:
char multiple[10];
scanf("%s", multiple);

Here you don’t use the & operator. You’re using the array name just like a 
pointer.

Always keep in mind, though, that arrays are not pointers, and there’s an 
important difference between them. You can change the address contained in a 
pointer, but you can’t change the address referenced by an array name.

char multiple[] = "My string";

#include <stdio.h>
int main(void)
{
	long multiple[] = {15L, 25L, 35L, 45L};
	long * p = multiple;
	for(int i = 0 ; i<sizeof(multiple)/sizeof(multiple[0]) ; i++)
		printf("\naddress p+%d (&multiple[%d]): %d *(p+%d) value: %d",
				i,	i,	p+i,	i,	*(p+i));
	return 0;
}

Output:
address p+0 (&multiple[0]): 0x7ffc30ba67a0 *(p+0) value: 15
address p+1 (&multiple[1]): 0x7ffc30ba67a8 *(p+1) value: 25
address p+2 (&multiple[2]): 0x7ffc30ba67b0 *(p+2) value: 35
address p+3 (&multiple[3]): 0x7ffc30ba67b8 *(p+3) value: 45

The compiler realizes that when you add 1 to an address value, what you actually 
want to do is access the next variable of that type.

On a computer that stores type long in 8 bytes, incrementing a
pointer to long by 1 will increase the address value by 8.

an array name is just a
fixed address and is not a pointer.

When you declare a one-dimensional array, placing [n1] after the array name tells the compiler that it’s an array with n1 elements. 
When you declare a two-dimensional array by placing [n2] for the second dimension 
after the [n1] for the first dimension, the compiler creates
 an array of size n1, in which each element is an array of size n2.

So when you access this two-dimensional array using the array name with a single 
index value, board[0] for example, you’re actually referencing the address of one
of the subarrays. 
Using the two-dimensional array name by itself references the address of the 
beginning of the whole array of subarrays, which is also the address of the 
beginning of the first subarray.

To summarize
board
board[0]
&board[0][0]

all have the same value, but they aren’t the same thing.

This also means that the expression board[1] results in the same address as the 
expression &board[1][0].

char board[3][3] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'}
};

printf("value of board[0][0] 	: %c\n", board[0][0]);
printf("value of *board[0] 	: %c\n", *board[0]);
printf("value of **board	: %c\n", **board);


/* List all elements of the array */
for(int i = 0; i < 9; i++)
	printf(" board: %c\n", *(*board + i));

---------------
char *pboard = *board;
/* A pointer to char */

You could have initialized pboard by using the
following:

char *pboard = &board[0][0];

This amounts to the same thing. You might think you could initialize pboard using
this statement:

pboard = board;	/* Wrong level of indirection! */

This is wrong. Strictly speaking, this isn’t legal, because
pboard and board have different levels of indirection. 
That’s a great jargon phrase that just means that pboard refers to an address that contains a value of type char, whereas board refers to an address that refers to an address containing a value of type char. 
There’s an extra level with board compared to pboard. Consequently,
pboard needs one * to get to the value and board needs two.

Some compilers will allow you to get away with this and just give you a warning 
about what you’ve done. However, it is an error, so you shouldn’t do it!

Remember that
adding an integer i to an address will add i times the length of each element.

Using Memory As You Go
----------------------

When you explicitly allocate memory at runtime in a program, space is reserved for
 you in a memory area called the heap. 
There’s another memory area called the stack in which space to store function 
arguments and local variables in a function is allocated.

When the execution of a function is finished, the space allocated to store 
arguments and local variables is freed. 
The memory in the heap is controlled by you. As you’ll see in this chapter, when 
you allocate memory on the heap, it is up to you to keep track of when the 
memory you have allocated is no longer required and free the space 
you have  allocated to allow it to be reused.

Dynamic Memory Allocation: The malloc() Function

The simplest standard library function that allocates memory at runtime is called malloc(). 
You need to include the <stdlib.h> header file in your program when you use this 
function.

When you use the malloc() function, you specify the number of bytes of memory 
that you want allocated as the argument. The function returns the address of the 
first byte of memory allocated in response to your request.

A typical example of dynamic memory allocation might be this:
int *pNumber = (int *)malloc(100);

Here, you’ve requested 100 bytes of memory and assigned the address of this memory block to pNumber. This whole block can
hold 25 int values on my computer, where they require 4 bytes each.

Notice the cast, (int *), that you use to convert the address returned by the 
function to the type “pointer to int.” You’ve done this because malloc() is a 
general-purpose function that’s used to allocate memory for any type of data. 

The function has no knowledge of what you want to use the memory for, so it 
actually returns a pointer of type “pointer to void,” which, as I indicated 
earlier, is written as void *. 
Pointers of type void * can point to any kind of data.

However, you can’t dereference a pointer of type “pointer to void” because what it
 points to is unspecified.

You could request any number of bytes, subject only to the amount of free memory 
on the computer and the limit on malloc() imposed by a particular implementation.

If the memory that
you request can’t be allocated for any reason, malloc() returns a pointer with the
 value NULL.

Remember that this is the equivalent of 0 for pointers.

It’s always a good idea to check any dynamic memory
request immediately using an if statement to make sure the memory is actually 
there before you try to use it.

if(pNumber == NULL)
{
/*Code to deal with no memory allocated */
}

you could at least display a 
message "Not enough memory" and terminate the program.

It would be very useful to allocate memory for 75 items of type int, for example.
You can do this with the following statement:

pNumber = (int *) malloc(75*sizeof(int));

As you’ve seen already, sizeof is an operator that returns an unsigned integer of
type size_t that’s the count of the number of bytes required to store its 
argument.

/* Allocate sufficient memory to store the number of primes required */
primes = (unsigned long *)malloc(total*sizeof(unsigned long));

The maximum number of primes that you can specify depends on two things: 
the memory available on your computer, and the amount of memory that your compiler’s implementation of malloc() can allocate at one time.

The argument to malloc() is of type size_t so the integer type that
corresponds to size_t will limit the number of bytes you can specify. 
If size_t corresponds to a 4-byte unsigned integer, you will be able to allocate 
up to 4,294,967,295 bytes at one time.

The value of an assignment expression is the value that’s stored in the variable 
on the left of the assignment operator. 
Thus, the value of the expression (found = (trial % *(primes+i))) will be 
the value that’s stored in found as a result of this.

Memory Allocation with the calloc() Function:

The calloc() function that is declared in the <stdlib.h> header offers a couple of
 advantages over the malloc() function.

First, it allocates memory as an array of elements of a given size, and second,
it initializes the memory that is allocated so that all bits are zero.

The calloc() function requires you to supply two argument values, 
the number of elements in the array, and the size of the array element,
both arguments being of type size_t.

The function still doesn’t know the type of the elements in the
array so the address of the area that is allocated is returned as type void *.

using calloc() to allocate memory for an array of 75 elements of type int:
int *pNumber = (int *) calloc(75, sizeof(int));

The return value will be NULL if it was not possible to allocate the memory 
requested, so you should still check for this.

This is very similar to using malloc() but the big plus is that you know the
memory area will be initialized to zero.

it is suggested that calloc is clear-alloc and malloc is mem-alloc.

Releasing Dynamically Allocated Memory
--------------------------------------

Memory that you allocate on the heap will be automatically released when your 
program ends, but it is better to explicitly release the memory when you are done
 with it, even if it’s just before you exit from the program.

In more complicated situations, you can easily have a memory leak.

A memory leak occurs when you allocate some memory dynamically and you do not 
retain the reference to it, so you are unable to release the memory. 
This often occurs within a loop, and because you do not release the memory when it
 is no longer required, your program consumes more and more of the available 
memory and eventually may occupy it all.

To release the memory for a block of dynamically allocated memory whose address 
you have stored in the pointer pNumber, you just write the statement:

free(pNumber);

The free() function has a formal parameter of type void *, and because 
any pointer type can be automatically converted to this type, 
you can pass a pointer of any type as the argument to the function.

As long as pNumber contains the address that was returned by malloc() or calloc() when the memory was allocated, the entire block of memory that was allocated will
be freed for further use.

If you pass a null pointer to the free() function the function does nothing. 
You should avoid attempting to free the same memory area twice, as the behavior of
 the free() function is undefined in this instance and therefore unpredictable.

Reallocating Memory:

The realloc() function enables you to reuse memory that you previously allocated 
using malloc() or calloc() (or realloc()).

The realloc() function expects two argument values to be supplied: 
a pointer containing an address that was previously returned by a call to malloc()
, calloc() or realloc(), and the size in bytes of the new memory that you want 
allocated.

The realloc() function releases the previously allocated memory referenced by the
 pointer that you supply as the first argument, then reallocates 
the same memory area to fulfill the new requirement specified by the second 
argument.

Obviously the value of the second argument should
not exceed the number of bytes that was previously allocated. 
If it is, you will only get a memory area allocated that is equal to the size of 
the previous memory area.

there’s quite a lot of work involved in reallocating memory
because you typically need to be sure that an existing block is large enough for 
the new requirement.
Most of the time in such situations it will be best to just free the old memory 
block explicitly and allocate a completely new block.

Here are some basic guidelines for working with memory that you allocate 
dynamically:

1) Avoid allocating lots of small amounts of memory. Allocating memory on the heap carries some overhead with it, so allocating many small blocks of memory will 
carry much more overhead than allocating fewer larger blocks.

2) Only hang on to the memory as long as you need it.
As soon as you are finished with a block
of memory on the heap, release the memory.

3) Always ensure that you provide for releasing memory that you have allocated.

4) Make sure you do not inadvertently overwrite the address of memory you have 
allocated on the heap before you have released it; otherwise your program will 
have a memory leak.


You can declare a variable of type “pointer to char” like this:

char *pString = NULL;

So far, you’ve created a pointer but not a place to store a string. To store
a string, you need to allocate some memory.

String Input with More Control:

The getchar() function that’s declared in <stdio.h> provides a much more primitive operation in that it reads only a single character at a time, but it does enable
 you to control when you stop reading characters. 

The getchar() function reads a single character from the keyboard and returns it 
as type int.

You can read a string terminated by '\n' into an array, buffer, like this:
char buffer[100];				/* String input buffer */
char *pbuffer = buffer;				/* Pointer to buffer */
while((*pbuffer++ = getchar()) != '\n');	

*pbuffer = '\0';		/* Add null terminator */

Note that this retains
the '\n' character as part of the string. If you don’t want to do this, you can 
adjust the address where you store the '\0' to overwrite the '\n'.

This doesn’t prevent the possibility of exceeding the 100 bytes available in the 
array, so you can use this safely only when you’re sure that the array is large 
enough. However, you could rewrite the
loop to check for this:

size_t index = 0;
for(; index<sizeof(buffer) ; i++)
  if((*(pbuffer+index) = getchar()) == '\n')
  {
    *(pbuffer + index++) = '\0';
    break;
  }
if( (index ==sizeof(buffer) &&( (*(pbuffer+index-1) != '\0) )
  {
    printf("\nYou ran out of space in the buffer.");
    return 1;
  }

You could create an array of pointers to store the locations of the
three strings:

char *pS[3] = { NULL };

You learned in Chapter 5 that if you supply fewer
initial values than elements in an array initializer list, the remaining elements
 will be initialized with 0. Thus just putting a list with one value, NULL, will 
initialize all the elements of an array of pointers
of any size to NULL.

The variable must be defined as const to allow you to use it as an array dimension; array dimensions can only be specified by constant expressions.

const size_t BUFFER_LEN = 512;		/* Size of input buffer	*/
char buffer[BUFFER_LEN];		/* Store for strings	*/

You can also initialize a pointer with the address of a constant string:

char *pString = "To be or not to be";

This statement allocates sufficient memory for the string, places the constant 
string in the memory allocated and, after allocating space for it, sets the value
of the pointer pS as the address of the first byte of the string.

The
problem with this is that there is nothing to prevent you from modifying 
the string with a statement such as the following:

*(pString+3) = 'm';

The const keyword doesn’t help in this case. If you declare pString as const, 
it’s the pointer that is constant, not what it points to.

let's assume we've read string in buffer, then next:
Once a string has been entered, you use the malloc() function to request 
sufficient memory to hold the string exactly:

pS[i] = (char*)malloc(pbuffer-buffer); /* Get memory for string	*/

if(pS[i] == NULL)	/* Check we actually got some...*/
{
 printf("\nOut of memory – ending program.");
 return 1;			/* ...Exit if we didn't	*/
}

/* Copy string from buffer to new memory */
strcpy(pS[i], buffer);

This uses the library function, strcpy(), to copy the contents of buffer to the
 memory pointed to by pS[i].

Take care not to confuse the arguments when using the strcpy() function; 
the second argument is the source and
the first argument is the destination for the copy operation. 
Getting them mixed up is usually disastrous, because
copying continues until a '\0' is found.

cool method to read in buffer:

pbuffer = buffer ;
/* Read a string of up to BUFFER_LEN characters */
while((pbuffer - buffer < BUFFER_LEN-1) &&
((*pbuffer++ = getchar()) != '\n'));

The function free() is complementary to malloc(), and
it releases memory previously allocated by malloc(). It only requires 
the pointer to the memory allocated as an argument. 
Although memory will be freed at the end of the program automatically, it’s good practice to free memory
as soon as you no longer need it.

another method to read strings:

printf("\nEnter successive lines, pressing Enter at the"
" end of each line.\nJust press Enter to end.\n\n");
while((*fgets(buffer, BUFFER_LEN, stdin) != '\n') && (i < NUM_P))
{
	pS[i] = (char*)malloc(strlen(buffer) + 1);
	if(pS[i]==NULL)
	/* Check for no memory allocated
	{
		printf(" Memory allocation failed. Program terminated.\n");
		return 1;
	}
	strcpy(pS[i++], buffer);
}

As soon as you collect each input line in buffer, you allocate the correct 
amount of memory to accommodate the line by using the malloc() function. 
You get the count of the number of bytes that you need by using the
strlen() function and adding 1 for the '\0' at the end.

cool bubble sorting:

while(!sorted)
{
	sorted = true;
	for(i = 0 ; i<last_string-1 ; i++)
		if(strcmp(pS[i], pS[i + 1]) > 0)
		{
		  sorted = false;	/* We were out of order */
		  pTemp= pS[i];		/* swap */
		  pS[i] = pS[i + 1];
		  pS[i + 1] = pTemp;
		}
}

free pointer:
 free(pS[i]);
 pS[i] = NULL;

don't forget to assign pointer to NULL after freeing memory

improved calculator:

the scanf() function doesn’t allow you to read a complete string
that contains spaces, as it stops at the first whitespace character.

#define BUFFER_LEN 256

char input[BUFFER_LEN];
while(strcmp(fgets(input, BUFFER_LEN, stdin), "quit\n") != 0)
{
	/* Code to implement the calculator */
}


input ends when user type "quit"

You set the input string to a length of 256. This should be enough because most
 computers keyboard buffers are 255 characters.

The fgets() function stores a newline character when you press the Enter to end 
entry of as line.

size_t input_length = 0;

input_length = strlen(input);	/* Get the input string length */
input[--input_length] = '\0';	/* Remove newline at the end */

to remove spaces:

for(to = 0, index = 0 ; index<=input_length ; index++)
	if(input[index] != ' ')		/* If it is not a space */
	  input[to++] = input[index]; 	/* Copy the character */

The variable input_length
has been declared as type size_t to be compatible with the type returned by the 
strlen() function.

to copy only digits:

you have a for loop that copies however many digits there
are to number_string:

for( ; isdigit(*(input+index)) ; index++)
	*(number_string+number_length++) = *(input+index);


