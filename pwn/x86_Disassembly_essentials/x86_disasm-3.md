#The Stack

All x86 architectures use a **stack** as a temporary storage area in 
RAM that allows the processor to quickly store and retrieve data in 
memory. The current top of the stack is pointed to by the `esp` 
register.

The stack "grows" downward, from high to low memory addresses, so 
values recently pushed onto the stack are located in memory addresses 
**above** the `esp` pointer. No register specifically points to the 
bottom of the stack, although most operating systems monitor the stack 
bounds to detect both "underflow" (popping an empty stack) and 
"overflow" (pushing too much information on the stack) conditions.

When a value is popped off the stack, the value remains sitting in 
memory until overwritten. However, you should never rely on the content
of memory addresses below `esp`, because other functions may overwrite 
these values without your knowledge.

##Push and Pop

The following lines of ASM code are basically equivalent:

```asm
push eax
```

is equal

```asm
sub esp, 4
mov DWORD PTR SS:[esp], eax
```

```asm
pop eax
```

is equal

```asm
mov eax, DWORD PTR SS:[esp]
add esp, 4
```

but the single command actually performs much faster than the 
alternative.

##ESP In Action

Let's say we want to quickly discard 3 items we pushed earlier onto the
stack, without saving the values (in other words "clean" the stack). 
The following works:

```asm
pop eax
pop eax
pop eax
```

However there is a faster method. We can simply perform some basic 
arithmetic on `esp` to make the pointer go "above" the data items, so 
they cannot be read anymore, and can be overwritten with the next round
of `push` commands.

```asm
add esp, 12  ; 12 is 3 DWORDs (4 bytes * 3)
```

Likewise, if we want to reserve room on the stack for an item bigger 
than a DWORD, we can use a subtraction to artificially move `esp` 
forward. We can then access our reserved memory directly as a memory 
pointer, or we can access it indirectly as an offset value from `esp` 
itself.

Say we wanted to create an array of byte values on the stack, 100 items
long. We want to store the pointer to the base of this array in `edi`. 
How do we do it? Here is an example:

```asm
sub esp, 100  ; num of bytes in our array
mov edi, esp  ; copy address of 100 bytes area to edi
```

To destroy that array, we simply write the instruction

```asm
add esp, 100
```

##Reading Without Popping

To read values on the stack without popping them off the stack, `esp` 
can be used with an offset. For instance, to read the 3 DWORD values 
from the top of the stack into `eax` (but without using a `pop` 
instruction), we would use the instructions:

```asm
mov eax, DWORD PTR SS:[esp]
mov eax, DWORD PTR SS:[esp + 4]
mov eax, DWORD PTR SS:[esp + 8]
```

Remember, since `esp` moves **downward** as the stack grows, data on 
the stack can be accessed with a *positive* offset. A negative offset 
should never be used because data "above" the stack cannot be counted.

The operation of reading from the stack without popping is often 
referred to as "peeking".

##Data Allocation

There are **two** areas in the computer memory where a program can 
store data. The first, the one that we have been talking about, is the
**stack**. It is a linear LIFO buffer that allows fast allocations and
deallocations, but has a limited size. 

The **heap** is typically a non-linear data storage area, typically 
implemented using linked lists, binary trees, or other more exotic 
methods. Heaps are slightly more difficult to interface with and to 
maintain than a stack, and allocations/deallocations are performed 
more slowly. However, heaps can grow as the data grows, and new heaps can be allocated when data quantities become too large.

As we shall see, explicitly declared variables are allocated on the 
stack. Stack variables are finite in number, and have a definite size.
Heap variables can be variable in number and in size.

#Functions and Stack Frames

Functions are frequently set up with a **"stack frame"** to allow 
access to both function parameters, and automatic function variables.
The idea behind a stack frame is that each subroutine can act 
independently of its location on the stack, and each subroutine can 
act as if it is the top of the stack.

When a function is called, a new stack frame is created at the current`esp` location. A stack frame acts like a partition on the stack. All 
items from previous functions are higher up on the stack, and should 
not be modified. 

##Standard Entry Sequence

For many compilers, the standard function entry sequence is the 
following piece of code (`X` is the total size, in bytes, of all 
*automatic* variables used in the function):

```asm
push ebp
mov ebp, esp
sub esp, X
```

