An array is a fixed number of data items that are all of the same type.

The data items in an array are referred to as elements.

long numbers[10];

The number between square brackets defines how many elements you want to store in your array and is called the array dimension.

Each individual value in the array is identified by what is called an index value.

The index values for
the elements in the preceding numbers array would run from 0 to 9.

You can think of the index value for an array
element as the offset from the first element. The first element is the first 
element, so it has an offset of 0.

You can use a simple integer to explicitly reference the element that you want to access. 
Alternatively, you can use an integer expression that’s evaluated during the 
execution of the program.

int numbers[10];

for(int i = 0; i < count; i++)
{
  printf("%2d> ",i+1);
  scanf("%d", &numbers[i]);
  sum += numbers[i];
}

By using %2d as the format
specifier, you ensure that each value is output in a two-character field, so the 
numbers are aligned.

A Reminder About Memory:

To identify each byte so that its contents may be accessed, the byte is labeled 
with a number starting from 0 for the first byte in memory and going up to 
whatever number of bytes there are in memory. This label for a byte is called its
 address.

You’ve already been using the address of operator, &, extensively with the scanf() function.

Just using the variable name by itself as an argument
to a function makes the value stored in the variable available to the function. 

Prefixing the variable name with the address of operator and using that as the 
argument to the function makes the address of the variable available to the 
function.

Each variable that you use in a program takes up a certain amount of memory, 
measured in bytes, and the exact amount of memory is dependent on the type of the
variable.

long a = 1L;
long b = 2L;
long c = 3L;

printf("A variable of type long occupies %d bytes.", sizeof(long));
printf("\nHere are the addresses of some variables of type long:");
printf("\nThe address of a is: %p The address of b is: %p", &a, &b);

The address of operator is the & that precedes the name of each variable. 
You also used a new format specifier, %p, to output the address of the variables. This format specifier is for outputting a memory address, and the value is
presented in hexadecimal format. 

A memory address is typically 16, 32, or 64 bits, and the size of the address will
determine the maximum amount of memory that can be referenced.

output:
A variable of type long occupies 8 bytes.
Here are the addresses of some variables of type long:
The address of a is: 0x7ffdeeb2c5b0 The address of b is: 0x7ffdeeb2c5b8
The address of c is: 0x7ffdeeb2c5c0

Caution:
 If the addresses for the variables are separated by greater amounts than the size
 value, it is most likely because you compiled the program as a debug version. 
In debug mode your compiler may allocate extra
space to store additional information about the variable that will be used when 
you’re executing the program in debug mode.

long number[4];

When you declare an array, you give the compiler all the information it needs to 
allocate the memory for the array. 

You tell it the type of value, which will determine the number of bytes that
each element will require, and how many elements there will be.

The array name identifies where in memory the array begins.

The address of an array element is going to be the address
where the array starts, plus the index value for the element multiplied by the 
number of bytes
required to store each element of the type stored in the array.

To output the address of the third element of an array called number, you could 
write the following:
printf("\n%p", &number[2]);

The address of type (void *)

for example, you can try:
int number[4];
number[2] = 13;

printf("\nThe address of number[2] is: %p\n", (void *)number[2]);

output:
The address of number[2] is: 0xd


Initializing an Array:
----------------------

To initialize the elements of an array, you just specify the list of initial 
values between braces and separated by commas in the declaration. For example

double values[5] = { 1.5, 2.5, 3.5, 4.5, 5.5 };

To initialize the whole array, there should be one value for each element. 
If there are fewer initializing values than elements, the elements 
without initializing values will be set to 0.

you are not obliged to supply the size of the array when you specify a
list of initial values. The compiler can deduce the number of elements from the 
list of values:

int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

Here the size of the array is determined by the number of initial values in the 
list so the primes array will have ten elements.

The sizeof operator works with arrays too. You can declare an array with the 
following statement:

double values[5] = { 1.5, 2.5, 3.5, 4.5, 5.5 };

Now you can output the number of bytes that the array occupies with the following statement:

printf("\nThe size of the array, values, is %d bytes.", sizeof values);

output:
The size of the array, values, is 40 bytes.

You can therefore use the sizeof operator
to calculate the number of elements in an array:

int element_count = sizeof values/sizeof values[0];

Multidimensional Arrays

float carrots[25][50];

float numbers[3][5];

You can visualize this array as having three rows and five columns.
They’re actually stored in memory sequentially by row

int numbers[3][4] = {
			{ 10, 20, 30, 40 },	/* Values for first row */
			{ 15, 25, 35, 45 },	/* Values for first row */
			{ 47, 48, 49, 50 }	/* Values for third row */
		};


The remaining elements in a row that are
left when the initial values have all been assigned will be initialized to 0.

For arrays of three or more dimensions, the process is extended. 
A three-dimensional array, for example, will have three levels of nested braces, 
with the inner level containing sets of initializing
values for a row:

int numbers[2][3][4] = {
			{

				{ 10, 20, 30, 40 },
				{ 15, 25, 35, 45 },
				{ 47, 48, 49, 50 }
			},
			{
				{ 10, 20, 30, 40 },
				{ 15, 25, 35, 45 },
				{ 47, 48, 49, 50 }
			}
		};

so you have three levels
of nested braces for a three-dimensional array.

This is true generally; for instance, a six-dimensional
array will have six levels of nested braces

float cranium;
...
your_head = (int)(8.0*cranium);

Note that you need the cast to type int here to avoid a warning message from
the compiler. 
The code will still work if you omit the cast, but the compiler must then insert the cast to type int.

Because this cast potentially loses information, the compiler will issue 
a warning.

The parentheses around the
expression (8.0*cranium) are also necessary; without them, you would only cast 
the value 8.0 to type int, not the whole expression.


Tic-Tac Toe

/* The main game loop. The game continues for up to 9
/* As long as there is no winner
for(int i = 0; i<9 && winner==0; i++)
{
	...
}









