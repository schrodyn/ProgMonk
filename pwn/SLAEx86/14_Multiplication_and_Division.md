#Multiplication and Division

Welcome back! In this exercise we will look at Multiply and Divide
instructions in Assembly language.

So the multiply instruction and divide both of them are very 
interesting. But before we are getting to the specifics, conceptually
saying if your register sizes are 8 bit and you multiply two numbers
there are two possibilities:

  * the result of multiplication would still feel inside 8 bits
  * the result of multiplication extends beyond 8 bits

And this is really what the multiply instruction MUL (Unsigned 
Multiply) addresses in its different forms. So there are 3 variations.
One is when you use an 8-bit register or memory location, the other is
16-bit register or memory location, and the third is 32-bit register 
or memory location

Now, in the first case when you want to multiply with an 8-bit number
it is **implied** that the first number for the multiplication is 
stored in `al` register and the second number can be refferenced to
memory location or another register. And the output of the product
is going be stored in `ax` register. Mind yourself, `ax` has both
`ah` and `al` and this is where when I talk about multiplication
product being larger then 8 bits. Now, the important part here that
microprocessor would want to tell you that "Hey! The value which I
have in product is actually larger than 8 bits". And the way it 
indicates that is by setting OF = 1 and CF = 1 or Overflow and Carry
Flags if the upper half of result is non-zero. What I mean by 
upper-half in `ax` `ah` would be non-zero which is expected if 
of course the product is going to be greater than 8 bits which is the
size of `al`.

So, again summarizing, `al` multiply with 8-bit number which is in a
register or refferenced from memory the output is in `ax` register
which can be broken up in `ah` and `al`. In case `ah` is used, then
the Overflow and Carry bits are set to 1. Now, if you move that
from the perspective of 16-bit multiplication, then we use the `ax` 
register and the output is stored in the combination of `dx` and `ax`
registers. So this is where `dx` register comes in. So the upper half
of the result would be in `dx` and the lower half would be in `ax`.
Now, if the result leads to a non-zero `dx` than the OF = 1 and CF = 1.

Similarly, when you move into 32-bit multiplication, we use the `eax`
register and multiply that with a register or a memory location which
is 32 bit length and the output is stored in `edx` and `eax` register
combination. 

Now, when you look at this theoretically right just on the slide
something might be confusing. So let's jump right into the code.

Let's open `MulDiv.nasm`

Now, I mentioned that in multiplication the imply register is the
`eax` register by default. I am going to show you multiplication
8 bit using registers first

```asm
        ; unsigned r/m8 multiplication
        mov eax, 0x0

        mov al, 0x10
        mov bl, 0x2
        mul bl

        mov al, 0xFF
        mul bl
```

So let's take this very simple example up first and move to the
gdb to look at how all of this looks like

```
gdb -q ./MulDiv
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
Breakpoint 1 at 0x8048080
```

Let's run

```
(gdb) run
```

```
(gdb) disassemble 
Dump of assembler code for function _start:
=> 0x08048080 <+0>:	mov    eax,0x0
   0x08048085 <+5>:	mov    al,0x10
   0x08048087 <+7>:	mov    bl,0x2
   0x08048089 <+9>:	mul    bl
   0x0804808b <+11>:	mov    al,0xff
   0x0804808d <+13>:	mul    bl
   0x0804808f <+15>:	mov    eax,0x0
   0x08048094 <+20>:	mov    ebx,0x0
   0x08048099 <+25>:	mov    ax,0x1122
   0x0804809d <+29>:	mov    bx,0x2
   0x080480a1 <+33>:	mul    bx
...

``` 

So we want to go ahead and monitor `al` register and `bl` register
and of course `ax` to monitor the output. So let's write quick
gdb hook

```
(gdb) define hook-stop
>print/x $al
>print/x $bl
>print/x $ax
>disassemble $eip,+10
>end
```

Now let's move on executing the next instruction

```
(gdb) nexti
$1 = 0x0
$2 = 0x0
$3 = 0x0
Dump of assembler code from 0x8048085 to 0x804808f:
=> 0x08048085 <_start+5>:	mov    al,0x10
   0x08048087 <_start+7>:	mov    bl,0x2
   0x08048089 <_start+9>:	mul    bl
   0x0804808b <_start+11>:	mov    al,0xff
   0x0804808d <_start+13>:	mul    bl
End of assembler dump.
0x08048085 in _start ()
```

