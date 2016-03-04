#Syllabus and Review

##Course Terminology

**Machine** -- A computer, server, sometimes refers to the actual CPU

**Binary** -- An *executable* such as an .EXE, ELF, MachO or other code
containers that run on a **machine**

**Malware** -- A malicious **binary** meant to persist on a machine 
such as a Rootkit or Remote Access Tool (RAT)

**Vulnerability** -- A bug in a binary that can be leveraged by an 
**exploit**

**Exploit** (as a noun) -- Specially crafted data that utilizes 
**vulnerabilities** to force the binary into doing something unintended

 By this definition, exploits are not explicitly malware.

**0day** -- A previously unknown or unpatched **vulnerability** that
can be used by an **exploit**

An **0day** can also be an **exploit** using the unpatched **vuln**

In security, **pwn**/**pwning** commonly refers to vulnerability 
research and exploit development

The right bugs (vulnerabilities) found in binaries can be used by 
exploits to hijack code execution

Once code execution is achieved by an attacker..
 
  * Gain privileged information
  * Download or install malware
  * Steal data
  * Wreak any sort of havoc on the machine

##Course Roadmap

We start off with the fundamentals required: Basic reverse 
engineering, memory corruption, classical exploitation

Different classes of **vulnerabilities** are introduced and how they 
can be leveraged in exploitation: Stack smashes, format strings, 
signed/unsigned, Heap, UAF, etc

Modern **exploit** mitigations are introduced and how they can
be bypassed in exploitation: DEP, ASLR, GS/Cookies

Quick Linux Overview

  * UNIX-like open source kernel used by many open source
  operating systems distros

  * Written in C and assembly

  * ELF (Executable and Linkable Format) files for **binaries**

Get info on commands/man pages that might do whatever:

```
apropos [whatever]
```

##x86 Assembly

An assembly instruction set introduced in 1978 by Intel

  * 1978 - 16bit
  * 1985 - 32bit
  * 2001 - 64bit (Itanium)
  * 2003 - 64bit (AMD64)

Overwrought CISC, a total playground for exploitation. As low level as 
we’ll go

##x86 Assembly Syntax

All assembly languages are made up of **instruction** sets. 
Instructions are generally simple arithmetic operations that take 
registers or constant values as arguments.

We’ll be using the Intel syntax in this class

Important Registers

  * `EAX EBX ECX EDX` - General purpose registers
  * `ESP` - Stack pointer, “top” of the current stack frame (lower 
  memory)
  * `EBP` - Base pointer, “bottom” of the current stack frame (higher 
  memory)
  * `EIP` - Instruction pointer, pointer to the *next* instruction to
  be executed by the CPU

  * `EFLAGS` - stores flag bits
    * `ZF` - zero flag, set when result of an operation equals zero
    * `CF` - carry flag, set when the result of an operation is too 
    large/small
    * `SF` - sign flag, set when the result of an operation is 
    negative


##Moving Data

* `mov ebx, eax` --  Move the value in `eax` to `ebx`
* `mov eax, 0xDEADBEEF` --  Move 0xDEADBEEF into `eax`
* `mov edx, DWORD PTR [0x41424344]` --  Move the 4-byte value at 
address 0x41424344 into `edx`
* `mov ecx, DWORD PTR [edx]` -- Move the 4-byte value at the address 
in `edx`, into `ecx`
* `mov eax, DWORD PTR [ecx+esi*8]` -- Move the value at the address 
`ecx+esi*8` into `eax`

##Arithmetic Operations

* `sub edx, 0x11` -- edx = edx - 0x11; /
* `add eax, ebx` -- eax = eax + ebx; 
* `inc edx` -- edx++;
* `dec ebx` -- ebx--;
* `xor eax, eax` --  eax = eax ^ eax;
* `or edx, 0x1337` -- edx = edx | 0x1337;

##Some Conditional Jumps

* `jz $LOC` -- Jump to $LOC if ZF = 1
* `jnz $LOC` -- Jump to $LOC if ZF = 0
* `jg $LOC` -- Jump to $LOC if the result of a comparison is the 
"destination is greater than the source"

##Stack Manipulation

  * `push ebx` -- Subtract 4 from the stack pointer to move it towards  lower memory (zero,) and copy the value in `EBX` on top of the stack

```asm
sub esp, 4
mov DWORD PTR [esp], ebx
```

  * `pop ebx` -- Copy the value off the top of the stack and into 
  `EBX`, the add 4 to the stack pointer to move it towards higher 
  memory (0xFFFFFFFF)

```asm
mov ebx, DWORD PTR [esp]
add esp, 4
```

##Calling / Returning

  * `call some_function` --  Calls the code at `some_function`. We 
  need to push the **return address** onto the stack, then branch to 
  `some_function`

```asm
push eip
mov eip, some_function ; not actually valid
```

  * `ret` -- Used to return from a function call. Pops the top of the 
  stack to `eip`

```asm
pop eip ; not actually valid
```

  * `nop` -- ‘**no op**eration’ - does nothing

##Additional Material

Related Readings:

  *  Hacking: The Art of Exploitation, Chapter 0x200: Programming - 
  C programming and GDB

  *  Practical Reverse Engineering (Dang et al) -- Chapter 1 (x86)




