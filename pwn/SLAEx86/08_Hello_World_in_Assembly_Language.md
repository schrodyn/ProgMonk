#Hello World in Assembly Language

Hello! In this video we'll finally look at a "Hello World" program
and how you can code it up in assembly.

Now, before we move in all of that we need to understand the
different instruction sets available for IA-32. So you first have
the general purpose instructions set which typically uses the
general purpose registers and the EFLAGs. Then you have x87 FPU
Instructions set which is you know anything you want to do 
with floating point numbers you could basically use this instructions
set. After that you have the extensions which include MMX / SSE /
SSE2 / SSE3 / SSE4 and anything Intel decides to come up later as
an extension.

Now, we've seen in the very first video that go ahead to create
the final machine language code we need to take an assembly file 
and use an assembler, linker combination to convert it to
machine code that CPU can understand.

Now, this is done using `nasm` + `ld` in our case. `nasm` is 
an assembler called the "netwide assembler" which is free and open 
source. And `ld` is basically a linker which links the object files,
shared libraries, anything you want it to finally create the 
executable. The executable which we will create is in ELF format
which is the most common format on Linux-based systems.

Ok, so in order to create "Hello World" program what exectly do we need
to do? So, as I mentioned to you previously in the last video all the
program code needs to be inside what is called a TEXT section.

Let's create a file called `hello_world.asm`. Now, in order to enter
a comment we can basically use a `;` followed by your comment.
In "Hello World" program as you imagine you want to display the string
"hello world!" and this will have to be in DATA section where only
the initialized data. Inside the DATA section you want to mention 
the "hello world!" string. To do that you actually need to use `db`
or "define byte" with which we can define a series of bytes. And this
is what `nasm` understands. 

```asm
section .data

	message: db "hello world!"
```

So this is how we define "hello world" string inside the DATA section.
And we indentified the string with the label `message` which you will
later on use in the program.

As we defined the string in DATA section the next logical step as you
imagine we have the TEXT section which could contain procedures, a lot
of code so it's important to be able to tell the linker where the
entry point of the program is actually going to be. And this is where
we are going to identify the entry point using `_start` identifier. To
do that we should declare it as `global` which basically says "Hey! We 
are going to start here."

```asm

global _start

section .text

_start:
```

And now comes in the next part where we need to figure out how to print
to screen and how to exit gracefully. Ok, now this is where we need to
understand a little bit more about how all of this works. As far as 
printing to screen is concerned you could ask how you could use all 
this low level features of interfacing with hardware and all of that.
But as you could imagine that is not something we should like to do.
Instead we'd like to leverage OS for this more complicated tasks.
And when I mean more complicated tasks it would be for example 
writing to disk, printing on screen, interfacing with something devices
and things like that where writing code from scratch could really be
very very combuston. This is where **system calls** come in.

System calls provide a very simple interface for user space programs
to request the Kernel to do complicated tasks for them.

Now, How do System calls work? User Space program calls the **system
call**  and once this is done, so as an example one way for invoking
system calls is to generate an interrupt `int 0x80`, the CPU go ahead
and check the **Interrupt Handlers Table** and invoke the **System Call
Handler**. Now, the **system call handler** basically is a kernel mode 
program. And this **system call handler** goes ahead and figures out 
specific **system call routine** we are interested in. As an example, 
if you want to write to the screen there is a specific system call
for that, if you want to read something from input that's basically
a separate system call. Now, where all of this system calls are 
mentioned? But before this there are multiple ways by which you can
invoke this system calls in user space programs. On IA-32 you can do
this by using `int 0x80` or `SYSENTER` instruction. It's important to
note here that modern implementations typically use something called
**VDSO** or **Virtual Dynamic Shared Object** which is the more 
efficient way of doing system calls. Now how VDSO works and all of 
that is probably be not in what I will cover in this course simply
because it requires you to understand more about how internal
functions, how the loader works and a bunch of other things. However,
if you are interested you can refer to the link below (in PDF slides)
and understand how system call is actually implemented in the kernel.

