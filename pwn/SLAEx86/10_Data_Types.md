#Data Types

Hello! Welcome to this video on Data Types. So as far as the IA-32
architecture goes the fundamental data types include **Byte** which
is 8 bits, **Word** which is 16 bits, **Double Word** which is 32
bits, **Quad Word** which is 64 bits, and as you probably already
guessed **Double Quad Word** which is 128.

Now, in each of this cases you have the notion of **Signed** and
**Unsigned**. Now, In the case of **Unsigned** the whole 32 bits
as in example of **Unsigned Double Word** are dedicated to storing
the value. In the case of **Signed Double Word** 31 bits are dedicated
to storing the value while the last bit, the most significant one, 
is used to store the sign which can be positive or negative.

Now, before we are moving in more assembly language there are a couple
of things you need to know about NASM which is assembler we will use
for this any entire series. NASM is Case Sensitive and what is really 
means you need to be very very observing about naming your labels.
The second thing is that accessing memory has a very very simple
way to be done in nasm in comparing to many other assemblers.
So let me give you an example and this is something you could
correlate with the previous Hello World program we just wrote.

So let say we defined `message` with `db` which is `define byte` as 
a sequence of 4 bytes 0xAA, 0xBB, 0xCC, 0xDD.

```
message db 0xAA, 0xBB, 0xCC, 0xDD
```

when we move in `eax` `message` (`mov eax, message`) this actually 
moves **the address** of where this 4 bytes are located into the `eax`
register. Now, if you actually want to go ahead and move this 4 values
then you need to refference using this `[]` square brackets

```
mov eax, [message]
```

So this instruction with the square brackets actually moves 
**the value** refferenced by `message` into `eax` register.
So this is an important concept.

Now, we've talked about data and you have initialized data and
you have an unitialized data. So in NASM you can define initialized
data using a bunch of instructions such as `db`, `dw`, `dd`, `dq`, `dt`
etc. depending on your data type you are interested in. Now, `db` is
"define byte" where we can define byte or a sequence of bytes

```asm
db	0x55
db	0x55,0x56,0x57
```

you can even have character constants in them

```asm
db 'a',0x55
```

or even strings for this matter as you could see in this Hello World
program

```asm
db	'hello',13,10,'$'
```

Now the kitting(?) you need to remember is byte is basically for
one single byte, where word is for two. So as for example you
define word and just use 'a' as a character

```
dw	'a'
```

then in reality this actually translates to 0x61 0x00, 0x61 stands for
ascii value of 'a'. This ensures that all word is actually occupied and
this applies to any other data type.

Now, in order to declare **Uninitialized Data** the two most popular 
instructions are `resb` and `resw`. `res` stands for "reserve", `b` is
for "byte", and `w` is for "word". The first instruction

```
buffer:		resb	64
```

reserves 64 bytes and

```
wordvar:	resw	1
```

reserves a single word. Now, as you already encounter in the previous
video uninitialized data is placed in the **bss** section. Now, all
of this is actually presented in the NASM manual and so I recomend to
going through the Manual where specific data types are covered
in page 30.

Apart from this there are two special tokens which are used alot when
dealing with NASM. The first is the dollar sign `$` which evaluates
to the **current line** and `$$` which evaluates to the beginning of
the **current section**. Now, this are used in times when we need to
figure out the offset of the current instruction from something else
most all the time.

So let me give you an example of how we use `$`. Now, you may remember
in the Hello World program we defined `message`

```asm
message		db		'hello, world'
```

and in the very next line we set `msglen` equals `$-message`

```asm
msglen		equ		$-message
```

Now, let's try interpret what this means. I said that `$` evaluates
to the current line so it is actually refference to the beginning of
this line starting at `msglen` which is basically the end of the
previous line. While `message` refferences the beginning of the
'hello, world' string. So `$-message` will definetely be the
string length from "h" to "d" and this is how we enable to compute
the string length using `$-message`.

Now, `$` sometimes as you also read in the manual is used to create
infinite loops like `jmp $` which basically would mean keep coming
back with the same location.

Now, apart from the `equ`, `times` is also another interesting 
instruction which we will use. `times` is nothing but replication
of one instruction multiple times and it can apply to **both** data as
well as instruction. What do I mean by that? Well. let's look at this
definition

```asm
zerobuf:	times 64 db 0
```

what it means is repeat `db 0` 64 times. In a similar way, when we 
look at instruction as

