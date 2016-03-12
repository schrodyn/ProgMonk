#crackme0x06

It's the same as previous crackme but with one modification.
Let's go ahead and see what it is

```
gdb -q ./crackme0x06
```

```
(gdb) set disassembly-flavor intel
(gdb) break main
```

```
(gdb) r
```

Let's disassemble `main`

```
(gdb) disassemble 
Dump of assembler code for function main:
   0x08048607 <+0>:     push   ebp
   0x08048608 <+1>:     mov    ebp,esp
   0x0804860a <+3>:     sub    esp,0x88
=> 0x08048610 <+9>:     and    esp,0xfffffff0
   0x08048613 <+12>:    mov    eax,0x0
   0x08048618 <+17>:    add    eax,0xf
   0x0804861b <+20>:    add    eax,0xf
   0x0804861e <+23>:    shr    eax,0x4
   0x08048621 <+26>:    shl    eax,0x4
   0x08048624 <+29>:    sub    esp,eax
   0x08048626 <+31>:    mov    DWORD PTR [esp],0x8048763
   0x0804862d <+38>:    call   0x80483b8 <printf@plt>
   0x08048632 <+43>:    mov    DWORD PTR [esp],0x804877c
   0x08048639 <+50>:    call   0x80483b8 <printf@plt>
   0x0804863e <+55>:    lea    eax,[ebp-0x78]
   0x08048641 <+58>:    mov    DWORD PTR [esp+0x4],eax
   0x08048645 <+62>:    mov    DWORD PTR [esp],0x8048787
   0x0804864c <+69>:    call   0x8048398 <scanf@plt>
   0x08048651 <+74>:    mov    eax,DWORD PTR [ebp+0x10]
   0x08048654 <+77>:    mov    DWORD PTR [esp+0x4],eax
   0x08048658 <+81>:    lea    eax,[ebp-0x78]
   0x0804865b <+84>:    mov    DWORD PTR [esp],eax
   0x0804865e <+87>:    call   0x8048588 <check>
   0x08048663 <+92>:    mov    eax,0x0
   0x08048668 <+97>:    leave  
   0x08048669 <+98>:    ret    
End of assembler dump.
```

As you may notice this `main` is pretty much the same as in previous
crackme BUT to `check` function we also pass something that stored
at `[ebp+0x10]` address. What is it? Ok, in `[ebp+0x08]` we have `argc`, at `[ebp+0xc]` we have `argv` and at `[ebp+0x10]` address we have
`envp`.

Now, let's check what's format string for `scanf` to be sure that it's
string

```
(gdb) x/s 0x8048787
0x8048787:      "%s"
```

So we have call `check` function where first argument is input string
and the second argument is `envp`. Let's go ahead and disassemble
`check` function

```
(gdb) disassemble check
Dump of assembler code for function check:
   0x08048588 <+0>:     push   ebp
   0x08048589 <+1>:     mov    ebp,esp
   0x0804858b <+3>:     sub    esp,0x28
   0x0804858e <+6>:     mov    DWORD PTR [ebp-0x8],0x0
   0x08048595 <+13>:    mov    DWORD PTR [ebp-0xc],0x0
   0x0804859c <+20>:    mov    eax,DWORD PTR [ebp+0x8]
   0x0804859f <+23>:    mov    DWORD PTR [esp],eax
   0x080485a2 <+26>:    call   0x80483a8 <strlen@plt>
   0x080485a7 <+31>:    cmp    DWORD PTR [ebp-0xc],eax
   0x080485aa <+34>:    jae    0x80485f9 <check+113>
   0x080485ac <+36>:    mov    eax,DWORD PTR [ebp-0xc]
   0x080485af <+39>:    add    eax,DWORD PTR [ebp+0x8]
   0x080485b2 <+42>:    movzx  eax,BYTE PTR [eax]
   0x080485b5 <+45>:    mov    BYTE PTR [ebp-0xd],al
   0x080485b8 <+48>:    lea    eax,[ebp-0x4]
   0x080485bb <+51>:    mov    DWORD PTR [esp+0x8],eax
   0x080485bf <+55>:    mov    DWORD PTR [esp+0x4],0x804873d
   0x080485c7 <+63>:    lea    eax,[ebp-0xd]
   0x080485ca <+66>:    mov    DWORD PTR [esp],eax
   0x080485cd <+69>:    call   0x80483c8 <sscanf@plt>
   0x080485d2 <+74>:    mov    edx,DWORD PTR [ebp-0x4]
   0x080485d5 <+77>:    lea    eax,[ebp-0x8]
   0x080485d8 <+80>:    add    DWORD PTR [eax],edx
   0x080485da <+82>:    cmp    DWORD PTR [ebp-0x8],0x10
   0x080485de <+86>:    jne    0x80485f2 <check+106>
   0x080485e0 <+88>:    mov    eax,DWORD PTR [ebp+0xc]
   0x080485e3 <+91>:    mov    DWORD PTR [esp+0x4],eax
   0x080485e7 <+95>:    mov    eax,DWORD PTR [ebp+0x8]
   0x080485ea <+98>:    mov    DWORD PTR [esp],eax
   0x080485ed <+101>:   call   0x804851a <parell>
   0x080485f2 <+106>:   lea    eax,[ebp-0xc]
   0x080485f5 <+109>:   inc    DWORD PTR [eax]
   0x080485f7 <+111>:   jmp    0x804859c <check+20>
   0x080485f9 <+113>:   mov    DWORD PTR [esp],0x804874e
   0x08048600 <+120>:   call   0x80483b8 <printf@plt>
   0x08048605 <+125>:   leave  
   0x08048606 <+126>:   ret
```

