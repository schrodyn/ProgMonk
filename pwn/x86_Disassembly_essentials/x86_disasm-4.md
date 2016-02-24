#Branching

Computer science professors tell their students to avoid jumps and 
goto instructions, to avoid the proverbial "spaghetti code."
Unfortunately, assembly only has jump instructions to control program 
flow.

##If-Then

Let's consider a generic `if` statement in pseudo-code followed by its 
equivalent form using jumps:

```
if (condition) then
	do_action;
```

```
if not (condition) then goto end;
	do_action;
end:
```

What does this code do? In English, the code checks the condition and 
performs a jump only if it is `false`. With that in mind, let's compare
some actual C code and its Assembly translation:

```c
if(x == 0)
{
	x = 1;
}
x++;
```

```asm
mov eax, $x
cmp eax, 0
jne end
mov eax, 1
end:
inc eax
mov $x, eax
```

Note that when we translate to assembly, we need to *negate* the 
condition of the jump because--like we said above--we only jump if the 
condition is false.

Negating a comparison may be tricky if you're not paying attention. 
Here are the correct dual forms:

Instruction	| Meaning
------------|---------
JNE			| **J**ump if **n**ot **e**qual
JE			| **J**ump if **e**qual
JG			| **J**ump if **g**reater
JLE			| **J**ump if **l**ess than or **e**qual
JL			| **J**ump if **l**ess than
JGE			| **J**ump if **g**reater or **e**qual

And here are some examples.

```asm
mov eax, $x                     //move x into eax
cmp eax, $y                     //compare eax with y
jg end                          //jump if greater than
inc eax
move $x, eax                    //increment x
end:
...
```

Is produced by these C statements:

```c
if(x <= y)
{
	x++;
}
```

As you can see, `x` is incremented only if it is **less than or equal 
to** `y`. Thus, if it is greater than `y`, it will not be incremented 
as in the assembler code. Similarly, the C code

```c
if(x < y)
{
	x++;
}
```

produces this assembler code:

```asm
mov eax, $x                        //move x into eax
cmp eax, $y                        //compare eax with y
jge end                            //jump if greater than or equal to
inc eax
move $x, eax                       //increment x
end:
...
```

X is incremented in the C code only if it is **less than** y, so the 
assembler code now jumps if it's greater than or equal to y. This kind
of thing takes practice, so we will try to include lots of examples in
this section.

##If-Then-Else

Let us now look at a more complicated case: the **If-Then-Else** 
instruction.

```
if (condition) then
  do_action
  else
    do_alternative_action;
```

```
if not (condition) goto else;
	do_action;
	goto end;
else:
	do_alternative_action;
end:
```

Now, what happens here? Like before, the if statement only jumps to 
the `else` clause when the condition is false. However, we must also 
install an *unconditional* jump at the end of the "then" clause, so we
don't perform the else clause directly afterwards.

Now, here is an example of a real C If-Then-Else:

```c
if(x == 10)
{
	x = 0;
}
else
{
	x++;
}
```

Which gets translated into the following assembly code:

```asm
mov eax, $x
cmp eax, 0x0A ;0x0A = 10
jne else
mov eax, 0
jmp end
else:
inc eax
end:
mov $x, eax
```

##Switch-Case

**Switch-Case** structures can be very complicated when viewed in 
assembly language, so we will examine a few examples. 

Lets say we have a general `switch` statement, but with an extra label
at the end, as such:

```c
switch (x)
{
	//body of switch statement
}
end_of_switch:
```

Now, every `break` statement will be immediately replaced with the 
statement

```asm
jmp end_of_switch
```

But what do the rest of the statements get changed to? The `case` 
statements can each resolve to any number of arbitrary integer values.
How do we test for that? The answer is that we use a "Switch Table". 
Calculating specific label and jumping to it. Better explanation in 
Linux for C programmers wikibook.

#Variables

We've already seen some mechanisms to create local storage on the 
stack. This chapter will talk about some other variables, including 
**global variables**, **static variables**, variables labled 
**"const"**, **"register"**, and **"volatile"**.

##How to Spot a Variable

Variables come in 2 distinct flavors: those that are created **on the 
stack** (local variables), and those that are accessed via a 
**hardcoded memory** address (global variables). Any memory that is 
accessed via a hard-coded address is usually a *global* variable. 
Variables that are accessed as an offset from `esp`, or `ebp` are 
frequently *local* variables.

###Hardcoded address

Anything hardcoded is a value that is stored as-is in the binary, and 
is not changed at runtime. For instance, the value `0x2054` is 
hardcoded, whereas the current value of variable `X` is not hard-coded
and may change at runtime.