For example, here is a C function code fragment and the resulting 
assembly instructions:

```c
void MyFunction()
{
  int a, b, c;
    ...
```

```asm
_MyFunction:
  push ebp     ; save the value of ebp
  mov ebp, esp ; ebp now points to the top of the stack
  sub esp, 12  ; space allocated on the stack for the local variables
```

This means local variables can be accessed by referencing `ebp`. 
Consider the following C code fragment and corresponding assembly 
code:

```c
a = 10;
b = 5;
c = 2;
```

```asm
mov [ebp -  4], 10  ; location of variable a
mov [ebp -  8], 5   ; location of b
mov [ebp - 12], 2   ; location of c
```

This all seems well and good, but what is the purpose of `ebp` in this
setup? Why save the old value of `ebp` and then point `ebp` to the top
of the stack, only to change the value of `esp` with the next 
instruction? The answer is *function parameters*.

Consider the following C function declaration:

```c
void MyFunction2(int x, int y, int z)
{
  ...
}
```

It produces the following assembly code:

```asm
_MyFunction2:
  push ebp 
  mov ebp, esp
  sub esp, 0     ; no local variables, most compilers will omit this line
```

Which is exactly as one would expect. So, what exactly does `ebp` do, 
and where are the function parameters stored? The answer is found when
we call the function.

Consider the following C function call:

```c
MyFunction2(10, 5, 2);
```

This will create the following assembly code (using a Right-to-Left 
calling convention called CDECL, explained later):

```asm
push 2
push 5
push 10
call _MyFunction2
```

**Note:** Remember that the **call** x86 instruction is basically 
equivalent to

```asm
push eip + 2 ; return address is current address + size of two instructions
jmp _MyFunction2
```

It turns out that the function arguments are all passed on the stack! 
Therefore, when we move the current value of the stack pointer (`esp`)
into `ebp`, we are pointing `ebp` directly at the function arguments.

As the function code pushes and pops values, `ebp` is not affected by 
`esp`. Remember that pushing basically does this:

```asm
sub esp, 4   ; "allocate" space for the new stack item
mov [esp], X ; put new stack item value X in
```

This means that first the return address and then the old value of 
`ebp` are put on the stack. Therefore `[ebp]` points to the location 
of the old value of `ebp`, `[ebp + 4]` points to the return address, 
and `[ebp + 8]` points to the first function argument.

Here is a (crude) representation of the stack at this point:

```
:    : 
|  2 | [ebp + 16] (3rd function argument)
|  5 | [ebp + 12] (2nd argument)
| 10 | [ebp + 8]  (1st argument)
| RA | [ebp + 4]  (return address)
| FP | [ebp]      (old ebp value)
|    | [ebp - 4]  (1st local variable)
:    :
:    :
|    | [ebp - X]  (esp - the current stack pointer. The use of push / pop is valid now)
```

The stack pointer value may change during the execution of the current
function. In particular this happens when:

  * parameters are passed to another function;
  * the pseudo-function `alloca()` is used.

The value of `esp` cannot be reliably used to determine (using the 
appropriate offset) the memory location of a specific local variable. 
To solve this problem, many compilers access local variables using 
negative offsets from the `ebp` registers. This allows us to assume 
that the same offset is always used to access the same variable (or 
parameter). For this reason, the `ebp` register is called the **frame 
pointer**, or FP.

##Standard Exit Sequence

The Standard Exit Sequence must undo the things that the Standard 
Entry Sequence does. To this effect, the Standard Exit Sequence must 
perform the following tasks, in the following order:

  1. Remove space for local variables, by reverting `esp` to its old 
  value.

  2. Restore the old value of `ebp` to its old value, which is on top 
  of the stack.

  3. Return to the calling function with a `ret` command.

As an example, the following C code:

```c
void MyFunction3(int x, int y, int z)
{
	int a, int b, int c;
	...
	return;
}
```

Will create the following assembly code:

```asm
_MyFunction3:
  push ebp
  mov ebp, esp
  sub esp, 12 ; sizeof(a) + sizeof(b) + sizeof(c)
  ;x = [ebp + 8], y = [ebp + 12], z = [ebp + 16]
  ;a = [ebp - 4] = [esp + 8], b = [ebp - 8] = [esp + 4], c = [ebp - 12] = [esp]
  mov esp, ebp
  pop ebp
  ret 12 ; sizeof(x) + sizeof(y) + sizeof(z)
```

