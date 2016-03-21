#Build instructions


```
nasm -f elf32 execve_shellcode.nasm

ld -m elf_i386 -o execve_shellcode execve_shellcode.o

gcc -m32 -fno-stack-protector -z execstack shellcode.c -o shellcode
```
