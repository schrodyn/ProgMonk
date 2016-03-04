#02 Exercise Solutions

##crackme0x00a

**First Method**

Use `xxd` as hex viewer

```
xxd crackme0x00a | less
```

and find password in the bottom of the binary

```
00000fd0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000fe0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000ff0: 0000 0000 289f 0408 0000 0000 0000 0000  ....(...........
00001000: c683 0408 d683 0408 e683 0408 f683 0408  ................
00001010: 0684 0408 1684 0408 2684 0408 0000 0000  ........&.......
00001020: 0000 0000 6730 3064 4a30 4221 0000 0000  ....g00dJ0B!....
00001030: 4743 433a 2028 5562 756e 7475 2f4c 696e  GCC: (Ubuntu/Lin
00001040: 6172 6f20 342e 362e 312d 3975 6275 6e74  aro 4.6.1-9ubunt
00001050: 7533 2920 342e 362e 3100 002e 7379 6d74  u3) 4.6.1...symt
00001060: 6162 002e 7374 7274 6162 002e 7368 7374  ab..strtab..shst
00001070: 7274 6162 002e 696e 7465 7270 002e 6e6f  rtab..interp..no
00001080: 7465 2e41 4249 2d74 6167 002e 6e6f 7465  te.ABI-tag..note
```

answer: `g00dJ0B!`

**Second Method**

Just issue

```
strings crackme0x00a | less
```

and find there password

##crackme0x00b

```
strings -e L crackme0x00b
```

option `-e` is enconding, `L` is 32-bit littleendian, default is `s`
-- single-7-bit-byte characters (ASCII, ISO 8859, etc)

Answer: `w0wgreat`

##crackme0x01

```
objdump -d crackme0x01 | less
```

Here in `main` we find these lines

```
 8048426:       e8 e1 fe ff ff          call   804830c <scanf@plt>
 804842b:       81 7d fc 9a 14 00 00    cmpl   $0x149a,-0x4(%ebp)
 8048432:       74 0e                   je     8048442 <main+0x5e>
 8048434:       c7 04 24 4f 85 04 08    movl   $0x804854f,(%esp)
 804843b:       e8 dc fe ff ff          call   804831c <printf@plt>
 8048440:       eb 0c                   jmp    804844e <main+0x6a>
 8048442:       c7 04 24 62 85 04 08    movl   $0x8048562,(%esp)
 8048449:       e8 ce fe ff ff          call   804831c <printf@plt>
```

as we may guess answer is $0x149a, let's try it, but we need to 
convert it from hex to decimal

```
echo $((0x149a))
```

Let's test it

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: 5274
Password OK :)
```

##crackme0x02

```
objdump -d crackme0x02 | less
```

Here in `main` we find these lines

```
 8048426:       e8 e1 fe ff ff          call   804830c <scanf@plt>
 804842b:       c7 45 f8 5a 00 00 00    movl   $0x5a,-0x8(%ebp)
 8048432:       c7 45 f4 ec 01 00 00    movl   $0x1ec,-0xc(%ebp)
 8048439:       8b 55 f4                mov    -0xc(%ebp),%edx
 804843c:       8d 45 f8                lea    -0x8(%ebp),%eax
 804843f:       01 10                   add    %edx,(%eax)
 8048441:       8b 45 f8                mov    -0x8(%ebp),%eax
 8048444:       0f af 45 f8             imul   -0x8(%ebp),%eax
 8048448:       89 45 f4                mov    %eax,-0xc(%ebp)
 804844b:       8b 45 fc                mov    -0x4(%ebp),%eax
 804844e:       3b 45 f4                cmp    -0xc(%ebp),%eax
 8048451:       75 0e                   jne    8048461 <main+0x7d>
 8048453:       c7 04 24 6f 85 04 08    movl   $0x804856f,(%esp)
 804845a:       e8 bd fe ff ff          call   804831c <printf@plt>
 804845f:       eb 0c                   jmp    804846d <main+0x89>
 8048461:       c7 04 24 7f 85 04 08    movl   $0x804857f,(%esp)
 8048468:       e8 af fe ff ff          call   804831c <printf@plt>
 804846d:       b8 00 00 00 00          mov    $0x0,%eax
 8048472:       c9                      leave  
 8048473:       c3                      ret  
