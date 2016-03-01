#Exercise 1.2.1: IA-32 Architecture

Hello! So in the theory class what we talked about was the general
purpose registers, segment registers, EFLAGS, FPU, MMX and all of that.
But now the question arises how do you look at all of these CPU 
registers, right? So to do this exercise let's log on to the Ubuntu
system and here is the thing, right, why your system is running?
At the only where you can analyze CPU registers is the context of
running program. And for this we require a program such as a 
**debugger** which can help us  pose and fliss(?) the program and in
that frozen state allowing us to analyze CPU registers etc. Now
on Linux GDB or the GNU Debugger is the de facto debugging tool which
is available. Now I also have related series on GDB and probably post 
the link when we need to start assembly instruction set. And at that
time you can go ahead and look at gdb in more depth. Anyway what we
are going to do is let's pick up one of the programs on the system.
So let's pick up `/bin/bash` and run `gdb` on it.

```
$ gdb /bin/bash
```

And this is going to load up `gdb` for analysis with `bash`. Now let's
create the break point for the `main` function.

```
(gdb) break main
```

and let's run the program

```
(gdb) run
```

And as you noticed we've set up breakpoint.

Right now if you've never programmed before it doesn't make any sense.
Don't worry, in the later video I'm going to point you to `gdb` and
where to pick up all the basics.

Ok, now to look at the CPU registers we are going to use 

```
(gdb) info registers
eax            0x1	1
ecx            0xbffff404	-1073744892
edx            0xbffff394	-1073745004
ebx            0xb7fa1ff4	-1208344588
esp            0xbffff35c	0xbffff35c
ebp            0xbffff368	0xbffff368
esi            0x0	0
edi            0x0	0
eip            0x805eba6	0x805eba6 <main+6>
eflags         0x246	[ PF ZF IF ]
cs             0x73	115
ss             0x7b	123
ds             0x7b	123
es             0x7b	123
fs             0x0	0
gs             0x33	51
```

Now if you notice on the left handside this has the register names such
as `eax` and then here you have the value inside the register. And if
you notice all CPU registers are listed here.

Now there are a bunch of things which you probably want to look at.

You would remember I have said that each of this registers can be 
broken up into further 16 bit registers. So `eax` actually can have
`ax` which is accessed separately. And one of the way is in which you 
can print the contents of the register is to use `display` command.

```
(gdb) display /x $eax
1: /x $eax = 0x1
```

And now if I wanted to print `ax` which is the lower 16 bits I would be
going to do `$ax`

```
(gdb) display /x $ax
2: /x $ax = 0x1
```

And you would notice that it prints the value 1. Now which is 
of course going to be true because 0x1 is nothing but many zeroes and 
one. Now if you did the same thing for the `ecx` register 

```
(gdb) display /x $ecx
3: /x $ecx = 0xbffff404
(gdb) display /x $cx
4: /x $cx = 0xf404
```

You actually find that `f404` (lower part of `0xbffff404`) is what got
printed. 

Let's now see printing `ch` and `cl` separately. So you could 
basically do 

```
(gdb) display /x $ch
5: /x $ch = 0xf4
```

which you could be probably predicting. And then we are going to see 
`cl` which is gonna be `0x04`

```
(gdb) display /x $cl
6: /x $cl = 0x4
```

Now you could try out a couple of others if you want.

Same as in the case of `esp` where as I mentioned you can go ahead
and basically get the lower half nor the upper half. 

Now segment registers also mentioned here and they contain different
values depending as we said only on memory access schema which we are 
going to use. Linux basically uses the protected mode and flat
memory models which will be there in the later video.

And then after that you have the `eflags` and if you remember there
are different flags like Zero Flag (ZF), Sign Flag (SF) and all of 
that. When you basically look at `eflags` here is the value you can
convert this into individual bits and then compare it with the whole
bit mask to figure out which one flags are enabled and which one 
isn't. `gdb` makes this easy for you by specifically mentioning the 
flags which are enabled. 

```
eflags         0x246	[ PF ZF IF ]
```

So `ZF` is Zero Flag and similarly a couple of other flags are enabled
as well. So this is how we can look at the `eflags`.

