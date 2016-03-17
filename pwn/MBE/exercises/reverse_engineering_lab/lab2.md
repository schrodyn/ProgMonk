#lab2

As always we start with gdb

```
gdb -q ./lab2
```

And see all defined functions

```
(gdb) info functions 
All defined functions:

Non-debugging symbols:
0x080483c0  printf@plt
0x080483d0  signal@plt
0x080483e0  __stack_chk_fail@plt
0x080483f0  puts@plt
0x08048400  __gmon_start__@plt
0x08048410  __libc_start_main@plt
0x08048420  __isoc99_scanf@plt
```

Hmm.. It's interesting thing that we don't have defined `main` 
function. In such situation we will use old trick. Let's apply
`objdump` for finding the entry point of "main" (the address of 
first instruction of user code that will be executed).

```
objdump -d lab2 -M intel | less
```

there we find `__libc_start_main` routine.

```
Disassembly of section .text:

08048430 <.text>:
 8048430:       31 ed                   xor    ebp,ebp
 8048432:       5e                      pop    esi
 8048433:       89 e1                   mov    ecx,esp
 8048435:       83 e4 f0                and    esp,0xfffffff0
 8048438:       50                      push   eax
 8048439:       54                      push   esp
 804843a:       52                      push   edx
 804843b:       68 90 86 04 08          push   0x8048690
 8048440:       68 20 86 04 08          push   0x8048620
 8048445:       51                      push   ecx
 8048446:       56                      push   esi
 8048447:       68 e9 84 04 08          push   0x80484e9
 804844c:       e8 bf ff ff ff          call   8048410 <__libc_start_main@plt>
```

The first argument for `libc_start_main` is the **address** of code 
which will be executed after `libc` passes control to user code.

So let's find this address `0x80484e9` in object dump output

```
 80484e9:       55                      push   ebp
 80484ea:       89 e5                   mov    ebp,esp
 80484ec:       57                      push   edi
 80484ed:       53                      push   ebx
 80484ee:       83 e4 f0                and    esp,0xfffffff0
 80484f1:       83 ec 40                sub    esp,0x40
 80484f4:       65 a1 14 00 00 00       mov    eax,gs:0x14
 80484fa:       89 44 24 3c             mov    DWORD PTR [esp+0x3c],eax
 80484fe:       31 c0                   xor    eax,eax
 8048500:       50                      push   eax
 8048501:       31 c0                   xor    eax,eax
 8048503:       74 03                   je     8048508 <__isoc99_scanf@plt+0xe8>
 8048505:       83 c4 04                add    esp,0x4
 8048508:       58                      pop    eax
```

as we can see it's indeed nothing but something that called 
**function prologue**.

Ok, we now know the "main" address. Let's jump to gdb and debug it.

```
(gdb) set disassembly-flavor intel
```

setup breakpoint 

```
(gdb) b *0x80484e9
Breakpoint 1 at 0x80484e9
```

and run the program

```
(gdb) r
```

we stopped at our breakpoint. Let's try `disassemble` what we have

```
(gdb) disassemble 
No function contains program counter for selected frame.
```

Oops! Yes, surely gdb can't find any function routine we are in,
so we must define explicitely the addresses for disassembly

