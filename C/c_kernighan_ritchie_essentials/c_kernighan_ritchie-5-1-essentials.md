#Pointers and Arrays

A pointer is a variable that contains the address of a variable.
Pointers and arrays are closely related.

The type `void *` (pointer to `void`) replaces `char *` as the proper 
type for a generic pointer.

##Pointers and Addresses

Let us begin with a simplified picture of how memory is organized. 
A typical machine has an array of consecutively numbered or addressed 
memory cells that may be manipulated individually or in contiguous 
groups.

One common situation is that any byte can be a char, a pair of one-byte
cells can be treated as a short integer, and four adjacent bytes form 
a long. A *pointer* is a group of cells (often two or four) that can 
hold an address.

The unary operator `&` gives the address of an object, so the statement

```c
p = &c;
```

assigns the address of `c` to the variable `p`, and `p` is said to 
"point to" `c`.

The `&` operator only applies to objects in memory: variables and array
elements. It cannot be applied to expressions, constants, or `register`
variables.

The unary operator `*` is the *indirection* or *dereferencing* 
operator; when applied to a pointer, it accesses the object the pointer
points to.

```c
int x = 1, y = 2, z[10];
int *ip;          /* ip is a pointer to int */

ip = &x;          /* ip now points to x */
y = *ip;          /* y is now 1 */
*ip = 0;          /* x is now 0 */
ip = &z[0];       /* ip now points to z[0] */
```

The declaration of the pointer `ip`

```c
int *ip;
```

is intended as a mnemonic; it says that the expression `*ip` is an 
`int`. This reasoning applies to function declarations as well. 
For example,

```c
double *dp, atof(char *);
```

says that in an expression `*dp` and `atof(s)` have values of double, 
and that the argument of `atof` is a pointer to char.

You should also note the implication that a pointer is constrained to 
point to a particular kind of object: every pointer points to a 
specific data type. (There is one exception: a "pointer to void" is 
used to hold any type of pointer but cannot be dereferenced itself.) 

If `ip` points to the integer `x`, then `*ip` can occur in any context 
where `x` could, so

```c
*ip = *ip + 10;
```
increments `*ip` by 10.

The unary operators `*` and `&` bind more tightly than arithmetic 
operators, so the assignment

```c
y = *ip + 1
```

takes whatever `ip` points at, adds 1, and assigns the result to `y`, 
while

```c
*ip += 1
```

increments what ip points to, as do

```c
++*ip
```

and

```c
(*ip)++
```

The parentheses are necessary in this last example; without them, 
the expression would increment `ip` instead of what it points to, 
because unary operators like `*` and `++` associate right to left.

Finally, since pointers are variables, they can be used without 
dereferencing. For example, if `iq` is another pointer to int,

```c
iq = ip
```

copies the contents of `ip` into `iq`, thus making `iq` point to 
whatever `ip` pointed to.

#Pointers and Function Arguments

Since C passes arguments to functions by value, there is no direct way
for the called function to alter a variable in the calling function.

The way to obtain the desired effect is for the calling program to 
pass *pointers* to the values to be changed:

```c
swap(&a, &b);
```

```c
void swap(int *px, int *py)  /* interchange *px and *py */
{
	int temp;

	temp = *px;
	*px = *py;
	*py = temp;
}
```

Pointer arguments enable a function to access and change objects in 
the function that called it.

#Pointers and Arrays

Any operation that can be achieved by array subscripting can also be 
done with pointers. The pointer version will in general be faster but,
at least to the uninitiated, somewhat harder to understand.

The declaration

```c
int a[10];
```

defines an array of size 10, that is, a block of 10 consecutive 
objects named `a[0]`, `a[1]`, ...,`a[9]`.

The notation `a[i]` refers to the i-th element of the array. If `pa` 
is a pointer to an integer, declared as

```c
int *pa;
```

then the assignment

```c
pa = &a[0];
```

sets `pa` to point to element zero of a; that is, `pa` contains the 
address of `a[0]`.

Now the assignment

```c
x = *pa;
```

will copy the contents of `a[0]` into `x`.

If `pa` points to a particular element of an array, then by definition
`pa+1` points to the next element, `pa+i` points `i` elements after 
`pa`, and `pa-i` points `i` elements before. Thus, if `pa` points to 
`a[0]`,

```c
*(pa+1)
```

refers to the contents of `a[1]`, `pa+i` is the address of `a[i]`, 
and `*(pa+i)` is the contents of `a[i]`.

Since the name of an array is a synonym for the location of the 
initial element, the assignment `pa=&a[0]` can also be written as

```c
pa = a;
```

Rather more surprising, at first sight, is the fact that a reference 
to `a[i]` can also be written as `*(a+i)`. In evaluating `a[i]`, 
C converts it to `*(a+i)` immediately; the two forms are equivalent.

As the other side of this coin, if `pa` is a pointer, expressions 
might use it with a subscript; `pa[i]` is identical to `*(pa+i)`.

There is one difference between an array name and a pointer that must 
be kept in mind. A pointer is a variable, so `pa=a` and `pa++` are 
legal. But an array name is not a variable; constructions like `a=pa` 
and `a++` are illegal.

When an array name is passed to a function, what is passed is the 
location of the initial element. Within the called function, this 
argument is a local variable, and so an array name parameter is a 
pointer, that is, a variable containing an address.

```c
/* strlen:  return length of string s */
int strlen(char *s)
{
	int n;

	for (n = 0; *s != '\0', s++)
		n++;
	return n;
}
```

Since `s` is a pointer, incrementing it is perfectly legal; `s++` has 
no effect on the character string in the function that called 
`strlen`, but merely increments `strlen`'s private copy of the 
pointer. 

That means that calls like

```c
strlen("hello, world");   /* string constant */
strlen(array);            /* char array[100]; */
strlen(ptr);              /* char *ptr; */
```

all work.

As formal parameters in a function definition,

```c
char s[];
```

and

```c
char *s;
```

are equivalent; we prefer the latter because it says more explicitly 
that the variable is a pointer.

It is possible to pass part of an array to a function, by passing a 
pointer to the beginning of the subarray. For example, if `a` is an 
array,

```c
f(&a[2])
```

and

```c
f(a+2)
```

both pass to the function `f` the address of the subarray that starts 
at `a[2]`.

Within `f`, the parameter declaration can read

```c
f(int arr[]) { ... }
```

or

```c
f(int *arr) { ... }
```

If one is sure that the elements exist, it is also possible to index 
backwards in an array; `p[-1]`, `p[-2]`, and so on are syntactically 
legal, and refer to the elements that immediately precede `p[0]`. 
Of course, it is illegal to refer to objects that are not within the 
array bounds.

#Address Arithmetic

If `p` is a pointer to some element of an array, then `p++` increments
`p` to point to the next element, and `p+=i` increments it to point 
`i` elements beyond where it currently does. These and similar 
constructions are the simples forms of pointer or address arithmetic.

#Character Pointers and Functions

There is an important difference between these definitions:

```
char amessage[] = "now is the time"; /* an array */
char *pmessage = "now is the time"; /* a pointer */
```

`amessage` is an array, just big enough to hold the sequence of 
characters and '\0' that initializes it. Individual characters within 
the array may be changed but `amessage` will always refer to the same 
storage. On the other hand, `pmessage` is a pointer, initialized to 
point to a string constant; the pointer may subsequently be modified 
to point elsewhere, but the result is undefined if you try to modify 
the string contents.

