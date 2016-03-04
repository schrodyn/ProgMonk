#Tools and Basic Reverse Engineering – Part 2 

##ELF Memory Layout

![](/pwn/MBE/images/img82.jpg)

##Virtual Memory Layout 

![](/pwn/MBE/images/img84.jpg)

##Physical Memory Layout 

![](/pwn/MBE/images/img87.jpg)

##Physical Memory Layout

![](/pwn/MBE/images/img89.jpg)

##GNU Debugger - Basics 

  * disassemble main (disas main) 
  * set disassembly-flavor intel 
  * break main (b main) 
  * run 
  * stepi (s), step into 
  * nexti (n), step over 

##GNU Debugger – Examine Memory

  * Examine memory: `x/NFU address`
  * N = number
  * F = format 
  * U = unit 

Examples

  * `x/10xb 0xdeadbeef`, examine 10 bytes in hex
  * `x/xw 0xdeadbeef`, examine 1 word in hex 
  * `x/s 0xdeadbeef`, examine null terminated string

GNU debugger may have Init File `~/.gdbinit`

##Tracing

* `ltrace`, library calls 
* `strace`, system calls 


