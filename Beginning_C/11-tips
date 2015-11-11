The keyword struct enables you to define a collection of variables of various 
types called a structure that you can treat as a single unit.

struct horse
{
int age;
int height;
} Silver;

This example declares a structure type called horse.
horse -- structure tag, or a tag name

In the example, an instance of the structure, called Silver, is declared at the 
same time that the structure is defined.

Silver is a variable of type horse.

Any type of variable can appear as a member of a structure, including arrays.

You can always find out the amount of memory that’s occupied by a structure using
the sizeof operator.

struct horse
{
  int age;
  int height;
  char name[20];
  char father[20];
  char mother[20];
};

struct horse Dobbin = {
			24, 17,"Dobbin", "Trigger", "Flossie"
		      };

struct horse Piebald, Bandy;

A structure variable name is not a pointer. You need a special syntax to access 
the members.
Dobbin.age = 12;

The period between the structure variable name and the member name is actually an operator that is called the member selection operator.

scanf("%s", My_first_horse.name);
scanf("%d", &My_first_horse.height);

printf("\n%s is %d years old, %d hands high,",
	My_first_horse.name, My_first_horse.age, My_first_horse.height);
printf(" and has %s and %s as parents.\n", My_first_horse.father,
					My_first_horse.mother );

Unnamed Structures
You don’t have to give a structure a tag name. When you declare a structure and 
any instances of that structure in a single statement, you can omit the tag name.

struct
{
int age;
int height;
char name[20];
char father[20];
char mother[20];
} My_first_horse;

A serious disadvantage with this approach is that you can no longer define further
 instances of the structure in another statement.

All the variables of this structure type that you want in your
program must be defined in the one statement.

struct horse My_horses[50];	/* Structure array declaration */

printf("\nEnter the name of the horse: ");
scanf("%s", My_horses[hcount].name);

If you want to reference the third element of the name array for the fourth 
structure element:
My_horses[3].name[2]

My_horses[1].height = (My_horses[2].height + My_horses[3].height)/2;

You can also use a complete structure element in an assignment statement:
My_horses[1] = My_horses[2];

This statement causes all the members of the structure My_horses[2] to be copied 
to the structure My_horses[1]

The only other operation that’s possible with a whole structure is to take its 
address using the & operator.

You can’t add, compare, or perform any other operations with a complete structure.
 To do those kinds of things, you must write your own functions.

Pointers to Structures:
struct horse *phorse;

phorse = &My_horses[1];

printf("\nThe name is %s.", (*phorse).name);

The parentheses around the dereferenced pointer are essential, because the 
precedence of the member selection operator (the period) is higher than that of 
the pointer-dereferencing operator *.

However, there’s another way of doing this, and it’s much more readable and 
intuitive:
printf("\nThe name is %s.", phorse->name );

The operator is sometimes called the "pointer to member" operator for obvious 
reasons.

Dynamic Memory Allocation for Structures:
To create dynamically allocated memory for structures, the only tool 
that’s missing is an array of pointers to structures:

struct horse *phorse[50];

This statement declares an array of 50 pointers to structures of type horse.
Only memory for the pointers has been allocated by this statement.

/* allocate memory to hold a structure */
phorse[hcount] = (struct horse*) malloc(sizeof(struct horse));

free(phorse[i]);

Variables other than type char are often stored beginning at an address that’s a 
multiple of 2 for 2-byte variables,
a multiple of 4 for 4-byte variables, and so on.

This is called boundary alignment and it has nothing to do with C but is a 
hardware requirement where it applies.

Arranging variables to be stored in memory like this makes the transfer
of data between the processor and memory faster.
This arrangement can result in unused bytes occurring between
member variables of different types, though, depending on their sequence.


You can also make a structure a member of a structure.
Furthermore, not only can pointers be members of a structure, but also a pointer 
to a structure can be a member of a structure.

You can specify a suitable structure with the tag name Date with this statement:
struct Date
{
  int day;
  int month;
  int year;
};

Now you can define the structure horse, including a date-of-birth variable, 
like this:

struct horse
{
  struct Date dob;
  int height;
  char name[20];
  char father[20];
  char mother[20];
};

struct horse Dobbin;
Dobbin.height = 14;
Dobbin.dob.day = 5;
Dobbin.dob.month = 12;
Dobbin.dob.year = 1962;

If you can find a good reason to do it, you can extend the notion of structures 
that are members of a structure to a structure that’s a member of a structure 
that’s a member of a structure.
Your C compiler is likely to provide for at least 15 levels of such convolution.

struct horse
{
  struct Date
  {
    int day;
    int month;
    int year;
  } dob;
  int height;
  char name[20];
  char father[20];
  char mother[20];
};

This has an interesting effect. Because the declaration is enclosed within the 
scope of the horse structure definition, it doesn’t exist outside it, and so it 
becomes impossible to declare a Date variable external to the horse structure.

Pointers to Structures As Structure Members:

struct horse
{
  int age;
  int height;
  char name[20];
  char father[20];
  char mother[20];
  struct horse *next;
};

Linked lists are invaluable in applications in which you need to process an 
unknown number of structures
The main advantages of a linked list relate to memory usage and ease of handling.

Bit-Fields in a Structure
Bit-fields provide a mechanism that allows you to define variables that are each 
one or more binary bits within a single integer word

Bit-fields are used most frequently when memory is at a premium and you’re in a 
situation in which you must use it as sparingly as possible.
Bit-fields will slow your program down appreciably compared to using standard 
variable types.

struct
{
unsigned int flag1 : 1;
unsigned int flag2 : 1;
unsigned int flag3 : 2;
unsigned int flag4 : 3;
} indicators;

This defines a variable with the name indicators that’s an instance of an 
anonymous structure containing four bit-fields with the names flag1 through flag4.

The first two bit-fields, being a single bit specified by the 1 in their 
definition, can only assume the values 0 or 1.
The third bit-field, flag3, has 2 bits and so it can have a value from 0 to 3.
The last bit-field, flag4, can have values from 0 to 7, because it has 3 bits.

These bit-fields are referenced in the same manner as other structure members, 
for example
indicators.flag4 = 5;
indicators.flag3 = indicators.flag1 = 1;

Structures As Arguments to Functions
It’s exactly the same as passing any other variable.

struct family
{
  char name[20];
  int age;
  char father[20];
  char mother[20];
};

bool siblings(struct family member1, struct family member2)
{
  if(strcmp(member1.mother, member2.mother) == 0)
    return true;
  else
    return false;
}

This function has two arguments, each of which is a structure.

Pointers to Structures As Function Arguments
Remember that a copy of the value of an argument is transferred to a function 
when it’s called.
If the argument is a large structure, it can take quite a bit of time, as well as
occupying whatever additional memory the copy of the structure requires.

So you should use a pointer to a structure as an argument.
This avoids the memory consumption and the copying time, because now only a copy 
of the pointer is made.

So we could rewrite siblings() function like this:

bool siblings(struct family *member1, struct family *member2)
{
  if(strcmp(member1->mother, member2->mother) == 0)
    return true;
  else
    return false;
}

if you don’t need to modify the values pointed to by a pointer
argument (you just want to access and use them, for instance), there’s a 
technique for getting a degree of protection, even though you’re passing pointers
to a function.

So...

bool siblings(struct family const *pmember1, struct family const *pmember2)
{
if(strcmp(pmember1->mother, pmember2->mother) == 0)
return true;
else
return false;
}

Any attempt to change those structures will cause an error message 
during compilation.

bool siblings(struct family *const pmember1, struct family *const pmember2)
{
  if(strcmp(pmember1->mother, pmember2->mother) == 0)
    return true;
  else
    return false;
}

The parameters here are “constant pointers to structures of type family,” 
not “pointers to constant structures.”

A Structure As a Function Return Value
There’s nothing unusual about returning a structure from a function either.

The function prototype, for example:
struct horse my_fun(void);

This is a prototype for a function taking no arguments that returns a structure 
of type horse.