Example of a hardcoded address:

```asm
mov eax, [0x77651010]
```

OR

```asm
mov ecx, 0x77651010
mov eax, [ecx]
```

Example of a non-hardcoded (softcoded?) address:

```asm
mov ecx, [esp + 4]
add ecx, ebx
mov eax, [ecx]
```

In the last example, the value of `ecx` is calculated at run-time, 
whereas in the first 2 examples, the value is the same every time.
RVAs (Relative Virtual Address) are considered hard-coded addresses, 
even though the loader needs to "fix them up" to point to the correct 
locations.

##.BSS and .DATA sections

Both `.bss` and `.data` sections contain values which can change at 
run-time (e.g. variables). Typically, variables that are initialized 
to a non-zero value in the source are allocated in the `.data` section
(e.g. `int a = 10;`). Variables that are not initialized, or 
initialized with a zero value, can be allocated to the `.bss` section 
(e.g. `int arr[100];`).

Because all values of `.bss` variables are guaranteed to be zero at 
the start of the program, there is no need for the linker to allocate 
space in the binary file. Therefore, `.bss` sections do not take space
in the binary file, regardless of their size.

##Signed and Unsigned Variables

Integer formatted variables, such as `int`, `char`, `short` and `long`
may be declared `signed` or `unsigned` variables in the C source code.
There are two differences in how these variables are treated:

  1. Signed variables use signed instructions such as `add`, and 
  `sub`. Unsigned variables use unsigned arithmetic instructions such 
  as `addi`, and `subi`.

  2. Signed variables use signed branch instructions such as `jge` and
  `jl`. Unsigned variables use unsigned branch instructions such as 
  `jae`, and `jb`.

The difference between signed and unsigned instructions is the 
conditions under which the various flags for greater-than or less-than
(**overflow flags**) are set. The integer result values are exactly 
the same for both signed and unsigned data.

##Floating-Point Values

Floating point values tend to be 32-bit data values (for `float`) or 
64-bit data values (for `double`). These values are distinguished from
ordinary integer-valued variables because they are used with 
floating-point instructions.  

Floating point instructions typically start with the letter `f`. For 
instance, `fadd`, `fcmp`, and similar instructions are used with 
floating point values.

Of particular note are the `fload` instruction and variants. These 
instructions take an integer-valued variable and converts it into a 
floating point variable.

##Global Variables

Global variables do not have a limited scope like lexical variables do
inside a function body. Since the notion of lexical scope implies the 
use of the system stack, and since global variables are not lexical in
nature, they are typically not found on the stack.

Global variables tend to exist in the program as a **hard-coded 
memory** address, a location which never changes throughout program 
execution. These could exist in the DATA segment of the executable, or
anywhere else that a hard-coded memory address can be used to store 
data.

###"static" Variables

The C programming language specifies a special keyword `static` to 
define variables which are lexical to the function (they cannot be 
referenced from outside the function) but they maintain their values 
across function calls. 
Unlike ordinary lexical variables which are created on the stack when 
the function is entered and are destroyed from the stack when the 
function returns, static variables are **created once** and are 
**never destroyed**.

```c
int MyFunction(void) 
{
	static int x;
	...
}
```

Static variables in C are global variables, except the compiler takes 
precautions to prevent the variable from being accessed outside of the
parent function's scope. Like global variables, static variables are 
referenced using a hardcoded memory address, not a location on the 
stack like ordinary variables. However unlike globals, static 
variables are only used inside a single function.

There is no difference between a global variable which is only used in
a single function, and a static variable inside that same function. 
However, it's good programming practice to limit the number of global 
variables, so when disassembling, you should prefer interpreting these
variables as static instead of global.

###"extern" Variables

The `extern` keyword is used by a C compiler to indicate that a 
particular variable is global to the entire project, not just to a 
single source code file. Besides this distinction, and the slightly 
larger lexical scope of extern variables, they should be treated like 
ordinary global variables.

##Constants

Variables qualified with the **const** keyword (in C) are frequently 
stored in the `.data` section of the executable. Constant values can 
be distinguished because they are initialized at the beginning of the 
program, and are never modified by the program itself.

##"Volatile" memory

In C and C++, variables can be declared "volatile", which tells the 
compiler that the memory location can be accessed from *external* or 
*concurrent* processes, and that the compiler should not perform any 
optimizations on the variable.

For instance, if multiple threads were all accessing and modifying a 
single global value, it **would be bad** for the compiler to store 
that variable in a register sometimes, and flush it to memory 
infrequently. In general, Volatile memory **must be flushed** to 
memory after **every** calculation, to ensure that the most current 
version of the data is in memory when other processes come to look for
it.
