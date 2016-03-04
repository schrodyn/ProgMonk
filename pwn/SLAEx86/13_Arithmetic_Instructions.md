#Arithmetic Instructions

Hello guys! In this video we will look at arithmetic operations
and how we can implement it.

Now, before we move into arithmetic operations It's important to
revisit EFLAGS register. If you remember I said that `eflags` register
contains different flags which can probably be categorized into
states and control systems.

Arithmetic operations affect typically the states flags. So let me go
ahead and give you an example of what I mean by this. The only one
flag you need remember right now is the **Carry Flag**. So when you
add two numbers there are various possibilities. So when you add
instruction if there is an overflow then **Carry Flag** is set to
indicate to the programmer that there actually has been an extra value
which is passed on forward.

ADD hash format

```
ADD destination, source
```

with the variation 

```
ADC destionation, source
```

which uses the Carry Flag and basically add as well. Similarly
SUB (subtract) and SBB (sbutract borrow) which basically tells
you to subtract and additional value 1 from whatever you subtracting.

Let's open `Arithmetic.nasm`

The first what I am doing is the register based addition

```
        ; register based addition

        mov eax, 0
        add al, 0x22
        add al, 0x11

        mov ax, 0x1122
        add ax, 0x3344

        mov eax, 0xffffffff
        add eax, 0x10
```

The last one is slightly different because we already have loaded the
maximum possible value inside `eax` register and then add the value
0x10 to `eax`. This would cause basically for the result to be
larger then the destination register and the Carry Flag will be 
getting set.

Similarly, all I have done here is based using memory location for
addition rather than register

```
        ; memory based addition

        mov eax, 0
        add byte [var1], 0x22
        add byte [var1], 0x11

        add word [var2], 0x1122
        add word [var2], 0x3344

        mov dword [var3], 0xffffffff
        add dword [var3], 0x10
```

So here you would actually see that I defined 3 variables in the bottom
`var1`, `var2` and `var3` all of them right now contain value 0. `var1`
is a byte, `var2` is a word and `var3` is a double word

```
section .data

        var1    db      0x00
        var2    dw      0x0000
        var3    dd      0x00000000
```

and we use them for addition here. Exact the same principle as before.
After that I show you 3 commands which **set**, **clear** or 
**complement** the Carry Flag. So `clc` which is actually used to
clear Carry Flag, `stc` is to set Carry Flag, and `cmc` is used to
complement the Carry Flag. Once this is done I'll actually show you
an application of `adc` which is "add with carry" and then subtract
`sub` and subtract with borrow `sbb`.

And then we have a quick illustration of increment `inc` and decrement
`dec`.

So let's compile it and load into gdb

```
gdb -q ./Arithmetic
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
```

Then we run the program

```
(gdb) run
```

So let's go ahead and quickly look at the variables which are 
available for us for refference

```
(gdb) info variables
All defined variables:

Non-debugging symbols:
0x0804910c  var1
0x0804910d  var2
0x0804910f  var3
```

So we have `var1`,`var2` and `var3`. So now let me define the
hook-stop

```
(gdb) define hook-stop
Type commands for definition of "hook-stop".
End with a line saying just "end".
>print/x $eax
>x/xb &var1
>x/xh &var2
>x/xw &var3
>print $eflags
>disassemble $eip,+10
>end
```

Next actually do `nexti`

```
(gdb) nexti
$1 = 0x0
0x804910c <var1>:	0x00
0x804910d <var2>:	0x0000
0x804910f <var3>:	0x00000000
$2 = [ IF ]
Dump of assembler code from 0x8048085 to 0x804808f:
=> 0x08048085 <_start+5>:	add    al,0x22
   0x08048087 <_start+7>:	add    al,0x11
   0x08048089 <_start+9>:	mov    ax,0x1122
   0x0804808d <_start+13>:	add    ax,0x3344
End of assembler dump.
0x08048085 in _start ()
```

