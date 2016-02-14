> "The supreme art of war is to subdue the enemy without fighting." 
>
> - Sun Tzu, The Art of War

#UNIX History

In 1965 *Bell Labs* was involved in a project with *MIT* and 
*General Electric* to develop a time-sharing system, called 
*Multiplexed Information and Computing Service* **(Multics)**, 
allowing multiple users to access a mainframe simultaneously.

Ken Thompson, a programmer in the Labs' computing research department, 
had worked on Multics. 

By April 1969, AT&T (Bell Labs) made a decision to withdraw Multics 
and go with GECOS. **General Comprehensive Operating System (GCOS)** is
a family of operating systems oriented toward mainframe computers. 
The original version of GCOS was developed by *General Electric* from 
1962; originally called **GECOS** (the General Electric Comprehensive 
Operating Supervisor). The operating system is still used today in its 
most recent versions (GCOS 7 and GCOS 8).


When Bell Labs withdrew from the Multics research consortium, Ken 
Thompson was left with some Multics-inspired ideas about how to build a
file system. He was also left without a machine on which to play a game
he had written called [Space Travel](https://en.wikipedia.org/wiki/Space_Travel_(video_game)), a science-fiction simulation that 
involved navigating a rocket through the solar system. But it needed a 
more efficient and less expensive machine to run on, and eventually he 
found a little-used PDP-7 at Bell Labs.

When Multics was withdrawn Ken Thompson and Dennis Ritchie needed to 
rewrite an operating system to play space travel on this smaller 
machine (a DEC PDP-7 [Programmed Data Processor 4K memory for user 
programs]).

![](/pwn/images/pdp7.jpeg)

On the PDP-7, in 1969, a team of Bell Labs researchers led by Thompson 
and Ritchie, including Rudd Canaday, developed a hierarchical file 
system, the concepts of *computer processes* and *device files*, a 
*command-line interpreter*, and some small utility programs.
The resulting system, much smaller than the envisioned Multics system, 
was to become **UNICS** *(UNiplexed Information and Computing Service)*
or **Unix** -- an 'emasculated Multics'. In summer 1969 Unix was 
developed.

When the Computing Sciences Research Center wanted to use Unix on a 
machine larger than the PDP-7, while another department needed a word 
processor, Thompson and Ritchie added text processing capabilities to 
Unix and received funding for a **PDP-11/20**. A text formatting 
program called [roff](https://en.wikipedia.org/wiki/Roff) and a 
[text editor](https://en.wikipedia.org/wiki/Text_editor) were added. 
All were written in PDP-11/20 assembly language.

For the first time in 1970, the Unix operating system was officially 
named and ran on the PDP-11/20.

![](/pwn/images/kd.jpg)

First official edition of Unix was released 11/03/1971.
The main thing missing was pipes.

As the system grew in complexity and the research team wanted more 
users, the need for a manual grew apparent. The 
[UNIX Programmer's Manual](https://en.wikipedia.org/wiki/UNIX_Programmer%27s_Manual) was published on 3 November 1971; 
commands were documented in the "[man page](https://en.wikipedia.org/wiki/Man_page)" format that is still used, offering terse reference 
information about usage as well as bugs in the software, and listing 
the authors of programs to channel questions to them.

It included over 60 commands like: **b** (compile B program); 
**boot** (reboot system); **cat** (concatenate files); **chdir** 
(change working directory); **chmod** (change access mode); 
**chown** (change owner); **cp** (copy file); **ls** (list directory 
contents); **mv** (move or rename file); **roff** (run off text); 
**wc** (get word count); **who** (who is one the system). 

##C Beginning

The original Unix operating system was written in assembler, and the 
applications in a mix of assembler and an interpreted language called 
B, which had the virtue that it was small enough to run on the PDP-7. 
But B was not powerful enough for systems programming, so Dennis 
Ritchie added data types and structures to it. The resulting C 
language evolved from B beginning in 1971.

In 1972, Unix was rewritten in the higher-level language C 
programming language, contrary to the general notion at the time 
"that something as complex as an operating system, which must deal 
with time-critical events, has to be written exclusively in assembly 
language".

Although assembly did not disappear from the man pages until 
[Version 8](https://en.wikipedia.org/wiki/Research_Unix#Versions), 
the migration to C resulted in much more portable software, requiring 
only a relatively small amount of machine-dependent code to be 
replaced when porting Unix to other computing platforms. 

In 1973 Thompson and Ritchie finally succeeded in rewriting Unix in 
their new language. The Unix operating system was first presented 
formally to the outside world at the 1973 Symposium on Operating 
Systems Principles, where Ritchie and Thompson delivered a paper. This
led to requests for the system. Ken Thompson quietly began answering 
requests by shipping out tapes and disk packs - each, according to 
legend, with a note signed "Love, Ken".

In 1974 AT&T released 
[Version 5](https://en.wikipedia.org/wiki/Research_Unix) Unix and 
licensed it to educational institutions, and licensed 1975's 
[Version 6](https://en.wikipedia.org/wiki/Research_Unix) to companies 
for the first time. While commercial users were rare because of the 
$20,000 cost, the latter was the most widely used version into the 
early 1980s. While anyone could purchase a license, the terms were 
very restrictive; licensees only received the source code, on an 
[as is](https://en.wikipedia.org/wiki/As_is) basis.

Versions of the Unix system were determined by editions of its user 
manuals; for example, "Fifth Edition UNIX" and "UNIX Version 5" have 
both been used to designate the same version.

