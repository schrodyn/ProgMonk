#Control Instructions

Hello all! Welcome to this video in which we will discuss the control
instructions in assembly language.

Now, control instructions essentilally control the **flow of the program**. And the direction in which program should going is typically
based on different "events" or calculations such as calculation
lead to 0. And the programmer may decide to do something else
based on that event. Now, in assembly language control instructions
depend on alot on the flags to determine what to do.

Now, as far as branching is conserned, there are two options we have:
one is **unconditional**, and the other is **conditional**. Let's look
at both of these. 

Now, unconditional branching is actually accomplished using `jmp` 
instruction and is comparable to the `goto` statement in C programming
if you've done it before. The unconditional jumping has two types.
One is called **Near Jump** and the other is **Far Jump**. The Near
Jump is when you want to jump to an instruction which is there in 
the current code segment which is the segment doesn't need to be 
changed. While the Far Jump is when you jump to the totally different 
segment.

The Near Jump has the subtype called the **Short Jump** which is 
basically jumping to an instruction -128 to +127 from current position.

Conditional jumps are more interesting because this is where decision
making based on different events happening in your code. So `Jxx`.
The reason I mentioned `Jxx` is there are tons and tons of conditionals
available, so `jz` is jump if zero, `jnz` is jump if non zero, `ja`
is jump on above and all of this uses flags to decide jump or not, so,
for example, `jz` will jump if zero flag is set. Intel Manual is the
best refference.

And let me take you directly to the intel manual, and this is Volume 2
of Intel Manual, and if you search for `Jcc` -- Jump if Condition Is Met you would see a ton of them. There is really a lot of them and you
need to famiralize with many of them. And many of them depends entirely
on refference states of the flags. So, please, reffer to the Volume Two
of the Manual in order to figure out exhaustive listing of all jumps
allowed.

Now, one of the things with conditional jumps is they **can not** be 
used with Far Jumps. And the Intel Manual gives you interesting 
solution, it is wherenever you need to do far jump based on a 
conditional you could use this format

```
JNZ label1
JMP Far_Label

label1:
```

So in reality if you think about it hitting `JMP Far_Label` this 
actually mean unconditional far jump. Now to be honest the Intel Manual
is the best refference. There is no point to copy paste from it here.
You should read appropriate pages here.

Now, let me actually jumping and show you the usage of both
unconditional and conditional jumps. Let's open `Control.nasm`.

Now, what I've done is modified hello world program which we had.
So let me show you how it works. Let's trace through the `_start`
because it is an entry point

```
global _start

section .text
_start:

        jmp Begin

NeverExecute:

        mov eax, 0x10
        xor ebx, ebx

Begin:
        mov eax, 0x5
```

So at the beginning we have an instruction which says just to jump to
the label `Begin`. Now if you notice I have little section of code
starting from label `NeverExecute` which basically is never going to
execute. The reason is the `jmp` instruction is unconditional which
means if you hit `jmp` it is going to happen. There is no way `jmp`
instruction to be ignored. And that's the reason why this

```
NeverExecute:

		mov eax, 0x10
		xor ebx, ebx
```

piece of code is never going to be executed. Now, what I want to do is
print "Hello World" 5 times using conditional jumps. There are 
multiple ways to do this. I want to use zero flag.

```
Begin:
        mov eax, 0x5

PrintHW:

        push eax

        ; Print hello world using write syscall
        mov eax, 0x4
        mov ebx, 1
        mov ecx, message
        mov edx, mlen
        int 0x80

        pop eax
        dec eax
        jnz PrintHW


        mov eax, 0x1
        mov ebx, 0xa            ; sys_exit syscall
        int 0x80

section .data

        message: db "Hello World! "
```

Now, here two key concepts you need to consider. The first is we are
using `eax` register to store the number 5 and we should use the 
same register `eax` to invoke the syscall, because the syscall number
needs to be in `eax` register. What this means is that we essentially
end up overwritting counter. So there are multiple ways for remaining
consistent state. Very simple way is to use the stack. So what we 
want to do is before we invoke the syscall we store the state of
the program (which could be multiple registers, but in our case just
`aex` register by invoking `push eax`) on the stack. 

Now, after we are done with executing the syscall we would like to
recover that value of `eax` so that we can decrement it and check if
we have finished running the print "Hello Wolrd" part of the program.

Now, let me go ahead and use the gdb to trace through the program. So
that we know exactly how the states of the program has changed over
time.

```
gdb -q ./Control
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
Breakpoint 1 at 0x8048080
(gdb) run
Starting program: /home/osboxes/SLAE-Code/Control/Control 

Breakpoint 1, 0x08048080 in _start ()
(gdb) disassemble 
Dump of assembler code for function _start:
=> 0x08048080 <+0>:	jmp    0x8048089 <Begin>
End of assembler dump.
```

First we encounter is the jump instruction. This is what we are going
to do. What we'd like to monitor is `eax` register. So 

```
(gdb) display/x $eax
(gdb) define hook-stop
>disassemble $eip,+10
>end
```

And we'll do `nexti`