Here `$1 = 0x0` is actually value of `eax` and `$2 = [ IF ]` is 
actually our flags that is set, if you are confused about what each  
of this means you can look this up based on the slides, all of them
have been mentioned here. So IF is Interrupt Enable Flag. We will
be touching upon a couple of this flags relevant to our discussion
as when they required. Or you can pick up Intel manual and have a
look.

After a couple of instructions:

```
(gdb) 
$11 = 0xffffffff
0x804910c <var1>:	0x00
0x804910d <var2>:	0x0000
0x804910f <var3>:	0x00000000
$12 = [ PF IF ]
Dump of assembler code from 0x8048096 to 0x80480a0:
=> 0x08048096 <_start+22>:	add    eax,0x10
   0x08048099 <_start+25>:	mov    eax,0x0
   0x0804809e <_start+30>:	add    BYTE PTR ds:0x804910c,0x22
End of assembler dump.
0x08048096 in _start ()
```

We are going to add 0x10 to `eax` which will cause the carry. What
you will notice is that in the flags the Carry Flag will be set (CF).

```
(gdb) 
$13 = 0xf
0x804910c <var1>:	0x00
0x804910d <var2>:	0x0000
0x804910f <var3>:	0x00000000
$14 = [ CF PF IF ]
Dump of assembler code from 0x8048099 to 0x80480a3:
=> 0x08048099 <_start+25>:	mov    eax,0x0
   0x0804809e <_start+30>:	add    BYTE PTR ds:0x804910c,0x22
End of assembler dump.
0x08048099 in _start ()
```

And if you notice the Carry Flag is now set. Of course, now the value
of `eax` has to be enterpreted in conjuction with the fact that Carry
Flag was set. If the programmer would want to use the real value of
the addition. 

after `mov eax,0x0` we have

```
(gdb) 
$15 = 0x0
0x804910c <var1>:	0x00
0x804910d <var2>:	0x0000
0x804910f <var3>:	0x00000000
$16 = [ CF PF IF ]
Dump of assembler code from 0x804809e to 0x80480a8:
=> 0x0804809e <_start+30>:	add    BYTE PTR ds:0x804910c,0x22
   0x080480a5 <_start+37>:	add    BYTE PTR ds:0x804910c,0x11
End of assembler dump.
```

Now, if you remember the source code we are going to add the value 
0x22 to the variable `var1`.

```
(gdb) 
$17 = 0x0
0x804910c <var1>:	0x22
0x804910d <var2>:	0x0000
0x804910f <var3>:	0x00000000
$18 = [ PF IF ]
Dump of assembler code from 0x80480a5 to 0x80480af:
=> 0x080480a5 <_start+37>:	add    BYTE PTR ds:0x804910c,0x11
   0x080480ac <_start+44>:	add    WORD PTR ds:0x804910d,0x1122
End of assembler dump.
0x080480a5 in _start ()
```

Then we also add 0x11 to it

```
(gdb) 
$19 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x0000
0x804910f <var3>:	0x00000000
$20 = [ PF IF ]
Dump of assembler code from 0x80480ac to 0x80480b6:
=> 0x080480ac <_start+44>:	add    WORD PTR ds:0x804910d,0x1122
   0x080480b5 <_start+53>:	add    WORD PTR ds:0x804910d,0x3344
End of assembler dump.
```

Now we are going to work with `var2` where we are going to store 
the value 0x1122 into the variable `var2`. Note here we manipulate
memory directly without any registers. It is important.

```
(gdb) 
$21 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x1122
0x804910f <var3>:	0x00000000
$22 = [ PF IF ]
Dump of assembler code from 0x80480b5 to 0x80480bf:
=> 0x080480b5 <_start+53>:	add    WORD PTR ds:0x804910d,0x3344
   0x080480be <_start+62>:	mov    DWORD PTR ds:0x804910f,0xffffffff
End of assembler dump.
```

And again add 0x3344 to the `var2`