```

Seems some arithmetic going here and than becomes comparing. Ok, here 
you have two options: run it in gdb and see what is going on, it's
more easy option :) or calculate it by hand. Let's do it by hand

Let `var1` be `-0x8(%ebp)`, `var2` be `-0xc(%ebp)` then

1. `movl   $0x5a,-0x8(%ebp)` == `var1 = 0x5a`
2. `movl   $0x1ec,-0xc(%ebp)` == `var2 = 0x1ec`
3. `mov    -0xc(%ebp),%edx` == `edx = var2` 

```asm
lea    -0x8(%ebp),%eax
add    %edx,(%eax)
```

means that `var1 += edx`

```asm
mov    -0x8(%ebp),%eax
imul   -0x8(%ebp),%eax
```

means that `eax = var1 * var1`

```
mov    %eax,-0xc(%ebp)
mov    -0x4(%ebp),%eax
cmp    -0xc(%ebp),%eax
```

means `var2 = eax; eax = (ebp-0x4);` and compare `var2` with `eax`

As we may guess `(ebp-0x4)` is pointer to the address of readen value,
so we have

```
var1 = 0x5a;
var2 = 0x1ec;
var1 += var2;
eax = var1 * var1;
var2 = eax;
eax = (ebp-0x4);
compare var2 with eax
```

so we should to enter `(0x5a + 0x1ec)^2 = 338724`. Let's try it

```
$ ./crackme0x02
IOLI Crackme Level 0x02
Password: 338724
Password OK :)
```

Okay :)

##crackme0x03

Let's run it in gdb

```
gdb -q ./crackme0x03
```

```
(gdb) set disassembly-flavor intel
(gdb) info functions
Non-debugging symbols:
0x080482f8  _init
0x08048320  __libc_start_main@plt
0x08048330  scanf@plt
0x08048340  strlen@plt
0x08048350  printf@plt
0x08048360  _start
0x080483b0  __do_global_dtors_aux
0x080483e0  frame_dummy
0x08048414  shift
0x0804846e  test
0x08048498  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048595  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
---Type <return> to continue, or q <return> to quit---
```

Let's see what is in `main`

```
(gdb) disassemble main
Dump of assembler code for function main:
   0x08048498 <+0>:     push   ebp
   0x08048499 <+1>:     mov    ebp,esp
   0x0804849b <+3>:     sub    esp,0x18
   0x0804849e <+6>:     and    esp,0xfffffff0
   0x080484a1 <+9>:     mov    eax,0x0
   0x080484a6 <+14>:    add    eax,0xf
   0x080484a9 <+17>:    add    eax,0xf
   0x080484ac <+20>:    shr    eax,0x4
   0x080484af <+23>:    shl    eax,0x4
   0x080484b2 <+26>:    sub    esp,eax
   0x080484b4 <+28>:    mov    DWORD PTR [esp],0x8048610
   0x080484bb <+35>:    call   0x8048350 <printf@plt>
   0x080484c0 <+40>:    mov    DWORD PTR [esp],0x8048629
   0x080484c7 <+47>:    call   0x8048350 <printf@plt>
   0x080484cc <+52>:    lea    eax,[ebp-0x4]
   0x080484cf <+55>:    mov    DWORD PTR [esp+0x4],eax
   0x080484d3 <+59>:    mov    DWORD PTR [esp],0x8048634
   0x080484da <+66>:    call   0x8048330 <scanf@plt>
   0x080484df <+71>:    mov    DWORD PTR [ebp-0x8],0x5a
   0x080484e6 <+78>:    mov    DWORD PTR [ebp-0xc],0x1ec
   0x080484ed <+85>:    mov    edx,DWORD PTR [ebp-0xc]
   0x080484f0 <+88>:    lea    eax,[ebp-0x8]
   0x080484f3 <+91>:    add    DWORD PTR [eax],edx
   0x080484f5 <+93>:    mov    eax,DWORD PTR [ebp-0x8]
   0x080484f8 <+96>:    imul   eax,DWORD PTR [ebp-0x8]
   0x080484fc <+100>:   mov    DWORD PTR [ebp-0xc],eax
   0x080484ff <+103>:   mov    eax,DWORD PTR [ebp-0xc]
   0x08048502 <+106>:   mov    DWORD PTR [esp+0x4],eax
   0x08048506 <+110>:   mov    eax,DWORD PTR [ebp-0x4]
   0x08048509 <+113>:   mov    DWORD PTR [esp],eax
   0x0804850c <+116>:   call   0x804846e <test>
   0x08048511 <+121>:   mov    eax,0x0
   0x08048516 <+126>:   leave  
   0x08048517 <+127>:   ret    
