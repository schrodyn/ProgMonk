The most effective way to understand how an operating system works is through
abstraction—a fancy way of saying that you can ignore most of the details.

Software developers use abstraction as a tool when building an operating system
and its applications.

When building a software
component, developers typically don’t think much about the internal structure of
other components, but they
do care about what other components they can use and how to use them.

#Levels and Layers of Abstraction in a Linux System

A layer or level -- classification of a component according to where that
component sits between the user and the hardware.

A Linux system has three main levels:

  1. User processes (GUI, Servers, Shell)
  2. Linux Kernel (System Calls, Process Management, Memory Management,
  Device Drivers)
  3. Hardware (Processor(CPU), Main Memory (RAM), Disks, Network Ports)

kernel -- the core of the operating system
kernel -- software residing in memory that tells the CPU what to do

kernel manages hardware and acts primarily as an interface
between the hardware and any running program.

Processes -- the running programs that the kernel manages

All processes make up system's upper level called user space.

The kernel runs in kernel mode. The user processes run in user mode.

Code running in kernel mode has unrestricted access to the
processor and main memory.

The area that only the kernel can access is called kernel space.

User mode, in comparison, restricts access to a (usually quite small) subset of
memory and safe CPU operations.

#Hardware: Understanding Main Memory

In its most raw form, main memory is just a big storage area for a bunch of 0s and
1s. Each 0 or 1 is called a bit. This is where
the running kernel and processes reside — they’re just big collections of bits.

All input and output from peripheral devices flows through main memory,
also as a bunch of bits. A CPU is just an operator on memory;
it reads its instructions and data from the memory and writes data back out to the
memory.

A state (in reference to memory, processes, the kernel) -- is a particular
arrangement of bits.

A single process can easily consist of millions of bits in memory.

#The Kernel

Nearly everything that the kernel does revolves around main memory.

One of the kernel’s tasks is to split memory into many subdivisions,
and it must maintain certain state information about those subdivisions
at all times.

Each process gets its own share of memory, and the
kernel must ensure that each process keeps to its share.

#Process Management

Process management describes the starting, pausing, resuming, and terminating of
 processes.

A context switch -- The act of one process giving up control of the CPU to another
process

Each piece of time -- called a time slice -- gives a process enough time for
significant computation.

The context switch answers the important question of when the kernel runs.
The answer is that it runs between process time slices during a context switch.

Modern CPUs include a memory management unit (MMU) that
enables a memory access scheme called virtual memory.

When the process accesses some of its memory, the MMU
intercepts the access and uses a memory address map to translate the memory
location from the process into an actual physical memory location on the machine.

#System Calls and Support

system call is an interaction between a process and the kernel.

Two system calls, fork() and exec(), are important to understanding how processes start up:

fork() When a process calls fork(), the kernel creates a nearly identical copy of the process.

exec() When a process calls exec(program), the kernel starts program, replacing the current process.

when you enter ls into a terminal window:
the shell that's running inside the terminal window calls fork() to create a copy
of the shell -> and then the new copy of the shell calls exec(ls)

#User Space

The main memory that the kernel allocates for user processes is called user space.
Because a process is simply a state (or image) in memory, user space also refers
to the memory for the entire collection of running processes.

#Users

A user is an entity that can run processes and own files.
A user is associated with a username.
The kernel does not manage the usernames; instead, it identifies users by simple
numeric identifiers called userids.

Every user-space process has a user owner,
and processes are said to run as the owner.

Groups are sets of users. The primary purpose of groups is to allow a user to share file access to other users in a group.

User processes make up the environment that you
directly interact with; the kernel manages processes and hardware.
Both the kernel and processes reside in memory.
