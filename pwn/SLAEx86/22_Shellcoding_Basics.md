#Shellcoding Basics

Hello all! Welcome to module two: Introduction to Shellcoding. And in
this video we will look at shellcoding basics.

Now, if I want to define **shellcode** in the very easy to understand
way it's basically machine code with a special or specific purpose.
Examples could be spawning a local `/bin/sh` shell, binding to a port
and then spawning a shell when someone connects to this port from
the outside world, creating a new account on the machine and tons of
other things.

Now, the most important thing about shellcode is because it is in the
machine code format it can be executed directly by the CPU -- no 
further assembling/linking or compilation of any kind is really
required. Which means it can be consumed in its current form by the 
CPU and executed.

Now, how is shellcode delivered? Broadly there are two ways. One is
as a part of an exploit. You heard about payloads when you use 
metasploit such as reverse tcp, an account payload, you know, creating
a shell payload etc. These are examples of shellcoding being used
for payload creation. And in this case the size of the payload is
very important and typically smaller the size of the shellcode the
better. One of the other thing is important is something called
**bad characters**. Now, bad characters are really those characters
which if they are a part of the shellcode may prevent running the
exploit entirely which is your exploit may fail simply because of
bad characters being there. The most common one is the null character
0x00. And typically when shellcoding this is one character which we 
will almost always avoid. There are other bad characters possible
as well and it depends entirely on the program you are exploiting
and its specific exploitation conditions which you need to analyze. 
Later on I'll give you a link to a couple of free videos I've created
for exploit research while some of these explained more.

The other way to deliver shellcode is to basically add it to an 
executable. This could be then running shellcode as a separate thread
or overwriting the original executable functionality in entirely.
In this case size of the shellcode and bad characters are typically 
not a concern. 

Now, the important thing is that in this series we are going to 
concentrate on shellcode creation and **not** exploit research which
is a much larger and a bigger topic. However, once we do other 
assembly language courses I will launch a course entirely on exploit
research on Linux and Windows where you would get a taste for all of 
that.

Shellcode Resources. I think in my personal experience **shell-storm**
is probably the best place. You also have shellcode listed on 
**explit-db** and on the **projectshellcode**. Check out all of this
resources. We will be using some of the sample shellcode in some our
videos from this resources.

So now the question really arises that while we are shellcoding how
do you test the shellcode? So to test our shellcode we need a skeleton
program which eventually will pass the control to our shellcode
to execute and that would allow us to analyze if this shellcode has
been properly executed and done its stuff or not.

Now, the most common form of that is a simple C program file where
basically you go ahead and define your shellcode as an `unsigned char`
array and then pass control to it. Here you may notice a lot of 
interesting pointer arithmetic happening and what I am going to do is
I'll not explain this from a C programming perspective rather let's
take an example shellcode from **shellcode-storm** and then use it
and then analyze what is happening in this program.

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"SHELLCODE ";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
``` 

So let's go to the shell-storm and let me copy the shellcode out.
Now there is a place where you need to put your shellcode.

```
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x12\x31\xc9\x5e\x56\x5f\xb1\x15\x8a\x06\xfe\xc8\x88\x06\x46"
"\xe2\xf7\xff\xe7\xe8\xe9\xff\xff\xff\x32\xc1\x32\xca\x52\x69\x30"
"\x74\x69\x01\x69\x30\x63\x6a\x6f\x8a\xe4\xb1\x0c\xce\x81";
main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

Now let's compile the program

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

So we are disabling stack protection as well as we specifically 
mention that we want stack to be executable. Now this is just creating
conditions so that our shellcode can run without any problem.
In the real world scenario of course the exploit conditions would
dictate as how you would do things like disabling dependence like

And now let me run this shellcode.

```
$ ./shellcode 
Shellcode Length:  46
$ ls
Analysis    Insertion-Encoder  Polymorphic-Demo  shellcode.c
Crypter     MMX-XOR-Encoder    XOR-Encoder
Execve	    Metasploit	       exit
HelloWorld  NOT-Encoder        shellcode
```

So fantastic! we have a shell. So now let's go ahead and analyze
how this little C program allows us to test the shellcode.

Let me load up our friend gdb

```
gdb -q ./shellcode
```

