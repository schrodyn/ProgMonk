#HelloWorld Shellcode using Stack

Hello all! In this video we will look at a different way to write
the same HelloWorld shellcode which we did with the JMP-CALL-POP
technique in the previous video.

Now, the biggest issue which we faced in the last video was that
we wanted to get a refference to the "Hello World" string in the
shellcode and we basically had to do JMP-CALL-POP to push it on
the stack and pop it back from the stack.

Now, the technique we will discuss in this video would involve 
pushing the "Hello World" string itself on the stack dynamically
and then using `esp` to refference the "Hello World" message
and to use it along with the `ecx` register.

Now, the important thing you need to remember is that because the stack
grows from high memory to low memory you will have to push the "hello
world" string in **reverse** order followed by the terminating 0x00
byte. So which means when we pushing on to the stack we will have to 
pushing 0x00 byte first, then `\n`, then `d`, then `l`, then `r` etc.

This is the key-catching here and If you understand this part I think
you would see that when we use the stack again for things like `execve`
shellcode etc. all of that would be entirely clear.

Now, let's going to our machine and pick up previous program which is
`HelloWorldShellcode.nasm` and rename it to 
`HelloWorldShellcode-Stack.nasm`.

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

Now, let's remove some of the things which is specific to JMP-CALL-POP
and comments

```asm
global _start

section .text

_start:

        xor eax, eax
        mov al, 0x4

        xor ebx, ebx
        mov bl, 0x1

        pop ecx

        xor edx, edx
        mov dl, 13

        int 0x80


        xor eax, eax
        mov al, 0x1

        xor ebx, ebx
        mov bl, 0x5

        int 0x80

        message: db "Hello World!", 0xA
```

Now, the important thing if you note right now is that `ecx` register
is the one where we need to go ahead and refference the "Hello World"
string. So there are two parts of the problem

  * A -- to push the "Hello World" string on the stack

  * B -- to get the refference to that string inside `ecx` register

Now if you recall I said you will have to push the string in the 
reverse order. How to do that? I am going use python to reverse this
string 

```
>>> code = "Hello World\n"
>>> code[::-1]
'\ndlroW olleH'
```

Now what I want to do is to convert this to hex

```
>>> code[::-1].encode('hex')
'0a646c726f57206f6c6c6548'
```

The first thing which we need to push is 0x00 byte. You could do it
in a different ways. I'd prefer just to push an entire register in
which has 0x00 in it. Let me remove `pop ecx`.

Now, I don't want to do `xor ecx, ecx` and use it again because that
just increases the number of instructions. So I am going to use `edx`
register to push nulls on the stack:

```
global _start

section .text

_start:

        xor eax, eax
        mov al, 0x4

        xor ebx, ebx
        mov bl, 0x1

        xor edx, edx
        push edx

        mov dl, 13

        int 0x80


        xor eax, eax
        mov al, 0x1

        xor ebx, ebx
        mov bl, 0x5

        int 0x80

        message: db "Hello World!", 0xA
```

So you are pushing 4 nulls on the stack. Now, the next thing would be
to push the hello world string in reverse order. So let me pick up
first 4 bytes. So we have

```
push 0x0a646c72
```

similar we have

```
push 0x6f57206f
push 0x6c6c6548
```

```asm
global _start

section .text

_start:

        xor eax, eax
        mov al, 0x4

        xor ebx, ebx
        mov bl, 0x1

        xor edx, edx
        push edx

        push 0x0a646c72
        push 0x6f57206f
        push 0x6c6c6548


        mov dl, 13

        int 0x80


        xor eax, eax
        mov al, 0x1

        xor ebx, ebx
        mov bl, 0x5

        int 0x80

        message: db "Hello World!", 0xA
```

Now, the top of the stack can be refferenced by `esp` which is really
the address where the string "Hello World" starts from Low Memory to
High Memory.

So what we really have to do is `mov ecx, esp`

```
        push 0x0a646c72
        push 0x6f57206f
        push 0x6c6c6548

        mov ecx, esp
```

there we go. So now everything is set.

```asm
global _start

section .text

_start:

        xor eax, eax
        mov al, 0x4

        xor ebx, ebx
        mov bl, 0x1

        xor edx, edx
        push edx

        push 0x0a646c72
        push 0x6f57206f
        push 0x6c6c6548

        mov ecx, esp

        mov dl, 13

        int 0x80


        xor eax, eax
        mov al, 0x1

        xor ebx, ebx
        mov bl, 0x5

        int 0x80
```