```
(gdb) 
$23 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x00000000
$24 = [ PF IF ]
Dump of assembler code from 0x80480be to 0x80480c8:
=> 0x080480be <_start+62>:	mov    DWORD PTR ds:0x804910f,0xffffffff
End of assembler dump.
0x080480be in _start ()
```

It is 0x4466 after addition. Now we are going to look at `var3`

```
(gdb) 
$25 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0xffffffff
$26 = [ PF IF ]
Dump of assembler code from 0x80480c8 to 0x80480d2:
=> 0x080480c8 <_start+72>:	add    DWORD PTR ds:0x804910f,0x10
   0x080480cf <_start+79>:	clc    
   0x080480d0 <_start+80>:	stc    
   0x080480d1 <_start+81>:	cmc    
End of assembler dump.
```

And now we are going again add 0x10 to it what actually cause the
Carry Flag to be set. Here we go

```
(gdb) 
$27 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$28 = [ CF PF IF ]
Dump of assembler code from 0x80480cf to 0x80480d9:
=> 0x080480cf <_start+79>:	clc    
   0x080480d0 <_start+80>:	stc    
   0x080480d1 <_start+81>:	cmc    
   0x080480d2 <_start+82>:	mov    eax,0x0
   0x080480d7 <_start+87>:	stc    
   0x080480d8 <_start+88>:	adc    al,0x22
End of assembler dump.
```

And carry was happened because CF was set. Now the Carry Flag is set
and we want to clear the Carry Flag. To do that there is the 
instruction `clc` or "clear Carry". So notice that after the next
instruction CF will disapear. 

```
(gdb) 
$29 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$30 = [ PF IF ]
Dump of assembler code from 0x80480d0 to 0x80480da:
=> 0x080480d0 <_start+80>:	stc    
   0x080480d1 <_start+81>:	cmc    
   0x080480d2 <_start+82>:	mov    eax,0x0
   0x080480d7 <_start+87>:	stc    
   0x080480d8 <_start+88>:	adc    al,0x22
End of assembler dump.
```

There we go. We don't CF anymore. But we'd like to set it again on
demand so we use `stc` command which is "set Carry"

```
(gdb) 
$31 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$32 = [ CF PF IF ]
Dump of assembler code from 0x80480d1 to 0x80480db:
=> 0x080480d1 <_start+81>:	cmc    
   0x080480d2 <_start+82>:	mov    eax,0x0
   0x080480d7 <_start+87>:	stc    
   0x080480d8 <_start+88>:	adc    al,0x22
   0x080480da <_start+90>:	stc    
End of assembler dump.
```

And now the Carry Flag is set. `cmc` basically go ahead and inverts
the current conditional complements, the condition of the Carry Flag.
So if it is set it will be unset. And if it is unset it will be set.
So `cmc` now will basically remove the Carry Flag.

```
(gdb) 
$33 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$34 = [ PF IF ]
Dump of assembler code from 0x80480d2 to 0x80480dc:
=> 0x080480d2 <_start+82>:	mov    eax,0x0
   0x080480d7 <_start+87>:	stc    
   0x080480d8 <_start+88>:	adc    al,0x22
   0x080480da <_start+90>:	stc    
   0x080480db <_start+91>:	adc    al,0x11
End of assembler dump.
```

This exactly what happened. Now I actually want to show you "add with
carry". And "add with carry" will only work if the Carry Flag is
actually set. So let's `mov eax,0x0` and `stc` to be done.

```
(gdb) 
$37 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$38 = [ CF PF IF ]
Dump of assembler code from 0x80480d8 to 0x80480e2:
=> 0x080480d8 <_start+88>:	adc    al,0x22
   0x080480da <_start+90>:	stc    
   0x080480db <_start+91>:	adc    al,0x11
   0x080480dd <_start+93>:	mov    ax,0x1122
   0x080480e1 <_start+97>:	stc    
End of assembler dump.
```

