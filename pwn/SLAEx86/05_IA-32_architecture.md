#IA-32 Architecture

In this video we will talk about IA-32 Architecture. Now before we
begin let's trying visualize a computer in the form of basic blocks.

The first block you would build is the CPU or the Central Processor
Unit which is very different instructions are decoded and executed.

Then we would have a **Memory block** where program data etc. get 
stored.

The next is **I/O Devices**. Like your monitor, LAN card etc.

And then finally **System Bus** which connects them all.

Now let's take the firct CPU and understand that better.

You could break up the CPU into the **control unit** which is
responsible for retrieving and decoding instructions, and
then you have the **execution unit** where actually execution of
the instruction happens. 

Now as you can imagine while the CPU executes it would require
temporary storage of data etc. which you would wat to use in 
the different computations. This is where **registers** come in
which you could visualize as internal memory locations inside
the CPU which act as **temporary variables**.

Now while different computation operations are going on that could
be different divence(?) such as a computation leading to a zero
value and the CPU would want to indicate all of this to the programmer
or to the program. And it does this by using **flags** register which
is the part of CPU.

Now let actually look at the registers in more detail. Now here is the
logical diagram of IA-32 registers which primarely consists of the
general purpose registers, Segment registers, Flags, EIP, Floating
Point Unit Registers which now is pretty much integrated into CPU,
and MMX and XMM registers.

Let's look at each of this to understand them better.

There are **eight** general purpose registers general purpose 
registers. The first four are `eax`,`ebx`,`ecx` and `edx`. Now for
IA-32 as you probably already guessed they are 32 bit wide.
Now the interesting part is even in those 32 bits we can go ahead
and access the lower 16 bits using the `ax` register. Just basically
nothing but subset of `eax`. Now more interestingly we can actually
break down `ax` further in two `AL`(A Low) which is basically
the bit 0 to bit 7 and `AH`(A High) which is bit 8 to bit 15.

Now the best part is that `ebx`,`ecx` and `edx` work exactly the
same and in our assembly program we can refference not just `eax`
but also `ax`,`ah`,`al`,`bh`,`bl` and all of that. Which gives
us a lot of freedom to clear around with these registers.

Apart from these four we have four more which is `esp`,`ebp`,`esi` and
`edi`. Now unlike the first four this can only be further broken
up in two 16-bit half. So for `esp` you have `sp`, for `ebp` it is `bp`
, for `esi` it's `si` and `edi` it is `di`.

So this eight are what constitute the **general purpose registers** on
IA-32.

Now let's look at the common functionality for which these registers
are used. `eax` is called `accumulator register` and typically is
used for storing operands and result data from different calls like
a syscalls etc. `ebx` is the `base register` and more often than not
points to data. `ecx` is used in loops as counter it's called as 
`counter register`, `edx` again is the data or an I/0 pointer it's 
called `data register`. `esi` and `edi` basically again typically
used as data pointer registers for memory operations. When we will
look at string manipulation in assembly we will actually use `esi` and
`edi` along with **direction flag**. `esp` or the `stack pointer 
register`. `esp` is probably only one which we would not go ahead
and arbitrarely use for any purpose. `esp` is specifically used just
for stack manipulation. Now if you do not know what the stack is 
what it stands for etc. do not worry, we will cover that in the latter
video. Finally, `ebp` which again is actually used as a data pointer
register (usually in stack).

Now apart from these eight general purpose registers you also have
**segment registers** which are `cs`,`ds`,`ss`,`es`,`fs` and `gs`.
Now the `cs` is *code segment*, `ds` is *data segment*, `ss` is
*stack segment* and `es`,`fs` and `gs` again point to different
data segments.

Now the using of these registers, you know the segment registers, 
depends very much on the **memory model** we are adopting. So you
know the usage would be very different if we had taken a **flat memory
model** or a **segmented memory model**. Again if the different 
memory models is something new to you, don't worry we will cover
this in the next video.

Now apart from these you basically have the **eflags** register which
is used to indicate different things happening in the CPU. So as
an example if an instruction leads to a zero value that the **zero 
flag** get set. We will take up the flag register in more detail when
we'll look at conditional statements in assembly language.

Apart from this you have `eip` which is basically the holy-grail for
shellcoding and for exploit research and that contains instruction
pointer which is basically the **next** instruction to be executed.
`eip` is 32 bit wide and the segment registers are just 16-bit wide.

Now apart from these you also have the Floating Point Unit (FPU) or
x87. Now the floating point unit has different data registers for
manipulating floating point numbers and the size of these registers
is 80 bits. They are named as `ST(0)` to `ST(7)`. And interestingly 
these data registers behave like a stack. We'll take up more details
on FPU, `ST(0)` to `ST(7)` etc. when we take up FPU later on 
while programming on assembly.

Now apart from this Intel basically come up with a lot of other
extensions to the instruction set. SIMD or a Single Instruction
Multiple Data was one of the main enhancements done by Intel and
this instruction extensions include MMX instruction set, SSE, SSE2
and SSE3. Now these extension instruction sets use MMX and XMM
Registers.

Now what is this registers? Well, MMX actually care about(?) of the
FPU registers. So if you remember I had mentioned the FPU registers
are 80 bits wide. Well the first lower 64 bits is what is assigned
to the MMX registers. XMM are actually even bigger registers and
they are actually 128 bits in size. And you have `XMM0` to `XMM7`
which is eight XMM registers are available. Now we will look at some
of these registers when we'll look at more advanced assembly
language programming in later videos.

In these video all I wanted to do was introduce you to the different
registers and now coming to the lab exercise what I would like you
to do is inspect the general purpose, segment, EFLAGS, FPU. MMX, XMM
registers on your Ubuntu system. Look at the values they hold as far
as general purpose registers. See if you can look at the lower 16 bit
separately like `ax` or even further with `ah`, `al` etc.