So let me go ahead assemble and link it

```
nasm -f elf32 HelloWorld-Stack.asm
ld -o HelloWorld-Stack HelloWorld-Stack.o
```

and let me run it

```
./HelloWorld-Stack 
Hello World
```

Let me use `objdump` just to ensure that all 0x00 are removed.

```
$ objdump -d HelloWorld-Stack -M intel

HelloWorld-Stack:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	31 c0                	xor    eax,eax
 8048062:	b0 04                	mov    al,0x4
 8048064:	31 db                	xor    ebx,ebx
 8048066:	b3 01                	mov    bl,0x1
 8048068:	31 d2                	xor    edx,edx
 804806a:	52                   	push   edx
 804806b:	68 72 6c 64 0a       	push   0xa646c72
 8048070:	68 6f 20 57 6f       	push   0x6f57206f
 8048075:	68 48 65 6c 6c       	push   0x6c6c6548
 804807a:	89 e1                	mov    ecx,esp
 804807c:	b2 0d                	mov    dl,0xd
 804807e:	cd 80                	int    0x80
 8048080:	31 c0                	xor    eax,eax
 8048082:	b0 01                	mov    al,0x1
 8048084:	31 db                	xor    ebx,ebx
 8048086:	b3 05                	mov    bl,0x5
 8048088:	cd 80                	int    0x80
```

Everything is OK. Fantastic. no 0x00. Finally I am going to use my 
little script

```
$ shellcode-dump.sh HelloWorld-Stack
"\x31\xc0\xb0\x04\x31\xdb\xb3\x01\x31\xd2\x52\x68\x72\x6c\x64\x0a\x68\x6f\x20\x57\x6f\x68\x48\x65\x6c\x6c\x89\xe1\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80"
```

Let's pick up this up from here and paste it to `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\x31\xc0\xb0\x04\x31\xdb\xb3\x01\x31\xd2\x52\x68\x72\x6c\x64\x0a\x68\x6f\x20\x57\x6f\x68\x48\x65\x6c\x6c\x89\xe1\xb2\x0d\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xb3\x05\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

compile `shellcode.c`

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

```
$ ./shellcode 
Shellcode Length:  42
Hello World
```

there we go.

Now, let's go ahead and analyze this. And what we really want to do
is verify that you know on the stack things are pushed really that
is expected.

```
gdb -q ./shellcode
```

```
(gdb) set disassembly-flavor intel
(gdb) break main
Breakpoint 1 at 0x80483e8
(gdb) print/x &code
$1 = 0x804a040
```

let's set a breakpoint to the `code` location

```
(gdb) break *0x804a040
Breakpoint 2 at 0x804a040
```

```
(gdb) run
(gdb) c
```

```
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:		xor    eax,eax
   0x0804a042 <+2>:		mov    al,0x4
   0x0804a044 <+4>:		xor    ebx,ebx
   0x0804a046 <+6>:		mov    bl,0x1
   0x0804a048 <+8>:		xor    edx,edx
   0x0804a04a <+10>:	push   edx
   0x0804a04b <+11>:	push   0xa646c72
   0x0804a050 <+16>:	push   0x6f57206f
   0x0804a055 <+21>:	push   0x6c6c6548
   0x0804a05a <+26>:	mov    ecx,esp
   0x0804a05c <+28>:	mov    dl,0xd
   0x0804a05e <+30>:	int    0x80
   0x0804a060 <+32>:	xor    eax,eax
   0x0804a062 <+34>:	mov    al,0x1
   0x0804a064 <+36>:	xor    ebx,ebx
   0x0804a066 <+38>:	mov    bl,0x5
   0x0804a068 <+40>:	int    0x80
   0x0804a06a <+42>:	add    BYTE PTR [eax],al
End of assembler dump.
```

So this is really beginning of our shellcode and if you notice what
we really want to track is how does the stack is going to be set up
rightly. So let's go ahead and do that. Let me examine 20 bytes on
the stack and print them as characters.