Although you can return a structure from a function like this, it’s often more 
convenient to return a pointer to a structure.

struct Family *get_person(void); /* Prototype for input function */

struct Date
{
  int day;
  int month;
  int year;
};

struct Family
{
  struct Date dob;
  char name[20];
  char father[20];
  char mother[20];
  struct Family *next;
  struct Family *previous;
};

the memory allocated by malloc() is more permanent. It remains until you free it 
yourself within the program somewhere, or until you exit from the program 
completely.

Within the function get_person(), you declare this pointer:
struct Family *temp;	/* Define temporary structure pointer */
temp = (struct Family*) malloc(sizeof(struct Family));

The last statement in the function get_person() is the following:
return temp;	/* Return address of Family structure */

This returns a copy of the pointer to the structure that it has created.

Even though temp will no longer exist after the return, 
the address that it contains that points to the memory block 
obtained from malloc() is still valid.

Binary Trees
Each node typically contains an item of data, plus pointers to two subsidiary 
nodes, a left node and a right node.

If either subsidiary node does not exist, the corresponding pointer is NULL.

struct Node *createnode(long value)
{
  /* Allocate memory for a new node */
  struct Node *pNode = (struct Node *)malloc(sizeof(struct Node));
  pNode->item = value;
  pNode->count = 1;
  pNode->pLeft = pNode->pRight = NULL;
  return pNode;
}

/* List the node values in ascending sequence */
void listnodes(struct Node *pNode)
{
  if(pNode->pLeft != NULL)
    listnodes(pNode->pLeft);
  
  for(int i = 0; i<pNode->count ; i++)
    printf("\n%10ld", pNode->item);

  if(pNode->pRight != NULL)
    listnodes(pNode->pRight);

}

All you have to do to output the values in ascending sequence is to call 
listnodes() with the root node pointer as the argument, like this:

listnodes(pRoot);	/* Output the contents of the tree */


Unions
The facility in C that allows the same memory area to be shared by a number of 
different variables is called a union.

union u_example
{
  float decval;
  int *pnum;
  double my_value;
} U1;

This statement declares a union with the tag name u_example, which shares memory 
between a float value decval, a pointer to an integer pnum, and a double variable
my_value.

union u_example U2, U3;

U1.decval = 2.5;
U2.decval = 3.5 * U1.decval;

union u_example *pU;

you can modify members of the union, via the pointer:
pU = &U2;
U1.decval = pU->decval;

struct my_structure
{
  int num1;
  float num2;
  union
  {
    int *pnum;
    float *pfnum;
  } my_U
} samples[5];

This is often described as an anonymous union.


One technique that is often adopted is to embed a union in a struct that also
has a member that specifies what type of value is currently stored in the union.

/* Type code for data in union */
#define TYPE_LONG 1
#define TYPE_FLOAT 2
#define TYPE_CHAR 3

struct Item
{
  int u_type;
  union
  {
    long integer;
    float floating;
    char ch;
  } u;
} var;

You could set a value for var like this:
var.u.floating = 2.5f;
var.u_type = TYPE_FLOAT;


Suppose you have a structure for geometric points with three coordinates, x, y, 
and z, that you define with the following statement:

struct pts
{
  int x;
  int y;
  int z;
};


You can now define an alternative name for declaring such structures:

typedef struct pts Point;

This statement specifies that the name Point is a synonym for struct pts.

You could combine the typedef and the structure declaration as follows:

typedef struct pts
{
  int x;
  int y;
  int z;
} Point;

you can use a statements such as this:
Point start_pt;
Point end_pt;

Simplifying Code Using typedef

typedef struct pts *pPoint;
Now, when you want to declare some pointers, you can just write this:

pPoint pfirst;
pPoint plast;

int(*pfun)(int, int);	/* Function pointer declaration */

typedef int (*function_pointer)(int, int); /* Function pointer type */

This doesn’t declare a variable of type “pointer to a function.”
so you can replace the original declaration of pfun with this statement:

function_pointer pfun;


