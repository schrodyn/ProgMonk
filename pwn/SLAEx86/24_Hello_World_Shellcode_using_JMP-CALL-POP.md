#HelloWorld Shellcode using JMP-CALL-POP

Hello all! In this video let's try writing an interesting piece of
shellcode. So in module 1 we've looked at how to create "hello world"
program in assembly. How about creating shellcode which prints 
"hello world" on the screen?

And that's exactly what we are going to do in this video. The technique
we will use to create this shellcode is called **JMP-CALL-POP** 
technique. Let's move ahead to understand more about it.

Now, what we are going to do is I will take the "hello world" program
which was created in module 1 and first modify it to ensure that
all 0x00 opcode instructions go away and replaced. So all `mov eax,0x1`
and all of that will be replaced with equivalent instructions which
don't end up with 0x00 value in them.

The second thing which is very very important is that shellcode **can 
not** have any **hardcoded addresses**. And the reason is simple --
the same piece of shellcode is going to run in different environments
within different programs depending on if you've used with an exploit
or other circumstances. And if we hardcode addresses those addresses
would not be valid once in all of these different circumstances. And
it is important to create **dynamically figure out addresses** inside
our shellcode as when necessary.

And to do that the technique we will use is called **JMP-CALL-POP**.

```
JMP short Call_shellcode:

shellcode:
		pop ecx
		...
		...
		...

Call_shellcode:

		call shellcode:
		HelloWorld db "Hello World!"

```

Now, let me go ahead and first get writing the code before I show you
what **JMP-CALL-POP** is. Let's open `HelloWorld.asm`. Our modified 
version will be stored in `HelloWorldShellcode.nasm`.

```asm
global _start

section .text

_start:

        ; print hello world on the screen

        mov eax, 0x4
        mov ebx, 0x1
        mov ecx, message
        mov edx, mlen
        int 0x80


        ; exit the program gracefully

        mov eax, 0x1
        mov ebx, 0x5
        int 0x80



section .data

        message: db "Hello World!", 0xA
        mlen    equ  $-message
```

Now, the first thing what I want is to move all instructions which 
end up creating 0x00 values in our shellcode.

So `mov eax, 0x4` should become

```asm
xor eax, eax
mov al, 0x4
```

Now `mov ebx, 0x1` should become

```asm
xor ebx, ebx
mov bl, 0x1
```

The next line is tricky `mov ecx, message` which actually moves the
address of "hello world" string inside the `ecx` register. So let's
step it for a time.

The next instruction is `mov edx, mlen`. Now, `mlen` is the length of
this message. In our case `"Hello World!", 0xA` is 13 characters.
So we replace `mov edx, mlen` with

```asm
xor edx, edx
mov dl, 13
```

And similarly for `exit` routine we need to do

```
xor eax, eax
mov al, 0x1
```

and for `ebx` register we have

```
xor ebx, ebx
```

As you may see it is the same program as we had before. So let's 
verify that all 0x00 has been removed

```
$ objdump -d HelloWorld -M intel

HelloWorld:     file format elf32-i386


Disassembly of section .text:

08048080 <_start>:
 8048080:	31 c0                	xor    eax,eax
 8048082:	b0 04                	mov    al,0x4
 8048084:	31 db                	xor    ebx,ebx
 8048086:	b3 01                	mov    bl,0x1
 8048088:	b9 a0 90 04 08       	mov    ecx,0x80490a0
 804808d:	31 d2                	xor    edx,edx
 804808f:	b2 0d                	mov    dl,0xd
 8048091:	cd 80                	int    0x80
 8048093:	31 c0                	xor    eax,eax
 8048095:	b0 01                	mov    al,0x1
 8048097:	31 db                	xor    ebx,ebx
 8048099:	b3 05                	mov    bl,0x5
 804809b:	cd 80                	int    0x80
```

And if you notice all 0x0 has been removed. If you look at disassembly
on the right you would notice that inside `ecx` 
(`mov    ecx,0x80490a0`) we seem to be moving address that is 
something which we don't desire. Now let's go ahead and remedy this
because if we use this in our shellcode it won't work. Let's verify
this. I've created simple shell script called `shellcode-dump.sh`

