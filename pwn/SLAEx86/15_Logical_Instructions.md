#Logical Instructions

Hello all! In this video we will look at logical instructions in
assembly.

Now, logical instructions are similar to what you've seen in other
programming languages like C, Python etc. The very base instructions
of course should be `and`, `or`, `xor` and `not`. Now, the 
*destination* operand for `and` can be a **register** or **memory 
location**, while the *source* can be a **register**, **memory**, or 
**immediate value**.

```
AND r/m, r/m/imm (8,16,32 bits)
```

It's 8, 16 and 32 bits allowed. However, you can not do `and` between
two memory locations. Now the relationship is that this is going to 
be "bitwise AND" operation and the output of that is stored in the
*destination* as the first argument. Similarly, you have `or`,`xor`
and `not` which is inverse value.

So let me give you a code to see how this fits together. Let's open
`Logical.nasm`

So I have illustrated the usage of `and`, `or`, `xor` and `not` as part
of this exercise file

```
        ; AND usage

        mov al, 0x10
        and al, 0x01

        and byte [var1], 0xaa
        and word [var2], 0x1122
```

note that the values `var1`, `var2` and `var3` mentioned at the bottom

```
section .data

        var1    db      0xaa
        var2    dw      0xbbcc
        var3    dd      0x11223344
```

and the first instruction is to do an `and` operation between the value
0xaa and `var1`. And store the result in the location by `var1`.

```
and byte [var1], 0xaa
```

in the second `and` operation

```
and word [var2], 0x1122
```

we basically going ahead and doing an `and` between `var2` and value
0x1122, of course the destination for storing the result is basically
`var2`. Similarly I have illustration of `or`, `xor` and `not`.

So let's actually go ahead and assemble and link it and run with gdb

```
gdb -q ./Logical
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
(gdb) run
```

Now let's disassemble

```
(gdb) disassemble 
Dump of assembler code for function _start:
=> 0x08048080 <+0>:		mov    al,0x10
   0x08048082 <+2>:		and    al,0x1
   0x08048084 <+4>:		and    BYTE PTR ds:0x80490d0,0xaa
   0x0804808b <+11>:	and    WORD PTR ds:0x80490d1,0x1122
   0x08048094 <+20>:	mov    al,0x10
   0x08048096 <+22>:	or     al,0x1
   0x08048098 <+24>:	or     BYTE PTR ds:0x80490d0,0xaa
   0x0804809f <+31>:	mov    eax,0x0
   0x080480a4 <+36>:	or     eax,0x0
   0x080480a7 <+39>:	xor    DWORD PTR ds:0x80490d3,0x11223344
   0x080480b1 <+49>:	xor    DWORD PTR ds:0x80490d3,0x11223344
   0x080480bb <+59>:	mov    eax,0xffffffff
   0x080480c0 <+64>:	not    eax
   0x080480c2 <+66>:	not    eax
   0x080480c4 <+68>:	mov    eax,0x1
   0x080480c9 <+73>:	mov    ebx,0xa
   0x080480ce <+78>:	int    0x80
End of assembler dump.
```

So in this case what we are doing is we want to track `al`, we want
to track `eax` register, and we want to track `var1`, `var2`, and 
`var3` for this entire exercise.

So we go ahead and type

```
(gdb) display/x $al
1: /x $al = 0x0
(gdb) display/x $eax
2: /x $eax = 0x0
```

Then we define hook inside which I would like to **examine** memory,
and if you remember `var1` is 1 byte, `var2` is 2 bytes, and `var3` is
4 bytes. So in order to examine it

```
(gdb) define hook-stop
>x/xb &var1
>x/xh &var2
>x/xw &var3
>disassemble $eip,+10
>end
```

So let's me type `nexti`

```
(gdb) nexti
0x80490d0 <var1>:	0xaa
0x80490d1 <var2>:	0xbbcc
0x80490d3 <var3>:	0x11223344
Dump of assembler code from 0x8048082 to 0x804808c:
=> 0x08048082 <_start+2>:	and    al,0x1
   0x08048084 <_start+4>:	and    BYTE PTR ds:0x80490d0,0xaa
   0x0804808b <_start+11>:	and    WORD PTR ds:0x80490d1,0x1122
End of assembler dump.
0x08048082 in _start ()
2: /x $eax = 0x10
1: /x $al = 0x10
```

As you can see the first instruction was to move 0x10 to `al`, so what
is actually done. The next instruction is going to be `and` `al` with
the number 0x1. If you write them bitwise you will actually see that
there is no overlape between two bits which is the `and` is going to
go ahead and result in zero.

```
(gdb) nexti
0x80490d0 <var1>:	0xaa
0x80490d1 <var2>:	0xbbcc
0x80490d3 <var3>:	0x11223344
Dump of assembler code from 0x8048084 to 0x804808e:
=> 0x08048084 <_start+4>:	and    BYTE PTR ds:0x80490d0,0xaa
   0x0804808b <_start+11>:	and    WORD PTR ds:0x80490d1,0x1122
End of assembler dump.
0x08048084 in _start ()
2: /x $eax = 0x0
1: /x $al = 0x0
```

Now if you look at the next statement we want to do `and` with 
basically `var1` and value 0xaa. Now 0xaa is exactly the value of 
`var1` and hence this `and` operation would basically yield exactly
the same result which is 0xaa.

```
(gdb) nexti
0x80490d0 <var1>:	0xaa
0x80490d1 <var2>:	0xbbcc
0x80490d3 <var3>:	0x11223344
Dump of assembler code from 0x804808b to 0x8048095:
=> 0x0804808b <_start+11>:	and    WORD PTR ds:0x80490d1,0x1122
   0x08048094 <_start+20>:	mov    al,0x10
End of assembler dump.
0x0804808b in _start ()
2: /x $eax = 0x0
1: /x $al = 0x0
```

And this is the reason why `var1` remains unchanged even though it
was overwritten. Now, the second case we are going now to do bitmask
anding `var2` with the value 0x1122. Of course this will have some 
effect.

```
(gdb) nexti
0x80490d0 <var1>:	0xaa
0x80490d1 <var2>:	0x1100
0x80490d3 <var3>:	0x11223344
Dump of assembler code from 0x8048094 to 0x804809e:
=> 0x08048094 <_start+20>:	mov    al,0x10
   0x08048096 <_start+22>:	or     al,0x1
   0x08048098 <_start+24>:	or     BYTE PTR ds:0x80490d0,0xaa
End of assembler dump.
0x08048094 in _start ()
2: /x $eax = 0x0
1: /x $al = 0x0
```

The rest are the same.

Now, apart from these you also have other instructions such as

  * `SAR` -- Shift Arithmetic Right
  * `SHR` -- Shift Logical Right
  * `ROR`
  * `ROL`

  etc.

I leave it as an exercise to you to looking more of them. The `and`,
`or`, `xor`, `not` are probably the most fundamental ones which you
will encounter when you will do shellcoding.
