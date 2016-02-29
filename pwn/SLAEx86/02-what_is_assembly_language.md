#Module 1: 32-bit ASM on Linux

Hello! Welcome to Module 1 of the SLAE course. Now in module 1 we 
will look at assembly language basics on Linux systems and we
will specifically looking at 32-bit assembly on intel architecture.

Before we begin into the nitty-gritty details of assembly language
the first question which of course arises is **what is assembly
language?**

Now If I quickly summarize what is assembly language. It is low-level
programming language with which we can communicate dirrectly to
the microprocessor. Typically assembly language is very specific
to the processor family and has almost one-to-one correspondence
with machine code the processor understands.

Now what does all of that mean in the little man language: Very simply
your processor only speaks machine language which is binary. 

Unfortanutely, we still haven't mastered speaking in binary of machine
language.

And this is where Assembly language basically acts as a bridge. Now we
as humans or group of programmers write what we want the processor to
do in **assembly** language and then we use **assembler** and 
**linker** which are two programs you will encounter later on this 
video series which go ahead and translate your assembly language
into machine language which your processor can understand. So this 
is very simple illustration. This entire course is on how you can
master this assembly language and how the processor do your biding(?)

Now if you want to pick up how assembly language is related to more
high-level languages like C then your C source code file first goes 
through the *C preprocessor* stage. Which then passes to the compiler
which output assembly language file which is the input to the assembler
which gives object code file. And the the linker would take your object
file any other shared libraries and then output the actual executable
which in the case of Linux will have ELF format. And then finally
is loaded by **loader** for execution right if you want to run it.

Now one of the most important things is unlike high level languages
like C and Java which are very very portable, assembly language is
very specific to the processor family. And sometimes even between 
different processors there are instructions which may differ and
which are very processor specific. So for example you one of the
latest i7 processors may have a couple of assembly language 
instruction which your older processors may not understand. So
what it really means is we need to understand assembly language
for a very specific processor and this is not portable. As an example,
Intel assembly is very different from ARM assembly, which is different
from MIPS assembly.

Now Intel Architecture basicly specifies Assembly language for two
families. One is IA-32 and IA-64.

In this course we will look at IA-32 Assembly and see how we can learn
assembly language and run on it programs on the Linux OS.

If you are asking yourself the question why to learn 32-bit assembly
when the world is moving to 64-bit machines? And answer basically is
even today a large number of machines out there are still running 
IA-32 architecture. And without knowing 32-bit assembly it is very
difficult to grasp many of more complicated concepts of 64-bit
assembly. At the very same time 32-bit shellcoding, encoders, decoders,
packers, anything low level relating to exploit research and reverse
engineering is typically very different from what you will encounter
in the 64-bit world. And hence this is important to understand IA-32
before moving to IA-64 and more complicated architectures.

Ok, so without further redo let's go ahead and do the very first lab
exercise and this is installing ubuntu 12.04 LTS 32-bit Edition in
Virtualbox. This is the first exercise.

The second exercise is figuring out details about your CPU on the
Ubuntu System. So how do you know if you are on a 32/64-bit CPU?
If your CPU has capabilities such as FPU, MMX, SSE, SSE2 etc.
How do you find all of this details in your Ubuntu system?