```sh
#!/bin/sh

if [ $# -lt 1 ]
then
        echo "usage: $0 elf_file"
        echo -e "\nprints shellcode string fetched from objdump"

        exit 1
fi

objdump -d $1|grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d:|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g'
```

So to dump shellcode from `HelloWorld` elf-file now we can call

```
$ shellcode-dump.sh HelloWorld
"\x31\xc0\xb0\x04\x31\xdb\xb3\x01\xb9\xa0\x90\x04\x08\x31\xd2\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80"
```

Let's copy paste this shellcode in `shellcode.c`

```
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\x31\xc0\xb0\x04\x31\xdb\xb3\x01\xb9\xa0\x90\x04\x08\x31\xd2\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}

```

And compile it

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

run it, you notice that nothing happening and it is printing something
arbitrary, also things could go wrong such as `seg fault` and other
wells. So clearly this doesn't work. Let's go back to our assembly
language program.

Now, the problem is really that we want to refference `message` with
its address and this is not desirable. What we want is to be able to
figure out the address of `message` dynamically at runtime and
we just don't want to care how this address will change.

So let's first make `message` the part of the code segment rather than
be a part of the data segment.

```asm
global _start

section .text

_start:

        ; print hello world on the screen

        xor eax, eax
        mov al, 0x4

        xor ebx, ebx
        mov bl, 0x1

        mov ecx, message

        xor edx, edx
        mov dl, 13

        int 0x80


        ; exit the program gracefully

        xor eax, eax
        mov al, 0x1

        xor ebx, ebx
        mov bl, 0x5

        int 0x80

        message: db "Hello World!", 0xA
```

Now, what is a **JMP-CALL-POP** technique? Remind yourself first of
the `call` instruction and how it works. It saids wherenever we call
label, the **next** instruction after that call label instruction,
the address of that instruction is going to be pushed on the stack.
That is a return address which is used once when we are inside the 
procedure call-by-call we hit a `ret` and we would like to go back
to executing the next instruction after call.

So **JMP-CALL-POP** technique exploits these. How does it do that?

```
JMP short Call_shellcode:

shellcode:
		pop ecx
		...
		...
		...

Call_shellcode:

		call shellcode:
		HelloWorld db "Hello World!"

```

First, it just a short jump and remind yourself that short jumps use
offsets between -128 to 127 so it's really no hardcoded value, it's 
just an offset from the current location. So 
`JMP short Call_shellcode` jumps to `Call_shellcode` label which
ends up executing `call shellcode`.

Now, at this instance we end up pushing the **next** instruction
address on the stack, the next instruction basically is nothing but
the address of the HelloWorld string in the program. So the address
of the HelloWorld string gets pushed on the stack.

As soon as `call shellcode` executes we jump to the `shellcode` label
where we issue `pop ecx` instruction. Now `pop` instruction is going
to pop out the word from the stack and if you remember the top of
the stack currently contains the address of the HelloWorld string
which goes inside the `ecx` register. And now we have the address of
the HelloWorld string inside our shellcode without having to 
explicitly hardcoded.

So let me go ahead and make some modifications.

```asm
global _start

section .text

_start:

        jmp short call_shellcode

shellcode:

        ; print hello world on the screen

        xor eax, eax
        mov al, 0x4

        xor ebx, ebx
        mov bl, 0x1

        pop ecx

        xor edx, edx
        mov dl, 13

        int 0x80


        ; exit the program gracefully

        xor eax, eax
        mov al, 0x1

        xor ebx, ebx
        mov bl, 0x5

        int 0x80

call_shellcode:

        call shellcode
        message: db "Hello World!", 0xA
```

Let me assemble and link these

```
nasm -f elf32 HelloWorld.asm 
ld -o HelloWorld HelloWorld.o
```

Let me extract the shellcode

```
shellcode-dump.sh HelloWorld
"\xeb\x19\x31\xc0\xb0\x04\x31\xdb\xb3\x01\x59\x31\xd2\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x0a"
```