```
(gdb) x/20cb $esp
0xbffff2ac:	48 '0'	-124 '\204'	4 '\004'	8 '\b'	16 '\020'	-123 '\205'	4 '\004'	8 '\b'
0xbffff2b4:	42 '*'	0 '\000'	0 '\000'	0 '\000'	-12 '\364'	-97 '\237'	4 '\004'	8 '\b'
0xbffff2bc:	97 'a'	-124 '\204'	4 '\004'	8 '\b'
```

Let's do the following. Let me define hook-stop

```
(gdb) define hook-stop
>disassemble 
>x/20cb $esp
>end
```

and `stepi` until the `push` instructions

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	xor    eax,eax
   0x0804a042 <+2>:	mov    al,0x4
   0x0804a044 <+4>:	xor    ebx,ebx
   0x0804a046 <+6>:	mov    bl,0x1
   0x0804a048 <+8>:	xor    edx,edx
=> 0x0804a04a <+10>:	push   edx
   0x0804a04b <+11>:	push   0xa646c72
   0x0804a050 <+16>:	push   0x6f57206f
   0x0804a055 <+21>:	push   0x6c6c6548
   0x0804a05a <+26>:	mov    ecx,esp
   0x0804a05c <+28>:	mov    dl,0xd
   0x0804a05e <+30>:	int    0x80
   0x0804a060 <+32>:	xor    eax,eax
   0x0804a062 <+34>:	mov    al,0x1
   0x0804a064 <+36>:	xor    ebx,ebx
   0x0804a066 <+38>:	mov    bl,0x5
   0x0804a068 <+40>:	int    0x80
   0x0804a06a <+42>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff2ac:	48 '0'	-124 '\204'	4 '\004'	8 '\b'	16 '\020'	-123 '\205'	4 '\004'	8 '\b'
0xbffff2b4:	42 '*'	0 '\000'	0 '\000'	0 '\000'	-12 '\364'	-97 '\237'	4 '\004'	8 '\b'
0xbffff2bc:	97 'a'	-124 '\204'	4 '\004'	8 '\b'
0x0804a04a in code ()
```

after the next instruction first 4 values on the stack become zeros

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	xor    eax,eax
   0x0804a042 <+2>:	mov    al,0x4
   0x0804a044 <+4>:	xor    ebx,ebx
   0x0804a046 <+6>:	mov    bl,0x1
   0x0804a048 <+8>:	xor    edx,edx
   0x0804a04a <+10>:	push   edx
=> 0x0804a04b <+11>:	push   0xa646c72
   0x0804a050 <+16>:	push   0x6f57206f
   0x0804a055 <+21>:	push   0x6c6c6548
   0x0804a05a <+26>:	mov    ecx,esp
   0x0804a05c <+28>:	mov    dl,0xd
   0x0804a05e <+30>:	int    0x80
   0x0804a060 <+32>:	xor    eax,eax
   0x0804a062 <+34>:	mov    al,0x1
   0x0804a064 <+36>:	xor    ebx,ebx
   0x0804a066 <+38>:	mov    bl,0x5
   0x0804a068 <+40>:	int    0x80
   0x0804a06a <+42>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff2a8:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	48 '0'	-124 '\204'	4 '\004'	8 '\b'
0xbffff2b0:	16 '\020'	-123 '\205'	4 '\004'	8 '\b'	42 '*'	0 '\000'	0 '\000'	0 '\000'
0xbffff2b8:	-12 '\364'	-97 '\237'	4 '\004'	8 '\b'
0x0804a04b in code ()
```

Now, remember we are pushing the "Hello World" string in the reverse
order. So because of the next instruction we now see

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	xor    eax,eax
   0x0804a042 <+2>:	mov    al,0x4
   0x0804a044 <+4>:	xor    ebx,ebx
   0x0804a046 <+6>:	mov    bl,0x1
   0x0804a048 <+8>:	xor    edx,edx
   0x0804a04a <+10>:	push   edx
   0x0804a04b <+11>:	push   0xa646c72
=> 0x0804a050 <+16>:	push   0x6f57206f
   0x0804a055 <+21>:	push   0x6c6c6548
   0x0804a05a <+26>:	mov    ecx,esp
   0x0804a05c <+28>:	mov    dl,0xd
   0x0804a05e <+30>:	int    0x80
   0x0804a060 <+32>:	xor    eax,eax
   0x0804a062 <+34>:	mov    al,0x1
   0x0804a064 <+36>:	xor    ebx,ebx
   0x0804a066 <+38>:	mov    bl,0x5
   0x0804a068 <+40>:	int    0x80
   0x0804a06a <+42>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff2a4:	114 'r'	108 'l'	100 'd'	10 '\n'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
