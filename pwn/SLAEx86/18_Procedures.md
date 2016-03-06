#Procedures

Welcome back! In this video we will look at procedures in assembly
language.

So a **procedure** is a set of instructions grouped together. So,
as an example, in Hello World all those instruction sets that we
had, which is setting up the register with the right value for the
syscall number, the pointer to the message you want to print, 
the length etc. all belong together. So you could think about simple
procedure which would just print "hello world".

Now, in NASM procedures are just defined by **labels**. There is no
specific keyword for procedures in NASM. It's important to note that
other assemblers do have keywords such as `PROC` which are available
to create procedures. Now, in reality the reason why it doesn't matter
is in assembly language there is really no procedures for C. It is just
another piece of code which you want to call in specific way. Now, 
calling a procedure happens using the `call` instruction followed
by procedure name

```
CALL Procedure_Name
```

Now, how a procedure looks like. You first define a procedure name
which is nothing but a label and then after that you write all the code
you want to be executed, when the procedure would be called. And once
it's done we end procedure with the `ret` instruction or return
instruction which tells the CPU "Hey, we now have done executing the
procedure and we go back to next instruction from where this procedure
was called.

```
ProcedureName:

	...code...
	...code...
	...code...
	RET
```

Now, let me trying and show you a piece of code to make you understand
what I mean here. Open `Procedure.nasm`.

Now, this is again the same hello world program but I've done one
interesting modification. I've moved code responsible for printing
in procedure using label `HelloWorldProc`, and after the printing
is done I call the return instruction

```
HelloWorldProc:

        ; Print hello world using write syscall

        mov eax, 0x4
        mov ebx, 1
        mov ecx, message
        mov edx, mlen
        int 0x80
        ret   ; signifies end of procedure


_start:

        mov ecx, 0x10

PrintHelloWorld:

        push ecx
        call HelloWorldProc
        pop ecx
        loop PrintHelloWorld
```

There are two things which you need to keep in mind about procedures.
When `ret` instruction called how the CPU knows what next instruction
to execute? This is where what assembly language instruction does
that as soon as `call` command is hit the address of the **next**
instruction is actually pushed on the stack. And the `ret` instruction
pops that value from the stack into the `eip` register so that CPU
knows this is the next instruction to be executed after the procedure
returns. This is the only key concept you need to keep in mind.

Now, let's go ahead and follow through gdb to see if all of that true.

```
gdb -q ./Procedure
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
(gdb) run
```

We can look at different functions

```
(gdb) info functions 
All defined functions:

Non-debugging symbols:
0x08048080  HelloWorldProc
0x08048097  _start
0x0804809c  PrintHelloWorld
0xb7fff400  __kernel_sigreturn
0xb7fff40c  __kernel_rt_sigreturn
0xb7fff414  __kernel_vsyscall
```

And if you notice now basically all your labels you have listed 
including `HelloWorldProc`. Let's disassemble it

```
(gdb) disassemble HelloWorldProc 
Dump of assembler code for function HelloWorldProc:
   0x08048080 <+0>:	mov    eax,0x4
   0x08048085 <+5>:	mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490b4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	ret    
End of assembler dump.
```

or

```
(gdb) disassemble 0x08048080
Dump of assembler code for function HelloWorldProc:
   0x08048080 <+0>:	mov    eax,0x4
   0x08048085 <+5>:	mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490b4
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	ret    
End of assembler dump.
```

Now, let's step through the program.

```
(gdb) nexti
0x0804809c in PrintHelloWorld ()
(gdb) disassemble 
Dump of assembler code for function PrintHelloWorld:
=> 0x0804809c <+0>:	push   ecx
   0x0804809d <+1>:	call   0x8048080 <HelloWorldProc>
   0x080480a2 <+6>:	pop    ecx
   0x080480a3 <+7>:	loop   0x804809c <PrintHelloWorld>
   0x080480a5 <+9>:	mov    eax,0x1
   0x080480aa <+14>:	mov    ebx,0xa
   0x080480af <+19>:	int    0x80
End of assembler dump.
```

So the next command is just push `ecx` on the stack.

```
(gdb) define hook-stop
>disassemble 
>end
```

