#Moving Data

Hello! We are going to deal with moving data around.

So if I would conceptualize any assembly language instruction
you could break it up into the **label** which access an indetifier
as to where the instruction is located. With respect to program
the **instruction** itself and **operand** is nothing but
arguments to the instruction itself. 

Now, operands could be **registers**, **memory locations** or 
**immediate** values. Immediate values basically the value itself
which is for example 10x80, 102030 which is an actual value rather
than any form of refference to it.

Now, we've already looked at the `mov` instruction in the previous
program. Let's study it in more detail. It's most probably the most
common instruction in assembly for moving data between registers
and memory and registers, immediate values into registers and
immediate value into memory. You will be encountering this instruction
quite alot.

The other interesting instruction is a `lea` or **Load Effective 
Address** which allows us to load pointer values. So as an example is

```asm
lea eax,[label]
```

which basically is going to load pointer inside `eax` and that pointer
is nothing but the exact memory location where label is defined.

Now, the third instruction is "exchange" `xchg` which exchanges or
swaps values. So `XCHG Register, Register` would swap the values in
each of this registers. You can also exchange values between register
and memory location: `XCHG Register, Memory`.

Now, let's actually go into the code and look at how this 3 
instructions are layed out. Open `MovingData.nasm`

What I have done is I created simple examples to understanding 
instructions. Now the way to do this we should try to create
your own little instructions to illustrate how specific assembly
language work. 

So the first one is moving an immediate value to the register.

```
mov eax, 0xaaaaaaaa
mov al, 0xbb
mov ah, 0xcc
mov ax, 0xdddd

mov ebx, 0
mov ecx, 0
```

Similarly, you can move between registers. So as an example I basically
moving value of `eax` into the `ebx` register. There are other examples

```
mov ebx, eax
mov cl, al
mov ch, ah
mov cx, ax

mov eax, 0
mov ebx, 0
mov ecx, 0
```

And you can also move from memory into register. So I have defined
`sample` as the label here in the **data** section.

```
section .data

sample: db 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22
```

And if you notice here moving data from `sample` into the registers:

```
mov al, [sample]
mov ah, [sample +1]
mov bx, [sample]
mov ecx, [sample]
```

`ecx` is going to load 32 bits from the beginning of `sample`.

Similarly, you can move from register into memory

```asm
mov eax, 0x33445566
mov byte [sample], al
mov word [sample], ax
mov dword [sample], eax
```

So you have similar options. You can move immediate value into
memory directly as well. So here is an example where overwriting
the beginning of `sample` what is the first 4 bytes with the value
`0x33445566`.

```asm
mov dword [sample], 0x33445566
```

And then we move on to the demo of `lea` where we are loading the
**address** of where `sample` is located into the `eax` register,
and then loading that into `ebx` register by using `eax` register.

```
lea eax, [sample]
lea ebx, [eax]
```

`xchg` is another interesting thing we talked about where we are
moving two values in `eax` and `ebx` and all we do is call `xchg`

```
mov eax, 0x11223344
mov ebx, 0xaabbccdd

xchg eax, ebx
```

Now let me go ahead and assemble, link this code so that we can see
how this works using gdb.

```
gdb -q ./MovingData
Reading symbols from /home/osboxes/SLAE-Code/MovingData/MovingData...(no debugging symbols found)...done.
(gdb) set disassembly-flavor intel
(gdb) break _start
Breakpoint 1 at 0x8048080
(gdb) run
Starting program: /home/osboxes/SLAE-Code/MovingData/MovingData 

Breakpoint 1, 0x08048080 in _start ()
```

And if we disassemble

```
(gdb) disassemble
Dump of assembler code for function _start:
=> 0x08048080 <+0>:		mov    eax,0xaaaaaaaa
   0x08048085 <+5>:		mov    al,0xbb
   0x08048087 <+7>:		mov    ah,0xcc
   0x08048089 <+9>:		mov    ax,0xdddd
   0x0804808d <+13>:	mov    ebx,0x0
   0x08048092 <+18>:	mov    ecx,0x0
   0x08048097 <+23>:	mov    ebx,eax
   0x08048099 <+25>:	mov    cl,al
   0x0804809b <+27>:	mov    ch,ah
   0x0804809d <+29>:	mov    cx,ax
   0x080480a0 <+32>:	mov    eax,0x0
   0x080480a5 <+37>:	mov    ebx,0x0
   0x080480aa <+42>:	mov    ecx,0x0
   0x080480af <+47>:	mov    al,ds:0x8049108
   0x080480b4 <+52>:	mov    ah,BYTE PTR ds:0x8049109
   0x080480ba <+58>:	mov    bx,WORD PTR ds:0x8049108
   0x080480c1 <+65>:	mov    ecx,DWORD PTR ds:0x8049108
   0x080480c7 <+71>:	mov    eax,0x33445566
   0x080480cc <+76>:	mov    ds:0x8049108,al
   0x080480d1 <+81>:	mov    ds:0x8049108,ax
   0x080480d7 <+87>:	mov    ds:0x8049108,eax
   0x080480dc <+92>:	mov    DWORD PTR ds:0x8049108,0x33445566
   0x080480e6 <+102>:	lea    eax,ds:0x8049108
   0x080480ec <+108>:	lea    ebx,[eax]
   0x080480ee <+110>:	mov    eax,0x11223344
   0x080480f3 <+115>:	mov    ebx,0xaabbccdd
   0x080480f8 <+120>:	xchg   ebx,eax
   0x080480f9 <+121>:	mov    eax,0x1
   0x080480fe <+126>:	mov    ebx,0x0
   0x08048103 <+131>:	int    0x80
End of assembler dump.
```