```asm
times 100 movsb
```

this just means `movsb` needs to be repeated 100 times.

Ok, now we've looked some data types and how to use them in NASM. Now,
let's look at how data actually get stored in memory and this is where
IA-32 uses what is called **little-endian** format. Now, if you've 
never heard endianness and all of that stuff go ahead and have a look
at the Endianness page on Wikipedia. What it's really mean for us
all little-endianness means is the **lower** byte would get stored in 
**lower** memory while **higher** byte would get stored in **higher**
memory. So as an example let say that register which is 32 bits wide
has the value `0x0A0B0C0D` in it and you are now using the instruction
to store this value into memory. On a little-endian architecture 
what will happen is `0D` would get stored in lower memory, `0C` in
the next byte which is `a+1` if `0D` was in `a`, `0B` would be in 
`a+2`, while `0A` would be in `a+3`. This is very very important
because when you will read memory you need to ensure that you respect
little-endianness while going ahead and figuring things out about 
the data we got stored.

Now, we are going to be using gdb quite alot in this entire series.
And my recommendation would be to master gdb at some level. Of course,
like all SecurityTube courses we don't like to leave you to figure out
this by yourself and this is where I already created the 
**SecurityTube GNU Debugger Expert** course and certification and
all video for this course are included with SLAE absolutelly free of
charge. So inside your course folder there would actually be another
folder SGDE click on that, view all of that if you've never used
gdb, it starts from the very basics so that you can understand 
how to use this fantastic tool to get an advantage.

Ok, so having that now let's go to the exercise of this specific
video. You should open `DataTypes.nasm`. So `DataTypes.nasm` typically
looks exactly what you've seen in the previous video for Hello World.
The only thing I have done is I have modified the **data** section,
and I have added a new **bss** section. So lets me explain.

Now, in the **data** section I've defined 6 variables and marked
them with labels from `var1` up to `var6`. Now `var1` is nothing
but a byte.

```
var1:	db 0xAA
```

where `var2` is a sequence of the bytes

```
var2:	db 0xBB, 0xCC, 0xDD
```

`var3` is a word 

```
var3:	dw 0xEE
```

and if you recall it actually be 0xEE 0x00

and after that you actually have `dd` which is 4 bytes and then a 
couple of other variations

```
        var1:   db 0xAA
        var2:   db 0xBB, 0xCC, 0xDD
        var3:   dw 0xEE
        var4:   dd 0xAABBCCDD
        var5:   dd 0x112233
        var6:   TIMES 6 db 0xFF
```

`var6` is going to be using `times` to its advantage and then
we are going to new section which I defined for uninitialized data
which is **bss**. There we defined `var7` as basically reserves
100 bytes and `var8` reserves 20 words.

```
section .bss

        var7:   resb 100
        var8:   resw 20
```

One of other things I have done is a simple shell script which will
go ahead and assemble with `nasm` and link with `ld` and give us the
output. So to do this

```
#!/bin/bash

echo '[+] Assembling with Nasm ...'
nasm -f elf32 -o $1.o $1.nasm

echo '[+] Linking ...'
ld -o $1 $1.o

echo '[+] Done!'
```

All we now need to do is hit `compile.sh` and then basically give
the name of the program.

```
$ ./compile.sh DataTypes
```

Now what I am going to do is run `DataTypes` and as expected it just
prints "hello world" and exits. All other variables we defined are
pretty much unused in this point. Now let's go ahead and load this up
in gdb and figure out what is happening.

```
gdb ./DataTypes
```

We can use `-q` option to load gdb silently. Now, there are a couple
of things we need to do, we first need to setup breakpoint so that
we can look at all of this data. Now, to setup breakpoint let's say if
you don't have any info about the binary one of the way to look at
the different functions which are available

```
$ gdb -q ./DataTypes 
Reading symbols from /home/osboxes/SLAE-Code/DataTypes/DataTypes...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048080  _start
``` 

In this case we can clearly see `_start` here. Now, in case you don't
have this and you want to know the **entry point** of the given 
executable then one of the things you can do is use different programs
available on the Linux system to figure out the entry point. So, as
an example, I could actually invoke a `shell` command from gdb and
than the command be `readelf`, `-h` to print all headers and than 
give it the binary

```
(gdb) shell readelf -h DataTypes
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x8048080
  Start of program headers:          52 (bytes into file)
  Start of section headers:          240 (bytes into file)
  Flags:                             0x0
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         2
  Size of section headers:           40 (bytes)
  Number of section headers:         7
  Section header string table index: 4
```