0xbffff2ac:	48 '0'	-124 '\204'	4 '\004'	8 '\b'	16 '\020'	-123 '\205'	4 '\004'	8 '\b'
0xbffff2b4:	42 '*'	0 '\000'	0 '\000'	0 '\000'
0x0804a050 in code ()
```

`114 'r' 108 'l' 100 'd' 10 '\n'`. Awesome. Let's `push` a little bit
more.

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	xor    eax,eax
   0x0804a042 <+2>:	mov    al,0x4
   0x0804a044 <+4>:	xor    ebx,ebx
   0x0804a046 <+6>:	mov    bl,0x1
   0x0804a048 <+8>:	xor    edx,edx
   0x0804a04a <+10>:	push   edx
   0x0804a04b <+11>:	push   0xa646c72
   0x0804a050 <+16>:	push   0x6f57206f
=> 0x0804a055 <+21>:	push   0x6c6c6548
   0x0804a05a <+26>:	mov    ecx,esp
   0x0804a05c <+28>:	mov    dl,0xd
   0x0804a05e <+30>:	int    0x80
   0x0804a060 <+32>:	xor    eax,eax
   0x0804a062 <+34>:	mov    al,0x1
   0x0804a064 <+36>:	xor    ebx,ebx
   0x0804a066 <+38>:	mov    bl,0x5
   0x0804a068 <+40>:	int    0x80
   0x0804a06a <+42>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff2a0:	111 'o'	32 ' '	87 'W'	111 'o'	114 'r'	108 'l'	100 'd'	10 '\n'
0xbffff2a8:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	48 '0'	-124 '\204'	4 '\004'	8 '\b'
0xbffff2b0:	16 '\020'	-123 '\205'	4 '\004'	8 '\b'
0x0804a055 in code ()
```

And so on.

```
(gdb) stepi
Dump of assembler code for function code:
   0x0804a040 <+0>:	xor    eax,eax
   0x0804a042 <+2>:	mov    al,0x4
   0x0804a044 <+4>:	xor    ebx,ebx
   0x0804a046 <+6>:	mov    bl,0x1
   0x0804a048 <+8>:	xor    edx,edx
   0x0804a04a <+10>:	push   edx
   0x0804a04b <+11>:	push   0xa646c72
   0x0804a050 <+16>:	push   0x6f57206f
   0x0804a055 <+21>:	push   0x6c6c6548
   0x0804a05a <+26>:	mov    ecx,esp
=> 0x0804a05c <+28>:	mov    dl,0xd
   0x0804a05e <+30>:	int    0x80
   0x0804a060 <+32>:	xor    eax,eax
   0x0804a062 <+34>:	mov    al,0x1
   0x0804a064 <+36>:	xor    ebx,ebx
   0x0804a066 <+38>:	mov    bl,0x5
   0x0804a068 <+40>:	int    0x80
   0x0804a06a <+42>:	add    BYTE PTR [eax],al
End of assembler dump.
0xbffff29c:	72 'H'	101 'e'	108 'l'	108 'l'	111 'o'	32 ' '	87 'W'	111 'o'
0xbffff2a4:	114 'r'	108 'l'	100 'd'	10 '\n'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
0xbffff2ac:	48 '0'	-124 '\204'	4 '\004'	8 '\b'
0x0804a05c in code ()
```

```
(gdb) print/x $ecx
$3 = 0xbffff29c
```

`mov ecx, esp` basically ensures that `ecx` basically refferences 
the top of the stack at this point which is now refferencing the 
"Hello World" string. The rest of the stuff you are already familiar
with.

```
(gdb) c
Continuing.
Hello World
```

So hopefully this technique is clear and the whole idea is to setup
the stack in such a way the string is on the stack and because we
have control in figuring out the top of the stack using `esp`. That's
it.

Now, keep this two techniques in mind in very solid way. Most of the 
stuff would probably involve using one of this two techniques.

Try this out and I would really say about from "Hello World" try out 
other strings which probably are not multiple of 4 so you would have
to setup the stack in a slightly different way.
 