you would see a ton of output here. `eip` is currently pointing here

```
=> 0x08048080 <+0>:	mov    eax,0xaaaaaaaa
```

in another we should have source code opened, so we can compare 
instructions nowhere we are. So what do we want to do now is for
a couple of examples I would want to step through the program and
locate how the registers are changing.

So let's go ahead and do this in intelligent way. In the beginning 
what I really said about displaying `eax` register. And let's go
ahead and do that. 

So there are a different ways to look at registers. You could,
for example, use `print` command and look at the register

```
(gdb) print/x $eax
$1 = 0x0
```

Right, You could use `display` command as well. Now what I really want
is that every time the program stops `eax`,`ebx`,`ecx` the output of
these registers should be printed on the screen. And what I want is
that we should be able to see the exact location where we are 
currently running, part of code should be disassembled.

Also we would want to inspect `sample` and for that we are going to
use the `examine` command. And hopefully all of this is known to you:

```
(gdb) help x
```

So with examine command we want to print it as individual bytes and
if you look at the length of `sample`, `sample` is 8 bytes.

So you basically want this command

```
(gdb) x/8xb &sample

0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
```

And you would actually have this eight values which is exactly what
we have in source code. Now in order to do this in not have to repeat all the commands we will use a feature in gdb called **hooking**. Now
the whole idea is you want gdb to execute a bunch of commands as you
are stepping through the command. So let's define the hook, let's call
it `hook-stop` which basically tells you to write whatever you want
to be executed every time the program stops.

```
(gdb) define hook-stop
Type commands for definition of "hook-stop".
End with a line saying just "end".
>
```

So I want to print the value of `eax` register

```
> print/x $eax
```

I want to print the value of `ebx` register

```
> print/x $ebx
```

print the value of `ecx` register

```
> print/x $ecx
```

And then I want to print basically the value of `sample`

```
> x/8xb &sample
```

And then finally I just want to disassemble every small part of the
program and don't last long listing, for this we use `disassemble`
command, `$eip` is the current instruction we are going to execute
next, and `+10` is the number of lines to choose of.

```
> disassemble $eip,+10
```

And then I end my hook 

```
> end
```

Let's look at the next instruction

```
(gdb) disassemble $eip,+10
Dump of assembler code from 0x8048080 to 0x804808a:
=> 0x08048080 <_start+0>:	mov    eax,0xaaaaaaaa
   0x08048085 <_start+5>:	mov    al,0xbb
   0x08048087 <_start+7>:	mov    ah,0xcc
   0x08048089 <_start+9>:	mov    ax,0xdddd
End of assembler dump.
```

And now we are going to use `nexti` to step by program. And the very
first instruction we are going to move the value `0xaaaaaaaa` into
`eax`.

```
(gdb) nexti
$2 = 0xaaaaaaaa
$3 = 0x0
$4 = 0x0
0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x8048085 to 0x804808f:
=> 0x08048085 <_start+5>:	mov    al,0xbb
   0x08048087 <_start+7>:	mov    ah,0xcc
   0x08048089 <_start+9>:	mov    ax,0xdddd
   0x0804808d <_start+13>:	mov    ebx,0x0
End of assembler dump.
0x08048085 in _start ()
``` 

And if you notice this `$2 = 0xaaaaaaaa` is `eax`, this `$3 = 0x0` is
`ebx`, this `$4 = 0x0` is `ecx`. And all dollars has been moved in.

Now the next instruction we are going to move `0xbb` into `al`.

