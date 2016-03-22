#lab3

Let's start using gdb

```
gdb -q ./lab3
```

```
(gdb) info functions 
All defined functions:

Non-debugging symbols:
0x08048400  printf@plt
0x08048410  __stack_chk_fail@plt
0x08048420  puts@plt
0x08048430  __gmon_start__@plt
0x08048440  exit@plt
0x08048450  __libc_start_main@plt
0x08048460  putchar@plt
0x08048470  __isoc99_scanf@plt
0x08048480  ptrace@plt
```

There we also don't have `main` function. So we go ahead and use
`objdump`

```
objdump -d lab3 -M intel | less
```

There we find these lines

```
Disassembly of section .text:

08048490 <.text>:
 8048490:       31 ed                   xor    ebp,ebp
 8048492:       5e                      pop    esi
 8048493:       89 e1                   mov    ecx,esp
 8048495:       83 e4 f0                and    esp,0xfffffff0
 8048498:       50                      push   eax
 8048499:       54                      push   esp
 804849a:       52                      push   edx
 804849b:       68 40 89 04 08          push   0x8048940
 80484a0:       68 d0 88 04 08          push   0x80488d0
 80484a5:       51                      push   ecx
 80484a6:       56                      push   esi
 80484a7:       68 69 88 04 08          push   0x8048869
 80484ac:       e8 9f ff ff ff          call   8048450 <__libc_start_main@plt>
```

So we go back to gdb and set breakpoint at `0x8048869` address.

```
(gdb) b *0x8048869
Breakpoint 1 at 0x8048869
```

Trying to run it

```
(gdb) r
Starting program: /home/mind/documents/Pwn_Party/MBE/lab1/lab3 
######
#     #  ######  #####   #    #   ####    ####      #    #    #   ####
#     #  #       #    #  #    #  #    #  #    #     #    ##   #  #    #
#     #  #####   #####   #    #  #       #          #    # #  #  #
#     #  #       #    #  #    #  #  ###  #  ###     #    #  # #  #  ###
#     #  #       #    #  #    #  #    #  #    #     #    #   ##  #    #
######   ######  #####    ####    ####    ####      #    #    #   ####


    #     ####
    #    #
    #     ####
    #         #
    #    #    #
    #     ####


   ##
  #  #
 #    #
 ######
 #    #
 #    #


  ####   #####   #    #   #####   ####   #    #
 #    #  #    #  #    #     #    #    #  #    #
 #       #    #  #    #     #    #       ######
 #       #####   #    #     #    #       #    #   ###
 #    #  #   #   #    #     #    #    #  #    #   ###
  ####   #    #   ####      #     ####   #    #   ###
[Inferior 1 (process 1288) exited with code 0255]
```

Something interesting is going there :) Let's understand more deeply

```
$ objdump -f lab3

lab3:     file format elf32-i386
architecture: i386, flags 0x00000112:
EXEC_P, HAS_SYMS, D_PAGED
start address 0x08048490
```

Now we setup breakpoint at start address and start debug

```
(gdb) b *0x08048490
Breakpoint 1 at 0x8048490
```

```
(gdb) set disassembly-flavor intel
(gdb) define hook-stop
>disassemble $eip,+50
>end
```

and make a lot of `nexti` until the message won't show

```
(gdb) nexti
Dump of assembler code from 0xf7e224b3 to 0xf7e224e5:
=> 0xf7e224b3 <__libc_start_main+131>:  call   DWORD PTR [esp+0x7c]
   0xf7e224b7 <__libc_start_main+135>:  add    esp,0x10
   0xf7e224ba <__libc_start_main+138>:  mov    eax,DWORD PTR [esp+0x8]
   0xf7e224be <__libc_start_main+142>:  mov    eax,DWORD PTR [eax-0xe8]
   0xf7e224c4 <__libc_start_main+148>:  mov    edx,DWORD PTR [eax+0x20c]
   0xf7e224ca <__libc_start_main+154>:  test   edx,edx
   0xf7e224cc <__libc_start_main+156>:  jne    0xf7e225d2 <__libc_start_main+418>
   0xf7e224d2 <__libc_start_main+162>:  test   ebx,ebx
   0xf7e224d4 <__libc_start_main+164>:  jne    0xf7e225ab <__libc_start_main+379>
   0xf7e224da <__libc_start_main+170>:  sub    esp,0xc
   0xf7e224dd <__libc_start_main+173>:  lea    eax,[esp+0x20]
   0xf7e224e1 <__libc_start_main+177>:  push   eax
   0xf7e224e2 <__libc_start_main+178>:  call   0xf7e35a10 <_setjmp>
End of assembler dump.
0xf7e224b3 in __libc_start_main () from /usr/lib32/libc.so.6
(gdb) 
######
#     #  ######  #####   #    #   ####    ####      #    #    #   ####
#     #  #       #    #  #    #  #    #  #    #     #    ##   #  #    #
#     #  #####   #####   #    #  #       #          #    # #  #  #
#     #  #       #    #  #    #  #  ###  #  ###     #    #  # #  #  ###
#     #  #       #    #  #    #  #    #  #    #     #    #   ##  #    #
######   ######  #####    ####    ####    ####      #    #    #   ####


    #     ####
    #    #
    #     ####
    #         #
    #    #    #
    #     ####


   ##
  #  #
 #    #
 ######
 #    #
 #    #


  ####   #####   #    #   #####   ####   #    #
 #    #  #    #  #    #     #    #    #  #    #
 #       #    #  #    #     #    #       ######
 #       #####   #    #     #    #       #    #   ###
 #    #  #   #   #    #     #    #    #  #    #   ###
  ####   #    #   ####      #     ####   #    #   ###
[Inferior 1 (process 3751) exited with code 0255]
Error while running hook_stop:
No registers.
```

