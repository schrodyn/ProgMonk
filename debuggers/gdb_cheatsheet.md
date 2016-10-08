# GDB Cheatsheet

1) To compile binary with debug symbols:

```
gcc -ggdb main.c -o main
```

or

```
gcc -g main.c -o main
```

2) `list` -- list source code. For example

```
list 1
```

List will only work if you have the source code of the program being 
debugged.


3) `info functions` -- List all the functions in the binary -> Will be 
different for `NO_DEBUG` version

4) `info sources` --  Identify the source file of the binary.
Identify source files for which symbols have been read in.

5) `info variables` -- List the variables. Will not list local 
variables. Will only list global and static variables

6) `info scope <TAB>` -- Looking into local variable. For example

```
info scope AddNumbers
```

7) Rip debug symbols off a binary
 
The Debug symbols have just been copied. They have not been stripped 
from the binary. 

```
objcopy --only-keep-debug Add_Sub_DEBUG DEBUG_symbols
```

or


```
$ gdb ./Add_Sub_DEBUG
(gdb) maint print symbols DEBUG_symbols2
```

Write a dump of debugging symbol data into the file `DEBUG_symbols2`.

9) Stripping symbols off a binary

```
strip --strip-debug Add_Sub_DEBUG
```

In order to further remove them use `--strip-unneeded` option

10) Adding Debug symbols to a Binary -> 2 options

-> Add it to Binary

```
objcopy --add-gnu-debuglink=DEBUG_symbols Add_Sub_DEBUG_stripped
```

-> Load the symbol files within GDB

```
$ gdb ./Add_Sub_DEBUG_stripped
(gdb) symbol-file DEBUG_symbols
```

11) `nm` -- list symbols from object files

```
man nm
nm ./Add_Sub_DEBUG
```

In sorted order by address:

```
nm -n ./Add_Sub_DEBUG
```

Display size of symbols:

```
nm -S ./Add_Sub_DEBUG
```

## Breakpoints, Examining Registers and Memory

```
(gdb) break main
(gdb) run
(gdb) info registers
(gdb) info breakpoints
```

Disable/Enable/Delete breakpoints:

```
(gdb) disable 1
(gdb) enable 1
(gdb) delete 1
```

Continue executing:

```
(gdb) c
(gdb) continue
```

Examine memory:

```
(gdb) help x
Examine memory: x/FMT ADDRESS.
ADDRESS is an expression for the memory address to examine.
FMT is a repeat count followed by a format letter and a size letter.
Format letters are o(octal), x(hex), d(decimal), u(unsigned decimal),
  t(binary), f(float), a(address), i(instruction), c(char), s(string)
  and z(hex, zero padded on the left).
Size letters are b(byte), h(halfword), w(word), g(giant, 8 bytes).
The specified number of objects of the specified size are printed
according to the format.
```

Print variables:

```
(gdb) print argv[0]
(gdb) print FunctionShouldNotExecure
```

Disassemble functions:

```
(gdb) disassemble main
```

Step instructions:

```
(gdb) step
(gdb) stepi
```

also there is `nexti` instructions. It's simillar but
jumps off function calls.

type `help` for more detail

```
(gdb) help step
Step program until it reaches a different source line.
Usage: step [N]
Argument N means step N times (or till program stops for another reason).
(gdb) help stepi
Step one instruction exactly.
Usage: stepi [N]
Argument N means step N times (or till program stops for another reason).
```

## Modifying Registers and Memory

```
(gdb) set {char} 0xbffff870 = 67
(gdb) set {int} 0xbffff870 = 67
(gdb) set {char} (0xbffff870 + 1) = 'B'
(gdb) set $eax = 10
```

Convenience Variables:

```
(gdb) set $i = 10
(gdb) print $i
$1 = 10
(gdb) set $demo = "BBBB"
(gdb) print $demo
$2 = "BBBB"
(gdb) set $dyn = (char *)malloc(10)
```

Call functions (also from linked libraries):

```
(gdb) call strcpy($dyn, argv[1])
(gdb) call EchoInput(argv[2])
```

## Other features

Set disassembly syntax:

```
(gdb) set disassembly-flavor intel
```

disassemble and break variants:

```
(gdb) disassemble $eip
(gdb) break * 0x080484f0
```

## Conditional Breakpoints

1) When you have source code:

```
(gdb) break 11
Breakpoint 1 at 0x8048490: file conb.c, line 11.
(gdb) info breakpoints
Num     Type           Disp Enb Address    What
1       breakpoint     keep y   0x08048490 in main at conb.c:11
(gdb) condition 1 counter == 5
(gdb) info breakpoints
Num     Type           Disp Enb Address    What
1       breakpoint     keep y   0x08048490 in main at conb.c:11
        stop only if counter == 5
```

2) When you don't have source:

```
(gdb) break *0x080484cb
Breakpoint 1 at 0x80484cb
(gdb) condition 1 $eax != 0
(gdb) info breakpoints
Num     Type           Disp Enb Address    What
1       breakpoint     keep y   0x080484cb <main+78>
        stop only if $eax != 0
```

## Hooks

We can invoke some actions on some events.
Now the whole idea is you want gdb to execute a bunch of commands as 
you are stepping through the command. So let's define the hook, which 
basically tells you to write whatever you want to be executed every 
time the program stops.

```
(gdb) define hook-stop
>disassemble $eip,+10
>end
```

Also to always display some info you can use `display` command:

```
(gdb) display/x $eax
```

Now every time the program stops the value of the register `eax` 
should be printed on the screen.