copy the shellcode out and paste inside `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x19\x31\xc0\xb0\x04\x31\xdb\xb3\x01\x59\x31\xd2\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x0a";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

compile it

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

and then run it

```
$ ./shellcode 
Shellcode Length:  45
Hello World!
```

There we go! HelloWorld now prints on the screen. Fantastic.

The **JMP-CALL-POP** technique is something we would need to verify
ourselves using gdb and it is essentially important. I think in the
beginning you should verify the execution of every single piece of
shellcode which you write and which you use by using gdb.

So let me go ahead and load up gdb with `shellcode`

```
gdb -q ./shellcode
```

```
(gdb) set disassembly-flavor intel
(gdb) break main
Breakpoint 1 at 0x80483e8
(gdb) run
```

```
(gdb) disassemble 
Dump of assembler code for function main:
   0x080483e4 <+0>:	push   ebp
   0x080483e5 <+1>:	mov    ebp,esp
   0x080483e7 <+3>:	push   edi
=> 0x080483e8 <+4>:	and    esp,0xfffffff0
   0x080483eb <+7>:	sub    esp,0x30
   0x080483ee <+10>:	mov    eax,0x804a040
   0x080483f3 <+15>:	mov    DWORD PTR [esp+0x1c],0xffffffff
   0x080483fb <+23>:	mov    edx,eax
   0x080483fd <+25>:	mov    eax,0x0
   0x08048402 <+30>:	mov    ecx,DWORD PTR [esp+0x1c]
   0x08048406 <+34>:	mov    edi,edx
   0x08048408 <+36>:	repnz scas al,BYTE PTR es:[edi]
   0x0804840a <+38>:	mov    eax,ecx
   0x0804840c <+40>:	not    eax
   0x0804840e <+42>:	lea    edx,[eax-0x1]
   0x08048411 <+45>:	mov    eax,0x8048510
   0x08048416 <+50>:	mov    DWORD PTR [esp+0x4],edx
   0x0804841a <+54>:	mov    DWORD PTR [esp],eax
   0x0804841d <+57>:	call   0x8048300 <printf@plt>
   0x08048422 <+62>:	mov    DWORD PTR [esp+0x2c],0x804a040
   0x0804842a <+70>:	mov    eax,DWORD PTR [esp+0x2c]
   0x0804842e <+74>:	call   eax
   0x08048430 <+76>:	mov    edi,DWORD PTR [ebp-0x4]
   0x08048433 <+79>:	leave  
   0x08048434 <+80>:	ret    
End of assembler dump.
```

I think if you remember the address which we are interested in is

```
(gdb) print/x &code
$1 = 0x804a040
```

where `code` or our shellcode is located. Let's verify

```
(gdb) shell cat shellcode.c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x19\x31\xc0\xb0\x04\x31\xdb\xb3\x01\x59\x31\xd2\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80\xe8\xe2\xff\xff\xff\x48\x65\x6c\x6c\x6f\x20\x57\x6f\x72\x6c\x64\x21\x0a";

main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

	int (*ret)() = (int(*)())code;

	ret();

}
```
(gdb) x/45xb 0x804a040
0x804a040 <code>:	0xeb	0x19	0x31	0xc0	0xb0	0x04	0x31	0xdb
0x804a048 <code+8>:	0xb3	0x01	0x59	0x31	0xd2	0xb2	0x0d	0xcd
0x804a050 <code+16>:	0x80	0x31	0xc0	0xb0	0x01	0x31	0xdb	0xb3
0x804a058 <code+24>:	0x05	0xcd	0x80	0xe8	0xe2	0xff	0xff	0xff
0x804a060 <code+32>:	0x48	0x65	0x6c	0x6c	0x6f	0x20	0x57	0x6f
0x804a068 <code+40>:	0x72	0x6c	0x64	0x21	0x0a
```

And if you compare they are the same. So we know that our shellcode
is in memory. Just to verify that it is exactly what you have let's
disassemble it by the way by using the `i` operation which is
basically examine as instruction

