#Libc and NASM

Hello all! In this video we will talk about how to use `libc` functions
in our assembly language programs. 

Now, you are probably asking yourself what in syscalls going up for
just about everything? The answer is yes, but if you start using those
low level syscalls then if you want to create a slightly complicated
program you would just end up using too many of them, lot more code
to write and definetely this is not something which is desirable 
because assembly language is very very difficult to debug if it is
compared to other high-level languages.

So what we really want is to be able to leverage standard C library
on Linux which is `libc` which has tons of useful functions. And in
this video we focus on calling `libc` functions from assembly.

Now, the only thing we need to remember when you invoke libc library
calls is the following:

  * All the library calls which you want to invoke in your program
  need to be defined using the keyword `extern` right at the top

  * wherenever you call the library function the arguments to that
  function need to be put on the stack and they need to be put
  in **reverse** order. what do I mean by that if you call libc
  function which takes 4 arguments `a`,`b`,`c` and `d`. Then you
  need to put on the stack in order 

```
push d
push c
push b
push a
```

  * now here is the other important thing to maintain appropriate state
  for `esp`, ensure that after libc function call returns you adjust
  the stack.

  * finally, you would want to compile the progam now with `gcc` and
  link not with `ld` because it's much easier to do with `gcc`. Now,
  `gcc` expects `main` entry point to be there instead of `_start` or
  any other random name.

Now, however you note that `gcc` does have option with which you can
give another starting point. Having said that let me take you to the
code now. Open `Libc.nasm`. And what I have done is I recreated the
"hello world" program written using syscalls now using libc function
calls.

Now, the two function calls I am going to use are `printf` and `exit`.


```asm
extern printf
extern exit

global main

section .text
main:

        push message
        call printf
        add esp, 0x4  ; adjust the stack

        mov eax, 0xa
        call exit



section .data

        message: db "Hello World!", 0xA, 0x00
        mlen     equ $-message
```

So what I told you was the arguments to the libc function calls need to
be there on the stack. Now, how do you find out that?

```
man 3 printf
```

And this gives you the Programmer Manual page for `printf`. Now, the 
easiest way to invoke `printf` is just to feed it the pointer to
the string you want to print. So I have already defined `message` here
and of course you would want message to be **null** terminated.
And what I am doing is pushing the address of `message` on the stack
and then I just `call printf`. Then after `printf` returns I adjust 
the stack size by 0x4. This is just going to ensure that the stack is
back at where it was before.  After that I am going to move the value
0xa into `eax` and then call `exit`. `exit` just exits the program.

So let's go ahead and assemble and link this.

```
nasm -f elf32 -o Libc.o Libc.nasm
gcc -o Libc Libc.o
```

Now instead of `ld` I am using `gcc`. Also one thing I've forgot to
mention is now entry point is `main`. Now let;s load up this in gdb

```
gdb -q ./Libc
```

```
(gdb) set disassembly-flavor intel
(gdb) break main
(gdb) run
```

Now let's disassemble

```
(gdb) disassemble 
Dump of assembler code for function main:
=> 0x08048420 <+0>:	push   0x804a018
   0x08048425 <+5>:	call   0x8048320 <printf@plt>
   0x0804842a <+10>:	add    esp,0x4
   0x0804842d <+13>:	mov    eax,0xa
   0x08048432 <+18>:	call   0x8048340 <exit@plt>
   0x08048437 <+23>:	nop
   0x08048438 <+24>:	nop
   0x08048439 <+25>:	nop
   0x0804843a <+26>:	nop
   0x0804843b <+27>:	nop
   0x0804843c <+28>:	nop
   0x0804843d <+29>:	nop
   0x0804843e <+30>:	nop
   0x0804843f <+31>:	nop
End of assembler dump.
```

And here let's go ahead and track the stack as well.

```
(gdb) x/xw $esp
0xbffff32c:	0xb7e394e3
```

Now I've got next instruction

```
(gdb) stepi
```

Now If I look at the top of the stack we basically have pointer to
the message we want to print. You can verify