Here we check if string is empty

```
   0x0804858e <+6>:     mov    DWORD PTR [ebp-0x8],0x0
   0x08048595 <+13>:    mov    DWORD PTR [ebp-0xc],0x0
   0x0804859c <+20>:    mov    eax,DWORD PTR [ebp+0x8]
   0x0804859f <+23>:    mov    DWORD PTR [esp],eax
   0x080485a2 <+26>:    call   0x80483a8 <strlen@plt>
   0x080485a7 <+31>:    cmp    DWORD PTR [ebp-0xc],eax
   0x080485aa <+34>:    jae    0x80485f9 <check+113>
```

if it is then we have this `printf` statement

```
   0x080485f9 <+113>:   mov    DWORD PTR [esp],0x804874e
   0x08048600 <+120>:   call   0x80483b8 <printf@plt>
```

Let's see what is there

```
(gdb) x/s 0x804874e
0x804874e:      "Password Incorrect!\n"
```

Ok, we could guess this scenario. Otherwise, when string is not empty,
we think, think and understand that we calculate sum of digits of
our password and if it's equal to 16 then call `parell`

```
   0x080485d5 <+77>:    lea    eax,[ebp-0x8]
   0x080485d8 <+80>:    add    DWORD PTR [eax],edx
   0x080485da <+82>:    cmp    DWORD PTR [ebp-0x8],0x10
   0x080485de <+86>:    jne    0x80485f2 <check+106>
   0x080485e0 <+88>:    mov    eax,DWORD PTR [ebp+0xc]
   0x080485e3 <+91>:    mov    DWORD PTR [esp+0x4],eax
   0x080485e7 <+95>:    mov    eax,DWORD PTR [ebp+0x8]
   0x080485ea <+98>:    mov    DWORD PTR [esp],eax
   0x080485ed <+101>:   call   0x804851a <parell>
```

what do we pass to `parell`? the first argument is also pointer to our
input string, and the second argument is `envp` since at `[ebp+0xc]`
address there is the second argument to `check` function. 

So let's disassemble `parell` function

```
(gdb) disassemble parell 
Dump of assembler code for function parell:
   0x0804851a <+0>:     push   ebp
   0x0804851b <+1>:     mov    ebp,esp
   0x0804851d <+3>:     sub    esp,0x18
   0x08048520 <+6>:     lea    eax,[ebp-0x4]
   0x08048523 <+9>:     mov    DWORD PTR [esp+0x8],eax
   0x08048527 <+13>:    mov    DWORD PTR [esp+0x4],0x804873d
   0x0804852f <+21>:    mov    eax,DWORD PTR [ebp+0x8]
   0x08048532 <+24>:    mov    DWORD PTR [esp],eax
   0x08048535 <+27>:    call   0x80483c8 <sscanf@plt>
   0x0804853a <+32>:    mov    eax,DWORD PTR [ebp+0xc]
   0x0804853d <+35>:    mov    DWORD PTR [esp+0x4],eax
   0x08048541 <+39>:    mov    eax,DWORD PTR [ebp-0x4]
   0x08048544 <+42>:    mov    DWORD PTR [esp],eax
   0x08048547 <+45>:    call   0x80484b4 <dummy>
   0x0804854c <+50>:    test   eax,eax
   0x0804854e <+52>:    je     0x8048586 <parell+108>
   0x08048550 <+54>:    mov    DWORD PTR [ebp-0x8],0x0
   0x08048557 <+61>:    cmp    DWORD PTR [ebp-0x8],0x9
   0x0804855b <+65>:    jg     0x8048586 <parell+108>
   0x0804855d <+67>:    mov    eax,DWORD PTR [ebp-0x4]
   0x08048560 <+70>:    and    eax,0x1
   0x08048563 <+73>:    test   eax,eax
   0x08048565 <+75>:    jne    0x804857f <parell+101>
   0x08048567 <+77>:    mov    DWORD PTR [esp],0x8048740
   0x0804856e <+84>:    call   0x80483b8 <printf@plt>
   0x08048573 <+89>:    mov    DWORD PTR [esp],0x0
   0x0804857a <+96>:    call   0x80483e8 <exit@plt>
   0x0804857f <+101>:   lea    eax,[ebp-0x8]
   0x08048582 <+104>:   inc    DWORD PTR [eax]
   0x08048584 <+106>:   jmp    0x8048557 <parell+61>
   0x08048586 <+108>:   leave  
   0x08048587 <+109>:   ret    
End of assembler dump.
``` 

