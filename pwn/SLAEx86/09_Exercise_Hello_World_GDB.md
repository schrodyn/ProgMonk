#Hello World GDB

In this exercise we will look at how you will use GDB and step
through the "hello world" program and observe things like 
CPU registers, Memory Location etc.

Now, let's load the `hello_world` binary in `gdb`

```
$ gdb ./hello_world
```

And in order to go ahead and pose the execution of the binary we
need to set a breakpoint. Let's do it

```
(gdb) break _start
``` 

To go ahead and set breakpoint `_start` which you remember what was
defined as the entry point into the program. And now let's run the
program

```
(gdb) run
```

As soon as the program was run we hit the breakpoint `_start` and
now if we want to disassemble code we need to use `disassemble`
command

```
(gdb) disassemble
Dump of assembler code for function _start:
=> 0x08048080 <+0>:	mov    $0x4,%eax
   0x08048085 <+5>:	mov    $0x1,%ebx
   0x0804808a <+10>:	mov    $0x80490a4,%ecx
   0x0804808f <+15>:	mov    $0xc,%edx
   0x08048094 <+20>:	int    $0x80
   0x08048096 <+22>:	mov    $0x1,%eax
   0x0804809b <+27>:	mov    $0x5,%ebx
   0x080480a0 <+32>:	int    $0x80
End of assembler dump.
```

Now, what you may notice is that on the right side is assembly language
of course but something looks different. In assembly language you 
typically have two kinds of syntax one is AT&T and other is Intel.
AT&T is the native syntax for Linux where GAS assembler uses that.
However, we are interested in using only Intel syntax because when
we go to Windows assembly you would see that pretty much all tools
uses Intel syntax exclusively. So idea is to be coherent as far as
Intel and AT&T are conserned you could practically right the same 
instructions in two different representations.

So, we would like to see the output in Intel syntax. To do that we
gonna actually `set disassembly-flavor intel`

```
(gdb) set disassembly-flavor intel
```

Once you do this and make `disassemble` you should see

```
(gdb) disassemble
Dump of assembler code for function _start:
=> 0x08048080 <+0>:		mov    eax,0x4
   0x08048085 <+5>:		mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490a4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
   0x0804809b <+27>:	mov    ebx,0x5
   0x080480a0 <+32>:	int    0x80
```

our familiar Intel syntax coming back.

Now, if you remember this part of the code

```
 => 0x08048080 <+0>: 	mov    eax,0x4
    0x08048085 <+5>: 	mov    ebx,0x1
    0x0804808a <+10>:    mov   ecx,0x80490a4
    0x0804808f <+15>:    mov   edx,0xc
```

this is nothing but printing "hello world" to the screen and
this is the part

```
    0x08048094 <+20>:    int    0x80
    0x08048096 <+22>:    mov    eax,0x1
    0x0804809b <+27>:    mov    ebx,0x5
    0x080480a0 <+32>:    int    0x80
```

where we exit gracefully. Now let's go ahead and step to this program
and look how the registers changes. So in order to look at the CPU
registers we use `info registers` command. And this basically
show us the current register state.

```
(gdb) info registers
eax            0x0	0
ecx            0x0	0
edx            0x0	0
ebx            0x0	0
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0x8048080	0x8048080 <_start>
eflags         0x202	[ IF ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x0	0
```

Now, let's step through this instructions. If you look at the left 
handside this is a little arrow `=>` points to the next instruction
to be executed. To step through the instructions one after another
we use `stepi`

```
(gdb) stepi
0x08048085 in _start ()
```

And now if we hit `disassemble` 

```
(gdb) disassemble
Dump of assembler code for function _start:
   0x08048080 <+0>:	mov    eax,0x4
=> 0x08048085 <+5>:	mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490a4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
   0x0804809b <+27>:	mov    ebx,0x5
   0x080480a0 <+32>:	int    0x80
End of assembler dump.
```

you will notice that this instruction 

```
0x08048080 <+0>: mov    eax,0x4
```

has been executed and this is the next instruction 

```
=> 0x08048085 <+5>: mov    ebx,0x1
```

to be executed. Now before this instruction has been executed the 
value of `eax` was 0. After that it should have 0x4 value.

```
(gdb) info registers
eax            0x4	4
ecx            0x0	0
edx            0x0	0
ebx            0x0	0
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
...
```

Now at this point the register `ebx` has the value 0 in it. And next
instruction should move the value 0x1 inside it. So let's `stepi`

