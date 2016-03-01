#CPU Modes and Memory Management

Hello! In this video we will talk about the CPU modes and memory
management.

Now, IA-32 has three modes of operation. The first one is called
**Real Mode**. The processor is in Real Mode by default when it
was at power up or it is reset.

Now, the problem with the real mode is that in that specific mode
it can only access up to 1MB of memory, nothing more. And that can be
no memory protection or **Privilege Levels** such as differenciation
between Kernel and User space be applied to the entire system.

The second mode is what is called **Protected Mode**. And in protected
mode the processor can access up to 4GB of memory. And memory 
protection, privilege levels, multi-tasking etc. is possible. Now,
this also has a special Virtual-8086 mode to go ahead and execute
softwares which are got outdated from the previous generation.

Now, the third mode is the **System Management Mode** which is 
typically used for power management tasks and a couple of security 
related tasks. 

We will only be concerned with protected mode and pretty much as far
as real mode, smm or even Virtual-8086 mode is concerned you want
to look it up and check out other websites. But we will focus only
on protected mode because this is what almost all modern operating
systems use including Linux of course.

Now, apart from the CPU modes as you could probably imagine memory 
modeling is also very very inportant. How does the processor go ahead
and address the memory.

This is where basically there are three options. The first is **flat
memory model** in which addresses are pretty much linear across an
address space and you can access any of these addresses directly.
The second is **segmented model** in which you basically use a 
**segment selector** and an **offset** in that segment to go ahead and
figure out how to access specific locations. And then finally you have
**real-address mode model** which is basically a special case of the
segmented model.

Now, again for all purposes of this video series we will only look at 
flat model.

You are probably asking why I made bolt(?) of this two choices. This is
because 32-bit Linux basically uses protected mode and the flat memory
model for functioning. And this all we are going to look at. And during
this functioning it has up to 4GB Addressable space. And as we've 
mentioned memory protection, privilege levels, multi-tasking is 
possible. 

Now, the segment registers which we've spoken about in the previous 
video as far as in protected mode, flat memory model is concerned 
actually point to something called **segment descriptors** which could
either be GDT, LDT and IDT which stand for Global, Local and Interrupt
Descriptor Table.

Now, we mean not have to looking to too much of this just for pure 
assembly and shellcoding series. Maybe later on when we'll look at
more interesting stuff such as malware, reversing of all of that.
We could look at GDT, LDT and IDT. Or if you are interesting in 
writing your own operating system.

Ok, so what we spoke about in the last couple of slides was basically 
the fact that we can access up to 4GB of memory. Now what does this 
mean for any process running on the 32-bit Linux system? Well, This is 
where virtual memory modeling comes in to picture. Now, virtual memory
model allows the process to think that it basically is the **only** 
process on the system which actually occupies the entire 4GB of space. 
And this 4GB is divided into 1GB which actually belongs to the Kernel
for various important things it requires and then 3GB is User Space,
which is where process code and all of those things are loaded. 

Now, if I break this up further this is how it would look like. 
Typically you actually have the process program code loaded inside
a segment called the **Text segment** or the **Text section**. 
You have all initialized data stored the **Data segment**. Then you
have **BSS** which is where only an uninitialized get stored.

Now, apart from this you have the **Heap** which is responsible for
Dynamic Memory allocation. We could go ahead and map Shared Libraries
and even shared files and things like that into the process. And 
of course we would also have something called the **Stack** which is 
where function arguments, local variables and all of that get stored.
And then finally you have the **Kernel Space** which extends to the
last 1GB of memory.

Now, the important thing is that once you have this structure in your
mind it's going to be very very easy to understand how all of this 
things play together.

Now, let's go ahead and look at real example. I've actually said this
is how a process would look like in memory. Now, is it possible for
us to go ahead and actualy see real process on Linux layout?

Well, to look at process organisation in memory there are multiple 
ways. If your system supports `proc` then `proc/pid/maps` gives you
how this program memory actually looks like or how it is mapped.

`pmap` is also in capability which allows you to do the same thing. 
And then finally we could also use GDB to attach to a process and view
how it is layed out in memory.

So let's go ahead and do all of this three. In the very first example
let's actually look at our own memory.

```
$ ps
  PID TTY          TIME CMD
 2113 pts/0    00:00:00 bash
 2307 pts/0    00:00:00 ps
```

So basically what I actually do is pick up the pid and do