```
(gdb) x/45i 0x804a040
   0x804a040 <code>:	jmp    0x804a05b <code+27>
   0x804a042 <code+2>:	xor    eax,eax
   0x804a044 <code+4>:	mov    al,0x4
   0x804a046 <code+6>:	xor    ebx,ebx
   0x804a048 <code+8>:	mov    bl,0x1
   0x804a04a <code+10>:	pop    ecx
   0x804a04b <code+11>:	xor    edx,edx
   0x804a04d <code+13>:	mov    dl,0xd
   0x804a04f <code+15>:	int    0x80
   0x804a051 <code+17>:	xor    eax,eax
   0x804a053 <code+19>:	mov    al,0x1
   0x804a055 <code+21>:	xor    ebx,ebx
   0x804a057 <code+23>:	mov    bl,0x5
   0x804a059 <code+25>:	int    0x80
   0x804a05b <code+27>:	call   0x804a042 <code+2>
   0x804a060 <code+32>:	dec    eax
   0x804a061 <code+33>:	gs
   0x804a062 <code+34>:	ins    BYTE PTR es:[edi],dx
   0x804a063 <code+35>:	ins    BYTE PTR es:[edi],dx
   0x804a064 <code+36>:	outs   dx,DWORD PTR ds:[esi]
   0x804a065 <code+37>:	and    BYTE PTR [edi+0x6f],dl
   0x804a068 <code+40>:	jb     0x804a0d6
   0x804a06a <code+42>:	and    DWORD PTR fs:[edx],ecx
   0x804a06d <code+45>:	add    BYTE PTR [eax],al
   ...
```

And if you notice you have your jumps and your `xor`s and your 
`pop ecx` and all that stuff. And starting from this point 

```
   0x804a060 <code+32>:	dec    eax
   0x804a061 <code+33>:	gs
   0x804a062 <code+34>:	ins    BYTE PTR es:[edi],dx
   0x804a063 <code+35>:	ins    BYTE PTR es:[edi],dx
   0x804a064 <code+36>:	outs   dx,DWORD PTR ds:[esi]
   0x804a065 <code+37>:	and    BYTE PTR [edi+0x6f],dl
   0x804a068 <code+40>:	jb     0x804a0d6
   0x804a06a <code+42>:	and    DWORD PTR fs:[edx],ecx
   0x804a06d <code+45>:	add    BYTE PTR [eax],al
```

this is nothing but HelloWorld embedded just after the `call` 
instruction. That is something you should verify as well:

```
(gdb) x/s 0x804a060
0x804a060 <code+32>:	 "Hello World!\n"
```

there we go. Remember while using different platforms addreses may 
differ but the principle is the same. Now, let me setup breakpoint
at the beginning of our shellcode because what we want is to analyze

```
(gdb) b *0x804a040
(gdb) c
```

We hit our shellcode. Let me disassemble

```
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:	jmp    0x804a05b <code+27>
   0x0804a042 <+2>:	xor    eax,eax
   0x0804a044 <+4>:	mov    al,0x4
   0x0804a046 <+6>:	xor    ebx,ebx
   0x0804a048 <+8>:	mov    bl,0x1
   0x0804a04a <+10>:	pop    ecx
   0x0804a04b <+11>:	xor    edx,edx
   0x0804a04d <+13>:	mov    dl,0xd
   0x0804a04f <+15>:	int    0x80
   0x0804a051 <+17>:	xor    eax,eax
   0x0804a053 <+19>:	mov    al,0x1
   0x0804a055 <+21>:	xor    ebx,ebx
   0x0804a057 <+23>:	mov    bl,0x5
   0x0804a059 <+25>:	int    0x80
   0x0804a05b <+27>:	call   0x804a042 <code+2>
   0x0804a060 <+32>:	dec    eax
   0x0804a061 <+33>:	gs
   0x0804a062 <+34>:	ins    BYTE PTR es:[edi],dx
   0x0804a063 <+35>:	ins    BYTE PTR es:[edi],dx
   0x0804a064 <+36>:	outs   dx,DWORD PTR ds:[esi]
   0x0804a065 <+37>:	and    BYTE PTR [edi+0x6f],dl
   0x0804a068 <+40>:	jb     0x804a0d6
   0x0804a06a <+42>:	and    DWORD PTR fs:[edx],ecx
   0x0804a06d <+45>:	add    BYTE PTR [eax],al
End of assembler dump.
```

Now is the interesting part. Let the game begin. Let me go ahead and
examine the data stuff we are interested in. I think we are intersted
in how the top of the stack changes because we said that the `call`
instruction which we have here

```
   0x0804a05b <+27>:    call   0x804a042 <code+2>
```