```
(gdb) stepi
0x0804808a in _start ()
(gdb) info registers
eax            0x4	4
ecx            0x0	0
edx            0x0	0
ebx            0x1	1
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0x804808a	0x804808a <_start+10>
...
```

And you will notice that `ebx` currently has the value 0x1 in it.

```
(gdb) disassemble 
Dump of assembler code for function _start:
   0x08048080 <+0>:		mov    eax,0x4
   0x08048085 <+5>:		mov    ebx,0x1
=> 0x0804808a <+10>:	mov    ecx,0x80490a4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
   0x0804809b <+27>:	mov    ebx,0x5
   0x080480a0 <+32>:	int    0x80
End of assembler dump.
```

The next instruction should load a large value `0x80490a4` inside
`ecx`. Now if we compare this with the source code you would remember
that in `ecx` what we are moving is the **memory address** where
"hello world" is located. And looks like this is address `0x80490a4`
where it is located. And if you want to look at this address you
should use command `x/s` which is going to examine memory but print
this location as a **string**.

```
(gdb) x/s 0x80490a4
0x80490a4 <message>:	 "hello world!"
```

And you would notice that we have "hello world" string here. 

Let's go further

```
(gdb) stepi
0x0804808f in _start ()
(gdb) info registers
eax            0x4	4
ecx            0x80490a4	134516900
edx            0x0	0
ebx            0x1	1
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0x804808f	0x804808f <_start+15>
...
```

You can see that value of `ecx` is now `0x80490a4`. After the next 
`stepi` command the value of `edx` became `0xc`. And the very next
command is actually to generate the **system call** by going ahead
and initiate interrupt 0x80.

```
(gdb) disassemble
Dump of assembler code for function _start:
   0x08048080 <+0>:		mov    eax,0x4
   0x08048085 <+5>:		mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490a4
   0x0804808f <+15>:	mov    edx,0xc
=> 0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
   0x0804809b <+27>:	mov    ebx,0x5
   0x080480a0 <+32>:	int    0x80
End of assembler dump.
```

So when we do `stepi` in that

```
(gdb) stepi
hello world!0x08048096 in _start ()
```

You would notice that "hello world!" was printed as expected and now 
we are in the next instruction. Now, at this point if you look at
register state 

```
(gdb) info registers
eax            0xc	12
ecx            0x80490a4	134516900
edx            0xc	12
ebx            0x1	1
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0x8048096	0x8048096 <_start+22>
...
```

What you would actually notice is that `eax` has the value 0xc which
is the return value of the **write** system call which we called. And
from what I remember `write` basically returns the number of bytes
that has been written. And which is nothing but again this is the
string length of "hello world!" which is 12.

Now, the next instruction is going to put the value 1 inside `eax`
which it did

```
(gdb) stepi
0x0804809b in _start ()
(gdb) info registers
eax            0x1	1
ecx            0x80490a4	134516900
edx            0xc	12
ebx            0x1	1
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0x804809b	0x804809b <_start+27>
...
```

The next instruction 

```
(gdb) disassemble
Dump of assembler code for function _start:
   0x08048080 <+0>:		mov    eax,0x4
   0x08048085 <+5>:		mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490a4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
=> 0x0804809b <+27>:	mov    ebx,0x5
   0x080480a0 <+32>:	int    0x80
End of assembler dump.
```

moves into `ebx` the value 0x5 which will be return code of the 
program.

```
(gdb) stepi
0x080480a0 in _start ()
(gdb) info registers
eax            0x1	1
ecx            0x80490a4	134516900
edx            0xc	12
ebx            0x5	5
esp            0xbffff3d0	0xbffff3d0
ebp            0x0	0x0
esi            0x0	0
edi            0x0	0
eip            0x80480a0	0x80480a0 <_start+32>
...
```

Don't care about any other registers and we generate interrupt in the
next instruction

```
(gdb) disassemble
Dump of assembler code for function _start:
   0x08048080 <+0>:	mov    eax,0x4
   0x08048085 <+5>:	mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490a4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
   0x0804809b <+27>:	mov    ebx,0x5
=> 0x080480a0 <+32>:	int    0x80
End of assembler dump.
```

And once this is generated program exits gracefully.

```
(gdb) stepi
[Inferior 1 (process 3278) exited with code 05]
```

And as you can see gdb tells you that process 3278 has exited, 
however, exit code is 05 which is what the exit code we had loaded.
So, this is how we can trace programs, look at the CPU registers state
at every moment from gdb and understand what is happening.

In the beginning I would really recommend using this technique to
understand how each and every operation works in assembly language.
