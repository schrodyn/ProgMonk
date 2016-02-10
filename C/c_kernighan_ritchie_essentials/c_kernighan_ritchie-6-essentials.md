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

#Typedef

C provides a facility called `typedef` for creating new data type 
names. For example, the declaration

```c
typedef int Length;
```

makes the name `Length` a synonym for `int`. The type `Length` can be 
used in declarations, casts, etc., in exactly the same ways that the 
`int` type can be:

```c
Length len, maxlen;
Length *lengths[];
```

Similarly, the declaration

```c
typedef char *String;
```

makes `String` a synonym for `char *` or character pointer, which may 
then be used in declarations and casts:

```c
String p, lineptr[MAXLINES], alloc(int);
int strcmp(String, String);
p = (String) malloc(100);
```

Notice that the type being declared in a `typedef` appears in the 
position of a variable name, not right after the word `typedef`.

As a more complicated example, we could make `typedef`s for the tree 
nodes shown earlier in this chapter:

```c
typedef struct tnode *Treeptr;

typedef struct tnode { /* the tree node: */
	char *word;           /* points to the text */
	int count;            /* number of occurrences */
	struct tnode *left;   /* left child */
	struct tnode *right;  /* right child */
} Treenode;
```

This creates two new type keywords called `Treenode` (a structure) and
`Treeptr` (a pointer to the structure). Then the routine `talloc` 
could become

```c
Treeptr talloc(void)
{
	return (Treeptr) malloc(sizeof(Treenode));
}
```

It must be emphasized that a `typedef` declaration does not create a 
new type in any sense; it merely adds a new name for some existing 
type. Nor are there any new semantics: variables declared this way 
have exactly the same properties as variables whose declarations are 
spelled out explicitly. 

In effect, `typedef` is like `#define`, except that since it is 
interpreted by the compiler, it can cope with textual substitutions 
that are beyond the capabilities of the preprocessor.

For example,

```c
typedef int (*PFI)(char *, char *);
```

creates the type `PFI`, for "pointer to function (of two `char *` 
arguments) returning `int`," which can be used in contexts like

```c
PFI strcmp, numcmp;
```

#Unions

A *union* is a variable that may hold (at different times) objects of 
different types and sizes, with the compiler keeping track of size and
alignment requirements. Unions provide a way to manipulate different 
kinds of data in a single area of storage, without embedding any 
machine-dependent information in the program.

As an example such as might be found in a compiler symbol table 
manager, suppose that a constant may be an `int`, a `float`, or a 
character pointer. The value of a particular constant must be stored 
in a variable of the proper type, yet it is most convenient for table 
management if the value occupies the same amount of storage and is 
stored in the same place regardless of its type. This is the purpose 
of a union - a single variable that can legitimately hold any of one 
of several types. The syntax is based on structures:

```c
union u_tag {
	int ival;
	float fval;
	char *sval;
} u;
```

The variable `u` will be large enough to hold the largest of the three
types; the specific size is implementation-dependent.

Any of these types may be assigned to `u` and then used in expressions
, so long as the usage is consistent. It is the programmer's 
responsibility to keep track of which type is currently stored in a 
union; the results are implementation-dependent if something is stored
as one type and extracted as another.

Syntactically, members of a union are accessed as

```c
union-name.member
```

or

```c
union-pointer->member
```

just as for structures. If the variable `utype` is used to keep track 
of the current type stored in `u`, then one might see code such as

```c
if (utype == INT)
	printf("%d\n", u.ival);
if (utype == FLOAT)
	printf("%f\n", u.fval);
if (utype == STRING)
	printf("%s\n", u.sval);
else
	printf("bad type %d in utype\n", utype);
```

Unions may occur within structures and arrays, and vice versa. The 
notation for accessing a member of a union in a structure (or vice 
versa) is identical to that for nested structures. For example, in the
structure array defined by

```c
struct {
	char *name;
	int flags;
	int utype;
	union {
		int ival;
		float fval;
		char *sval;
	} u;
} symtab[NSYM];
```

the member `ival` is referred to as

```c
symtab[i].u.ival
```

and the first character of the string `sval` by either of

```c
*symtab[i].u.sval
symtab[i].u.sval[0]
```

The same operations are permitted on unions as on structures: 
assignment to or copying as a unit, taking the address, and accessing 
a member.

A union may only be initialized with a value of the type of its 
**first** member; thus union `u` described above can only be 
initialized with an integer value.

