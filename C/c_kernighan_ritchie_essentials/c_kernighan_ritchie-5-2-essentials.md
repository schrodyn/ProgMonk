#Initialization of Pointer Arrays

Consider the problem of writing a function `month_name(n)`, which 
returns a pointer to a character string containing the name of the 
n-th month. This is an ideal application for an internal static array. 
`month_name` contains a private array of character strings, and returns
a pointer to the proper one when called. This section shows how that 
array of names is initialized.

```c
/* month_name:  return name of n-th month */
char *month_name(int n)
{
	static char *name[] = {
		"Illegal month",
		"January", "February", "March",
		"April", "May", "June",
		"July", "August", "September",
		"October", "November", "December"
	};

	return (n < 1 || n > 12) ? name[0] : name[n];
}
```

The initializer is a list of character strings; each is assigned to 
the corresponding position in the array. The characters of the i-th 
string are placed somewhere, and a pointer to them is stored in 
`name[i]`. Since the size of the array name is not specified, the 
compiler counts the initializers and fills in the correct number.

#Pointers vs. Multi-dimensional Arrays

Newcomers to C are sometimes confused about the difference between a 
two-dimensional array and an array of pointers, such as name in the 
example above. Given the definitions

```c
int a[10][20];
int *b[10];
```

then `a[3][4]` and `b[3][4]` are both syntactically legal references to
a single `int`. But `a` is a true two-dimensional array: 200 int-sized 
locations have been set aside, and the conventional rectangular 
subscript calculation 20 * *row* +*col* is used to find the element 
`a[row,col]`.

For `b`, however, the definition only allocates 10 pointers and does 
not initialize them; initialization must be done explicitly, either 
statically or with code. Assuming that each element of `b` does point 
to a twenty-element array, then there will be 200 `int`s set aside, 
plus ten cells for the pointers. The important advantage of the pointer
array is that the rows of the array may be of different lengths.

Although we have phrased this discussion in terms of integers, by far 
the most frequent use of arrays of pointers is to store character 
strings of diverse lengths, as in the function `month_name`.
Compare the declaration for an array of pointers:
```c
char *name[] = { "Illegal month", "Jan", "Feb", "Mar" };
```

with those for a two-dimensional array:

```c
char aname[][15] = { "Illegal month", "Jan", "Feb", "Mar" };
```

#Command-line Arguments

In environments that support C, there is a way to pass command-line 
arguments or parameters to a program when it begins executing. When 
`main` is called, it is called with two arguments.

The first (conventionally called `argc`, for argument count) is the 
number of command-line arguments the program was invoked with; the 
second (`argv`, for argument vector) is a pointer to an array of 
character strings that contain the arguments, one per string.

By convention, `argv[0]` is the name by which the program was invoked, 
so `argc` is at least 1. If `argc` is 1, there are no command-line 
arguments after the program name.

additionally, the standard requires that `argv[argc]` be a null 
pointer.

#Pointers to Functions

In C, a function itself is not a variable, but it is possible to 
define pointers to functions, which can be assigned, placed in arrays,
passed to functions, returned by functions, and so on.

The declarations should be studied with some care.

```c
int (*comp)(void *, void *)
```

which says that `comp` is a pointer to a function that has two 
`void *` arguments and returns an `int`.

The use of `comp` is consistent with the declaration: `comp` is a 
pointer to a function, `*comp` is the function, and

```c
(*comp)(v[i], v[left])
```

is the call to it. The parentheses are needed so the components are 
correctly associated; without them,

```c
int *comp(void *, void *)    /* WRONG */
```

says that `comp` is a function returning a pointer to an `int`, which 
is very different.