```
$ cat /proc/2113/maps

08048000-08124000 r-xp 00000000 08:01 393222     /bin/bash
08124000-08125000 r--p 000db000 08:01 393222     /bin/bash
08125000-0812a000 rw-p 000dc000 08:01 393222     /bin/bash
0812a000-0812f000 rw-p 00000000 00:00 0 
08ade000-08c96000 rw-p 00000000 00:00 0          [heap]
b72d4000-b72df000 r-xp 00000000 08:01 132032     /lib/i386-linux-gnu/libnss_files-2.15.so
b72df000-b72e0000 r--p 0000a000 08:01 132032     /lib/i386-linux-gnu/libnss_files-2.15.so
b72e0000-b72e1000 rw-p 0000b000 08:01 132032     /lib/i386-linux-gnu/libnss_files-2.15.so
b72e1000-b72eb000 r-xp 00000000 08:01 132036     /lib/i386-linux-gnu/libnss_nis-2.15.so
b72eb000-b72ec000 r--p 00009000 08:01 132036     /lib/i386-linux-gnu/libnss_nis-2.15.so
b72ec000-b72ed000 rw-p 0000a000 08:01 132036     /lib/i386-linux-gnu/libnss_nis-2.15.so
b72ed000-b7303000 r-xp 00000000 08:01 132026     /lib/i386-linux-gnu/libnsl-2.15.so
b7303000-b7304000 r--p 00015000 08:01 132026     /lib/i386-linux-gnu/libnsl-2.15.so
b7304000-b7305000 rw-p 00016000 08:01 132026     /lib/i386-linux-gnu/libnsl-2.15.so
...

```



Remembler you need to be root if you want to look at the processes 
owned by some other user.

And as you noticed this is a huge dump of how the process is actually 
layed out in memory. And you actually noticed that we have started 
this exact offset which is `08048000` which is typical for processes
running on Linux. And then you see a bunch of things. So what all of
these things mean? Let me break this stuff for you.

```
08048000-08124000 r-xp 00000000 08:01 393222     /bin/bash
```

Now, the first two things which you see the nothing but the **start** 
and **end address** of the section which we are looking at. The next
four values give you an understanding about **permissions** of this
section. Permissions include read, write, execute, and as far as
the last value is concerned this can either be `p` which this is 
`private` and not shared or an `s` which is the segment is actually
`shared`.

Now, depending on whether this was mapped from/on a file the next 
value is nothing but the offset inside the file for the memory mapped 
file. In case it is not mapped from a file this is actually set to
value 0.

After that you have the major and minor device number of the device
from where the file was loaded and then you actually have the 
**inode** number for the file. And finally you have the **File Path**
if that is applicable.

Now, apart from this we talked about other things which would be there
such as the `[heap]` and if you scroll down you have the `[stack]`  
which we spoke about which is in high memory. And `[vdso]` is 
basically used as a much more intelligent way to do syscalls. We 
probably look at this later on when we look at system calls.

Then in between you have shared libraries which get loaded up. Right
you can clearly see you have `libc-2.15.so` here. And then you also 
have a couple of sections which are unnamed anything. This called
**anonymous sections** that coresspond to sections that loaded by
`mmap`.

So this is how the process looks like when it is loaded into memory.

Now, another way you could go ahead and find this out is by running
it inside `gdb`. And to do that we would load up the process and
basically use `info proc mappings`.

Let me load up `gdb /bin/bash` and if I actually do 
`info proc mappings` it would say `No current process`.

So let's run this

```
(gdb) break main
Breakpoint 1 at 0x805eba6
(gdb) run
Starting program: /bin/bash 

Breakpoint 1, 0x0805eba6 in main ()
```

And now if we do `info proc mappings`

```
process 2358
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8124000    0xdc000        0x0 /bin/bash
	 0x8124000  0x8125000     0x1000    0xdb000 /bin/bash
	 0x8125000  0x812a000     0x5000    0xdc000 /bin/bash
	 0x812a000  0x812f000     0x5000        0x0 [heap]
	0xb7dfa000 0xb7dfb000     0x1000        0x0 
	0xb7dfb000 0xb7f9f000   0x1a4000        0x0 /lib/i386-linux-gnu/libc-2.15.so
	0xb7f9f000 0xb7fa0000     0x1000   0x1a4000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fa0000 0xb7fa2000     0x2000   0x1a4000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fa2000 0xb7fa3000     0x1000   0x1a6000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fa3000 0xb7fa7000     0x4000        0x0 
	0xb7fa7000 0xb7faa000     0x3000        0x0 /lib/i386-linux-gnu/libdl-2.15.so
	0xb7faa000 0xb7fab000     0x1000     0x2000 /lib/i386-linux-gnu/libdl-2.15.so
	0xb7fab000 0xb7fac000     0x1000     0x3000 /lib/i386-linux-gnu/libdl-2.15.so
	0xb7fac000 0xb7fc8000    0x1c000        0x0 /lib/i386-linux-gnu/libtinfo.so.5.9
	0xb7fc8000 0xb7fca000     0x2000    0x1b000 /lib/i386-linux-gnu/libtinfo.so.5.9
	0xb7fca000 0xb7fcb000     0x1000    0x1d000 /lib/i386-linux-gnu/libtinfo.so.5.9
	0xb7fdb000 0xb7fdd000     0x2000        0x0 
	0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
	0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
	0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
...

```