There

```
   0x08048520 <+6>:     lea    eax,[ebp-0x4]
   0x08048523 <+9>:     mov    DWORD PTR [esp+0x8],eax
   0x08048527 <+13>:    mov    DWORD PTR [esp+0x4],0x804873d
   0x0804852f <+21>:    mov    eax,DWORD PTR [ebp+0x8]
   0x08048532 <+24>:    mov    DWORD PTR [esp],eax
   0x08048535 <+27>:    call   0x80483c8 <sscanf@plt>
```

we call `sscanf` with our input string since `[ebp+0x8]` it is a
pointer to our string. The second argument is format string

```
(gdb) x/s 0x804873d
0x804873d:      "%d"
```

Ok, so we cast our string to integer and save result at `[ebp-0x4]`
address. After that call `dummy` function with two arguments

```
   0x0804853a <+32>:    mov    eax,DWORD PTR [ebp+0xc]
   0x0804853d <+35>:    mov    DWORD PTR [esp+0x4],eax
   0x08048541 <+39>:    mov    eax,DWORD PTR [ebp-0x4]
   0x08048544 <+42>:    mov    DWORD PTR [esp],eax
   0x08048547 <+45>:    call   0x80484b4 <dummy>
```

first parameter is resulted integer and the second is `envp`. It's 
important to note that after calling `dummy` we test `eax` 
(return value) on zero value and if it is we exit `parell`

```
   0x08048547 <+45>:    call   0x80484b4 <dummy>
   0x0804854c <+50>:    test   eax,eax
   0x0804854e <+52>:    je     0x8048586 <parell+108>
   0x08048550 <+54>:    mov    DWORD PTR [ebp-0x8],0x0
   0x08048557 <+61>:    cmp    DWORD PTR [ebp-0x8],0x9
   0x0804855b <+65>:    jg     0x8048586 <parell+108>
   0x0804855d <+67>:    mov    eax,DWORD PTR [ebp-0x4]
   0x08048560 <+70>:    and    eax,0x1
   0x08048563 <+73>:    test   eax,eax
   0x08048565 <+75>:    jne    0x804857f <parell+101>
   0x08048567 <+77>:    mov    DWORD PTR [esp],0x8048740
   0x0804856e <+84>:    call   0x80483b8 <printf@plt>
   0x08048573 <+89>:    mov    DWORD PTR [esp],0x0
   0x0804857a <+96>:    call   0x80483e8 <exit@plt>
```

if return value of `dummy` is not zero then we go over `je` and test
if integer at `[ebp-0x4]` is even, if it is then this `printf` and
`exit` are called

```
   0x08048567 <+77>:    mov    DWORD PTR [esp],0x8048740
   0x0804856e <+84>:    call   0x80483b8 <printf@plt>
   0x08048573 <+89>:    mov    DWORD PTR [esp],0x0
   0x0804857a <+96>:    call   0x80483e8 <exit@plt>
```

seems interesting, let's look what format string contains

```
(gdb) x/s 0x8048740
0x8048740:      "Password OK!\n"
```

So to solve this puzzle we need to return non-zero value from `dummy`
function. Let's disassemble it