Ok, after calling some routine at `[esp+0x7c]` address we exit. So we go ahead and want to know exactly what is going there.
For this purpose let's monitor the exact address of this routine

```
(gdb) x/xw $esp+0x7c
```

```
=> 0xf7e224b3 <__libc_start_main+131>:  call   DWORD PTR [esp+0x7c]
   0xf7e224b7 <__libc_start_main+135>:  add    esp,0x10
   0xf7e224ba <__libc_start_main+138>:  mov    eax,DWORD PTR [esp+0x8]
   0xf7e224be <__libc_start_main+142>:  mov    eax,DWORD PTR [eax-0xe8]
   0xf7e224c4 <__libc_start_main+148>:  mov    edx,DWORD PTR [eax+0x20c]
   0xf7e224ca <__libc_start_main+154>:  test   edx,edx
   0xf7e224cc <__libc_start_main+156>:  jne    0xf7e225d2 <__libc_start_main+418>
   0xf7e224d2 <__libc_start_main+162>:  test   ebx,ebx
   0xf7e224d4 <__libc_start_main+164>:  jne    0xf7e225ab <__libc_start_main+379>
   0xf7e224da <__libc_start_main+170>:  sub    esp,0xc
   0xf7e224dd <__libc_start_main+173>:  lea    eax,[esp+0x20]
   0xf7e224e1 <__libc_start_main+177>:  push   eax
   0xf7e224e2 <__libc_start_main+178>:  call   0xf7e35a10 <_setjmp>
End of assembler dump.
0xf7e224b3 in __libc_start_main () from /usr/lib32/libc.so.6
1: /x $esp+0x7c = 0xffffdbcc
```

restart program and when we enter `libc.so` address space than we can setup breakpoint at this location `0xffffdbcc`

```
(gdb) 
Dump of assembler code from 0xf7feed90 to 0xf7feedc2:
=> 0xf7feed90 <_dl_runtime_resolve+0>:  push   eax
   0xf7feed91 <_dl_runtime_resolve+1>:  push   ecx
   0xf7feed92 <_dl_runtime_resolve+2>:  push   edx
   0xf7feed93 <_dl_runtime_resolve+3>:  mov    edx,DWORD PTR [esp+0x10]
   0xf7feed97 <_dl_runtime_resolve+7>:  mov    eax,DWORD PTR [esp+0xc]
   0xf7feed9b <_dl_runtime_resolve+11>: call   0xf7fe85a0 <_dl_fixup>
   0xf7feeda0 <_dl_runtime_resolve+16>: pop    edx
   0xf7feeda1 <_dl_runtime_resolve+17>: mov    ecx,DWORD PTR [esp]
   0xf7feeda4 <_dl_runtime_resolve+20>: mov    DWORD PTR [esp],eax
   0xf7feeda7 <_dl_runtime_resolve+23>: mov    eax,DWORD PTR [esp+0x4]
   0xf7feedab <_dl_runtime_resolve+27>: ret    0xc
   0xf7feedae:  xchg   ax,ax
   0xf7feedb0 <_dl_runtime_profile+0>:  push   esp
   0xf7feedb1 <_dl_runtime_profile+1>:  add    DWORD PTR [esp],0x8
   0xf7feedb5 <_dl_runtime_profile+5>:  push   ebp
   0xf7feedb6 <_dl_runtime_profile+6>:  push   eax
   0xf7feedb7 <_dl_runtime_profile+7>:  push   ecx
   0xf7feedb8 <_dl_runtime_profile+8>:  push   edx
   0xf7feedb9 <_dl_runtime_profile+9>:  mov    ecx,esp
   0xf7feedbb <_dl_runtime_profile+11>: sub    esp,0x8
   0xf7feedbe <_dl_runtime_profile+14>: mov    DWORD PTR [esp+0x4],0xffffffff
```

```
(gdb) b *0xffffdbcc
Breakpoint 2 at 0xffffdbcc
```

and continue execution 

```
(gdb) c
Continuing.
Success!

[Inferior 1 (process 8151) exited normally]
Error while running hook_stop:
No registers.
```

Hmm.. now we have `Success!` message. not bad :)

If we now `stepi` at this location