Now, from our perspective which is pure security shellcoding and all
of that we will almost exclusively look at `int 0x80` for whatever
we do with respect to system calls.

Ok. so we said that there are so many system calls which may be 
available. The next logical question arises where are this system
calls defined? 

```
vim /usr/include/i386-linux-gnu/asm/unistd_32.h
```

And remember if you are using different system the location could be
different.

And this file mentiones the full list of system calls which are 
available for you. There are many many system calls.

Now, we go back to what we really wanted to do is to print to the 
screen and then exit the program. And this is where we need to use
apropriate system calls to do this. To write to the screen we 
basically want to use the `write` system call. And the write system
call has basically identified with the syscall number 4. And you 
would actually have a wrapper on it

```
man 2 write
```

The first input parameter is actually going to be **file descriptor**,
the second is going to be the **buffer** which actually contains what
you want to write into this file and then finally is the **length** of
what you want to write. 

Now, in our case we want to write to the stdout. For any program
by default stdin or input is mapped to file descriptor 0, stdout
is mapped to 1, and stderr is mapped to 2. So in our case the 
value of `fd` is going to be 1. Now, buffer or `buf` actually would
be pointing to the "hello world" string which we have, and finally
`count` is nothing but the length of "hello world" string.

Similarly, `exit` just requires us to give an exit status, typically 
the value is 0 is used to indicate that the program exit without any
problems. If you give a non-zero value that may have to indicate that
a program exited with error. Now, the question arises we have all of
this inputs which needs to be fed into the system call. How do you 
pass this inputs before invoking the system call?

This is where for IA-32 you are going to go ahead and pass this via
registers. The `eax` register must contain the **system call number**,
if you remember we found out that right number for this value is 4.
`ebx`,`ecx`,`edx`,`esi`,`edi` would contain the 1st, 2nd, 3rd, 4th
and 5th argument depending on number of arguments. If you have more
than that the ideal way would be to pass them in the form of 
structures. Now, once the system call returns (because every system
call typically has something return value) the return value is pretty
much always inside the `eax` register. This is important if you
would like to use the return value of the system call. 

In our case, we figured out that `eax` should contain system call
number, `ebx` = stdout which actually is 1, `ecx` = Pointer to 
"hello world" and `edx` = Length of "hello world".

```asm
; hello world program

global _start

section .text

_start:
        ; print the hello world

        mov eax, 0x4 ; write code number
        mov ebx, 0x1
        mov ecx, message
        mov edx, mlen
        int 0x80

        ; exit program

        mov eax, 0x1 ; exit code number
        mov ebx, 0x5
        int 0x80


section .data

        message: db "hello world!"
        mlen    equ $-message
```

So this is how you are able now to create very simple programs. Now,
let's go ahead and assemble and link this. So let's use `nasm`.

```
nasm -f elf32 -o hello_world.o hello_world.asm
```

`-f` means executable "format" which we want to create, in our case
it is elf32 and then we mention output file `-o hello_world.o` -- 
object file. What we need to do next is basically link it to the 
executable

```
ld -o hello_world hello_world.o
```

now run it

```
osboxes@osboxes:~/SLAEx86$ ./hello_world 
hello world!osboxes@osboxes:~/SLAEx86$
```

And if you want to look at return value of the program you just need
to issue command

```
echo $?
```

Which is exactly 5 in our case.

So this is how we can create very simple assembly language program
to print "hello world" on IA-32 Linux.

Now comes the exercise. What about analyzing and vizualizing all 
those micro instructions such as `mov eax, 0x1`, mov ebx, 0x1` and
things like that? How do we go ahead and look at all of that at
microscopic level? This is where gdb comes in. Use gdb to step through
"hello world" program, observe the CPU registers, see how data is 
being transfered, look at memory locations and a bunch of other 
things.