You would notice that we can see mapping similar for what we've seen
in the previous output.

To one of the other ways to do this is to use a little command called
`pmap` 

```
$ pmap -d 2113 
2113:   bash
Address   Kbytes Mode  Offset           Device    Mapping
08048000     880 r-x-- 0000000000000000 008:00001 bash
08124000       4 r---- 00000000000db000 008:00001 bash
08125000      20 rw--- 00000000000dc000 008:00001 bash
0812a000      20 rw--- 0000000000000000 000:00000   [ anon ]
08ade000    1764 rw--- 0000000000000000 000:00000   [ anon ]
b72d4000      44 r-x-- 0000000000000000 008:00001 libnss_files-2.15.so
b72df000       4 r---- 000000000000a000 008:00001 libnss_files-2.15.so
b72e0000       4 rw--- 000000000000b000 008:00001 libnss_files-2.15.so
b72e1000      40 r-x-- 0000000000000000 008:00001 libnss_nis-2.15.so
b72eb000       4 r---- 0000000000009000 008:00001 libnss_nis-2.15.so
b72ec000       4 rw--- 000000000000a000 008:00001 libnss_nis-2.15.so
b72ed000      88 r-x-- 0000000000000000 008:00001 libnsl-2.15.so
b7303000       4 r---- 0000000000015000 008:00001 libnsl-2.15.so
b7304000       4 rw--- 0000000000016000 008:00001 libnsl-2.15.so
b7305000       8 rw--- 0000000000000000 000:00000   [ anon ]
b7308000      32 r---- 0000000000000000 008:00001 bash.mo
b7310000      28 r--s- 0000000000000000 008:00001 gconv-modules.cache
b7317000    2048 r---- 0000000000000000 008:00001 locale-archive
b7517000       4 rw--- 0000000000000000 000:00000   [ anon ]
b7518000    1680 r-x-- 0000000000000000 008:00001 libc-2.15.so
b76bc000       4 ----- 00000000001a4000 008:00001 libc-2.15.so
b76bd000       8 r---- 00000000001a4000 008:00001 libc-2.15.so
b76bf000       4 rw--- 00000000001a6000 008:00001 libc-2.15.so
b76c0000      16 rw--- 0000000000000000 000:00000   [ anon ]
b76c4000      12 r-x-- 0000000000000000 008:00001 libdl-2.15.so
b76c7000       4 r---- 0000000000002000 008:00001 libdl-2.15.so
b76c8000       4 rw--- 0000000000003000 008:00001 libdl-2.15.so
b76c9000     112 r-x-- 0000000000000000 008:00001 libtinfo.so.5.9
b76e5000       8 r---- 000000000001b000 008:00001 libtinfo.so.5.9
b76e7000       4 rw--- 000000000001d000 008:00001 libtinfo.so.5.9
b76ee000      28 r-x-- 0000000000000000 008:00001 libnss_compat-2.15.so
b76f5000       4 r---- 0000000000006000 008:00001 libnss_compat-2.15.so
b76f6000       4 rw--- 0000000000007000 008:00001 libnss_compat-2.15.so
b76f7000       4 r---- 00000000005d8000 008:00001 locale-archive
b76f8000       8 rw--- 0000000000000000 000:00000   [ anon ]
b76fa000       4 r-x-- 0000000000000000 000:00000   [ anon ]
b76fb000     128 r-x-- 0000000000000000 008:00001 ld-2.15.so
b771b000       4 r---- 000000000001f000 008:00001 ld-2.15.so
b771c000       4 rw--- 0000000000020000 008:00001 ld-2.15.so
bfc8a000     132 rw--- 0000000000000000 000:00000   [ stack ]
mapped: 7180K    writeable/private: 2004K    shared: 28K
```

And here you actually see the maping here longs again. Now notice
the anonymous section that are actually marked accordingly rather than
just being left blanked as in the previous outputs. 

So this is exactly how you can figure out a process mapping. You can 
play around with `pmap`, more with `gdb` commands and all of that
stuff.

Now you understand how a process is loaded and how it looks like in 
memory.