Now, "add with carry" (`adc`) is going to add the value 0x22 to
the `al` register. If the Carry Flag is not set -- that's all, 
operation is over, if it is set -- the value 1 additionally added to 
it. So right now the CF is set which means that what will be stored
in `al` after this instruction would actually be 0x23.

```
(gdb) 
$39 = 0x23
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$40 = [ IF ]
Dump of assembler code from 0x80480da to 0x80480e4:
=> 0x080480da <_start+90>:	stc    
   0x080480db <_start+91>:	adc    al,0x11
   0x080480dd <_start+93>:	mov    ax,0x1122
   0x080480e1 <_start+97>:	stc    
   0x080480e2 <_start+98>:	adc    ax,0x3344
End of assembler dump.
```

Now I am going again set CF and then add 0x11 which basically mean
0x11 + 1 because Carry Flag is set.

```
(gdb) 
$43 = 0x35
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$44 = [ PF IF ]
Dump of assembler code from 0x80480dd to 0x80480e7:
=> 0x080480dd <_start+93>:	mov    ax,0x1122
   0x080480e1 <_start+97>:	stc    
   0x080480e2 <_start+98>:	adc    ax,0x3344
   0x080480e6 <_start+102>:	mov    eax,0xffffffff
End of assembler dump.
```

And when you make this you see that `eax` now is 0x23 + 0x12 = 0x35

Ok, you now understand how to `add` and `adc` works. Now let's look
at subtraction.

```
(gdb) 
$57 = 0xa
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$58 = [ CF AF IF ]
Dump of assembler code from 0x80480f4 to 0x80480fe:
=> 0x080480f4 <_start+116>:	sub    eax,0x5
   0x080480f7 <_start+119>:	stc    
   0x080480f8 <_start+120>:	sbb    eax,0x4
   0x080480fb <_start+123>:	inc    eax
   0x080480fc <_start+124>:	dec    eax
   0x080480fd <_start+125>:	mov    eax,0x1
End of assembler dump.
```

Now, let me subtract 0x5 from `eax`.

```
(gdb) 
$59 = 0x5
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$60 = [ PF IF ]
Dump of assembler code from 0x80480f7 to 0x8048101:
=> 0x080480f7 <_start+119>:	stc    
   0x080480f8 <_start+120>:	sbb    eax,0x4
   0x080480fb <_start+123>:	inc    eax
   0x080480fc <_start+124>:	dec    eax
   0x080480fd <_start+125>:	mov    eax,0x1
End of assembler dump.
```

Now lets set CF bit. As you notice the next instruction is `sbb`.
So if the Carry Flag is not set then 0x5 - 0x4 will be 0x1 but
because the Carry Flag was set the operation would be
0x5 - 0x4 - 1 which actually makes it 0.

```
(gdb) 
$63 = 0x0
0x804910c <var1>:	0x33
0x804910d <var2>:	0x4466
0x804910f <var3>:	0x0000000f
$64 = [ PF ZF IF ]
Dump of assembler code from 0x80480fb to 0x8048105:
=> 0x080480fb <_start+123>:	inc    eax
   0x080480fc <_start+124>:	dec    eax
   0x080480fd <_start+125>:	mov    eax,0x1
   0x08048102 <_start+130>:	mov    ebx,0xa
End of assembler dump.
```

Wherever any instruction operation ends up in zero then Zero Flag (ZF)
is automatically get set that what we see right now. And this is
really what the ZF is about.

To continue execution you should enter

```
(gdb) c
```

So hopefully `add`, "add with carry" (`adc`), `sub`, "subtract with
borrow" (`sbb`), `inc` and `dec` is clear now to you. These are 
fundamental instructions. There are many variations of these 
instructions which are available which you can look up yourself.
So see Intel Manual but the principle is simple. If you understand
this whole package then you pretty much understand the assembly
language.

Ok. so now for an exercise you will need to look at two more
instructions Multiply and Divide. Now, Multiply and Divide are
very interesting, very unique functions so it's `imul` and `idiv`.