And as expected the next instruction is to move value 0x10 inside 
`al`. So let's go ahead and run this

```
(gdb) 
$4 = 0x10
$5 = 0x0
$6 = 0x10
Dump of assembler code from 0x8048087 to 0x8048091:
=> 0x08048087 <_start+7>:	mov    bl,0x2
   0x08048089 <_start+9>:	mul    bl
   0x0804808b <_start+11>:	mov    al,0xff
   0x0804808d <_start+13>:	mul    bl
   0x0804808f <_start+15>:	mov    eax,0x0
End of assembler dump.
0x08048087 in _start ()
```

Now if you notice `al` is 0x10 which actually also means that `eax` 
would be 0x10 as well because `ah` is zero at this point.

```
(gdb) 
$7 = 0x10
$8 = 0x2
$9 = 0x10
Dump of assembler code from 0x8048089 to 0x8048093:
=> 0x08048089 <_start+9>:	mul    bl
   0x0804808b <_start+11>:	mov    al,0xff
   0x0804808d <_start+13>:	mul    bl
   0x0804808f <_start+15>:	mov    eax,0x0
End of assembler dump.
0x08048089 in _start ()
```

And if we go ahead and print the multiplication output it means that
in `al` will be value `0x10 * 0x2 = 0x20` as well as 0x20 in `eax`. 

```
$10 = 0x20
$11 = 0x2
$12 = 0x20
Dump of assembler code from 0x804808b to 0x8048095:
=> 0x0804808b <_start+11>:	mov    al,0xff
   0x0804808d <_start+13>:	mul    bl
   0x0804808f <_start+15>:	mov    eax,0x0
   0x08048094 <_start+20>:	mov    ebx,0x0
End of assembler dump.
0x0804808b in _start ()
```

And this is exactly what we've seen. Now, the variation is moving
0xff into `al`

```
$13 = 0xff
$14 = 0x2
$15 = 0xff
Dump of assembler code from 0x804808d to 0x8048097:
=> 0x0804808d <_start+13>:	mul    bl
   0x0804808f <_start+15>:	mov    eax,0x0
   0x08048094 <_start+20>:	mov    ebx,0x0
End of assembler dump.
0x0804808d in _start ()
```

And now when I multiply by `bl` you would need to see the interesting
things as well. So let's go ahead and print flags.

```
(gdb) print $eflags
$16 = [ IF ]
```

Right now this is the state. Now, after the multiplication the Carry
and Overflow Flags would be set. So let me run the next instruction

```
(gdb) nexti
$17 = 0xfe
$18 = 0x2
$19 = 0x1fe
Dump of assembler code from 0x804808f to 0x8048099:
=> 0x0804808f <_start+15>:	mov    eax,0x0
   0x08048094 <_start+20>:	mov    ebx,0x0
End of assembler dump.
0x0804808f in _start ()
```

And if you notice it is overflowed in `al` because `eax` is now 
`0x1fe` rather than just `0xfe`. And now if I look at the flags

```
(gdb) print $eflags
$20 = [ CF IF OF ]
```

You would see that CF and OF has been set. And this is exactly how 
multiplication works in the very simple way.

Now, all I have done in the remaining part is kind of extending this
to 16-bit multiplication here

```
        ; unsigned r/m16 multiplication

        mov eax, 0x0
        mov ebx, 0x0

        mov ax, 0x1122
        mov bx, 0x0002
        mul bx

        mov ax, 0x1122
        mov bx, 0x1122
        mul bx
```

And in each of this I basically have two demos: one in which there
is no carry or overflow happening and the second one in which
carry or overflow is happening. Similarly, I have this for 32-bit

```
        ; unsigned r/m32 multiplication

        mov eax, 0x11223344
        mov ebx, 0x00000002
        mul ebx

        mov eax, 0x11223344
        mov ebx, 0x55667788
        mul ebx
```

And then multiplication using memory locations which is `var1`, `var2`
and `var3`.

```
        ; multiplication using memory locations

        mul byte [var1]
        mul word [var2]
        mul dword [var3]
```

which I've defined down below here


```
section .data

        var1    db      0x05
        var2    dw      0x1122
        var3    dd      0x11223344
```

So trace it through gdb and try it on your own because it's exactly
the same process.

If we didn't run the program but want to disassemble it we need to 
pass the address where to start

```
(gdb) disassemble &_start
```

or directly `_start` :)

```
(gdb) disassemble _start
```

Let's say we are interesting in 32-bit multiplication at