```
(gdb) set disassembly-flavor intel
(gdb) break main
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

So there is a bunch of things happening here. What we are interested
in is really 

```
(gdb) shell cat shellcode.c 
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xeb\x12\x31\xc9\x5e\x56\x5f\xb1\x15\x8a\x06\xfe\xc8\x88\x06\x46"
"\xe2\xf7\xff\xe7\xe8\xe9\xff\xff\xff\x32\xc1\x32\xca\x52\x69\x30"
"\x74\x69\x01\x69\x30\x63\x6a\x6f\x8a\xe4\xb1\x0c\xce\x81";
main()
{

	printf("Shellcode Length:  %d\n", strlen(code));

	int (*ret)() = (int(*)())code;

	ret();

}
```

when control eventually passes to the `code` and it starts executing.
The interesting part comes in here

```
0x08048422 <+62>:	 mov    DWORD PTR [esp+0x2c],0x804a040
0x0804842a <+70>:    mov    eax,DWORD PTR [esp+0x2c]
0x0804842e <+74>:    call   eax
0x08048430 <+76>:    mov    edi,DWORD PTR [ebp-0x4]
0x08048433 <+79>:    leave
0x08048434 <+80>:    ret
```

which is `call eax`. This is really the place where what is happening
is we are going ahead and loading up the **address** of the location 
where our shellcode is stored and we pass control to it.

Now, let's go ahead and print the address of `code`.

```
(gdb) print/x &code
$1 = 0x804a040
```

and if you notice this is what basically move on to the stack

```
0x08048422 <+62>:    mov    DWORD PTR [esp+0x2c],0x804a040
```

and then after that if you want to go ahead and notice what is in 
there you could basically use examine command

```
(gdb) x/xw 0x804a040
0x804a040 <code>:	0xc93112eb
```

and if you start noticing this address what does really have is 
this code `\xeb\x12\x31\xc9` which is the first part of our shellcode.
So really if you go ahead and now examine individual bytes. Let's show
46 of them

```
(gdb) x/46xb 0x804a040
0x804a040 <code>:	0xeb	0x12	0x31	0xc9	0x5e	0x56	0x5f	0xb1
0x804a048 <code+8>:	0x15	0x8a	0x06	0xfe	0xc8	0x88	0x06	0x46
0x804a050 <code+16>:	0xe2	0xf7	0xff	0xe7	0xe8	0xe9	0xff	0xff
0x804a058 <code+24>:	0xff	0x32	0xc1	0x32	0xca	0x52	0x69	0x30
0x804a060 <code+32>:	0x74	0x69	0x01	0x69	0x30	0x63	0x6a	0x6f
0x804a068 <code+40>:	0x8a	0xe4	0xb1	0x0c	0xce	0x81
```

you would actually notice that this is exactly location where our
shellcode is. And if you look at disassembly eventually `call eax`
basically calls this specific location. Let me setup breakpoint here
so that we can ensure that it is exactly what is happening.

```
(gdb) break *0x0804842e
```

We continue running the program we hit breakpoint 2

```
(gdb) c
(gdb) disassemble 
Dump of assembler code for function main:
   0x080483e4 <+0>:	push   ebp
   0x080483e5 <+1>:	mov    ebp,esp
   0x080483e7 <+3>:	push   edi
   0x080483e8 <+4>:	and    esp,0xfffffff0
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
=> 0x0804842e <+74>:	call   eax
   0x08048430 <+76>:	mov    edi,DWORD PTR [ebp-0x4]
   0x08048433 <+79>:	leave  
   0x08048434 <+80>:	ret    
End of assembler dump.
```

And if I now look at contents of `eax` register

```
(gdb) print/x $eax
$2 = 0x804a040
```

you would notice that this is really nothing but the address of the
location where shellcode is loaded

```
(gdb) x/46xb 0x804a040
0x804a040 <code>:	0xeb	0x12	0x31	0xc9	0x5e	0x56	0x5f	0xb1
0x804a048 <code+8>:	0x15	0x8a	0x06	0xfe	0xc8	0x88	0x06	0x46
0x804a050 <code+16>:	0xe2	0xf7	0xff	0xe7	0xe8	0xe9	0xff	0xff
0x804a058 <code+24>:	0xff	0x32	0xc1	0x32	0xca	0x52	0x69	0x30
0x804a060 <code+32>:	0x74	0x69	0x01	0x69	0x30	0x63	0x6a	0x6f
0x804a068 <code+40>:	0x8a	0xe4	0xb1	0x0c	0xce	0x81
```

So which means `call eax` now would end up calling our shellcode. Let
me setup breakpoint at this place.

```
(gdb) break *0x804a040
(gdb) c
```

```
(gdb) disassemble 
Dump of assembler code for function code:
=> 0x0804a040 <+0>:	jmp    0x804a054 <code+20>
   0x0804a042 <+2>:	xor    ecx,ecx
   0x0804a044 <+4>:	pop    esi
   0x0804a045 <+5>:	push   esi
   0x0804a046 <+6>:	pop    edi
   0x0804a047 <+7>:	mov    cl,0x15
   0x0804a049 <+9>:	mov    al,BYTE PTR [esi]
   0x0804a04b <+11>:	dec    al
   0x0804a04d <+13>:	mov    BYTE PTR [esi],al
   0x0804a04f <+15>:	inc    esi
   0x0804a050 <+16>:	loop   0x804a049 <code+9>
   0x0804a052 <+18>:	jmp    edi
   0x0804a054 <+20>:	call   0x804a042 <code+2>
   0x0804a059 <+25>:	xor    al,cl
   0x0804a05b <+27>:	xor    cl,dl
   0x0804a05d <+29>:	push   edx
   0x0804a05e <+30>:	imul   esi,DWORD PTR [eax],0x69016974
   0x0804a064 <+36>:	xor    BYTE PTR [ebx+0x6a],ah
   0x0804a067 <+39>:	outs   dx,DWORD PTR ds:[esi]
   0x0804a068 <+40>:	mov    ah,ah
   0x0804a06a <+42>:	mov    cl,0xc
   0x0804a06c <+44>:	into   
   0x0804a06d <+45>:	add    DWORD PTR [eax],0x0
End of assembler dump.
```

And now you would notice that this disassembly here is really nothing 
but the shellcode. So now the rest of shellcode goes ahead and 
executes and gives us a `/bin/sh` shell.

Now, in further coming videos of course you are going to learn how
to create this shellcode on your own. Hopefully, in this introduction
video two things are clear: what is shellcode, how it looks like,
and how do you use the skeleton program which I showed to run and test
your shellcode. And I would really recommend you in the beginning to 
cut-paste a lot of shellcode available on shell-storm to get a feeling
what different shellcodes do. Of course you are probably thinking it
wouldn't be a really good idea to run other people shellcode. So run
it in a sandbox, hopefully most shellcode on shell-storm etc. has been
tested, however, run it in a VM :)