#Non-Standard Stack Frames

Frequently, reversers will come across a subroutine that doesn't set 
up a standard stack frame. Here are some things to consider when 
looking at a subroutine that does not start with a standard sequence:

##Using Uninitialized Registers

When a subroutine starts using data in an **uninitialized** register, 
that means that the subroutine expects external functions to put data 
into that register before it gets called. Some calling conventions 
pass arguments in registers, but sometimes a compiler will not use a 
standard calling convention.

##"static" Functions

In C, functions may optionally be declared with the `static` keyword, 
as such:

```c
static void MyFunction4();
```

The `static` keyword causes a function to have only local scope, 
meaning it may not be accessed by any external functions (it is 
strictly internal to the given code file). When an optimizing compiler
sees a static function that is only referenced by calls (no references
through function pointers), it "knows" that external functions cannot 
possibly interface with the static function (the compiler controls all
access to the function)

##Hot Patch Prologue

Some Windows functions set up a regular stack frame as explained 
above, but start out with the apparently non-sensical line

```asm
mov edi, edi;
```

This instruction is assembled into 2 bytes which serve as a 
**placeholder** for future function patches. Taken as a whole such a 
function might look like this:

```asm
nop               ; each nop is 1 byte long
nop
nop
nop
nop

FUNCTION:         ; <-- This is the function entry point as used by call instructions 
mov edi, edi      ; mov edi,edi is 2 bytes long
push ebp          ; regular stack frame setup
mov ebp, esp
```

If such a function needs to be replaced without reloading the 
application (or restarting the machine in case of kernel patches) it 
can be achieved by inserting a jump to the replacement function. A 
short jump instruction (which can jump +/- 127 bytes) requires 2 bytes
of storage space - just the amount that the `mov edi,edi` placeholder 
provides. A jump to any memory location, in this case to our 
replacement function, requires 5 bytes. These are provided by the 5 
no-operation bytes just preceding the function.

If a function thus patched gets called it will first jump back by 5 
bytes and then do a long jump to the replacement function. After the 
patch the memory might look like this

```asm
LABEL:
jmp REPLACEMENT_FUNCTION ; <-- 5 NOPs replaced by jmp

FUNCTION:
jmp short LABEL          ; <-- mov edi has been replaced by short jump backwards
push ebp          
mov ebp, esp             ; <-- regular stack frame setup as before
```

The reason for using a 2-byte `mov` instruction at the beginning 
instead of putting 5 nops there directly, is to **prevent corruption**
during the patching process. There would be a risk with replacing 5 
individual instructions if the instruction pointer is currently 
pointing at any one of them. Using a single `mov` instruction as 
placeholder on the other hand guarantees that the patching can be 
completed as an atomic transaction.

##Local Static Variables

Local static variables cannot be created on the stack, since the value
of the variable is preserved across function calls. We'll discuss 
local static variables and other types of variables in a later 
chapter.

#Functions and Stack Frame Examples

##Example: Number of Parameters

Given the following disassembled function (in MASM syntax), how many 
4-byte parameters does this function receive? How many variables are 
created on the stack? What does this function do?

```asm
_Question1:
  push ebp
  mov ebp, esp
  sub esp, 4
  mov eax, [ebp + 8]
  mov ecx, 2
  mul ecx
  mov [esp + 0], eax
  mov eax, [ebp + 12]
  mov edx, [esp + 0]
  add eax, edx
  mov esp, ebp
  pop ebp
  ret
```

The function above takes 2 4-byte parameters, accessed by offsets +8 
and +12 from `ebp`. The function also has 1 variable created on the 
stack, accessed by offset +0 from `esp`. The function is nearly 
identical to this C code:

```c
int Question1(int x, int y)
{
	int z;
	z = x * 2;
	return y + z;
}
```

##Example: Standard Entry Sequences

Does the following function follow the Standard Entry and Exit 
Sequences? if not, where does it differ?

```asm
_Question2:
  call _SubQuestion2
  mov ecx, 2
  mul ecx
  ret
```

The function does not follow the standard entry sequence, because it 
doesn't set up a proper stack frame with `ebp` and `esp`. The function
basically performs the following C instructions:

