#x86 Disassembly Essentials

##Using C and Assembly Language


Original book you can read [here](https://en.m.wikibooks.org/wiki/X86_Disassembly/Print_Version)

[here image]

This book is about the disassembly of x86 machine code into 
human-readable assembly, and the decompilation of x86 assembly code 
into human-readable C or C++ source code. Some topics covered will be 
common to all computer architectures, not just x86-compatible machines.

This book is for readers at the undergraduate level with experience 
programming in x86 Assembly and C or C++. This book is not designed to 
teach assembly language programming, C or C++ programming, or compiler/
assembler theory.

The reader should have a thorough understanding of x86 Assembly, 
C Programming, and possibly C++ Programming. This book is intended to 
increase the reader's understanding of the relationship between x86 
machine code, x86 Assembly Language, and the C Programming Language.

#What is Disassembly?

Computer programs are written originally in a human readable code form,
such as assembly language or a high-level language. These programs 
are then compiled into a binary format called **machine code**. This 
binary format is not directly readable or understandable by humans. 
Many programs -- such as malware, proprietary commercial programs, or 
very old legacy programs -- may not have the source code available to 
you.

This book outlines tools and techniques for attempting to convert the 
raw machine code of an executable file into equivalent code in assembly
language and the high-level languages C and C++.

We can figure out what a piece of malware does. We hope this leads us 
to figuring out how to block its harmful effects. Unfortunately, some 
malware writers use self-modifying code techniques (polymorphic 
camouflage, XOR encryption, scrambling), apparently to make it 
difficult to even detect that malware, much less disassemble it.

Disassembling code has a large number of practical uses. One of the 
positive side effects of it is that the reader will gain a better 
understanding of the relation between machine code, assembly language, 
and high-level languages.

#Assemblers and Compilers

**Assemblers** are significantly simpler than compilers, and are often 
implemented to simply **translate** the assembly code to binary machine
code via one-to-one correspondence. Assemblers rarely optimize beyond 
choosing the shortest form of an instruction or filling delay slots.

Because assembly is such a simple process, disassembly can often be 
just as simple. Assembly instructions and machine code words have a 
one-to-one correspondence, so each machine code word will exactly map 
to one assembly instruction.

##Assembler Concepts

Assemblers, on a most basic level, translate assembly instructions into
machine code with a one to one correspondence. They can also translate 
named variables into **hard-coded memory addresses** and **labels** 
into their relative code addresses.

Assemblers, in general, **do not** perform *code optimization*. The 
machine code that comes out of an assembler is equivalent to the 
assembly instructions that go into the assembler.

Some information about the program is lost during the assembly process.

First and foremost, program data is stored in the same raw binary 
format as the machine code instructions. This means that it can be 
difficult to determine which parts of the program are actually 
instructions. Notice that you can disassemble raw data, but the 
resultant assembly code will be nonsensical.

Second, textual information from the assembly source code file, such 
as variable names, label names, and code comments are **all destroyed**
during assembly. When you disassemble the code, the instructions will 
be the same, but all the other helpful information will be lost.

**Intel Syntax Assemblers**

  * MASM is Microsoft's assembler, an abbreviation for 
  "Macro Assembler", However, many people use it as an acronym for 
  "Microsoft Assembler" 

  * TASM, Borland's "Turbo Assembler,"

  * NASM, the "Netwide Assembler," is a free, portable, and 
  retargetable assembler that works on both Windows and Linux. 

  * FASM, the "Flat Assembler"

**(x86) AT&T Syntax Assemblers**

AT&T syntax is the *de facto* assembly standard on Unix and Unix-like 
operating systems.

  * The GNU Assembler (**GAS**) is the default back-end to the GNU 
  Compiler Collection (GCC) suite.

**GAS** is available as a part of either the GCC package or the GNU 
binutils package.

#Compilers

A **compiler** is a program that converts instructions from one 
language into equivalent instructions in another language. There is a 
common misconception that a compiler always directly converts a high 
level language into machine language, but this isn't always the case.

Many compilers convert code into assembly language, and a few even 
convert code from one high level language into another. Common 
examples of compiled languages are: C/C++, Fortran, Ada, and Visual 
Basic.

The figure below shows the common compile-time steps to building a 
program using the C programming language. The compiler produces object
files which are linked to form the final executable:

[here image]

Some compilers, such as the Microsoft C compiler, compile C and C++ 
source code directly into machine code. GCC on the other hand compiles
C and C++ into assembly language, and an assembler is used to convert 
that into the appropriate machine code. From the standpoint of a 
*disassembler*, it does not matter exactly how the original program 
was created.

Notice also that it is not possible to exactly reproduce the C or C++ 
code used originally to create an executable. It is, however, possible
to create code that compiles identically, or code that performs the 
same task.

C language statements **do not share** a one to one relationship with 
assembly language. Consider that the following C statements will 
typically all compile into the same assembly language code:

```c
*arrayA = arrayB[x++];

*arrayA = arrayB[x]; x++;

arrayA[0] = arrayB[x++];

arrayA[0] = arrayB[x]; x++;
```

Also, consider how the following loop constructs perform identical 
tasks, and are likely to produce similar or even identical assembly 
language code:

```c
for(;;) { ... }

while(1) { ... }

do { ... } while(1)
```

##GNU C Compiler

The GNU C compiler is part of the GNU Compiler Collection (GCC) suite.
The GNU GCC Compiler is the *de facto* standard compiler for Linux and
Unix systems. It is retargetable, allowing for many input languages 
(C, C++, Obj-C, Ada, Fortran, Go, etc...), and supporting multiple 
target OSes and architectures.

To produce an assembly listing file in GCC, use the following command 
line syntax:

```
gcc -S /path/to/sourcefile.c
```

For example, the following commandline:

```
gcc -S test.c
```

will produce an assembly listing file named `test.s`. Assembly listing
files generated by GCC will be in GAS format. On x86 you can select 
the syntax with `-masm=intel` or `-masm=att`.

#Disassemblers and Decompilers

In essence, a **disassembler** is the exact opposite of an assembler. 
Where an assembler converts code written in an assembly language into 
binary machine code, a *disassembler* reverses the process and 
attempts to recreate the assembly code from the binary machine code.

Since most assembly languages have a *one-to-one* correspondence with 
underlying machine instructions, the process of disassembly is 
relatively straight-forward, and a basic disassembler can often be 
implemented simply by reading in bytes, and performing a table lookup.

Many disassemblers have the option to output assembly language 
instructions in Intel or AT&T syntax.

#Disassembler Issues

There are a number of issues and difficulties associated with the 
disassembly process. The two most important difficulties are the 
division between code and data, and the loss of text information.

##Separating Code from Data

Since data and instructions are all stored in an executable as binary 
data, the obvious question arises: how can a disassembler tell code 
from data? Is any given byte a variable, or part of an instruction?

The problem wouldn't be as difficult if data were limited to the 
`.data` section (segment) of an executable (explained in a later 
chapter) and if executable code were limited to the `.code` section of
an executable, but this is often not the case. Data may be inserted 
directly into the code section (e.g. jump address tables, constant 
strings), and executable code may be stored in the data section 
(although new systems are working to prevent this for security 
reasons).

A technique that is often used is to identify the entry point of an 
executable, and find all code reachable from there, recursively. This 
is known as "code crawling".

Many interactive disassemblers will give the user the option to render
segments of code as either code or data, but non-interactive 
disassemblers will make the separation automatically. 

Disassemblers often will provide the instruction AND the corresponding
hex data on the same line, shifting the burden for decisions about the
nature of the code to the user. Scripting your own "crawler" in this 
way is more efficient; for large programs interactive disassembling 
may be impractical to the point of being unfeasible.

As a consequence, it is not possible to write a disassembler that will
correctly separate code and data for all possible input programs.

In practice a combination of interactive and automatic analysis and 
perseverance can handle all but programs specifically designed to 
thwart reverse engineering, like using encryption and decrypting code 
just prior to use, and moving code around in memory.

##Lost Information

User defined textual identifiers, such as variable names, label names,
and macros are removed by the assembly process. They may still be 
present in generated object files, for use by tools like debuggers and
relocating linkers, but the direct connection is lost and 
re-establishing that connection requires more than a mere 
disassembler. 

Operating system calls (like dll's in MS-Windows, or syscalls in 
Unices) may be reconstructed, as their names appear in a separate 
segment or are known beforehand. Many disassemblers allow the user to 
attach a name to a label or constant based on his understanding of the
code.

#Decompilers

Similar to Disassembly, **Decompilers** take the process a step 
further and actually try to reproduce the code in a high level 
language. Frequently, this high level language is C, because C is 
simple and primitive enough to facilitate the decompilation process.

#Debuggers

**Debuggers** are programs that allow the user to execute a compiled 
program one step at a time. You can see what instructions are executed
in which order, and which sections of the program are treated as code 
and which are treated as data. Debuggers allow you to analyze the 
program while it is running, to help you get a better picture of what 
it is doing.

A **breakpoint** is an instruction to the debugger that allows program
execution to be halted when a certain condition is met. For instance, 
when a program accesses a certain variable, or calls a certain API 
function, the debugger can pause program execution.

 **Windows Debuggers**

   * **SoftICE** (was taken off the market in April 2006)

   * **WinDbg** is a free piece of software from Microsoft that can 
   be used for local user-mode debugging, or even remote kernel-mode 
   debugging.

   * **IDA Pro** The multi-processor, multi-OS, interactive 
   disassembler by DataRescue.

   * **OllyDbg** is a free and powerful Windows debugger with a 
   built-in disassembly and assembly engine. Very useful for patching,
   disassembling, and debugging.

   * **Immunity Debugger** is a branch of OllyDbg v1.10, with built-in
   support for Python scripting and much more.


**Linux Debuggers**

  * **gdb** 

  The GNU debugger, comes with any normal Linux install.

  * **strace**, **ltrace**, and **xtrace**

  Lets you run a program while watching the actions it performs. 
  With `strace`, you get a log of all the system calls being made. 
  With `ltrace`, you get a log of all the library calls being made. 
  With `xtrace`, you get a log of some of the funtion calls being 
  made.

#Hex Editors

**Hex editors** are able to directly view and edit the binary of a 
source file, and are very useful for investigating the structure of 
proprietary closed-format data files. There are many hex editors in 
existence. 

##Linux Hex Editors only

**xxd and any text editor**

Produce a hex dump with `xxd`, freely edit it in your favorite text 
editor, and then convert it back to a binary file with your changes 
included.

#GNU Tools

The GNU packages have been ported to many platforms.

##GNU BinUtils

The GNU BinUtils package contains several small utilities that are 
very useful in dealing with binary files. The most important programs 
in the list are the GNU `objdump`, `readelf`, GAS assembler, and the 
GNU linker, although the reverser might find more use in `addr2line`, 
`c++filt`, `nm`, and `readelf`.

  * **objdump**

  Dumps out information about an executable including symbols and 
  assembly. It comes standard. It can be made to support non-native 
  binary formats.

```
objdump -p               displays a list of functions imported from 
other libraries, exported to and miscellaneous file header information
```

  * **readelf**

  Like `objdump` but more specialized for ELF executables.

  * **size** 

  Lists the sizes of the segments.

  * **nm** 

  Lists the symbols in an ELF file.

  * **strings** 

  Prints the strings from a file.

  * **file** 

  Tells you what type of file it is.

  * **kill** 

  Can be used to halt a program with the `sig_stop` signal.

  * **strace** 

  Trace system calls and signals.

  * **ltrace** 

  Displays runtime library call information for dynamically linked 
  executables.

  * **xtrace**

  It produces a trace of system or arbitrary function calls made by 
  the specified process.

