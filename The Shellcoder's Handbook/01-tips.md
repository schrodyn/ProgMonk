If
you confuse code and data, you’re in a world of trouble. So, this book is about
code and data, and what happens when the two become confused.

Before you begin to understand the concepts, you must be able to speak the
language. You will need to know a few definitions, or terms:

Vulnerability (n.): A flaw in a system’s security that can lead to an
attacker utilizing the system in a manner other than the designer
intended.
Also known as a security hole or security bug.

Exploit (v.): To take advantage of a vulnerability so that the target
system reacts in a manner other than which the designer intended.

Exploit (n.): The tool, set of instructions, or code that is used to take
advantage of a vulnerability. Also known as a Proof of Concept (POC).

0day (n.): An exploit for a vulnerability that has not been publicly disclosed. 
Sometimes used to refer to the vulnerability itself.

Fuzzer (n.): A tool or application that attempts all, or a wide range of,
unexpected input values to a system. The purpose of a fuzzer is to
determine whether a bug exists in the system, which could later be
exploited without having to fully know the target system’s internal
functioning.

INSTRUCTIONS AND DATA
A modern computer makes no real distinction between instructions and data.
If a processor can be fed instructions when it should be seeing data, it will 
happily go about executing the passed instructions. 
This characteristic makes system exploitation possible.

This book teaches you how to insert instructions when
the system designer expected data.

You will also use the concept of overflowing 
to overwrite the designer’s instructions with your own. 
The goal is to gain control of execution.

Memory Management

When a program is executed, it is laid out in an organized manner — various
elements of the program are mapped into memory. 
First, the operating system creates an address space in which the program 
will run. This address space includes the actual program instructions as well as 
any required data.

Next, information is loaded from the program’s executable file to the newly
created address space.
There are three types of segments: .text , .bss , and .data .

The .text segment is mapped as read-only, whereas .data and .bss are writable.
The .bss and .data segments are reserved for global variables.

The .data segment contains static initialized data, and the .bss segment contains
uninitialized data.

The final segment, .text , holds the program instructions.

Finally, the stack and the heap are initialized.
The stack stores local variables, information relating to function calls, 
and other information used to clean up the stack after a function
or procedure is called.

Another important feature of the stack is that it grows down the address
space: as more data is added to the stack, it is added at increasingly lower
address values.

The heap is another data structure used to hold program information, more
specifically, dynamic variables.
The heap is (roughly) a First In First Out (FIFO)
data structure. Data is placed and removed from the heap as it builds.

The heap grows up the address space: As data is added to the heap, it is added at
an increasingly higher address value

Memory Space diagram:
Shared Libraries
.text
.bss
Heap
Stack
env pointer
Argc

From a high level, registers can be grouped into four categories:
General purpose
Segment
Control
Other

General-purpose registers are used to perform a range of common mathemat-
ical operations. They include registers such as EAX , EBX , and ECX for the IA32,
and can be used to store data and addresses, offset addresses, perform counting
functions, and many other things.

A general-purpose register to take note of is the extended stack pointer register
( ESP ) or simply the stack pointer. ESP points to the memory address where the
next stack operation will take place.

The next class of register of interest is the segment register. Unlike the other
registers on an IA32 processor, the segment registers are 16 bit (other registers
are 32 bits in size). Segment registers, such as CS , DS , and SS , are used to
keep track of segments and to allow backward compatibility with 16-bit
applications.

Control registers are used to control the function of the processor. The most
important of these registers for the IA32 is the Extended Instruction Pointer 
( EIP ) or simply the Instruction Pointer. EIP contains the address of the next 
machine instruction to be executed.

The registers in the other category are simply registers that do not fit neatly 
into the first three categories. One of these registers is the
Extended Flags ( EFLAGS ) register, which comprises many single-bit registers
that are used to store the results of various tests performed by the processor.

Let’s take some common C and C++ code constructs and see what they look
like in assembly.

int number;
number++;

translates into:
number dw 0
mov eax,number
inc eax
mov number,eax

We use the Define Word ( DW ) instruction to define a value for our integer,
number .

Look at a simple if statement in C++:

int number;
if (number<0)
{
. . .more code . . .
}

in assembly:

number dw 0
mov eax,number
or eax,eax
jge label
<no>
label :<yes>

we jump
to label if number is greater than or equal to zero with Jump if Greater than or
Equal to ( JGE ).

using an array:
int array[4];
array[2]=9;

in assembly:
array dw 0,0,0,0
mov ebx,2
mov array[ebx],9


