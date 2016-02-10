#Structures

Structure is a collection of one or more variables, possibly of 
different types, grouped together under a single name for convenient 
handling. Structures help to organize complicated data, particularly in
large programs, because they permit a group of related variables to be 
treated as a unit instead of as separate entities.

One traditional example of a structure is the payroll record: an 
employee is described by a set of attributes such as name, address, 
social security number, salary, etc. Some of these in turn could be 
structures: a name has several components, as does an address and even 
a salary. Another example, more typical for C, comes from graphics: a 
point is a pair of coordinate, a rectangle is a pair of points, and so 
on.

#Basics of Structures

Let us create a few structures suitable for graphics. The basic object 
is a point, which we will assume has an x coordinate and a y coordinate
, both integers.

The two components can be placed in a structure declared like this:

```c
struct point {
	int x;
	int y;
};
```

The keyword `struct` introduces a structure declaration, which is a 
list of declarations enclosed in braces. An optional name called a 
*structure tag* may follow the word `struct` (as with `point` here).

The variables named in a structure are called *members*. A structure 
member or tag and an ordinary (i.e., non-member) variable can have the 
same name without conflict, since they can always be distinguished by 
context.

A `struct` declaration defines a type. The right brace that terminates 
the list of members may be followed by a list of variables, just as for
any basic type. That is,

```c
struct { ... } x, y, z;
```

is syntactically analogous to

```c
int x, y, z;
```

in the sense that each statement declares x, y and z to be variables of
the named type.

A structure declaration that is not followed by a list of variables 
reserves no storage; it merely describes a template or shape of a 
structure. If the declaration is tagged, however, the tag can be used 
later in definitions of instances of the structure.

For example, given the declaration of `point` above,

```c
struct point pt;
```

defines a variable `pt` which is a structure of type `struct point`.

A member of a particular structure is referred to in an expression by a
construction of the form

```
structure-name.member
```

The structure member operator "." connects the structure name and the 
member name. To print the coordinates of the point `pt`, for instance,

```c
printf("%d,%d", pt.x, pt.y);
```

Structures can be nested. One representation of a rectangle is a pair 
of points that denote the diagonally opposite corners:

```c
struct rect {
	struct point pt1;
	struct point pt2;
};
```

The `rect` structure contains two point structures. If we declare 
`screen` as

```c
struct rect screen;
```

then

```c
screen.pt1.x
```

refers to the x coordinate of the `pt1` member of screen.

#Structures and Functions

The only legal operations on a structure are copying it or assigning 
to it as a unit, taking its address with `&`, and accessing its 
members.

Copy and assignment include passing arguments to functions and 
returning values from functions as well. Structures may not be 
compared.

Let us investigate structures by writing some functions to manipulate 
points and rectangles. There are at least three possible approaches: 
pass components separately, pass an entire structure, or pass a 
pointer to it. Each has its good points and bad points.

The first function, `makepoint`, will take two integers and return a 
point structure:

```c
/* makepoint:  make a point from x and y components */
struct point makepoint(int x, int y)
{
	struct point temp;

	temp.x = x;
	temp.y = y;
	return temp;
}
```

The next step is a set of functions to do arithmetic on points. 
For instance,

```c
/* addpoints:  add two points */
struct point addpoint(struct point p1, struct point p2)
{
	p1.x += p2.x;
	p1.y += p2.y;
	return p1;
}
```

Here both the arguments and the return value are structures. Structure
parameters are passed by value like any others.

If a large structure is to be passed to a function, it is generally 
more efficient to pass a pointer than to copy the whole structure. 
Structure pointers are just like pointers to ordinary variables. 
The declaration

```c
struct point *pp;
```

says that `pp` is a pointer to a structure of type `struct point`. If 
`pp` points to a point structure, `*pp` is the structure, and 
`(*pp).x` and `(*pp).y` are the members. for example,

```c
struct point origin, *pp;

pp = &origin;
printf("origin is (%d,%d)\n", (*pp).x, (*pp).y);
```

The parentheses are necessary in `(*pp).x` because the precedence of 
the structure member operator `.` is higher then `*`. The expression 
`*pp.x` means `*(pp.x)`, which is illegal here because `x` is not a 
pointer.

Pointers to structures are so frequently used that an alternative 
notation is provided as a shorthand. If `p` is a pointer to a 
structure, then