End of assembler dump.
```

Let's see what is in 0x8048629 address since it is a pointer to
format string for `printf` 

```
(gdb) x/s 0x8048629
0x8048629:      "Password: "
```

Ok, as we see here

```
0x080484cc <+52>:    lea    eax,[ebp-0x4]
0x080484cf <+55>:    mov    DWORD PTR [esp+0x4],eax
0x080484d3 <+59>:    mov    DWORD PTR [esp],0x8048634
0x080484da <+66>:    call   0x8048330 <scanf@plt>
```

Result of `scanf` will be inserted into first local variable in `main`
since it has `ebp-0x4` address. Format string is at address 0x8048634
Let's look what is here

```
(gdb) x/s 0x8048634
0x8048634:      "%d"
```

Ok, so we scanf integer value. Now, let's go ahead and see that
the last part is exactly what was in `crackme0x02` except that now
we don't use `cmp` but call function `test`. Ok, let's see now
what it is.

```
(gdb) disassemble test
Dump of assembler code for function test:
   0x0804846e <+0>:     push   ebp
   0x0804846f <+1>:     mov    ebp,esp
   0x08048471 <+3>:     sub    esp,0x8
   0x08048474 <+6>:     mov    eax,DWORD PTR [ebp+0x8]
   0x08048477 <+9>:     cmp    eax,DWORD PTR [ebp+0xc]
   0x0804847a <+12>:    je     0x804848a <test+28>
   0x0804847c <+14>:    mov    DWORD PTR [esp],0x80485ec
   0x08048483 <+21>:    call   0x8048414 <shift>
   0x08048488 <+26>:    jmp    0x8048496 <test+40>
   0x0804848a <+28>:    mov    DWORD PTR [esp],0x80485fe
   0x08048491 <+35>:    call   0x8048414 <shift>
   0x08048496 <+40>:    leave  
   0x08048497 <+41>:    ret    