```
(gdb) x/xw $esp
0xbffff328:	0x0804a018
(gdb) x/s 0x0804a018
0x804a018 <message>:	 "Hello World!\n"
```

There we go. So we have "hello world" on there. So let's now move on
to the next instruction while keeping an eye to the top of the stack.

```
(gdb) disassemble 
Dump of assembler code for function main:
   0x08048420 <+0>:	push   0x804a018
=> 0x08048425 <+5>:	call   0x8048320 <printf@plt>
   0x0804842a <+10>:	add    esp,0x4
   0x0804842d <+13>:	mov    eax,0xa
   0x08048432 <+18>:	call   0x8048340 <exit@plt>
   0x08048437 <+23>:	nop
   0x08048438 <+24>:	nop
   0x08048439 <+25>:	nop
   0x0804843a <+26>:	nop
   0x0804843b <+27>:	nop
   0x0804843c <+28>:	nop
   0x0804843d <+29>:	nop
   0x0804843e <+30>:	nop
   0x0804843f <+31>:	nop
End of assembler dump.
```

Now if we `stepi` and `disassemble`

```
(gdb) stepi
0x08048320 in printf@plt ()
(gdb) disassemble 
Dump of assembler code for function printf@plt:
=> 0x08048320 <+0>:	jmp    DWORD PTR ds:0x804a000
   0x08048326 <+6>:	push   0x0
   0x0804832b <+11>:	jmp    0x8048310
End of assembler dump.
```

This would go ahead and continue to `printf` which is something we
don't really care about. So let's create breakpoint here

```
0x0804842a <+10>:    add    esp,0x4
```

```
(gdb) break *0x0804842a
Breakpoint 2 at 0x804842a
```

So the next instruction after `printf` finishes execution.

```
(gdb) c
Continuing.
Hello World!

Breakpoint 2, 0x0804842a in main ()
```

So `printf` has printed it. 

```
(gdb) disassemble 
Dump of assembler code for function main:
   0x08048420 <+0>:	push   0x804a018
   0x08048425 <+5>:	call   0x8048320 <printf@plt>
=> 0x0804842a <+10>:	add    esp,0x4
   0x0804842d <+13>:	mov    eax,0xa
   0x08048432 <+18>:	call   0x8048340 <exit@plt>
   0x08048437 <+23>:	nop
   0x08048438 <+24>:	nop
   0x08048439 <+25>:	nop
   0x0804843a <+26>:	nop
   0x0804843b <+27>:	nop
   0x0804843c <+28>:	nop
   0x0804843d <+29>:	nop
   0x0804843e <+30>:	nop
   0x0804843f <+31>:	nop
End of assembler dump.
```

Now if I continue to look at the top of the stack you could still
exactly be the same value as before and this is where we do our 
little pointer arithmetic and if I can go back here we want to reset
stack back to this value 0xbffff32c.

```
(gdb) nexti
0x0804842d in main ()
(gdb) disassemble 
Dump of assembler code for function main:
   0x08048420 <+0>:	push   0x804a018
   0x08048425 <+5>:	call   0x8048320 <printf@plt>
   0x0804842a <+10>:	add    esp,0x4
=> 0x0804842d <+13>:	mov    eax,0xa
   0x08048432 <+18>:	call   0x8048340 <exit@plt>
   0x08048437 <+23>:	nop
   0x08048438 <+24>:	nop
   0x08048439 <+25>:	nop
   0x0804843a <+26>:	nop
   0x0804843b <+27>:	nop
   0x0804843c <+28>:	nop
   0x0804843d <+29>:	nop
   0x0804843e <+30>:	nop
   0x0804843f <+31>:	nop
End of assembler dump.
```

and if you notice the stack now has been restored to what was 
previously pointed to.

```
(gdb) x/xw $esp
0xbffff32c:	0xb7e394e3
```

And after that we just go ahead and continue the program and exit.
So this is a very very simple example but this illustrates pretty
much any libc function which you can use in your assembly program.
All you need to ensure is the arguments go properly inside the stack
and then you make it properly called. And then you eventually after
the call you adjust the value of `esp` accordingly.