```
(gdb) disassemble $eip,+250
Dump of assembler code from 0x80484e9 to 0x80485e3:
=> 0x080484e9:  push   ebp
   0x080484ea:  mov    ebp,esp
   0x080484ec:  push   edi
   0x080484ed:  push   ebx
   0x080484ee:  and    esp,0xfffffff0
   0x080484f1:  sub    esp,0x40
   0x080484f4:  mov    eax,gs:0x14
   0x080484fa:  mov    DWORD PTR [esp+0x3c],eax
   0x080484fe:  xor    eax,eax
   0x08048500:  push   eax
   0x08048501:  xor    eax,eax
   0x08048503:  je     0x8048508
   0x08048505:  add    esp,0x4
   0x08048508:  pop    eax
   0x08048509:  mov    DWORD PTR [esp+0x4],0x80484e4
   0x08048511:  mov    DWORD PTR [esp],0x5
   0x08048518:  call   0x80483d0 <signal@plt>
   0x0804851d:  mov    DWORD PTR [esp+0x20],0x0
   0x08048525:  mov    eax,0x80486f0
   0x0804852a:  mov    DWORD PTR [esp],eax
   0x0804852d:  call   0x80483c0 <printf@plt>
   0x08048532:  int3   
   0x08048533:  mov    eax,0x8048701
   0x08048538:  lea    edx,[esp+0x28]
   0x0804853c:  mov    DWORD PTR [esp+0x4],edx
   0x08048540:  mov    DWORD PTR [esp],eax
   0x08048543:  call   0x8048420 <__isoc99_scanf@plt>
   0x08048548:  mov    DWORD PTR [esp+0x20],0x0
   0x08048550:  jmp    0x80485b8
   0x08048552:  int3   
   0x08048553:  mov    eax,DWORD PTR [esp+0x20]
   0x08048557:  add    eax,0x804a024
   0x0804855c:  movzx  ebx,BYTE PTR [eax]
   0x0804855f:  mov    eax,DWORD PTR [esp+0x20]
   0x08048563:  lea    ecx,[eax+0x1]
   0x08048566:  mov    edx,0x66666667
...
```

Ok, here we have the actual code. Let's understand what's going on.
In this part we have command prompt to enter password

```
   0x08048525:  mov    eax,0x80486f0
   0x0804852a:  mov    DWORD PTR [esp],eax
   0x0804852d:  call   0x80483c0 <printf@plt>
```

```
(gdb) x/s 0x80486f0
0x80486f0:      "Enter password: "
```

Everything seems similar but what is that?

```
0x08048532:  int3
```

Ok, it's interrupt 3 which also known as TRAP interrupt. Almost all
debuggers uses this interrupt to setup breakpoints. And what will be
after executing this instruction? gdb simply stops here, it
will think that it is breakpoint that you setuped at this location.

In this part

```
   0x08048533:  mov    eax,0x8048701
   0x08048538:  lea    edx,[esp+0x28]
   0x0804853c:  mov    DWORD PTR [esp+0x4],edx
   0x08048540:  mov    DWORD PTR [esp],eax
   0x08048543:  call   0x8048420 <__isoc99_scanf@plt>
```

we `scanf` our password with 

```
(gdb) x/s 0x8048701
0x8048701:      "%s"
```

as format string and store the scanned string at `[esp+0x28]` address.
After that we setup counter for loop (as we'll see later) to 0 and
jump to `0x80485b8` address. There we have

```
   0x080485b8:  mov    ebx,DWORD PTR [esp+0x20]
   0x080485bc:  mov    eax,0x804a024
   0x080485c1:  mov    DWORD PTR [esp+0x1c],0xffffffff
   0x080485c9:  mov    edx,eax
   0x080485cb:  mov    eax,0x0
   0x080485d0:  mov    ecx,DWORD PTR [esp+0x1c]
   0x080485d4:  mov    edi,edx
   0x080485d6:  repnz scas al,BYTE PTR es:[edi]
   0x080485d8:  mov    eax,ecx
   0x080485da:  not    eax
   0x080485dc:  sub    eax,0x1
   0x080485df:  cmp    ebx,eax
   0x080485e1:  jb     0x8048552
```

This part simply checks if counter stored at `[esp+0x20]` address
is less than length of the string stored at `0x804a024` address.

```
(gdb) x/s 0x804a024
0x804a024:      "kw6PZq3Zd;ekR[_1"
```

After that if counter is less than 16, then we jump to `0x8048552`
address.

```
   0x08048552:  int3   
   0x08048553:  mov    eax,DWORD PTR [esp+0x20]
   0x08048557:  add    eax,0x804a024
   0x0804855c:  movzx  ebx,BYTE PTR [eax]
   0x0804855f:  mov    eax,DWORD PTR [esp+0x20]
   0x08048563:  lea    ecx,[eax+0x1]
   0x08048566:  mov    edx,0x66666667
   0x0804856b:  mov    eax,ecx
   0x0804856d:  imul   edx
   0x0804856f:  sar    edx,0x3
   0x08048572:  mov    eax,ecx
   0x08048574:  sar    eax,0x1f
   0x08048577:  sub    edx,eax
   0x08048579:  mov    eax,edx
   0x0804857b:  shl    eax,0x2
   0x0804857e:  add    eax,edx
   0x08048580:  shl    eax,0x2
   0x08048583:  mov    edx,ecx
   0x08048585:  sub    edx,eax
   0x08048587:  mov    eax,edx
   0x08048589:  xor    eax,ebx
   0x0804858b:  mov    BYTE PTR [esp+0x27],al
   0x0804858f:  lea    eax,[esp+0x28]
   0x08048593:  add    eax,DWORD PTR [esp+0x20]
   0x08048597:  movzx  eax,BYTE PTR [eax]
   0x0804859a:  cmp    al,BYTE PTR [esp+0x27]
   0x0804859e:  je     0x80485b3
   0x080485a0:  mov    DWORD PTR [esp],0x8048704
   0x080485a7:  call   0x80483f0 <puts@plt>
```

There we load string at `0x804a024` address and make some shitty
arithmetic and after that's done check if our password's string
byte at the same location is the same as resulted byte after
computations. If the input byte is not correct then this string is
displayed

```
(gdb) x/s 0x8048704
0x8048704:      "Wrong!"
```

Ok, at this point you have two ways: the first is to understand 
what's going on there and write script (for example, on Python) that 
simulates this  behaviour and accepts encoded string as input. 
Another way is to debug, fetch 1 byte of correct password and then
re-run the program :)