```
(gdb) disassemble dummy
Dump of assembler code for function dummy:
   0x080484b4 <+0>:     push   ebp
   0x080484b5 <+1>:     mov    ebp,esp
   0x080484b7 <+3>:     sub    esp,0x18
   0x080484ba <+6>:     mov    DWORD PTR [ebp-0x4],0x0
   0x080484c1 <+13>:    mov    eax,DWORD PTR [ebp-0x4]
   0x080484c4 <+16>:    lea    edx,[eax*4+0x0]
   0x080484cb <+23>:    mov    eax,DWORD PTR [ebp+0xc]
   0x080484ce <+26>:    cmp    DWORD PTR [edx+eax*1],0x0
   0x080484d2 <+30>:    je     0x804850e <dummy+90>
   0x080484d4 <+32>:    mov    eax,DWORD PTR [ebp-0x4]
   0x080484d7 <+35>:    lea    ecx,[eax*4+0x0]
   0x080484de <+42>:    mov    edx,DWORD PTR [ebp+0xc]
   0x080484e1 <+45>:    lea    eax,[ebp-0x4]
   0x080484e4 <+48>:    inc    DWORD PTR [eax]
   0x080484e6 <+50>:    mov    DWORD PTR [esp+0x8],0x3
   0x080484ee <+58>:    mov    DWORD PTR [esp+0x4],0x8048738
   0x080484f6 <+66>:    mov    eax,DWORD PTR [ecx+edx*1]
   0x080484f9 <+69>:    mov    DWORD PTR [esp],eax
   0x080484fc <+72>:    call   0x80483d8 <strncmp@plt>
   0x08048501 <+77>:    test   eax,eax
   0x08048503 <+79>:    jne    0x80484c1 <dummy+13>
   0x08048505 <+81>:    mov    DWORD PTR [ebp-0x8],0x1
   0x0804850c <+88>:    jmp    0x8048515 <dummy+97>
   0x0804850e <+90>:    mov    DWORD PTR [ebp-0x8],0x0
   0x08048515 <+97>:    mov    eax,DWORD PTR [ebp-0x8]
   0x08048518 <+100>:   leave  
   0x08048519 <+101>:   ret    
End of assembler dump.
```

Here

```
   0x080484ba <+6>:     mov    DWORD PTR [ebp-0x4],0x0
   0x080484c1 <+13>:    mov    eax,DWORD PTR [ebp-0x4]
   0x080484c4 <+16>:    lea    edx,[eax*4+0x0]
   0x080484cb <+23>:    mov    eax,DWORD PTR [ebp+0xc]
   0x080484ce <+26>:    cmp    DWORD PTR [edx+eax*1],0x0
   0x080484d2 <+30>:    je     0x804850e <dummy+90>
```

we simply compare that `envp` pointer is not NULL, otherwise return 
0x0. After that there is the most interesting part

```
   0x080484e4 <+48>:    inc    DWORD PTR [eax]
   0x080484e6 <+50>:    mov    DWORD PTR [esp+0x8],0x3
   0x080484ee <+58>:    mov    DWORD PTR [esp+0x4],0x8048738
   0x080484f6 <+66>:    mov    eax,DWORD PTR [ecx+edx*1]
   0x080484f9 <+69>:    mov    DWORD PTR [esp],eax
   0x080484fc <+72>:    call   0x80483d8 <strncmp@plt>
```

We compare `envp` bytes with string at address 0x8048738, let's see
what is there

```
(gdb) x/s 0x8048738
0x8048738:      "LOLO"
```

and after 
that compare return value with zero i.e. if the first 3 bytes of
both strings are equal. If it is then we return 0x1.

So, finally, we have: the password should be even number, and sum of
digits is 16, `envp` should contain string "LOLO"

Password is not the problem: we can take "88" that satisfy these 
criterias. But what we can do with `envp`? Ok, there I used hack
that we debug it in gdb :) So we can modify environment variables.

Let's re-run the program

```
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
```

and see what is in `[ebp+0x10]` right now while we are in `main`
function

```
(gdb) x/xw ($ebp+0x10)
0xffffdb38:     0xffffdbcc
```

it contains the address of pointer to our environment variables, let's
check what is there

```
(gdb) x/s *0xffffdbcc
0xffffdd43:     "XDG_VTNR=1"
```

In my case it is the first environment variable, let's check it

```
(gdb) show environment 
XDG_VTNR=1
XDG_SESSION_ID=c1
TERM=xterm
SHELL=/bin/bash
WINDOWID=23068706
XTERM_SHELL=/bin/bash
USER=mind
...
```

Here we go. Let's rewrite the address 0xffffdd43 by "LOLO=LOL" string

```
(gdb) set {char [9]} 0xffffdd43 = "LOLO=LOL"
```

now check if it was overwritten

```
(gdb) x/s 0xffffdd43
0xffffdd43:     "LOLO=LOL"
```

Now let's continue execution of our program and enter "88" as password

```
(gdb) c
Continuing.
IOLI Crackme Level 0x06
Password: 88
Password OK!
[Inferior 1 (process 17708) exited normally]
```

there we go.