```
(gdb) 
$5 = 0xaaaaaabb
$6 = 0x0
$7 = 0x0
0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x8048087 to 0x8048091:
=> 0x08048087 <_start+7>:	mov    ah,0xcc
   0x08048089 <_start+9>:	mov    ax,0xdddd
   0x0804808d <_start+13>:	mov    ebx,0x0
End of assembler dump.
0x08048087 in _start ()
```

that is exactly what happened here. The next instruction would replace
`ah` with `0xcc`.

```
(gdb) 
$8 = 0xaaaaccbb
$9 = 0x0
$10 = 0x0
0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x8048089 to 0x8048093:
=> 0x08048089 <_start+9>:	mov    ax,0xdddd
   0x0804808d <_start+13>:	mov    ebx,0x0
   0x08048092 <_start+18>:	mov    ecx,0x0
End of assembler dump.
0x08048089 in _start ()
```

that's what exactly happened now. The next instruction would replace
`ax`.

```
(gdb) 
$11 = 0xaaaadddd
$12 = 0x0
$13 = 0x0
0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x804808d to 0x8048097:
=> 0x0804808d <_start+13>:	mov    ebx,0x0
   0x08048092 <_start+18>:	mov    ecx,0x0
End of assembler dump.
0x0804808d in _start ()
```

fantastic.

The next instruction is going to go ahead and move 0x0 into `ebx`
register and so on. Everything is understandable until this

```
(gdb) 
$38 = 0x0
$39 = 0x0
$40 = 0x0
0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480af to 0x80480b9:
=> 0x080480af <_start+47>:	mov    al,ds:0x8049108
   0x080480b4 <_start+52>:	mov    ah,BYTE PTR ds:0x8049109
End of assembler dump.
0x080480af in _start ()
```

Now we are going to move a byte located at `0x8049108` into `al`
register which means the byte 0xaa is going to be moved inside `al`.
Now we are going to move basically the next value at `0x8049109` 
inside `ah` register which does basically mean 0xbb. And similarly
we move a bunch of other values which you can play along with.

Now comes the interesting part.

```
(gdb) 
$53 = 0x33445566
$54 = 0xbbaa
$55 = 0xddccbbaa
0x8049108 <sample>:	0xaa	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480cc to 0x80480d6:
=> 0x080480cc <_start+76>:	mov    ds:0x8049108,al
   0x080480d1 <_start+81>:	mov    ds:0x8049108,ax
End of assembler dump.
0x080480cc in _start ()
```

So now what we are trying to do is we are going to overwrite the 
memory location `sample` by the value `al`. So `al` contains the
value 0x66. And as you notice this is exactly what happens.

```
(gdb) 
$56 = 0x33445566
$57 = 0xbbaa
$58 = 0xddccbbaa
0x8049108 <sample>:	0x66	0xbb	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480d1 to 0x80480db:
=> 0x080480d1 <_start+81>:	mov    ds:0x8049108,ax
   0x080480d7 <_start+87>:	mov    ds:0x8049108,eax
End of assembler dump.
0x080480d1 in _start ()
```

Similarly now overwriting is going to be happen with `ax`:

```
(gdb) 
$59 = 0x33445566
$60 = 0xbbaa
$61 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0xcc	0xdd	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480d7 to 0x80480e1:
=> 0x080480d7 <_start+87>:	mov    ds:0x8049108,eax
   0x080480dc <_start+92>:	mov    DWORD PTR ds:0x8049108,0x33445566
End of assembler dump.
0x080480d7 in _start ()
```

And if you notice 5566 is what has been written to `sample`: 
`0x66 0x55` because of little-endian format of storage. Then now
we are going to use `eax` to overwrite `sample`

```
(gdb) 
$62 = 0x33445566
$63 = 0xbbaa
$64 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0x44	0x33	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480dc to 0x80480e6:
=> 0x080480dc <_start+92>:	mov    DWORD PTR ds:0x8049108,0x33445566
End of assembler dump.
0x080480dc in _start ()
```

So 0x33445566 is basically `0x66    0x55    0x44    0x33`. And the
next instruction is doing exactly that.

Now what we are going to do is `lea` -- load effective address.

```
(gdb) 
$65 = 0x33445566
$66 = 0xbbaa
$67 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0x44	0x33	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480e6 to 0x80480f0:
=> 0x080480e6 <_start+102>:	lea    eax,ds:0x8049108
   0x080480ec <_start+108>:	lea    ebx,[eax]
   0x080480ee <_start+110>:	mov    eax,0x11223344
End of assembler dump.
0x080480e6 in _start ()
```

So the value of `eax` is going to be overwritten now by this 0x8049108
address.

