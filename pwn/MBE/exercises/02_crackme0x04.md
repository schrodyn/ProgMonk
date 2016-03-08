#02 exercises: crackme0x04

Let's look in our friend gdb

```
gdb -q ./crackme0x04
```

Let's take a look at defined functions

```
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x0804833c  _init
0x08048364  __libc_start_main@plt
0x08048374  scanf@plt
0x08048384  strlen@plt
0x08048394  printf@plt
0x080483a4  sscanf@plt
0x080483b4  exit@plt
0x080483d0  _start
0x08048420  __do_global_dtors_aux
0x08048450  frame_dummy
0x08048484  check
0x08048509  main
0x08048570  __libc_csu_init
0x080485e0  __libc_csu_fini
0x080485e5  __i686.get_pc_thunk.bx
0x080485f0  __do_global_ctors_aux
0x08048614  _fini
```

now make all standard things

```
(gdb) set disassembly-flavor intel
(gdb) break main
(gdb) run
```

Now disassemble it

```
(gdb) disassemble 
Dump of assembler code for function main:
   0x08048509 <+0>:     push   ebp
   0x0804850a <+1>:     mov    ebp,esp
   0x0804850c <+3>:     sub    esp,0x88
=> 0x08048512 <+9>:     and    esp,0xfffffff0
   0x08048515 <+12>:    mov    eax,0x0
   0x0804851a <+17>:    add    eax,0xf
   0x0804851d <+20>:    add    eax,0xf
   0x08048520 <+23>:    shr    eax,0x4
   0x08048523 <+26>:    shl    eax,0x4
   0x08048526 <+29>:    sub    esp,eax
   0x08048528 <+31>:    mov    DWORD PTR [esp],0x804865e
   0x0804852f <+38>:    call   0x8048394 <printf@plt>
   0x08048534 <+43>:    mov    DWORD PTR [esp],0x8048677
   0x0804853b <+50>:    call   0x8048394 <printf@plt>
   0x08048540 <+55>:    lea    eax,[ebp-0x78]
   0x08048543 <+58>:    mov    DWORD PTR [esp+0x4],eax
   0x08048547 <+62>:    mov    DWORD PTR [esp],0x8048682
   0x0804854e <+69>:    call   0x8048374 <scanf@plt>
   0x08048553 <+74>:    lea    eax,[ebp-0x78]
   0x08048556 <+77>:    mov    DWORD PTR [esp],eax
   0x08048559 <+80>:    call   0x8048484 <check>
   0x0804855e <+85>:    mov    eax,0x0
   0x08048563 <+90>:    leave  
   0x08048564 <+91>:    ret    
End of assembler dump.
```

Let's look at format string for `scanf`

```
(gdb) x/s 0x8048682
0x8048682:      "%s"
```

Now let's think what is happening here

```
0x08048540 <+55>:    lea    eax,[ebp-0x78]
0x08048543 <+58>:    mov    DWORD PTR [esp+0x4],eax
0x08048547 <+62>:    mov    DWORD PTR [esp],0x8048682
0x0804854e <+69>:    call   0x8048374 <scanf@plt>
0x08048553 <+74>:    lea    eax,[ebp-0x78]
0x08048556 <+77>:    mov    DWORD PTR [esp],eax
0x08048559 <+80>:    call   0x8048484 <check>
```

Scanned string will be placed to the address `[ebp-0x78]` and then
this address will be passed to the `check` function. Let's see what is
here

```
(gdb) disassemble check
Dump of assembler code for function check:
   0x08048484 <+0>:     push   ebp
   0x08048485 <+1>:     mov    ebp,esp
   0x08048487 <+3>:     sub    esp,0x28
   0x0804848a <+6>:     mov    DWORD PTR [ebp-0x8],0x0
   0x08048491 <+13>:    mov    DWORD PTR [ebp-0xc],0x0
   0x08048498 <+20>:    mov    eax,DWORD PTR [ebp+0x8]
   0x0804849b <+23>:    mov    DWORD PTR [esp],eax
   0x0804849e <+26>:    call   0x8048384 <strlen@plt>
   0x080484a3 <+31>:    cmp    DWORD PTR [ebp-0xc],eax
   0x080484a6 <+34>:    jae    0x80484fb <check+119>
   0x080484a8 <+36>:    mov    eax,DWORD PTR [ebp-0xc]
   0x080484ab <+39>:    add    eax,DWORD PTR [ebp+0x8]
   0x080484ae <+42>:    movzx  eax,BYTE PTR [eax]
   0x080484b1 <+45>:    mov    BYTE PTR [ebp-0xd],al
   0x080484b4 <+48>:    lea    eax,[ebp-0x4]
   0x080484b7 <+51>:    mov    DWORD PTR [esp+0x8],eax
   0x080484bb <+55>:    mov    DWORD PTR [esp+0x4],0x8048638
   0x080484c3 <+63>:    lea    eax,[ebp-0xd]
   0x080484c6 <+66>:    mov    DWORD PTR [esp],eax
   0x080484c9 <+69>:    call   0x80483a4 <sscanf@plt>
   0x080484ce <+74>:    mov    edx,DWORD PTR [ebp-0x4]
   0x080484d1 <+77>:    lea    eax,[ebp-0x8]
   0x080484d4 <+80>:    add    DWORD PTR [eax],edx
   0x080484d6 <+82>:    cmp    DWORD PTR [ebp-0x8],0xf
   0x080484da <+86>:    jne    0x80484f4 <check+112>
   0x080484dc <+88>:    mov    DWORD PTR [esp],0x804863b
   0x080484e3 <+95>:    call   0x8048394 <printf@plt>
   0x080484e8 <+100>:   mov    DWORD PTR [esp],0x0
   0x080484ef <+107>:   call   0x80483b4 <exit@plt>
   0x080484f4 <+112>:   lea    eax,[ebp-0xc]
   0x080484f7 <+115>:   inc    DWORD PTR [eax]
   0x080484f9 <+117>:   jmp    0x8048498 <check+20>
   0x080484fb <+119>:   mov    DWORD PTR [esp],0x8048649
   0x08048502 <+126>:   call   0x8048394 <printf@plt>
   0x08048507 <+131>:   leave  
   0x08048508 <+132>:   ret
```

