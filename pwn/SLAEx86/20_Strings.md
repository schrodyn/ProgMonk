#Strings

Hello, guys! In this video we will look at working with strings in
assembly language. Now, by this time I would start assuming that you
can look up the Intel Manual and figure out what each of this 
instructions does on your own. If you haven't good time to start
this research simply because as I mentioned there are literaly 
hundreds of instructions and It would be impossible to cover each and
every one of them.

  * `MOVS` (`MOVS`/`MOVSW`/`MOVSD`)
  * `CMPS` -- Compares
  * `SCAS` -- Subtracts
  * `LODS` -- Loads

`ESI` and `EDI` registers are typically used with `DF`.

But sometimes when you are writting shellcode or writting exploits
you may come across new instructions which you haven't seen before.
And the only way to demistify all of that is to consult Intel Manual
and look what they do.

So string instructions well the most common ones are `MOVS`,`CMPS`,
`SCAS` and `LODS`. Now, `MOVS` basically alows us copy one string to 
another, `CMPS` basically compares strings, and the remaining two are
for subtracting and loading. Now I leave remaining two to you as 
exercise.

As far as `MOVS` and `CMPS` are concerned the assumption is that the
**source** string is refferenced by `ESI` while the **destination**
string is refferenced by `EDI`. 

Now, let me actually go to source code to illustrate how it works. Open
`Strings.nasm`. All we are doing is simple copy a string from source to
destination. So here is a part of my program.

```
global _start

section .text
_start:

        ; copy a strign from source to destination

        mov ecx, sourceLen
        lea esi, [source]
        lea edi, [destination]


        cld  ; clear direction flag
        rep movsb
```

in my data segment I have defined a bunch of variables

```
section .data

        source: db "Hello World!", 0xA
        sourceLen equ $-source

        comparison: db "Hello"

        result1: db "Strings are Equal", 0xA
        result1Len equ $-result1

        result2: db "Strings are Unequal", 0xA
        result2Len equ $-result2

section .bss

        destination: resb 100
```

First is `source` which is "Hello World!" followed by newline (0xA),
then I have `comparison` that is just "Hello", then I have `result1`
which is "Strings are Equal" and `result2` which is "Strings are 
Unequal" and finally in **bss** segment I have initialized 100 bytes.

Now, `movs` itself has three variations one is `movsb`, the other is
`movsw` and the third is `movsd`. The `b` one is to move byte by byte.
And the `w` one is to move word by word. And the `d` one is to go ahead
and move 4 bytes every time. We will look at `movsb` in this example
you can try out others on your own. 

So the first thing if you remember would be to go ahead and figure out
the length of the string as already defined down below

```
_start:

        ; copy a strign from source to destination

        mov ecx, sourceLen
        lea esi, [source]
        lea edi, [destination]


        cld  ; clear direction flag
        rep movsb
```

So what I am going to do is have `esi` to point to the **source** 
string which basically is "hello world" string. And I am going to have
`edi` to point to the **destination** string which right now is 
nothing but 100 bytes just being reserved all zero values. And then
I really want to copy the string pointed by `esi` which is source 
into `edi` which is destination. Now, the way to do this is we are
going to move byte by byte and there has to be a way to run this
in some sort of a loop. And this is where `rep` instruction comes in.
This is similar to `loop` instruction and what `rep` does it is going
to repeat the instruction till the time `ecx` register does not become
zero. There are variations of `rep` which are also available which I
request you to look at. 

So the other thing apart from this is the **direction** in which the
copying is happening. Now, as you can imagine that source and 
destination are nothing but two memory addresses. So the question 
arises after each byte copy should we adavance the memory address or
should we go ahead and go back to examine memory which is subtract 
from existing address. This is dictated by the **direction flag**
in `eflags` register. Now, if you go ahead and set the direction flag
then the copying would basically happen in the higher memory to lesser
memory direction, while if you set it to zero, the copying would 
happen from low memory to high memory direction.

So what I am going to do is to go ahead and first figure out the 
number of iterations we need which is nothing but the length of the
string.

```
        mov ecx, sourceLen
```