is going to push the next address which is this value 0x0804a060
on to the stack. So let's define hook

```
(gdb) define hook-stop
>disassemble 
>x/xw $esp
>end
```

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a05b <code+27>
   0x0804a042 <+2>:	xor    eax,eax
   0x0804a044 <+4>:	mov    al,0x4
   0x0804a046 <+6>:	xor    ebx,ebx
   0x0804a048 <+8>:	mov    bl,0x1
   0x0804a04a <+10>:	pop    ecx
   0x0804a04b <+11>:	xor    edx,edx
   0x0804a04d <+13>:	mov    dl,0xd
   0x0804a04f <+15>:	int    0x80
   0x0804a051 <+17>:	xor    eax,eax
   0x0804a053 <+19>:	mov    al,0x1
   0x0804a055 <+21>:	xor    ebx,ebx
   0x0804a057 <+23>:	mov    bl,0x5
   0x0804a059 <+25>:	int    0x80
=> 0x0804a05b <+27>:	call   0x804a042 <code+2>
   0x0804a060 <+32>:	dec    eax
   0x0804a061 <+33>:	gs
   0x0804a062 <+34>:	ins    BYTE PTR es:[edi],dx
   0x0804a063 <+35>:	ins    BYTE PTR es:[edi],dx
   0x0804a064 <+36>:	outs   dx,DWORD PTR ds:[esi]
   0x0804a065 <+37>:	and    BYTE PTR [edi+0x6f],dl
   0x0804a068 <+40>:	jb     0x804a0d6
   0x0804a06a <+42>:	and    DWORD PTR fs:[edx],ecx
   0x0804a06d <+45>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff2ac:	0x08048430
```

currently top of the stack contains this 0x08048430 value. But once
`call` gets executed, execution will go to 0x804a042

```
0x0804a042 <+2>: xor    eax,eax
```

But you would notice that top of the stack would now contain the 
next instruction after `call` which basically would have to point to
this address 0x0804a060.

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a05b <code+27>
=> 0x0804a042 <+2>:	xor    eax,eax
   0x0804a044 <+4>:	mov    al,0x4
   0x0804a046 <+6>:	xor    ebx,ebx
   0x0804a048 <+8>:	mov    bl,0x1
   0x0804a04a <+10>:	pop    ecx
   0x0804a04b <+11>:	xor    edx,edx
   0x0804a04d <+13>:	mov    dl,0xd
   0x0804a04f <+15>:	int    0x80
   0x0804a051 <+17>:	xor    eax,eax
   0x0804a053 <+19>:	mov    al,0x1
   0x0804a055 <+21>:	xor    ebx,ebx
   0x0804a057 <+23>:	mov    bl,0x5
   0x0804a059 <+25>:	int    0x80
   0x0804a05b <+27>:	call   0x804a042 <code+2>
   0x0804a060 <+32>:	dec    eax
   0x0804a061 <+33>:	gs
   0x0804a062 <+34>:	ins    BYTE PTR es:[edi],dx
   0x0804a063 <+35>:	ins    BYTE PTR es:[edi],dx
   0x0804a064 <+36>:	outs   dx,DWORD PTR ds:[esi]
   0x0804a065 <+37>:	and    BYTE PTR [edi+0x6f],dl
   0x0804a068 <+40>:	jb     0x804a0d6
   0x0804a06a <+42>:	and    DWORD PTR fs:[edx],ecx
   0x0804a06d <+45>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff2a8:	0x0804a060
```

And if you notice that is exactly what is happened. The address of the
next instruction after `call` is now on the stack.

So we just now continue program and get HelloWorld.

```
(gdb) c
Continuing.
Hello World!
```

Fantastic.

We are going to use JMP-CALL-POP technique over and over and over 
again, many times. So please ensure that you do exactly the same 
analysis which I've shown you in this video and till the time you
are ensured about all the moving parts continue to run this again and
again and again.

This is a very critical concept is need to be understood perfectly.
Do not try to memorize the exact flow, it's easy to memorize but it 
is not going to help you, in any custom situation you would fail
to think creatively out of the box. So spend your time understanding
**why** we are doing, **what** we are doing and **how** basically the 
code executes in this JMP-CALL-POP triangle.