```
(gdb) 
$68 = 0x8049108
$69 = 0xbbaa
$70 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0x44	0x33	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480ec to 0x80480f6:
=> 0x080480ec <_start+108>:	lea    ebx,[eax]
   0x080480ee <_start+110>:	mov    eax,0x11223344
   0x080480f3 <_start+115>:	mov    ebx,0xaabbccdd
End of assembler dump.
0x080480ec in _start ()
```

Now `ebx` will be overwritten by the address refferenced by `eax`.

```
(gdb) 
$71 = 0x8049108
$72 = 0x8049108
$73 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0x44	0x33	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480ee to 0x80480f8:
=> 0x080480ee <_start+110>:	mov    eax,0x11223344
   0x080480f3 <_start+115>:	mov    ebx,0xaabbccdd
End of assembler dump.
0x080480ee in _start ()
```

Which now makes both of them equal. After next two instructions:

```
(gdb) 
$77 = 0x11223344
$78 = 0xaabbccdd
$79 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0x44	0x33	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480f8 to 0x8048102:
=> 0x080480f8 <_start+120>:	xchg   ebx,eax
   0x080480f9 <_start+121>:	mov    eax,0x1
   0x080480fe <_start+126>:	mov    ebx,0x0
End of assembler dump.
0x080480f8 in _start ()
```

And now we are going to exchange the values of `ebx` and `eax`

```
(gdb) 
$80 = 0xaabbccdd
$81 = 0x11223344
$82 = 0xddccbbaa
0x8049108 <sample>:	0x66	0x55	0x44	0x33	0xee	0xff	0x11	0x22
Dump of assembler code from 0x80480f9 to 0x8048103:
=> 0x080480f9 <_start+121>:	mov    eax,0x1
   0x080480fe <_start+126>:	mov    ebx,0x0
End of assembler dump.
0x080480f9 in _start ()
```

The remaining part is just exiting the program.

So if you noticed this is exactly the way I would want you to learn
each and every instruction. I've taken a ton of time right now
to single step through every instruction, looking at how it 
effecting the registers, looking at how changes are happening,
and this is exactly how you need to go ahead and do this yourself.

Now, you could have used `print` command to print the values of the
registers, alternatively you can actually use something called
`display` command 

```
display/x $eax
display/x $ebx
display/x $ecx
```

Now, `display` automatically prints the values every time the program
stops.

```
(gdb) display/x $eax
(gdb) display/x $ebx
(gdb) display/x $ecx
(gdb) break _start
Breakpoint 1 at 0x8048080
(gdb) r
Starting program: /home/osboxes/SLAE-Code/MovingData/MovingData 

Breakpoint 1, 0x08048080 in _start ()
3: /x $ecx = 0x0
2: /x $ebx = 0x0
1: /x $eax = 0x0
```

If I call `nexti` command you will notice that registers are displayed
automatically printing the value 

```
(gdb) nexti
0x08048085 in _start ()
3: /x $ecx = 0x0
2: /x $ebx = 0x0
1: /x $eax = 0xaaaaaaaa
```

Now, the point I'd like to make is definetely all instructions have 
other combinations as well. To be very honest, it's impossible to 
cover every single option of going ahead and using these instructions
or covering every single data moving instruction. The real idea is

  * cover the most important significant instructions which you will
  encounter.

  * If you use the technique I showed you with gdb on how to inspect
  registers and memory, ask these instructions do what they do, then
  conceptually you will understand instructions very very well. And 
  this is exactly how I would recommend you should learn every single
  instruction in assembly.

Ok, apart from this we also have something called the **stack** which 
is the temporary location for storing local variables, return 
addresses and all of that by processes and threads. Now, stack is 
actually a very unique data structure in the sense that it is actually
a LIFO data structure. What do I mean by that?

Let me give you a simple illustration, the first unit to remember
about the stack is that the stack for Intel architectures grows
from higher memory to lower memory, and at any given instance the
register which is responsible for tracking the top of the stack
is the `esp` register. So `esp` is nothing but the "extended stack"
pointer and it always points to the top of stack. Now, let say
you want to insert 32-bit value, then the instruction set you would
actually use is `push` along with register name in the most typical
form of use. Now, the best part is that `push` instruction will
automatically change the value of `esp` and you don't need to
do yourself.

The most important thing you need to remember is as the stack builds
up `esp` points to lower, lower and lower memory addresses because
the stack is increasing from high memory to low memory.

Another instruction is `pop`. When you da `pop` what happens is
the value is removed from the top of the stack and `esp` is adjusted
accordingly.

Now what I want you guys to do is to look at `push`/`pop` and any
other stack instructions you may encounter, write out a simple 
program just like I illustrated and examine how the stack grows
using `esp` and examine command and see how changes are happenning.