```c
p->member-of-structure
```

refers to the particular member. So we could write instead

```c
printf("origin is (%d,%d)\n", pp->x, pp->y);
```

The structure operators `.` and `->`, together with `()` for function 
calls and `[]` for subscripts, are at the top of the precedence 
hierarchy. For example, given the declaration

```c
struct {
	int len;
	char *str;
} *p;
```

then

```c
++p->len
```

increments `len`, not `p`, because the implied parenthesization is 
`++(p->len)`. Parentheses can be used to alter binding: `(++p)->len` 
increments `p` before accessing `len`, and `(p++)->len` increments `p`
afterward.

In the same way, `*p->str` fetches whatever `str` points to; 
`*p->str++` increments `str` after accessing whatever it points to 
(just like `*s++`); `(*p->str)++` increments whatever `str` points to;
and `*p++->str` increments `p` after accessing whatever `str` points 
to.

#Arrays of Structures

The structure declaration

```c
struct key {
	char *word;
	int count;
} keytab[NKEYS];
```

declares a structure type `key`, defines an array `keytab` of 
structures of this type, and sets aside storage for them. Each element
of the array is a structure.

This could also be written

```c
struct key {
	char *word;
	int count;
};

struct key keytab[NKEYS];
```

The structure initialization is analogous to earlier ones - 
the definition is followed by a list of initializers enclosed in 
braces:

```c
struct key {
	char *word;
	int count;
} keytab[] = {
	"auto", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"continue", 0,
	"default", 0,
	/* ... */
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0
};
```

The initializers are listed in pairs corresponding to the structure 
members. It would be more precise to enclose the initializers for each
"row" or structure in braces, as in

```c
{ "auto", 0 },
{ "break", 0 },
{ "case", 0 },
...
```

As usual, the number of entries in the array `keytab` will be computed
if the initializers are present and the `[]` is left empty.

The size of the array is the size of one entry times the number of 
entries, so the number of entries is just

```
size of keytab /size of struct key
```

C provides a compile-time unary operator called `sizeof` that can be 
used to compute the size of any object. The expressions

```c
sizeof object
```

and

```c
sizeof (type name)
```

yield an integer equal to the size of the specified object or type in 
bytes. (Strictly, sizeof produces an unsigned integer value whose type
, `size_t`, is defined in the header `<stddef.h>`.)

An object can be a variable or array or structure. A type name can be 
the name of a basic type like `int` or `double`, or a derived type 
like a structure or a pointer.

This computation is used in a `#define` statement to set the value of 
`NKEYS`:

```c
#define NKEYS (sizeof keytab / sizeof(struct key))
```

Another way to write this is to divide the array size by the size of a
specific element:

```c
#define NKEYS (sizeof keytab / sizeof(keytab[0]))
```

This has the advantage that it does not need to be changed if the type
changes.

A `sizeof` can not be used in a `#if` line, because the preprocessor 
does not parse type names. But the expression in the `#define` is not 
evaluated by the preprocessor, so the code here is legal.

When a function returns a complicated type like a structure pointer, 
as in

```c
struct key *binsearch(char *word, struct key *tab, int n)
```

the function name can be hard to see, and to find with a text editor. 
Accordingly an alternate style is sometimes used:

```c
struct key *
binsearch(char *word, struct key *tab, int n)
```

This is a matter of personal taste; pick the form you like and hold 
to it.

The question of the type declaration for a function like `malloc` is a
vexing one for any language that takes its type-checking seriously. In
C, the proper method is to declare that `malloc` returns a pointer to 
`void`, then explicitly coerce the pointer into the desired type with 
a cast. `malloc` and related routines are declared in the standard 
header `<stdlib.h>`. Thus `talloc` can be written as

```c
#include <stdlib.h>

/* talloc:  make a tnode */
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}
```

`strdup` merely copies the string given by its argument into a safe 
place, obtained by a call on `malloc`:

```c
char *strdup(char *s)   /* make a duplicate of s */
{
	char *p;

	p = (char *) malloc(strlen(s)+1); /* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}
```

`malloc` returns NULL if no space is available; `strdup` passes that 
value on, leaving error-handling to its caller.

Storage obtained by calling `malloc` may be freed for re-use by 
calling `free`.