```
(gdb) 
Dump of assembler code from 0xf7e224b3 to 0xf7e224e5:
=> 0xf7e224b3 <__libc_start_main+131>:  call   DWORD PTR [esp+0x7c]
   0xf7e224b7 <__libc_start_main+135>:  add    esp,0x10
   0xf7e224ba <__libc_start_main+138>:  mov    eax,DWORD PTR [esp+0x8]
   0xf7e224be <__libc_start_main+142>:  mov    eax,DWORD PTR [eax-0xe8]
   0xf7e224c4 <__libc_start_main+148>:  mov    edx,DWORD PTR [eax+0x20c]
   0xf7e224ca <__libc_start_main+154>:  test   edx,edx
   0xf7e224cc <__libc_start_main+156>:  jne    0xf7e225d2 <__libc_start_main+418>
   0xf7e224d2 <__libc_start_main+162>:  test   ebx,ebx
   0xf7e224d4 <__libc_start_main+164>:  jne    0xf7e225ab <__libc_start_main+379>
   0xf7e224da <__libc_start_main+170>:  sub    esp,0xc
   0xf7e224dd <__libc_start_main+173>:  lea    eax,[esp+0x20]
   0xf7e224e1 <__libc_start_main+177>:  push   eax
   0xf7e224e2 <__libc_start_main+178>:  call   0xf7e35a10 <_setjmp>
End of assembler dump.
0xf7e224b3 in __libc_start_main () from /usr/lib32/libc.so.6
(gdb) stepi
Dump of assembler code from 0x80488cc to 0x80488fe:
=> 0x080488cc:  ret    
   0x080488cd:  nop
   0x080488ce:  nop
   0x080488cf:  nop
   0x080488d0:  push   ebp
   0x080488d1:  push   edi
   0x080488d2:  push   esi
   0x080488d3:  push   ebx
   0x080488d4:  call   0x8048942
   0x080488d9:  add    ebx,0x171b
   0x080488df:  sub    esp,0x1c
   0x080488e2:  mov    ebp,DWORD PTR [esp+0x30]
   0x080488e6:  lea    edi,[ebx-0xf0]
   0x080488ec:  call   0x80483bc
   0x080488f1:  lea    eax,[ebx-0xf8]
   0x080488f7:  sub    edi,eax
   0x080488f9:  sar    edi,0x2
   0x080488fc:  test   edi,edi
End of assembler dump.
0x080488cc in ?? ()
```

`ret` instruction will take the address from top of the stack and pass there control. So let's see what is that address.

```
(gdb) x/xw $esp
0xffffdb4c:     0xf7e224b7
```

```
(gdb) stepi
Dump of assembler code from 0xf7e224b7 to 0xf7e224e9:
=> 0xf7e224b7 <__libc_start_main+135>:  add    esp,0x10
   0xf7e224ba <__libc_start_main+138>:  mov    eax,DWORD PTR [esp+0x8]
   0xf7e224be <__libc_start_main+142>:  mov    eax,DWORD PTR [eax-0xe8]
   0xf7e224c4 <__libc_start_main+148>:  mov    edx,DWORD PTR [eax+0x20c]
   0xf7e224ca <__libc_start_main+154>:  test   edx,edx
   0xf7e224cc <__libc_start_main+156>:  jne    0xf7e225d2 <__libc_start_main+418>
   0xf7e224d2 <__libc_start_main+162>:  test   ebx,ebx
   0xf7e224d4 <__libc_start_main+164>:  jne    0xf7e225ab <__libc_start_main+379>
   0xf7e224da <__libc_start_main+170>:  sub    esp,0xc
   0xf7e224dd <__libc_start_main+173>:  lea    eax,[esp+0x20]
   0xf7e224e1 <__libc_start_main+177>:  push   eax
   0xf7e224e2 <__libc_start_main+178>:  call   0xf7e35a10 <_setjmp>
   0xf7e224e7 <__libc_start_main+183>:  add    esp,0x10
End of assembler dump.
```

there we also find interesting call

```
(gdb) nexti
Dump of assembler code from 0xf7e22523 to 0xf7e22555:
=> 0xf7e22523 <__libc_start_main+243>:  call   DWORD PTR [esp+0x70]
   0xf7e22527 <__libc_start_main+247>:  add    esp,0x10
   0xf7e2252a <__libc_start_main+250>:  sub    esp,0xc
   0xf7e2252d <__libc_start_main+253>:  push   eax
   0xf7e2252e <__libc_start_main+254>:  call   0xf7e38730 <exit>
   0xf7e22533 <__libc_start_main+259>:  xor    ecx,ecx
   0xf7e22535 <__libc_start_main+261>:  jmp    0xf7e22462 <__libc_start_main+50>
   0xf7e2253a <__libc_start_main+266>:  mov    esi,DWORD PTR [esp+0x8]
   0xf7e2253e <__libc_start_main+270>:  mov    eax,DWORD PTR [esi+0x3848]
   0xf7e22544 <__libc_start_main+276>:  ror    eax,0x9
   0xf7e22547 <__libc_start_main+279>:  xor    eax,DWORD PTR gs:0x18
   0xf7e2254e <__libc_start_main+286>:  call   eax
   0xf7e22550 <__libc_start_main+288>:  mov    eax,DWORD PTR [esi+0x3840]
End of assembler dump.
```


```
(gdb) print/x $esp+0x70
$3 = 0xffffdbc0
```