Then I am going to load the address of the source string into `esi`,
load the address of the destination string into `edi`

```
        lea esi, [source]
        lea edi, [destination]
```

then I am going ahead and clear the direction flag which I want to
copy from low memory to high memory.

```
        cld  ; clear direction flag
```

And after that I am just going to repeat `movsb` which is moving one
byte from source to destination and this repeatation will happen
`sourceLen` number of times. Now, once this instruction is completed
if basically the copy happens succesfully if now we just print the
destination which should basically print exactly the same string as
`source` should be "hello world" followed by a newline.

So have a look at this.

```
gdb -q ./Strings
```

```
(gdb) set disassembly-flavor intel
(gdb) break _start
(gdb) run
```

```
(gdb) disassemble 
Dump of assembler code for function _start:
=> 0x08048080 <+0>:	mov    ecx,0xd
   0x08048085 <+5>:	lea    esi,ds:0x80490f0
   0x0804808b <+11>:	lea    edi,ds:0x8049128
   0x08048091 <+17>:	cld    
   0x08048092 <+18>:	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
   0x08048094 <+20>:	mov    eax,0x4
   0x08048099 <+25>:	mov    ebx,0x1
   0x0804809e <+30>:	mov    ecx,0x8049128
   0x080480a3 <+35>:	mov    edx,0xd
   0x080480a8 <+40>:	int    0x80
   0x080480aa <+42>:	mov    ecx,0xd
   0x080480af <+47>:	lea    esi,ds:0x80490f0
   0x080480b5 <+53>:	lea    edi,ds:0x8049128
   0x080480bb <+59>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080480bd <+61>:	je     0x80480cb <SetEqual>
   0x080480bf <+63>:	mov    ecx,0x8049114
   0x080480c4 <+68>:	mov    edx,0x14
   0x080480c9 <+73>:	jmp    0x80480d5 <Print>
End of assembler dump.
```

Let's also do something by which we can monitor copying that is 
happening.

```
(gdb) define hook-stop
>disassemble 
>x/15cb &destination
>end
```

`cb` display as characters. Now let me do next instruction

```
(gdb) nexti
Dump of assembler code for function _start:
   0x08048080 <+0>:	mov    ecx,0xd
=> 0x08048085 <+5>:	lea    esi,ds:0x80490f0
   0x0804808b <+11>:	lea    edi,ds:0x8049128
   0x08048091 <+17>:	cld    
   0x08048092 <+18>:	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
   0x08048094 <+20>:	mov    eax,0x4
   0x08048099 <+25>:	mov    ebx,0x1
   0x0804809e <+30>:	mov    ecx,0x8049128
   0x080480a3 <+35>:	mov    edx,0xd
   0x080480a8 <+40>:	int    0x80
   0x080480aa <+42>:	mov    ecx,0xd
   0x080480af <+47>:	lea    esi,ds:0x80490f0
   0x080480b5 <+53>:	lea    edi,ds:0x8049128
   0x080480bb <+59>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080480bd <+61>:	je     0x80480cb <SetEqual>
   0x080480bf <+63>:	mov    ecx,0x8049114
   0x080480c4 <+68>:	mov    edx,0x14
   0x080480c9 <+73>:	jmp    0x80480d5 <Print>
End of assembler dump.
0x8049128 <destination>:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
0x8049130 <destination+8>:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
0x08048085 in _start ()
```

Now if you notice `destination` at this pointis nothing but all zeroes
as expected because it is in **bss** segment. Now we are going to be
loading the source string address into `esi`, and load the destination
string address into `edi` and you notice this is nothing but the 
address space which we are watching right now.

```
0x8049128 <destination>:    0 '\000'    0 '\000'    0 '\000'
```

Make `nexti`. And now the copying is about to start and what you would
notice is basically first you would have "H" copied, "e" getting 
copied and all of that.