The last method is less painful. So we go ahead and use it. Let's
setup breakpoint at this line

```
0x0804859a:  cmp    al,BYTE PTR [esp+0x27]
```

```
(gdb) b *0x0804859a
Breakpoint 3 at 0x804859a
```

and look at this `[esp+0x27]` location at every step. 

```
(gdb) display/xb ($esp+0x27)
```

For convinience let's setup `hook-stop`

```
(gdb) define hook-stop
>disassemble $eip,+50
>end
```


At this point the life is more easy and happy :) We just use 
`continue` or `c` and look at the byte at `$esp+0x27` location
every time we stop there. After we know correct byte, we save it in
our mind (or vim, or python) and re-run the program in gdb, enter
correct byte, step through the program and fetch the next correct
byte. 

```
(gdb) c
Continuing.
Dump of assembler code from 0x804859a to 0x80485cc:
=> 0x0804859a:  cmp    al,BYTE PTR [esp+0x27]
   0x0804859e:  je     0x80485b3
   0x080485a0:  mov    DWORD PTR [esp],0x8048704
   0x080485a7:  call   0x80483f0 <puts@plt>
   0x080485ac:  mov    eax,0x1
   0x080485b1:  jmp    0x80485f9
   0x080485b3:  add    DWORD PTR [esp+0x20],0x1
   0x080485b8:  mov    ebx,DWORD PTR [esp+0x20]
   0x080485bc:  mov    eax,0x804a024
   0x080485c1:  mov    DWORD PTR [esp+0x1c],0xffffffff
   0x080485c9:  mov    edx,eax
   0x080485cb:  mov    eax,0x0
End of assembler dump.

Breakpoint 3, 0x0804859a in ?? ()
1: /x ($esp+0x27) = 0xffffdb27
16: x/xb ($esp+0x27)  0xffffdb27:       0x6a
```

So the first correct byte is 0x6a. Now we re-run our program under
gdb and insert this 0x6a byte as first character and fetch the second
correct character and so on.

After that we go ahead and achieve our full password

```py
>>> chr(0x6a)
'j'
>>> chr(0x75)
'u'
>>> chr(0x35)
'5'
>>> chr(0x54)
'T'
>>> chr(0x5f)
'_'
>>> chr(0x77)
'w'
>>> chr(0x34)
'4'
>>> chr(0x52)
'R'
>>> chr(0x6d)
'm'
>>> chr(0x31)
'1'
>>> chr(0x6e)
'n'
>>> chr(0x67)
'g'
>>> chr(0x5f)
'_'
>>> chr(0x55)
'U'
>>> chr(0x50)
'P'
>>> chr(0x21)
'!'
```

There we go.