Here we calculate length of our string

```
0x08048498 <+20>:    mov    eax,DWORD PTR [ebp+0x8]
0x0804849b <+23>:    mov    DWORD PTR [esp],eax
0x0804849e <+26>:    call   0x8048384 <strlen@plt>
```

and here we compare string length with zero since in `[ebp-0xc]` we
placed 0x0.

```
0x080484a3 <+31>:    cmp    DWORD PTR [ebp-0xc],eax
```

if we pass null-length string than string at 0x8048649 address will be
printed:

```
0x080484fb <+119>:   mov    DWORD PTR [esp],0x8048649
0x08048502 <+126>:   call   0x8048394 <printf@plt>
```

We may already guess that this is a fail message

```
(gdb) x/s 0x8048649
0x8048649:      "Password Incorrect!\n"
```

Ok, now let's go ahead and see what will be otherwise. Here

```
0x080484a8 <+36>:    mov    eax,DWORD PTR [ebp-0xc]
0x080484ab <+39>:    add    eax,DWORD PTR [ebp+0x8]
```

we put the address of the first element of char array to `eax`
register.

Then copy first byte of the array to `eax`

```
0x080484ae <+42>:    movzx  eax,BYTE PTR [eax]
0x080484b1 <+45>:    mov    BYTE PTR [ebp-0xd],al
```

There 

```
0x080484b4 <+48>:    lea    eax,[ebp-0x4]
0x080484b7 <+51>:    mov    DWORD PTR [esp+0x8],eax
0x080484bb <+55>:    mov    DWORD PTR [esp+0x4],0x8048638
0x080484c3 <+63>:    lea    eax,[ebp-0xd]
0x080484c6 <+66>:    mov    DWORD PTR [esp],eax
0x080484c9 <+69>:    call   0x80483a4 <sscanf@plt>
```

we convert the character string at the address `[ebp-0xd]` to integer
value and store result at address `[ebp-0x4]` since **format** string
at address 0x8048638 is

```
(gdb) x/s 0x8048638
0x8048638:      "%d"
```

After that comparison goes ahead and depending on its result we
see or doesn't see "Password OK" string

```
0x080484ce <+74>:    mov    edx,DWORD PTR [ebp-0x4]
0x080484d1 <+77>:    lea    eax,[ebp-0x8]
0x080484d4 <+80>:    add    DWORD PTR [eax],edx
0x080484d6 <+82>:    cmp    DWORD PTR [ebp-0x8],0xf
0x080484da <+86>:    jne    0x80484f4 <check+112>
0x080484dc <+88>:    mov    DWORD PTR [esp],0x804863b
0x080484e3 <+95>:    call   0x8048394 <printf@plt>
```

since at address 0x804863b there is a string

```
(gdb) x/s 0x804863b
0x804863b:      "Password OK!\n"
```

Let's debug it. First let's make a `fifo` file for better debugging
with which we could pipeline input to our binary

```
$ mkfifo exp
```

After that let's setup breakpoint at

```
0x0804849e <+26>:    call   0x8048384 <strlen@plt>
```

```
(gdb) b *0x0804849e
```

Now let's pipeline our test input in `exp` fifo file in another
terminal.

```
$ echo -e "\x0f\x3d\x3d\x3d" > exp
```

and in our gdb session rerun the program with input from `exp`

```
(gdb) r < exp
```

we stopped at first breakpoint, let's continue

