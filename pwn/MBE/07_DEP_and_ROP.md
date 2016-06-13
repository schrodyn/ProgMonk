#DEP & ROP

Theres a number of modern **exploit** mitigations that we’ve
generally been turning off for the labs and exercises:

  * DEP
  * ASLR
  * Stack Canaries
  * etc.

Today we turn one back on for the remainder of the course:

no more silly `-z execstack` in our gcc commands

**Data Execution Prevention** -- An exploit mitigation technique used 
to ensure that only **code segments** are ever marked as executable

Meant to mitigate code injection / shellcode payloads

Also known as **DEP**, **NX**, **XN**, **XD**, **W^X**

**DEP Basics:**

  * No segment of memory should ever be Writable and
  Executable at the same time, `‘W^X’`

  * Data should never be executable, only code

#History of DEP

When was DEP implemented?

  * August 14th, 2004 - Linux Kernel 2.6.8
  * August 25th, 2004 - Windows XP SP2
  * June 26th, 2006 - Mac OSX 10.5

Technologies in modern exploit mitigations are incredibly young, and 
the field of computer security is rapidly evolving.

**DEP** is one of the of the main mitigation technologies you must 
bypass in modern exploitation.

#Bypassing DEP

**DEP** stops an attacker from easily executing injected shellcode 
assuming they gain control of `EIP`: shellcode almost always ends up 
in a `RW-` region.

If you can’t inject (shell)code to do your bidding, you
must **re-use the existing code**! This is technique is usually some 
form of **ROP**.

#Course Terminology

**Return Oriented Programming**

  * A technique in exploitation to reuse existing code
  `gadgets` in a target binary as a method to bypass **DEP**

  * Also known as **ROP**

**Gadget**

  * A sequence of meaningful instructions typically followed
  by a **return** instruction

  * Usually multiple `gadgets` are chained together to
  compute malicious actions like `shellcode` does

  * These chains are called **ROP Chains**

ROP Chains are made up of **gadgets**

Example gadgets:

```asm
xor eax, eax
ret

pop ebx
pop eax
ret

add eax, ebx
ret
```

Usefull tool:

```
$ ropgadget --binary /bin/bash
```

#Understanding ROP

It is almost always possible to create a logically equivalent
**ROP chain** for a given piece of **shellcode**

exit(0) - shellcode:

```
xor eax, eax
xor ebx, ebx
inc eax
int 0x80
```

exit(0) - ROP chain:

```
xor eax, eax
ret

------

xor ebx, ebx
ret

-----

inc eax
ret

-----

int 0x80
```

We called `exit(0)` without using any sort of shellcode! 

#Relevant Tips/Tools/Commands

```
$ ropgadget --binary ./rop_exit > /tmp/gadgetzXYZ.txt
$ cat /tmp/gadgetzXYZ.txt | grep “pop eax” | grep …
```

#Stack Pivoting Tips

```
add esp, 0xXXXX
ret

--------

sub esp, 0xXXXX
ret

-------

ret 0xXXXX

-------

leave ; (mov esp, ebp)
ret

-------

xchg eXX, esp
ret
```

any gadgets that touch `esp` will probably be of interest for a 
pivot scenario

  * You may not find an exact pivot, or you may need to pivot
  multiple times!

  * You can always pad your **ROP Chains** with ROP NOPs which
  are simply **gadgets** that point to `ret`’s

#ret2libc

**‘ret2libc’** is a technique of ROP where you return to functions 
in standard libraries (libc), rather than using gadgets

If you know the addresses of the functions you want to ROP through 
in libc (assuming libc exists), ret2libc is easier than making 
a **ROP chain** with gadgets

##Common ret2libc Targets

  * `system()`
    * Executes something on the command line
    * `system(“cat flag.txt”);`

  *  (f) `open()` / `read()` / `write()`
    * Open/Read/Write a file contents









