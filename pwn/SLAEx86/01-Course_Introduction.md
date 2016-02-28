#Course Introduction

Hello! Welcome to the **SecurityTube Linux Assembly Expert** course
and certification Introduction Video. My name is *Vivek Ramachandran*
and I will be your course instructor.

In this introduction video I want to tell you briefly about what
the courses, what we will cover and what to expect from it, once you 
succesfully finish it.

##What is SLAE?

SLAE is basically geared(?) at teaching you to build assembly 
for intel architecture on the Linux platform.

Make no mistake. This is not simply just an assembly language 
programming course. Rather this is geared to infosec and we'll look
how you can apply your newly learned linux assembly skills to topics
like shellcoding, writing encoders, decoders, cryptors, and other
low-level applications which are required dissent(?) mastery of
assembly language.

Now we will be starting absolutely from scratch. I will teach you 
how to write the first "Hello world" program, then will be more to 
more challenging items so such as writing loops or probably going
ahead and even writing code for FPUs(?), MMX and all of that. But keep 
in mind I'll make this series of video course very very beginning 
friendly. Just like my all other courses.

So if you do not have any prior programming experience in assembly
or another languages you should still be able to pick up this course
pretty okey.

Okay, let me run you to the course syllabus. Now I will talk about 
the very basics of computer architecture more on IA-32/64 Family of
processors. From that we will look at compilers, assemblers, linkers.
And then different CPU modes and Addressing schemas. 

Now this is very hands-on practical course and we are going to be using
a lot of tools such as `nasm`, `ld`, `objdump`, `ndisasm` and a bunch
of others.

After we'll look at all of this basics of setupping environment,
understanding how the Linux operating system works in conjunction
with CPU. Then we will go ahead and looking to IA-32 Assembly Language
and create simple programs to run on the Linux OS. IA-32 language
will include understanding of the Registers and Flags, Program
Structure, Data Types, how do you move data back and forth beetwen
CPU, Memory and all of that, Reading and Writing to and from Memory,
conditional statements, loops, function prologues, epilogues,
procedures, exceptional handling and a bunch of other things.

Then we will look at things specific for Linux such as how do you 
use syscalls interface, then `int 0x80` and all of that stuff.
And then move on some more advanced stuff which is using FPU, MMX,
SSE and SSE2. And writing assembly using their instructions.

As you can see the first part of this course is scarely(?) geared 
towards providing you all the basics you required to understand on
how to write assembly language on 32-bit systems.

Now the second part of the course essentially builds(?) that applying
all the knowledge you gained to the domain of Information Security.

This is where we will start from the very basics of shellcoding on 
Linux, look at how you can get running with the execution environment,
create actual shellcode from scratch for exec, exit, bind and 
reverse tcp shells, staged shellcode, egg hunters, how you will use 
shellcode from 3rd party sites and understand how they works. 
You might have seen a lot of shellcode on exploitdb and a bunch of 
other websites. You will understand what's going on in them and
use them effectively. Simulating shellcode, locating different
system calls. How do you graph what is happening inside shellcode
for better understanding. And then we will going to encoders,
decoders and crypters which is probably one of the most interesting 
topics out there.

From the low level assembly apply to infosec perspective. Now many of
you use a Metasploit exploits and a bunch of fun stuff. But what goes
in to writing your own encoders, decoders and crypters? This is 
exactly what this part of the course will look at. Where you will 
learn how to write your own encoders and even go far beyound 
looking at the existing schemas to actually creating your own
custom encoders which will use random sequences, other mathematical
functions and things like that. From there on we will touch up on
the most interesting topics as far as low level assembly application
for infosec is concerned which is Polymorphism and polymorphic
engines. This is very hot topic we need to count(?) to wireless malware
or to create shellcode which is difficult to detect and fingerprint.
Once we are done with that there will be a bunch of exercises I will
share with you a mock exam and then you can actually give the 
Certification Exam if you so desire.