```
(gdb) c
(gdb) disassemble 
Dump of assembler code for function check:
   0x08048484 <+0>:     push   ebp
   0x08048485 <+1>:     mov    ebp,esp
   0x08048487 <+3>:     sub    esp,0x28
   0x0804848a <+6>:     mov    DWORD PTR [ebp-0x8],0x0
   0x08048491 <+13>:    mov    DWORD PTR [ebp-0xc],0x0
   0x08048498 <+20>:    mov    eax,DWORD PTR [ebp+0x8]
   0x0804849b <+23>:    mov    DWORD PTR [esp],eax
=> 0x0804849e <+26>:    call   0x8048384 <strlen@plt>
   0x080484a3 <+31>:    cmp    DWORD PTR [ebp-0xc],eax
   0x080484a6 <+34>:    jae    0x80484fb <check+119>
   0x080484a8 <+36>:    mov    eax,DWORD PTR [ebp-0xc]
   0x080484ab <+39>:    add    eax,DWORD PTR [ebp+0x8]
   0x080484ae <+42>:    movzx  eax,BYTE PTR [eax]
   0x080484b1 <+45>:    mov    BYTE PTR [ebp-0xd],al
   0x080484b4 <+48>:    lea    eax,[ebp-0x4]
   0x080484b7 <+51>:    mov    DWORD PTR [esp+0x8],eax
   0x080484bb <+55>:    mov    DWORD PTR [esp+0x4],0x8048638
```

After some time of debugging I've understand that we can pass only
from "1" to "9" as 1 byte for `sscanf` input. It means that it's 
impossible to right "correct" password (maybe I don't understand
something) but we can overflow buffer for storing our password :)

Let's use `pattern` python utility for generating pattern of 
characters, you can download it [here](https://github.com/Svenito/exploit-pattern). Now we want to pass control to this part of the program

```
0x080484dc <+88>:    mov    DWORD PTR [esp],0x804863b
0x080484e3 <+95>:    call   0x8048394 <printf@plt>
0x080484e8 <+100>:   mov    DWORD PTR [esp],0x0
0x080484ef <+107>:   call   0x80483b4 <exit@plt>
```

Let's again disassemble `main` function and set up breakpoint at `ret`
instruction.

```
(gdb) disassemble main
Dump of assembler code for function main:
   0x08048509 <+0>:     push   ebp
   0x0804850a <+1>:     mov    ebp,esp
   0x0804850c <+3>:     sub    esp,0x88
   0x08048512 <+9>:     and    esp,0xfffffff0
   0x08048515 <+12>:    mov    eax,0x0
   0x0804851a <+17>:    add    eax,0xf
   0x0804851d <+20>:    add    eax,0xf
   0x08048520 <+23>:    shr    eax,0x4
   0x08048523 <+26>:    shl    eax,0x4
   0x08048526 <+29>:    sub    esp,eax
   0x08048528 <+31>:    mov    DWORD PTR [esp],0x804865e
   0x0804852f <+38>:    call   0x8048394 <printf@plt>
   0x08048534 <+43>:    mov    DWORD PTR [esp],0x8048677
   0x0804853b <+50>:    call   0x8048394 <printf@plt>
   0x08048540 <+55>:    lea    eax,[ebp-0x78]
   0x08048543 <+58>:    mov    DWORD PTR [esp+0x4],eax
   0x08048547 <+62>:    mov    DWORD PTR [esp],0x8048682
   0x0804854e <+69>:    call   0x8048374 <scanf@plt>
   0x08048553 <+74>:    lea    eax,[ebp-0x78]
   0x08048556 <+77>:    mov    DWORD PTR [esp],eax
   0x08048559 <+80>:    call   0x8048484 <check>
   0x0804855e <+85>:    mov    eax,0x0
   0x08048563 <+90>:    leave  
   0x08048564 <+91>:    ret    
End of assembler dump.
```

```
(gdb) b *0x08048564
```


Now we start program with `exp` pipeline input

```
(gdb) r < exp
```

and generate long character pattern

```
$ pattern 500 > exp
```

```
(gdb) r < exp
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/mind/documents/Pwn_Party/MBE/challenges/crackme0x04 < exp
IOLI Crackme Level 0x04
Password: Password Incorrect!
Dump of assembler code from 0x8048564 to 0x8048573:
=> 0x08048564 <main+91>:        ret    
   0x08048565:  nop
   0x08048566:  nop
   0x08048567:  nop
   0x08048568:  nop
   0x08048569:  nop
   0x0804856a:  nop
   0x0804856b:  nop
   0x0804856c:  nop
   0x0804856d:  nop
   0x0804856e:  nop
   0x0804856f:  nop
   0x08048570 <__libc_csu_init+0>:      push   ebp
   0x08048571 <__libc_csu_init+1>:      mov    ebp,esp
End of assembler dump.
```

Ok, now let's what is in `esp`

```
(gdb) x/xw $esp
0xffffdb6c:     0x65413165
```

and now let's get offset of the pattern

```
$ pattern 0x65413165
Pattern 0x65413165 first occurrence at position 124 in pattern.
```

So we need to place in buffer 124 characters of pattern and then 
address of our `printf` statement. Let's do it in small script

```py
#!/usr/bin/env python2

import struct

pattern="Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0A"

exp=pattern+struct.pack("<L", 0x080484dc)

print exp
```

Let's test it

```
$ ./exp04.py | ./crackme0x04 
IOLI Crackme Level 0x04
Password: Password Incorrect!
Password OK!
```

Voila! We've got it :)