So we've looked basically the general purpose registers, the segment
registers, eflags. Now `eip` which is the next instruction to be
executed is inside `eip` register. Now in order to look at what is
the next instruction you can use `disassemble` command and then we
can tell it where to start disassembly. 

```
(gdb) disassemble $eip
Dump of assembler code for function main:
   0x0805eba0 <+0>:	push   %ebp
   0x0805eba1 <+1>:	mov    %esp,%ebp
   0x0805eba3 <+3>:	push   %edi
   0x0805eba4 <+4>:	push   %esi
   0x0805eba5 <+5>:	push   %ebx
=> 0x0805eba6 <+6>:	and    $0xfffffff0,%esp
   0x0805eba9 <+9>:	sub    $0x100,%esp
   0x0805ebaf <+15>:	mov    0xc(%ebp),%eax
   0x0805ebb2 <+18>:	mov    0x10(%ebp),%edx
   0x0805ebb5 <+21>:	mov    %gs:0x14,%ecx
   0x0805ebbc <+28>:	mov    %ecx,0xfc(%esp)
   0x0805ebc3 <+35>:	xor    %ecx,%ecx
   0x0805ebc5 <+37>:	movl   $0x1,0x4(%esp)
   0x0805ebcd <+45>:	mov    %eax,0x34(%esp)
   0x0805ebd1 <+49>:	mov    %edx,0x2c(%esp)
   0x0805ebd5 <+53>:	movl   $0x812e400,(%esp)
```

And at this point this is what `$eip` actually contains

```
 => 0x0805eba6 <+6>: and    $0xfffffff0,%esp
```

exactly `0x0805eba6` this value.

Now depending on which linux system you are running you may find that
this values may change between invocations and all of that.

Now let's look at the other interesting registers which we want to
look at FPU. Interestingly we have FPU, MMX and XMM and we need 
actually do `info registers` but it's not shown. By default gdb 
does not show you Floating Point registers, MMX and XMM. To have
gdb to show you all of those registers we use a command called
`info all-registers`. And if you notice we have all general purpose 
registers, eip, eflags, segment registers **and** we have `st0` to 
`st7` which we have spoken about is basically the FPU data registers
stack and if you go further down you'll actually find that we have
`ymm0` to `ymm7` and then `mm0` to `mm7`.

So keep this in mind later on when we will start playing around 
programs we will use gdb extensively to monitor what the program
is doing how the registers are changing and as far as for shellcoding
all of this concerned exploit research on Linux you have to have
a command gdb. It is the de facto debugger you can use on this
platform.

Now one of the thing which we want to pick up and which we should talk
about later as well is when we do `disassemble main` the assembly
language on the right side can be shown in two different modes. One
is called AT&T and the other is Intel. The default on Linux is 
basically AT&T syntax. In the whole of this course we will use
Intel syntax.

And this is very easy to tell gdb to change everything to Intel syntax

```
(gdb) set disassembly-flavor intel
```

And once you do `disassemble main` now you have everything in the
Intel syntax.

```
(gdb) disassemble main
Dump of assembler code for function main:
   0x0805eba0 <+0>:	push   ebp
   0x0805eba1 <+1>:	mov    ebp,esp
   0x0805eba3 <+3>:	push   edi
   0x0805eba4 <+4>:	push   esi
   0x0805eba5 <+5>:	push   ebx
=> 0x0805eba6 <+6>:	and    esp,0xfffffff0
   0x0805eba9 <+9>:	sub    esp,0x100
   0x0805ebaf <+15>:	mov    eax,DWORD PTR [ebp+0xc]
   0x0805ebb2 <+18>:	mov    edx,DWORD PTR [ebp+0x10]
   0x0805ebb5 <+21>:	mov    ecx,DWORD PTR gs:0x14
   0x0805ebbc <+28>:	mov    DWORD PTR [esp+0xfc],ecx
   0x0805ebc3 <+35>:	xor    ecx,ecx
   0x0805ebc5 <+37>:	mov    DWORD PTR [esp+0x4],0x1
   0x0805ebcd <+45>:	mov    DWORD PTR [esp+0x34],eax
   0x0805ebd1 <+49>:	mov    DWORD PTR [esp+0x2c],edx
   0x0805ebd5 <+53>:	mov    DWORD PTR [esp],0x812e400
```