Let's go `nexti` again

```
(gdb) nexti
Dump of assembler code for function PrintHelloWorld:
   0x0804809c <+0>:	push   ecx
=> 0x0804809d <+1>:	call   0x8048080 <HelloWorldProc>
   0x080480a2 <+6>:	pop    ecx
   0x080480a3 <+7>:	loop   0x804809c <PrintHelloWorld>
   0x080480a5 <+9>:	mov    eax,0x1
   0x080480aa <+14>:	mov    ebx,0xa
   0x080480af <+19>:	int    0x80
End of assembler dump.
0x0804809d in PrintHelloWorld ()
```

So we are now just about to call the procedure and as soon as the
`call` instruction shifts control to the procedure `HelloWorldProc`
the next instruction's address which is basically `0x080480a2` could
actually be pushed on to the stack. So let's also go ahead and
look at what is there on the top of the stack right now.

```
(gdb) x/xw $esp
0xbffff3ac:	0x00000010
```

It's just the value we pushed (`ecx`). Now as soon as I run the next
instruction 

```
(gdb) nexti
```

And now if I look at the stack 

```
(gdb) x/2xw $esp
0xbffff3a8:	0x080480a2	0x00000010
```

You will actually notice that the next value pushed in 0x080480a2 was
nothing but the address of the next instruction to be executed once
we return from the procedure. 

Let's go ahead and make `nexti` until `ret` instruction. One thing
is let's again go back to observing our stack

```
(gdb) x/2xw $esp
0xbffff3a8:	0x080480a2	0x00000010
```

and as soon as `ret` instruction is hit, basically it's equvivalent to
`pop $eip` which go ahead and pop this 0x080480a2 value inside `eip`
register and the next instruction to be executed would now basically
point to `pop ecx`. Let's see that it is true.

```
(gdb) nexti
Dump of assembler code for function PrintHelloWorld:
   0x0804809c <+0>:	push   ecx
   0x0804809d <+1>:	call   0x8048080 <HelloWorldProc>
=> 0x080480a2 <+6>:	pop    ecx
   0x080480a3 <+7>:	loop   0x804809c <PrintHelloWorld>
   0x080480a5 <+9>:	mov    eax,0x1
   0x080480aa <+14>:	mov    ebx,0xa
   0x080480af <+19>:	int    0x80
End of assembler dump.
0x080480a2 in PrintHelloWorld ()
```

And if you notice we are now at `pop ecx`. And right now top of the 
stack would only contain the value 0x10. So this is a very very 
keypoint you need to remember is that `eip` is pushed on the stack
when a call happens and `eip` is popped back from the stack when
return happens. This is really essence of procedures.

The rest of code should be familar to you.

There are multiple ways to send arguments to procedures. One of them 
is to pass them by a registers, the other is to pass them by the 
stack, and the third is pass as data structure in memory while those
refferences to the memory can be passed by the registers or stack.

When we look at how to use `libc` and all of that in assembly we will
talk about passing by the stack. Passing by a registers as you imagine
is clearly trivial.

Now, the other important aspect is inside the procedure a ton of 
things can be happening: registers may be changed, flags may be 
changed, the whole state of the program could be changed. This would
be entirely desirable such in this situations where you would want to
preserve the state of the current registers, flags when you enter the
procedure, so that you can restore them after the procedure.

So the simple example is basically what we have in our code which is
we preserve the value of `ecx` before we call a procedure because 
our procedure uses `ecx` as well so we preserve `ecx` and then we
restore `ecx` after the procedure returns.

There are different instruction sets you will look that how this
scheme can be done. 

`PUSHAD/POPAD` -- Saving/Restoring Registers

`PUSHFD/POPFD` -- Saving/Restoring Flags

`ENTER/LEAVE + RET` -- Saving/Restoring frame pointers

Apart from the flags as well as registers we
could save **frame pointers** which is basically `ebp` using the
`enter` and `leave` instruction sets.

Now I would want you to investigate `enter`/`leave` along with the
`ret` instructions and look at the next exercise which is you need
to write a program which now saves all registers and flags before
calling a procedure and it also should save/restore the frame pointer.