And if you notice this actually gives me the ELF header. The most 
important field here is the Entry point address which is `0x8048080`.
Which if you look in `_start` label position exactly the same.

Now, what we are going to do is setup breakpoint here on `_start` and
if you were following previous videos we need to 
`set disassembly-flavor intel`. You can also to figure out how to 
write gdb initialization script so that all of this stuff feded to 
the gdb automatically when it starts. So you don't need to retype 
this every single time and I leave this exercise to you.

Now, let's run this program.

```
(gdb) run
Starting program: /home/osboxes/SLAE-Code/DataTypes/DataTypes 

Breakpoint 1, 0x08048080 in _start ()
(gdb) disassemble
Dump of assembler code for function _start:
=> 0x08048080 <+0>:	mov    eax,0x4
   0x08048085 <+5>:	mov    ebx,0x1
   0x0804808a <+10>:	mov    ecx,0x80490b8
   0x0804808f <+15>:	mov    edx,0xc
   0x08048094 <+20>:	int    0x80
   0x08048096 <+22>:	mov    eax,0x1
   0x0804809b <+27>:	mov    ebx,0x5
   0x080480a0 <+32>:	int    0x80
End of assembler dump.
```

`disassemble` command show you actually instruction commands and we
have nothing to do with data. So lets actually use `info variables`
to look at all data available for us

```
(gdb) info variables
All defined variables:

Non-debugging symbols:
0x080490a4  var1
0x080490a5  var2
0x080490a8  var3
0x080490aa  var4
0x080490ae  var5
0x080490b2  var6
0x080490b8  message
0x080490c4  var7
0x08049128  var8
```

And as expected it prints `var1` to `var8` and `message` along with
its specific locations. So what we are going to do is open up
another terminal where we can basically monitor out programs. Now open
`DataTypes.nasm`. We declared here variables and now we want to use
gdb to inspect them while the program is loaded and running. So the
first thing we want to inspect is `var1` which is at `0x080490a4` this
address. So there are multiple ways to do that. You will use the
"examine" command `x/` and one of the ways is to use this address
directly

```
(gdb) x/xb 0x080490a4
0x80490a4 <var1>:	0xaa
```

which gives us the value 0xaa. Another alternate way would be to
fead the address of `var1`

```
(gdb) x/xb &var1
0x80490a4 <var1>:	0xaa
```

Both ways works. So we figured out `var1`. Now `var2` basically 
is the sequence of bytes so what we want is the sequence of 3 bytes

```
(gdb) x/3xb &var2
0x80490a5 <var2>:	0xbb	0xcc	0xdd
```

And as we clearly see it is 0xbb, 0xcc and 0xdd. Similarly, now if we
want to look at word at `var3`

```
(gdb) x/2xb &var3
0x80490a8 <var3>:	0xee	0x00
```

And if you notice it is 0xee, 0x00. Just is what we would expect. And
the next one is basically 4 bytes defined together `var4`. So let's
figure out the address of `var4`. So one of the way to do that is

```
(gdb) print &var4
$1 = (<data variable, no debug info> *) 0x80490aa
```

Now let's print each of this values as hex starting from this address

```
(gdb) x/xb 0x80490aa
0x80490aa <var4>:	0xdd
```

So you see 0xdd and basically 

```
(gdb) x/xb 0x80490ab
0x80490ab <var4+1>:	0xcc
```

this is the next address.

```
(gdb) x/xb 0x80490ac
0x80490ac <var4+2>:	0xbb
(gdb) x/xb 0x80490ad
0x80490ad <var4+3>:	0xaa
```

What we said was DD would actually be in the lowest address value
which is exactly what we see here. After that CC would be in the
next byte which is in higher memory and this is exactly what we see.
And then BB would be in the next byte. And AA would be in the next
higher byte. we see here `<var4>`, `<var4+1>`, `<var4+2>` and 
`<var4+3>` which shows that we are progressively going in higher
memory. This exactly what a little-endianness actually means.

Now, similarly you can explore other variables 

```
(gdb) x/xb &var6
0x80490b2 <var6>:	0xff
```

and I leave others as an exercise to you but please do this exercise
you are completely aware how data is getting stored in memory, how
it is defined and how you can figure out data in various segments
and look how they look like in program memory.