End of assembler dump.
```

we see that `ebp+0x8` in `test` function exactly what is `esp`
in `main` function, and `ebp+0xc` exactly is `esp+0x4` in 
`main` function.

Also the idea of `test` function as follows: make comparison and
call `shift` function with appropriate argument. Let's see what
at 0x80485ec as string

```
(gdb) x/s 0x80485ec
0x80485ec:      "Lqydolg#Sdvvzrug$"
```

Something strange. Now, we are going to disassemble `shift`
function

```
(gdb) disassemble shift
Dump of assembler code for function shift:
   0x08048414 <+0>:     push   ebp
   0x08048415 <+1>:     mov    ebp,esp
   0x08048417 <+3>:     sub    esp,0x98
   0x0804841d <+9>:     mov    DWORD PTR [ebp-0x7c],0x0
   0x08048424 <+16>:    mov    eax,DWORD PTR [ebp+0x8]
   0x08048427 <+19>:    mov    DWORD PTR [esp],eax
   0x0804842a <+22>:    call   0x8048340 <strlen@plt>
   0x0804842f <+27>:    cmp    DWORD PTR [ebp-0x7c],eax
   0x08048432 <+30>:    jae    0x8048450 <shift+60>
   0x08048434 <+32>:    lea    eax,[ebp-0x78]
   0x08048437 <+35>:    mov    edx,eax
   0x08048439 <+37>:    add    edx,DWORD PTR [ebp-0x7c]
   0x0804843c <+40>:    mov    eax,DWORD PTR [ebp-0x7c]
   0x0804843f <+43>:    add    eax,DWORD PTR [ebp+0x8]
   0x08048442 <+46>:    movzx  eax,BYTE PTR [eax]
   0x08048445 <+49>:    sub    al,0x3
   0x08048447 <+51>:    mov    BYTE PTR [edx],al
   0x08048449 <+53>:    lea    eax,[ebp-0x7c]
   0x0804844c <+56>:    inc    DWORD PTR [eax]
   0x0804844e <+58>:    jmp    0x8048424 <shift+16>
   0x08048450 <+60>:    lea    eax,[ebp-0x78]
   0x08048453 <+63>:    add    eax,DWORD PTR [ebp-0x7c]
   0x08048456 <+66>:    mov    BYTE PTR [eax],0x0
   0x08048459 <+69>:    lea    eax,[ebp-0x78]
   0x0804845c <+72>:    mov    DWORD PTR [esp+0x4],eax
   0x08048460 <+76>:    mov    DWORD PTR [esp],0x80485e8
   0x08048467 <+83>:    call   0x8048350 <printf@plt>
   0x0804846c <+88>:    leave  
   0x0804846d <+89>:    ret    
End of assembler dump.
``` 

We saw that `shift` seems to take one parameter at `esp`. Oh now,
it seems like holly shit, let's debug :)

```
(gdb) break main
Breakpoint 1 at 0x804849e
(gdb) run
Starting program: /home/mind/documents/Pwn_Party/MBE/challenges/crackme0x03 

Breakpoint 1, 0x0804849e in main ()
(gdb) define hook-stop
Type commands for definition of "hook-stop".
End with a line saying just "end".
>disassemble $eip,+20
>end
(gdb) display/8xw $esp
1: x/8xw $esp
0xffffdb50:     0xf7fbb3bc      0x080481f8      0x08049f0c      0x00000000
0xffffdb60:     0xf7fbb000      0xf7fbb000      0x00000000      0xf7e22527
```

Ok, now let's step to the `test` call

```
(gdb) 
Dump of assembler code from 0x804850c to 0x8048520:
=> 0x0804850c <main+116>:       call   0x804846e <test>
   0x08048511 <main+121>:       mov    $0x0,%eax
   0x08048516 <main+126>:       leave  
   0x08048517 <main+127>:       ret    
   0x08048518:  nop
   0x08048519:  nop
   0x0804851a:  nop
   0x0804851b:  nop
   0x0804851c:  nop
   0x0804851d:  nop
   0x0804851e:  nop
   0x0804851f:  nop
End of assembler dump.
0x0804850c in main ()
1: x/8xw $esp
0xffffdb40:     0x0000007b      0x00052b24      0x00000000      0xf7e3896b
0xffffdb50:     0xf7fbb3bc      0x080481f8      0x08049f0c      0x00052b24
```

and make breakpoint here 

```
(gdb) b *0x0804850c
Breakpoint 2 at 0x804850c
```

go ahead and enter to `shift` function

```
(gdb) disassemble shift
Dump of assembler code for function shift:
   0x08048414 <+0>:     push   ebp
   0x08048415 <+1>:     mov    ebp,esp
