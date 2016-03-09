#Exit Shellcode

Welcome back! In this video we will look at how to write shellcode
for the `exit` syscall. Now, let's jump directly into the code and
let me open `exit.nasm`.

```asm
global _start

section .text
_start:

        mov eax, 1
        mov ebx, 10
        int 0x80
```

And this should be similar to what you see in "hello world" program.
The `exit` syscall number is 1 we move in `eax` and if you want a 
non-zero return value than you can put any value apart from this so
I've put a value 10 inside `ebx`.

Ok, so let's go ahead and compile it.

```
nasm -f elf32 exit.nasm
ld -o exit exit.o
```

And let's actually look at machine code using a tool called 
`objdump` "object dump" which allows us to dump the **text** segment or
all segments, but we are interested in **text** segment right now


```
$ objdump -d exit

exit:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	b8 01 00 00 00       	mov    $0x1,%eax
 8048065:	bb 0a 00 00 00       	mov    $0xa,%ebx
 804806a:	cd 80                	int    $0x80
```

If you notice this in between here is actually final opcodes which 
are basically machine-consumable and the right handside is disassembly
which of course is exactly similar to what is in the shellcode 
`exit.nasm` file.

Now, this disassembly in AT&T syntax which is default syntax of 
`objdump`. To change that we are using `-M` option

```
$ objdump -d exit -M intel

exit:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	b8 01 00 00 00       	mov    eax,0x1
 8048065:	bb 0a 00 00 00       	mov    ebx,0xa
 804806a:	cd 80                	int    0x80
```

And now we see this has been shown Intel syntax. Now one we need to
do is copy the shellcode out and use it inside the file. So let me
open a new terminal.

You are probably wondering that this shellcode has a lot of nulls in
it. Why I am still using it? Just bare with me for a moment I'd like
to prove something to you.

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\xb8\x01\x00\x00\x00\xbb\x0a\x00\x00\x00\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

Now, the point which I am trying to illustrate is that nulls being in
the shellcode is a real problem from an exploitation perspective.
If you just transfer control to the `code` variable this part would
execute entirely this is not problem. And basically `exit` would 
happen.

Now, the removal of nulls is really due to the fact that an exploit
might break. As an example `strlen(code)` right now which I use to
estimate the length of the shellcode would break simply because it
would encounter a `null` byte. Let me illustrate this and quickly
compile it

```
gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
```

And let me run this

```
$ ./shellcode 
Shellcode Length:  2
```

And if you notice it runs and exited as expected. You would actually
do

```
echo $?
10
```

and see that return value was actually 10 which is what put inside
`ebx` while the shellcode length is came out to be 2 simply because
`strlen` can only accept **non-null** bytes. So in our case it is the 
first 2 bytes.

So the kitting here is to remember nulls as a binary are absolutely
fine. Nulls in shellcode which we will use in the exploit may break 
the exploit just like what you saw for `strlen`. 

So the next logical step of course is to remove this nulls. And if you
look at basically the instructions looks like `mov eax,0x1` has a
problem with nulls

```
08048060 <_start>:
 8048060:	b8 01 00 00 00       	mov    eax,0x1
 8048065:	bb 0a 00 00 00       	mov    ebx,0xa
 804806a:	cd 80                	int    0x80
```

and so `mov ebx,0xa`. And it can be obvious the reason is that `eax`
is the 32-bit register while 0x1 is the single byte value and all the
other nulls which you really see are just fillers for the other bytes.
Similar to what is happening in `mov ebx,0xa`. So let's modify
this shellcode and remove the nulls.

```
cp exit.nasm exit-no-null.nasm
vim exit-no-null.nasm
```

Now there are probably a billion ways which you could avoid nulls
and I'll just show you one of the ways.

We just want to load number 0x1 so you could just load `al` with 0x1
but the fact remains that you still have remaining 28 bits and they
need to be cleared as well. Now, moving a zero inside `eax` before
this would have exactly the same problem which would be 4 nulls for
all 4 bytes. Which means we need to indirectly basically put a zero
inside the `eax` register. The easiest way is to `xor eax,eax`.

Now, we can modify `mov eax,0x1` into `mov al,0x1` just to load the
value 0x1 inside `al`. Similarly, we can do `xor ebx,ebx` and load
`bl` with the value 10.

```
global _start

section .text
_start:

        xor eax,eax
        mov al, 1
        xor ebx,ebx
        mov bl, 10
        int 0x80
```

Now If I would compile this and link it and use `objdump`

```
$ objdump -d exit-no-null -M intel

exit-no-null:     file format elf32-i386


Disassembly of section .text:

08048060 <_start>:
 8048060:	31 c0                	xor    eax,eax
 8048062:	b0 01                	mov    al,0x1
 8048064:	31 db                	xor    ebx,ebx
 8048066:	b3 0a                	mov    bl,0xa
 8048068:	cd 80                	int    0x80
```

then you would notice that the resulting opcodes **do not** contain
nulls anywhere. Fantastic! So let's copy this out to `shellcode.c`
and verify the same

```c
#include<stdio.h>
#include<string.h>

unsigned char code[] = \
"\x31\xc0\xb0\x01\x31\xdb\xb3\x0a\xcd\x80";

main()
{

        printf("Shellcode Length:  %d\n", strlen(code));

        int (*ret)() = (int(*)())code;

        ret();

}
```

And as you are probably imagining it is stupid idea every time to 
copy-paste, you probably end up more time cut-coping-pasting shellcode
which is of course error-prone to begin with rather than shellcoding
process itself. So we need some shortcut to convert the `objdump`
shellcode into shellcode which is usable.

Before we will do all of this let me summarize what we've done. We
ensure that `eax` and `ebx` getting used with syscall number and 
exit code leads to 0x00 in shellcode.

Now, coming to extracting the object dump output and converting it
directly to shellcode we are going to use a little script in 
**commandlinefu**. And this is a fantastic site where there are tons
of quick command-line shortcuts. And this is the one [here](http://www.commandlinefu.com/commands/view/6051/get-all-shellcode-on-binary-file-from-objdump). Let's copy it and use in our case

```
$ objdump -d ./exit-no-null | grep '[0-9a-f]:'|grep -v 'file'|cut -f2 -d:|cut -f1-6 -d' '|tr -s ' '|tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|paste -d '' -s |sed 's/^/"/'|sed 's/$/"/g'

"\x31\xc0\xb0\x01\x31\xdb\xb3\x0a\xcd\x80"
```

**Note**

or create shell script if you prefer :)

And if you notice there are no nulls there anymore. Now, let's compile
`shellcode.c` and run it

```
$ gcc -fno-stack-protector -z execstack shellcode.c -o shellcode
$ ./shellcode 
Shellcode Length:  10
```

Fantastic! And `exit` happened.

```
echo $?
10
```

We've used 10 exit code to ensure that program has really executed
this part. But in reality you would notice if you look at the other
shellcode available you don't need to return non-zero value so
`mov bl,0xa` could be removed and further compact shellcode and
you could use this version as well.



