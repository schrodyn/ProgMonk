#Stack Exercise

Hi, guys! In this exercise we are going to look at how the stack
functions, we will primarely look at the `push` and `pop` instructions
and I would leave it to you as an exercise to look at `pushad` and
`popad` instructions.

Open `Stack.nasm`.

So as far as `push` and `pop` instructions are concerned on IA-32
you can push 16-bit or 32-bit values on the stack. Now, the values
you are pushing can be a register or can be refferenced from memory.

So let's me give you some example

```
push ax
pop bx

push eax
pop ecx
```

And in the second case I have pushing and popping using memory

```
push word [sample]
pop ecx

push dword [sample]
pop edx
```

where `sample` is declared in **data** section

```
section .data

sample: db 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22
```

Now let's run it under gdb

```
gdb -q ./Stack
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
```

```
(gdb) run
(gdb) disassemble
```

Now let's monitor `eax`,`ebx`,`ecx` registers as well as `esp` registerevery time the program stops

```
(gdb) display/x $eax
1: /x $eax = 0x0
(gdb) display/x $ebx
2: /x $ebx = 0x0
(gdb) display/x $ecx
3: /x $ecx = 0x0
```

Now we define hook for `esp`

```
(gdb) define hook-stop
```

Let's monitor 8 bytes from the beginning of `esp`.

```
>x/8xb $esp
```

Now we chunk this bytes into words

```
>x/4xh $esp
```

And also chunk this bytes into double words

```
>x/2xw $esp
```

And also add automatic disassemble of $eip

```
>disassemble $eip,+10
```

and end the hook

```
>end
```

Now let's step through the instruction series

```
(gdb) nexti
0xbffff3b0:	0x01	0x00	0x00	0x00	0x26	0xf5	0xff	0xbf
0xbffff3b0:	0x0001	0x0000	0xf526	0xbfff
0xbffff3b0:	0x00000001	0xbffff526
Dump of assembler code from 0x8048085 to 0x804808f:
=> 0x08048085 <_start+5>:	mov    ebx,0x0
   0x0804808a <_start+10>:	mov    ecx,0x0
End of assembler dump.
0x08048085 in _start ()
3: /x $ecx = 0x0
2: /x $ebx = 0x0
1: /x $eax = 0x66778899
```

**My note**

Ok, later you should monitor what's going on with $esp dump memory.
I am lazy to print all of this operations here :)

This is how we can use gdb and trace through the stack to understand
how values are getting pushed and popped from it. If you understand
this then you pretty much understand all of stack arithmetic. Now
my recomendation would be to write many more this instructions and
use exactly what I've done right now to follow what's happening on the
stack by floowing the value of `esp` as the instructions are getting
executed as well as monitoring registers which you are using along
with those instructions. There are some variations of `push` and `pop`
which I leave to you as an exercise to figure out.