```
(gdb) nexti
Dump of assembler code for function _start:
   0x08048080 <+0>:	mov    ecx,0xd
   0x08048085 <+5>:	lea    esi,ds:0x80490f0
   0x0804808b <+11>:	lea    edi,ds:0x8049128
   0x08048091 <+17>:	cld    
=> 0x08048092 <+18>:	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
   0x08048094 <+20>:	mov    eax,0x4
   0x08048099 <+25>:	mov    ebx,0x1
   0x0804809e <+30>:	mov    ecx,0x8049128
   0x080480a3 <+35>:	mov    edx,0xd
   0x080480a8 <+40>:	int    0x80
   0x080480aa <+42>:	mov    ecx,0xd
   0x080480af <+47>:	lea    esi,ds:0x80490f0
   0x080480b5 <+53>:	lea    edi,ds:0x8049128
   0x080480bb <+59>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080480bd <+61>:	je     0x80480cb <SetEqual>
   0x080480bf <+63>:	mov    ecx,0x8049114
   0x080480c4 <+68>:	mov    edx,0x14
   0x080480c9 <+73>:	jmp    0x80480d5 <Print>
End of assembler dump.
0x8049128 <destination>:	72 'H'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
0x8049130 <destination+8>:	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'	0 '\000'
0x08048092 in _start ()
```

And similarly whole string would be copied.

```
(gdb) 
Dump of assembler code for function _start:
   0x08048080 <+0>:	mov    ecx,0xd
   0x08048085 <+5>:	lea    esi,ds:0x80490f0
   0x0804808b <+11>:	lea    edi,ds:0x8049128
   0x08048091 <+17>:	cld    
   0x08048092 <+18>:	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
=> 0x08048094 <+20>:	mov    eax,0x4
   0x08048099 <+25>:	mov    ebx,0x1
   0x0804809e <+30>:	mov    ecx,0x8049128
   0x080480a3 <+35>:	mov    edx,0xd
   0x080480a8 <+40>:	int    0x80
   0x080480aa <+42>:	mov    ecx,0xd
   0x080480af <+47>:	lea    esi,ds:0x80490f0
   0x080480b5 <+53>:	lea    edi,ds:0x8049128
   0x080480bb <+59>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080480bd <+61>:	je     0x80480cb <SetEqual>
   0x080480bf <+63>:	mov    ecx,0x8049114
   0x080480c4 <+68>:	mov    edx,0x14
   0x080480c9 <+73>:	jmp    0x80480d5 <Print>
End of assembler dump.
0x8049128 <destination>:	72 'H'	101 'e'	108 'l'	108 'l'	111 'o'	32 ' '	87 'W'	111 'o'
0x8049130 <destination+8>:	114 'r'	108 'l'	100 'd'	33 '!'	10 '\n'	0 '\000'	0 '\000'
0x08048094 in _start ()
```

And we have all "hello world" available here. So this is how you can
use `movsb` along with `rep` to copy a string from source memory 
address to a destination memory address.

Now, the second thing we would like to look at is comparing two 
different strings to check if two string are equal or not. So this
can be done using `cmpsb` which also has the variations `cmpsw` and
`cmpsd` depending on using what chunks of memory you want to do
comparison. 

```
        ; string comparison with cmpsb

        ; compare source and destination
        mov ecx, sourceLen
        lea esi, [source]
        lea edi, [comparison]
        repe cmpsb

        jz SetEqual
        mov ecx, result2
        mov edx, result2Len
        jmp Print

SetEqual:
        mov ecx, result1
        mov edx, result1Len

Print:
        mov eax, 0x4
        mov ebx, 0x1
        int 0x80


        mov eax, 1
        mov ebx, 10             ; sys_exit syscall
        int 0x80
```

So, this works exactly how the previous ones works. We load up the
length of the string of source. We load up source, load up the 
comparison string which is just "Hello" which actually means this
comparison will fail. And then we use `repe`. Now, this would also go
ahead and check whether each of them in comparison are equal or not soread the documentation. Basically if both the strings are equal than
after `repe` is finished the Zero Flag would be set. If at any point
they are unequal than basically the Zero Flag would not be set. So
what we do is after `repe cmpsb` has been run we actually use
`jz SetEqual` which is if both of them are equal go to `SetEqual` and
in `Set Equal` all I do is I load up `result1` and its length so that
I can eventually print it on the screen. So as you notice `result1` is
the string "Strings are Equal". Now, in case that Zero Flag is not set
than this 