```c
int Question2()
{
	return SubQuestion2() * 2;
}
```

#Calling Conventions

**Calling conventions** are a standardized method for functions to be 
implemented and called by the machine. A calling convention specifies 
the method that a compiler sets up to access a subroutine.

In theory, code from any compiler can be interfaced together, so long 
as the functions all have the **same calling conventions**. In 
practice however, this is not always the case.

Calling conventions specify how **arguments** are **passed** to a function, how **return values** are **passed back** out of a function, how
the function is called, and how the function manages the stack and its
stack frame.

In short, the calling convention specifies how a function call in C or
C++ is converted into assembly language.

Needless to say, there are many ways for this translation to occur, 
which is why it's so important to specify certain standard methods. If
these standard conventions did not exist, it would be nearly 
impossible for programs created using different compilers to 
communicate and interact with one another.

There are three major calling conventions that are used with the C 
language: STDCALL, CDECL, and FASTCALL. In addition, there is another 
calling convention typically used with C++: THISCALL.

#Standard C Calling Conventions

The C language, by default, uses the CDECL calling convention, but 
most compilers allow the programmer to specify another convention via 
a specifier keyword. These keywords **are not** part of the ISO-ANSI C
standard.

If a calling convention other than CDECL is to be used, or if CDECL is
not the default for your compiler, and you want to manually use it, 
you must specify the calling convention keyword in the function 
declaration itself, and in any prototypes for the function. This is 
important because both the calling function and the called function 
need to know the calling convention.

##CDECL

In the CDECL calling convention the following holds:

  * Arguments are passed on the stack in **Right-to-Left** order, and 
  return values are passed in `eax`.

  * The **calling** function cleans the stack. This allows CDECL 
  functions to have *variable-length argument lists* (aka variadic 
  functions). For this reason the number of arguments is not appended 
  to the name of the function by the compiler, and the assembler and 
  the linker are therefore unable to determine if an incorrect number 
  of arguments is used.

Variadic functions usually have special entry code, generated by the 
`va_start()`, `va_arg()` C pseudo-functions.

Consider the following C instructions:

```c
_cdecl int MyFunction1(int a, int b)
{
	return a + b;
}
```

and the following function call:

```c
x = MyFunction1(2, 3);
```

These would produce the following assembly listings, respectively:

```asm
_MyFunction1:
push ebp
mov ebp, esp
mov eax, [ebp + 8]
mov edx, [ebp + 12]
add eax, edx
pop ebp
ret
```

and

```asm
push 3
push 2
call _MyFunction1
add esp, 8
```

When translated to assembly code, CDECL functions are almost always 
prepended with an underscore (that's why all previous examples have 
used `"_"` in the assembly code).

##STDCALL

STDCALL, also known as "WINAPI" (and a few other names, depending on 
where you are reading it) is used almost exclusively by Microsoft as 
the standard calling convention for the Win32 API. Since STDCALL is 
strictly defined by Microsoft, all compilers that implement it do it 
the same way.

  * STDCALL passes arguments right-to-left, and returns the value in 
  `eax`. (The Microsoft documentation erroneously claimed that 
  arguments are passed left-to-right, but this is not the case.)

  * The called function cleans the stack, unlike CDECL. This means 
  that STDCALL doesn't allow variable-length argument lists.

Consider the following C function:

```c
_stdcall int MyFunction2(int a, int b)
{
	return a + b;
}
```

and the calling instruction:

```c
x = MyFunction2(2, 3);
```

These will produce the following respective assembly code fragments:

```asm
:_MyFunction2@8
push ebp
mov ebp, esp
mov eax, [ebp + 8]
mov edx, [ebp + 12]
add eax, edx
pop ebp
ret 8
```

and

```asm
push 3
push 2
call _MyFunction2@8
```

There are a few important points to note here:

  1. In the function body, the ret instruction has an (optional) 
  argument that indicates **how many** bytes to pop off the stack when
  the function returns.

  2. STDCALL functions are name-decorated with a leading underscore, 
  followed by an `@`, and then the number (in bytes) of arguments 
  passed on the stack. This number will always be a multiple of 4, 
  on a 32-bit aligned machine.

##FASTCALL

The FASTCALL calling convention is not completely standard across all 
compilers, so it should be used with caution. In FASTCALL, the first 2
or 3 32-bit (or smaller) arguments are passed in registers, with the 
most commonly used registers being `edx`, `eax`, and `ecx`.