```
   0x080480b9 <+57>:	mul    ebx
   0x080480bb <+59>:	mov    eax,0x11223344
   0x080480c0 <+64>:	mov    ebx,0x55667788
   0x080480c5 <+69>:	mul    ebx
```

which is at `+59` from the beginning of `_start`.

One of the other ways to setup breakpoint is exactly what I'll show
you right now

```
(gdb) break *&_start +59
Breakpoint 1 at 0x80480bb
```

And there we go. Now let's run the program 

```
(gdb) run
```

Let's go to the second breakpoint that's what you are really
interested in

```
(gdb) c
Continuing.

Breakpoint 2, 0x080480bb in _start ()
```

```
(gdb) disassemble $eip,+10
Dump of assembler code from 0x80480bb to 0x80480c5:
=> 0x080480bb <_start+59>:	mov    eax,0x11223344
   0x080480c0 <_start+64>:	mov    ebx,0x55667788
End of assembler dump.
```

Now, let's setup for us some display

```
(gdb) display/x $edx
(gdb) display/x $eax
(gdb) display/x $ebx
```

And now I am going to setup display flags

```
(gdb) display $eflags
```

Now let's step through the program and also

```
(gdb) define hook-stop
>disassemble $eip,+15
>end
```

So after two `nexti` 

```
(gdb) nexti
Dump of assembler code from 0x80480c5 to 0x80480d4:
=> 0x080480c5 <_start+69>:	mul    ebx
   0x080480c7 <_start+71>:	mul    BYTE PTR ds:0x8049100
   0x080480cd <_start+77>:	mul    WORD PTR ds:0x8049101
End of assembler dump.
0x080480c5 in _start ()
4: $eflags = [ IF ]
3: /x $ebx = 0x55667788
2: /x $eax = 0x11223344
1: /x $edx = 0x0
```

As far as 32-bit multiplication goes, the result is going to be in
`edx` and `eax`. In `edx` will be higher 32 bits, and in `eax` will
be lower 32 bits of the result. So when I run the next command

```
(gdb) nexti
Dump of assembler code from 0x80480c7 to 0x80480d6:
=> 0x080480c7 <_start+71>:	mul    BYTE PTR ds:0x8049100
   0x080480cd <_start+77>:	mul    WORD PTR ds:0x8049101
   0x080480d4 <_start+84>:	mul    DWORD PTR ds:0x8049103
End of assembler dump.
0x080480c7 in _start ()
4: $eflags = [ CF IF OF ]
3: /x $ebx = 0x55667788
2: /x $eax = 0x117d820
1: /x $edx = 0x5b736a6
```

If you notice `edx` and `eax` together contain the output of the
multiplication. Now, this is something you can verify if you like
as well. Let's do it in python

```
>>> hex(0x11223344 * 0x55667788)
'0x5b736a60117d820'
```

Ok, so if you notice the Carry Flag and Overflow Flag both of them
have been set because the `edx` register is being used.

Now, let me discuss the division instruction DIV(Unsigned Divide). 
Division is like multiplication has a very interesting format. So when
you want to do
8-bit division you divide `ax` by 8-bit number which can be in a 
register or refferenced by memory, Then the **quotient** would be in 
`al` register while the remainder would be in `ah` register. When
we do this for 16-bit we use the combination of `dx` and `ax` to
denote the whole number, the quotient in the `ax` and remainder in
the `dx`. And when you do this for 32-bit `edx` and `eax` denote
the whole number and the quotient in the `eax` and the remainder in
the `edx`. 

Ok, the best way as you know is to illustrate it with the example

```
        ; division using r/m16

        mov dx, 0x0
        mov ax, 0x7788
        mov cx, 0x2
        div cx

        mov ax, 0x7788 + 0x1
        mov cx, 0x2
        div cx
```

I have two examples of division. So what I am doing in the first 
example I actually have `dx` set to zero so I am going to do 16-bit
division, `ax` is 0x7788 and I am going to divide it by content of
the `cx` register which is 0x2.

Let's call `disassemble` and now we are interested in this part of 
the code

```
   0x080480da <+90>:	mov    dx,0x0
   0x080480de <+94>:	mov    ax,0x7788
   0x080480e2 <+98>:	mov    cx,0x2
   0x080480e6 <+102>:	div    cx
```

And I show you the multiple ways to set up breakpoint. You can setup
in any way you are comforable with.