```
mov ecx, result2
mov edx, result2Len
```

is getting executed and after that we do unconditional jump to `Print`
which just goes to print it. And `result2` is specifically set to
"Strings are Unequal". So now if I go ahead and run this as you would
imagine we actually get `Strings are Unequal`.

```
$ ./Strings 
Hello World!
Strings are Unequal
```

Now use gdb

```
gdb -q ./Strings
(gdb) set disassembly-flavor intel
(gdb) break _start
(gdb) run
```

```
(gdb) disassemble 
Dump of assembler code for function _start:
=> 0x08048080 <+0>:	mov    ecx,0xd
   0x08048085 <+5>:	lea    esi,ds:0x80490f0
   0x0804808b <+11>:	lea    edi,ds:0x8049128
   0x08048091 <+17>:	cld    
   0x08048092 <+18>:	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
   0x08048094 <+20>:	mov    eax,0x4
   0x08048099 <+25>:	mov    ebx,0x1
   0x0804809e <+30>:	mov    ecx,0x8049128
   0x080480a3 <+35>:	mov    edx,0xd
   0x080480a8 <+40>:	int    0x80
   0x080480aa <+42>:	mov    ecx,0xd
   0x080480af <+47>:	lea    esi,ds:0x80490f0
   0x080480b5 <+53>:	lea    edi,ds:0x80490fd
   0x080480bb <+59>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080480bd <+61>:	je     0x80480cb <SetEqual>
   0x080480bf <+63>:	mov    ecx,0x8049114
   0x080480c4 <+68>:	mov    edx,0x14
   0x080480c9 <+73>:	jmp    0x80480d5 <Print>
End of assembler dump.
```

And set up breakpoint at `0x080480bd`

```
(gdb) break *0x080480bd
```

Let's continue

```
(gdb) c
Continuing.
Hello World!

Breakpoint 2, 0x080480bd in _start ()
```

So breakpoint to this hit just after the comparison is happened. And
at this point if we print `eflags`

```
(gdb) p $eflags
$1 = [ CF AF SF IF ]
```

we would actually see that Zero Flag has not been set.

Now instead of using the `comparison` string if I go ahead and use the
`destination` string because `destination` string has been overwritten
with the `source` string so both of them are equal.

Now if I run this first I'll get

```
$ ./Strings 
Hello World!
Strings are Equal
```

And you would see "Strings are Equal". And you could use gdb to load
up and see at flags after camparison was happened

```
(gdb) p $eflags
$1 = [ PF ZF IF ]
```

you would actually see that Zero Flag is set. Now you are noticing 
that we have `repz` here while we actually enter `repe` here. So
why this? This is similar to question I asked you previously. Look
at the opcode of both of this, right see if opcodes are same or not.
That will give you the answer.

```
(gdb) disassemble 
Dump of assembler code for function _start:
   0x08048080 <+0>:	mov    ecx,0xd
   0x08048085 <+5>:	lea    esi,ds:0x80490f0
   0x0804808b <+11>:	lea    edi,ds:0x8049128
   0x08048091 <+17>:	cld    
   0x08048092 <+18>:	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
   0x08048094 <+20>:	mov    eax,0x4
   0x08048099 <+25>:	mov    ebx,0x1
   0x0804809e <+30>:	mov    ecx,0x8049128
   0x080480a3 <+35>:	mov    edx,0xd
   0x080480a8 <+40>:	int    0x80
   0x080480aa <+42>:	mov    ecx,0xd
   0x080480af <+47>:	lea    esi,ds:0x80490f0
   0x080480b5 <+53>:	lea    edi,ds:0x8049128
   0x080480bb <+59>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
=> 0x080480bd <+61>:	je     0x80480cb <SetEqual>
   0x080480bf <+63>:	mov    ecx,0x8049114
   0x080480c4 <+68>:	mov    edx,0x14
   0x080480c9 <+73>:	jmp    0x80480d5 <Print>
End of assembler dump.
```

So basically this is how you can go ahead and do string operations 
in assembly. You can play around with many of these operations on your
own little programs. 