```
(gdb) nexti
Dump of assembler code from 0x8048089 to 0x8048093:
=> 0x08048089 <Begin+0>:	mov    eax,0x5
   0x0804808e <PrintHW+0>:	push   eax
   0x0804808f <PrintHW+1>:	mov    eax,0x4
End of assembler dump.
0x08048089 in Begin ()
1: /x $eax = 0x0
```

So unconditional jump has happened and now we are at `Begin` which
is going to move the value 0x5 into `eax`.

```
(gdb) nexti
Dump of assembler code from 0x804808e to 0x8048098:
=> 0x0804808e <PrintHW+0>:	push   eax
   0x0804808f <PrintHW+1>:	mov    eax,0x4
   0x08048094 <PrintHW+6>:	mov    ebx,0x1
End of assembler dump.
0x0804808e in PrintHW ()
1: /x $eax = 0x5
```

Ok, 5 has been moved. Now we are going to push the value of `eax` on
the stack. So let me examine the stack as well. Let me examine just 
the first double word which is 4 byte stored on the stack.

```
(gdb) x/xw $esp
0xbffff3b0:	0x00000001
```

Now once we executed this instruction `eax` will be stored on the top
of the stack. So let me go to the `nexti` and now I will look at the
stack

```
(gdb) x/xw $esp
0xbffff3ac:	0x00000005
```

We would notice that the value 5 is being stored on the top of the
stack. Now the next series of instructions is something that go ahead
and call the syscall so we just quickly step through it.

```
(gdb) 
Hello World! Dump of assembler code from 0x80480a5 to 0x80480af:
=> 0x080480a5 <PrintHW+23>:	pop    eax
   0x080480a6 <PrintHW+24>:	dec    eax
   0x080480a7 <PrintHW+25>:	jne    0x804808e <PrintHW>
   0x080480a9 <PrintHW+27>:	mov    eax,0x1
   0x080480ae <PrintHW+32>:	mov    ebx,0xa
End of assembler dump.
0x080480a5 in PrintHW ()
1: /x $eax = 0xd
```

And if you notice the first Hello World! was printed. And now we pop
the value back into `eax` because if you notice `eax` has the value
0xd in it. After the `pop` you have the value 0x5 again

```
(gdb) nexti
Dump of assembler code from 0x80480a6 to 0x80480b0:
=> 0x080480a6 <PrintHW+24>:	dec    eax
   0x080480a7 <PrintHW+25>:	jne    0x804808e <PrintHW>
   0x080480a9 <PrintHW+27>:	mov    eax,0x1
   0x080480ae <PrintHW+32>:	mov    ebx,0xa
End of assembler dump.
0x080480a6 in PrintHW ()
1: /x $eax = 0x5
```

Let's call `nexti` and see current flags

```
(gdb) nexti
(gdb) display $eflags
2: $eflags = [ IF ]
```

You will see that zero flag is not set. So let's go continue run the
program until `eax` becomes zero.

```
(gdb) 
Dump of assembler code from 0x80480a6 to 0x80480b0:
=> 0x080480a6 <PrintHW+24>:	dec    eax
   0x080480a7 <PrintHW+25>:	jne    0x804808e <PrintHW>
   0x080480a9 <PrintHW+27>:	mov    eax,0x1
   0x080480ae <PrintHW+32>:	mov    ebx,0xa
End of assembler dump.
0x080480a6 in PrintHW ()
2: $eflags = [ IF ]
1: /x $eax = 0x1
```

Now the next instruction is going to be `dec eax` which should make
zero and you will actually see that the flags will change as well.

```
(gdb) nexti
Dump of assembler code from 0x80480a7 to 0x80480b1:
=> 0x080480a7 <PrintHW+25>:	jne    0x804808e <PrintHW>
   0x080480a9 <PrintHW+27>:	mov    eax,0x1
   0x080480ae <PrintHW+32>:	mov    ebx,0xa
End of assembler dump.
0x080480a7 in PrintHW ()
2: $eflags = [ PF ZF IF ]
1: /x $eax = 0x0
```

There we go, `eax` is now zero and if you notice the Zero Flag is set
which means basically this would actually go to the other one and we
move on and go ahead finishing off the remaining part.

Now, you may notice interestingly that `jnz` has been replaced by
`jne`

```
(gdb) disassemble 
Dump of assembler code for function PrintHW:
   0x0804808e <+0>:	push   eax
   0x0804808f <+1>:	mov    eax,0x4
   0x08048094 <+6>:	mov    ebx,0x1
   0x08048099 <+11>:	mov    ecx,0x80490b8
   0x0804809e <+16>:	mov    edx,0xd
   0x080480a3 <+21>:	int    0x80
   0x080480a5 <+23>:	pop    eax
   0x080480a6 <+24>:	dec    eax
=> 0x080480a7 <+25>:	jne    0x804808e <PrintHW>
   0x080480a9 <+27>:	mov    eax,0x1
   0x080480ae <+32>:	mov    ebx,0xa
   0x080480b3 <+37>:	int    0x80
End of assembler dump.
```

And I leave this as an exercise to figure out why and how, why nasm
did this?

Anyway, the exercise I want you to do is to investigate on other
instructions set which can help us with doing loops that we did
right now with `jnz`, `jne` -- exactly `LOOP`, `LOOPZ`, `LOOPNZ` etc.