```
(gdb) break *0x080480da
Breakpoint 3 at 0x80480da
```

Let's continue

```
(gdb) c
Continuing.
Dump of assembler code from 0x80480da to 0x80480e9:
=> 0x080480da <_start+90>:	mov    dx,0x0
   0x080480de <_start+94>:	mov    ax,0x7788
   0x080480e2 <_start+98>:	mov    cx,0x2
   0x080480e6 <_start+102>:	div    cx
End of assembler dump.

Breakpoint 3, 0x080480da in _start ()
4: $eflags = [ CF IF OF ]
3: /x $ebx = 0x55667788
2: /x $eax = 0x55e7e500
1: /x $edx = 0x12b867
```

And now let me do three `nexti` and also verify `cx`

```
(gdb) nexti 
Dump of assembler code from 0x80480e6 to 0x80480f5:
=> 0x080480e6 <_start+102>:	div    cx
   0x080480e9 <_start+105>:	mov    ax,0x7789
   0x080480ed <_start+109>:	mov    cx,0x2
   0x080480f1 <_start+113>:	div    cx
   0x080480f4 <_start+116>:	mov    eax,0x1
End of assembler dump.
0x080480e6 in _start ()
4: $eflags = [ CF IF OF ]
3: /x $ebx = 0x55667788
2: /x $eax = 0x55e77788
1: /x $edx = 0x120000
(gdb) print/x $ecx
$1 = 0x2
```

And now I am going to divide by `cx`. So let's do `nexti` again

```
(gdb) nexti
Dump of assembler code from 0x80480e9 to 0x80480f8:
=> 0x080480e9 <_start+105>:	mov    ax,0x7789
   0x080480ed <_start+109>:	mov    cx,0x2
   0x080480f1 <_start+113>:	div    cx
   0x080480f4 <_start+116>:	mov    eax,0x1
End of assembler dump.
0x080480e9 in _start ()
4: $eflags = [ CF AF IF OF ]
3: /x $ebx = 0x55667788
2: /x $eax = 0x55e73bc4
1: /x $edx = 0x120000
```

And if you remember what I told you is that the result of division
16-bit the quotient would be in `ax` register while the remainder in
the `dx`. So the quotient in `ax` which is 0x3bc4 and the remainder
would be `dx` which evaluates to 0x0000. And if you'd like you could
verify the same thing as well.

```
>>> hex(0x7788 // 2)
'0x3bc4'
>>> hex(0x3bc4 * 2)
'0x7788'
```

which is exactly what is in `ax` and `dx` registers.

```
(gdb) print/x $ax
$2 = 0x3bc4
(gdb) print/x $dx
$3 = 0x0
```

Now, in the next division what I've done is just used one more i.e.
0x7789 instead of 0x7788 as value for `ax` 

```
=> 0x080480e9 <+105>:	mov    ax,0x7789
   0x080480ed <+109>:	mov    cx,0x2
   0x080480f1 <+113>:	div    cx
```

And What it really means is now we have remainder 1 in `dx`. Which
means `ax` should still contain 0x3bc4 value after the division and
`dx` would contain 1. Let's go through this applying 3 times `nexti`

```
(gdb) nexti
Dump of assembler code from 0x80480f4 to 0x8048103:
=> 0x080480f4 <_start+116>:	mov    eax,0x1
   0x080480f9 <_start+121>:	mov    ebx,0xa
   0x080480fe <_start+126>:	int    0x80
   0x08048100:	add    eax,0x33441122
End of assembler dump.
0x080480f4 in _start ()
4: $eflags = [ CF AF IF OF ]
3: /x $ebx = 0x55667788
2: /x $eax = 0x55e73bc4
1: /x $edx = 0x120001
```

And now if I go back to printing `ax` I will have the same value, 
however, when I print `dx` I would have the remainder 0x1 in it as
expected.

```
(gdb) print/x $ax
$4 = 0x3bc4
(gdb) print/x $dx
$5 = 0x1
```

So this is how multiplication and division works. I would advise you
to create more examples for yourself. So it's not really complicated.
Just You need to get an idea in mind that multiplication and division
requires **two** registers `eax` and `edx` together to get the job
done. And you need to closely watch the Carry and Overflow flags
for multiplication to figure out if there is an overflow.

Now, apart from just MUL and DIV which are for Unsigned Arithmetic,
you also have Signed Arithmetic where the sign of positive or 
negative is taken into account using IMUL and IDIV which I leave to
you as an exercise to try out.