=> 0x08048417 <+3>:     sub    esp,0x98
   0x0804841d <+9>:     mov    DWORD PTR [ebp-0x7c],0x0
   0x08048424 <+16>:    mov    eax,DWORD PTR [ebp+0x8]
   0x08048427 <+19>:    mov    DWORD PTR [esp],eax
   0x0804842a <+22>:    call   0x8048340 <strlen@plt>
   0x0804842f <+27>:    cmp    DWORD PTR [ebp-0x7c],eax
   0x08048432 <+30>:    jae    0x8048450 <shift+60>
   0x08048434 <+32>:    lea    eax,[ebp-0x78]
   0x08048437 <+35>:    mov    edx,eax
   0x08048439 <+37>:    add    edx,DWORD PTR [ebp-0x7c]
   0x0804843c <+40>:    mov    eax,DWORD PTR [ebp-0x7c]
   0x0804843f <+43>:    add    eax,DWORD PTR [ebp+0x8]
   0x08048442 <+46>:    movzx  eax,BYTE PTR [eax]
   0x08048445 <+49>:    sub    al,0x3
   0x08048447 <+51>:    mov    BYTE PTR [edx],al
   0x08048449 <+53>:    lea    eax,[ebp-0x7c]
   0x0804844c <+56>:    inc    DWORD PTR [eax]
   0x0804844e <+58>:    jmp    0x8048424 <shift+16>
   0x08048450 <+60>:    lea    eax,[ebp-0x78]
   0x08048453 <+63>:    add    eax,DWORD PTR [ebp-0x7c]
   0x08048456 <+66>:    mov    BYTE PTR [eax],0x0
   0x08048459 <+69>:    lea    eax,[ebp-0x78]
   0x0804845c <+72>:    mov    DWORD PTR [esp+0x4],eax
   0x08048460 <+76>:    mov    DWORD PTR [esp],0x80485e8
   0x08048467 <+83>:    call   0x8048350 <printf@plt>
   0x0804846c <+88>:    leave  
   0x0804846d <+89>:    ret    
End of assembler dump.
```

Here we think, think, think.. and see that here is the iterating loop 
over array: 

`DWORD PTR [ebp-0x7c]` is the counter, and `[ebp-0x78]` is the start 
address of array. Then `edx` is the pointer to the current element of
array we are iterating over.

`[ebp+0x8]` is the address of previous `esp` in `test` function.
I've understood! in `esp` we pass the address of another array, and
then also iterate it and save it in local array with some modification
and then print local array. We can say something like that

```
local[i] = upper[i] - 3;
```

So let's decypher previous strange string "Lqydolg#Sdvvzrug$" at
address 0x80485ec and use for this python

```
>>> s="Lqydolg#Sdvvzrug$"
>>> x = ''.join([chr(ord(ch)-3) for ch in s])
>>> x
'Invalid Password!'
```

Ok, so we may guess that another strange string at 0x80485fe address
is "good password" phrase :)

```
>>> s="Sdvvzrug#RN$$$#=,"
>>> x=''.join([chr(ord(ch)-3) for ch in s])
>>> x
'Password OK!!! :)'
```

That's it,

```
(gdb) disassemble test
Dump of assembler code for function test:
   0x0804846e <+0>:     push   ebp
   0x0804846f <+1>:     mov    ebp,esp
   0x08048471 <+3>:     sub    esp,0x8
   0x08048474 <+6>:     mov    eax,DWORD PTR [ebp+0x8]
   0x08048477 <+9>:     cmp    eax,DWORD PTR [ebp+0xc]
   0x0804847a <+12>:    je     0x804848a <test+28>
   0x0804847c <+14>:    mov    DWORD PTR [esp],0x80485ec
   0x08048483 <+21>:    call   0x8048414 <shift>
   0x08048488 <+26>:    jmp    0x8048496 <test+40>
   0x0804848a <+28>:    mov    DWORD PTR [esp],0x80485fe
   0x08048491 <+35>:    call   0x8048414 <shift>
   0x08048496 <+40>:    leave  
   0x08048497 <+41>:    ret    
End of assembler dump.
```

so we need equal values in `esp` and `esp+0x4` addresses while we are
in `main`. 

if we look in disassembled `main` we understand that password is 
the same as in previous example :) i.e. `(0x5a+0x1ec)^2 = 338724`

Let's try it

```
$ ./crackme0x03
IOLI Crackme Level 0x03
Password: 338724
Password OK!!! :)
```