Additional arguments, or arguments larger than 4-bytes are passed on 
the stack, often in Right-to-Left order (similar to CDECL). The 
calling function most frequently is responsible for cleaning 
the stack, if needed.

Because of the ambiguities, it is recommended that FASTCALL be used 
only in situations with 1, 2, or 3 32-bit arguments, where speed is 
essential.

The following C function:

```c
_fastcall int MyFunction3(int a, int b)
{
	return a + b;
}
```

and the following C function call:

```c
x = MyFunction3(2, 3);
```

Will produce the following assembly code fragments for the called, and
the calling functions, respectively:

```asm
:@MyFunction3@8
push ebp
mov ebp, esp ;many compilers create a stack frame even if it isn't used
add eax, edx ;a is in eax, b is in edx
pop ebp
ret
```

and

```asm
;the calling function
mov eax, 2
mov edx, 3
call @MyFunction3@8
```

The name decoration for FASTCALL prepends an `@` to the function name,
and follows the function name with `@x`, where `x` is the number (in 
bytes) of arguments passed to the function.

Many compilers still produce a stack frame for FASTCALL functions, 
especially in situations where the FASTCALL function itself calls 
another subroutine. However, if a FASTCALL function doesn't need a 
stack frame, optimizing compilers are free to omit it.

##C++ Calling Convention

C++ requires that non-static methods of a class be called by an 
instance of the class. Therefore it uses its own standard calling 
convention to ensure that pointers to the object are passed to the 
function: **THISCALL**.

##THISCALL

In THISCALL, the pointer to the class object is passed in `ecx`, the 
arguments are passed Right-to-Left on the stack, and the return value 
is passed in `eax`.

For instance, the following C++ instruction:

```cpp
MyObj.MyMethod(a, b, c);
```

Would form the following asm code:

```asm
mov ecx, MyObj
push c
push b
push a
call _MyMethod
```

At least, it *would* look like the assembly code above if it weren't 
for **name mangling**.

###Name Mangling

Because of the complexities inherent in function overloading, C++ 
functions are heavily name-decorated to the point that people often 
refer to the process as "Name Mangling".

Unfortunately C++ compilers are free to do the name-mangling 
differently since the standard does not enforce a convention. 
Additionally, other issues such as exception handling are also not 
standardized.

Since every compiler does the name-mangling differently, this book 
will not spend too much time discussing the specifics of the 
algorithm. Notice that in many cases, it's possible to determine which
compiler created the executable by examining the specifics of the 
name-mangling format.

Here are a few general remarks about THISCALL name-mangled functions:

  * They are recognizable on sight because of their complexity when 
  compared to CDECL, FASTCALL, and STDCALL function name decorations

  * They sometimes include the name of that function's class.

  * They almost always include the number and type of the arguments, 
  so that overloaded functions can be differentiated by the arguments 
  passed to it.

Here is an example of a C++ class and function declaration:

```cpp
class MyClass {
	MyFunction(int a);
}
	 
MyClass::MyFunction(2) { }
```

And here is the resultant mangled name:

```
?MyFunction@MyClass@@QAEHH@Z
```

###Extern "C"

In a C++ source file, functions placed in an `extern "C"` block are 
guaranteed not to be mangled. This is done frequently when libraries 
are written in C++, and the functions need to be exported without 
being mangled. Even though the program is written in C++ and compiled 
with a C++ compiler, some of the functions might therefore not be 
mangled and will use one of the ordinary C calling conventions 
(typically CDECL).

##Note on Name Decorations

When disassembling raw machine code, there will be no function names 
and no name decorations to examine. For this reason, you will need to 
pay more attention to the way parameters are passed, the way the stack
is cleaned, and other similar details.

While we haven't covered optimizations yet, suffice it to say that 
optimizing compilers can even make a mess out of these details. 
Functions which are not exported do not necessarily need to maintain 
standard interfaces, and if it is determined that a particular 
function does not need to follow a standard convention, some of the 
details will be optimized away. In these cases, it can be difficult to
determine what calling conventions were used (if any), and it is even 
difficult to determine where a function begins and ends.

##GNU C Compiler

We will be using 2 example C functions to demonstrate how GCC 
implements calling conventions:

```c
int MyFunction1(int x, int y)
{
	return (x * 2) + (y * 3);
}
```

and

```c
int MyFunction2(int x, int y, int z, int a, int b, int c)
{
	return x * y * (z + 1) * (a + 2) * (b + 3) * (c + 4);
}
```

GCC does not have commandline arguments to force the default calling 
convention to change from CDECL (for C), so they will be manually 
defined in the text with the directives: `__cdecl`, `__fastcall`, and 
`__stdcall`.

###CDECL

The first function (MyFunction1) provides the following assembly 
listing:

```asm
_MyFunction1:
pushl	%ebp
movl	%esp, %ebp
movl	8(%ebp), %eax
leal	(%eax,%eax), %ecx
movl	12(%ebp), %edx
movl	%edx, %eax
addl	%eax, %eax
addl	%edx, %eax
leal	(%eax,%ecx), %eax
popl	%ebp
ret
```

We can see that the `ret` instruction doesn't have an argument, so the
calling function is cleaning the stack. Since GCC doesn't 
provide us with the variable names in the listing, we have to deduce 
which parameters are which.

After the stack frame is set up, the first instruction of the function
is `movl 8(%ebp), %eax`. One we remember (or learn for the first time)
that GAS instructions have the general form:

```
instruction src, dest
```

We realize that the value at offset +8 from `ebp` (the last parameter 
pushed on the stack) is moved into `eax`. The form 
`leal(reg1,reg2), dest` adds the values in the parenthesis together, 
and stores the value in *dest*. 

Translated into Intel syntax, we get the instruction:

```asm
lea ecx, [eax + eax]
```

Which is clearly the same as a multiplication by 2.

##Example: C Calling Conventions

Identify the calling convention of the following C function:

```c
int MyFunction(int a, int b)
{
	return a + b;
}
```

The function is written in C, and has no other specifiers, so it is 
CDECL by default.

##Example: Named Assembly Function

Identify the calling convention of the function **MyFunction**:

```asm
:_MyFunction@12
push ebp
mov ebp, esp
...
pop ebp
ret 12
```

The function includes the decorated name of an STDCALL function, and 
cleans up its own stack. It is therefore an STDCALL function.

##Example: Unnamed Assembly Function

This code snippet is the entire body of an unnamed assembly function. 
Identify the calling convention of this function.

```asm
push ebp
mov ebp, esp
add eax, edx
pop ebp
ret
```

The function sets up a stack frame, so we know the compiler hasnt done
anything "funny" to it. It accesses registers which **arent** 
initialized yet, in the `edx` and `eax` registers. It is therefore a 
FASTCALL function.

##Example: Another Unnamed Assembly Function

```asm
push ebp 
mov ebp, esp
mov eax, [ebp + 8]
pop ebp
ret 16
```

The function has a standard stack frame, and the `ret` instruction has
a parameter to clean its own stack. Also, it accesses a parameter from
the stack. It is therefore an STDCALL function.

##Example: Name Mangling

What can we tell about the following function call?

```asm
mov ecx, x
push eax
mov eax, ss:[ebp - 4]
push eax
mov al, ss:[ebp - 3]
call @__Load?$Container__XXXY_?Fcii
```

Two things should get our attention immediately. The first is that 
before the function call, a value is stored into `ecx`. Also, the 
function name itself is heavily mangled. This example must use the C++
THISCALL convention. Inside the mangled name of the function, we can 
pick out two english words, "Load" and "Container". Without knowing 
the specifics of this name mangling scheme, it is not possible to 
determine which word is the function name, and which word is the class
name.

We can pick out two 32-bit variables being passed to the function, and
a single 8-bit variable. The first is located in `eax`, the second is 
originally located on the stack from offset -4 from `ebp`, and the 
third is located at `ebp` offset -3. In C++, these would likely 
correspond to two int variables, and a single char variable. Notice at
the end of the mangled function name are three lower-case characters 
"cii". We can't know for certain, but it appears these three letters 
correspond to the three parameters (char, int, int). We do not know 
from this whether the function returns a value or not, so we will 
assume the function returns `void`.

Assuming that "Load" is the function name and "Container" is the class
name (it could just as easily be the other way around), here is our 
function definition:

```cpp
class Container
{
	void Load(char, int, int);
}
```


