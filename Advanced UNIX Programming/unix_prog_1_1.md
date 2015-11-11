The subject of this book is UNIX system calls—the interface between 
the UNIX kernel and the user programs that run on top of it.

System calls are the only way to access kernel facilities such as 
the file system, the multitasking mechanisms, and the interprocess communication
 primitives.

System calls define what UNIX is. Everything else—subroutines and commands—is 
built on this foundation.

Files
-----
There are several kinds of UNIX files: regular files, directories, 
symbolic links, special files, named pipes (FIFOs), and sockets.

Regular Files:
Regular files contain bytes of data, organized into a linear array. Any byte or
sequence of bytes may be read or written.
Reads and writes start at a byte location specified by the file offset, which 
can be set to any value (even beyond the
end of the file). Regular files are stored on disk.

It isn’t possible to insert bytes into the middle of a file (spreading the file
apart), or to delete bytes from the middle (closing the gap).

Two or more processes can read and write the same file concurrently. The results
depend on the order in which the individual I/O requests occur and are in 
general unpredictable. To maintain order, there are file-locking facilities and
semaphores, which are system-wide flags that processes can test and set

Files don’t have names; they have numbers called i-numbers. An i-number is an
index into an array of i-nodes,
kept at the front of each region of disk that contains a UNIX file system.

Each i-node contains important information about one file.
It doesn't include either the name or the data bytes.
It does include the following:
* type of file (regular, directory, socket, etc.);
* number of links
* owner’s user and group ID;
* three sets of
access permissions—for the owner, the group, and others;
* size in bytes;
* time of last access;
* last modification;
* status change (when the i-node itself was last modified);
* and, of course, pointers to disk blocks containing the file’s contents;


Directories and Symbolic Links:
Each directory consists, conceptually, of a two-column table, with a name in one
column and its corresponding i-number in the other column.
A name/i-node pair is called a link.

When the UNIX kernel is told to access a file by name, it automatically 
looks in a directory to find the i-number.
Then it gets the corresponding i-node, which contains more information about the file (such as who can access it).
If the data itself is to be accessed, the i-node tells where to find it on the 
disk.

Directories, which are almost like regular files, occupy an i-node and have 
data. Therefore, the i-node corresponding to a particular name in a directory could be the i-node of another directory.

In following a relative path how does the kernel know where to start?
It simply keeps track of the i-number of the current directory for each process.
When a process changes its current directory, it
must supply a path to the new directory. 
That path leads to an i-number, which then is saved as the i-number of the new 
current directory.

An absolute path begins with a / and starts with the root directory. The kernel
simply reserves an i-number (2, say) for the root directory. This is established
when a file system is first constructed. 
There is a system call to change a process’s root directory 
(to an i-number other than 2).

Because the two-column structure of directories is used directly by the kernel, 
a program cannot write a directory as if it were a regular file.
Instead, a program manipulates a directory by using a special set of system 
calls.

It is possible for two or more links, in the same or different directories, to 
refer to the same i-number. This means that the same file may have more than one
name.

Removing a link to an i-node merely decrements the link count; 
when the count reaches zero, the kernel discards the file.

Multiple links to a file using i-numbers work only if the links are in the same file system, as i-numbers are unique only within a file system.

Special Files:
A special file is typically some type of device (such as a CD-ROM drive or 
communications link).
There are two principal kinds of device special files: block and character.
Block special files follow a particular model: The device contains an array of 
fixed-size blocks (say, 4096 bytes each), and a pool of kernel buffers are used as a cache to speed up I/O.

Character special files don’t have to follow any rules at all. They
might do I/O in very small chunks (characters) or very big chunks (disk tracks),
and so they’re too irregular to use the buffer cache.

Regular files and directories are accessed by
the file-system code in the kernel via a block special file, to gain the 
benefits of the buffer cache.

A special file has an i-node, but there aren’t any data bytes on disk for the 
i-node to point to. Instead, that part of the i-node contains a device number. 
This is an index into a table used by the kernel to find 
a collection of subroutines called a device driver.

When a system call is executed to perform an operation on a special file, the
appropriate device driver subroutine is invoked. 
What happens then is entirely up to the designer of the device driver;
since the driver runs in the kernel, and not as a user process, it can access—
and perhaps modify—any part of the kernel, any
user process, and any registers or memory of the computer itself.

Programs, Processes, and Threads
--------------------------------

A program is a collection of 
instructions and data that is kept in a regular file on disk.
In its i-node the file is marked executable, and the file’s contents are
arranged according to rules established by the kernel.

In order to run a program, the kernel is first asked to create a new process, 
which is an environment in which a program executes.
A process consists of three segments: 
instruction segment (is called the “text segment”), user data segment, 
and system data segment.

The program is used to initialize the instructions and user data.

In addition, the process may acquire resources (more memory, open files,
etc.) not present in the program.

Tip:
how interpretive languages like Java, Perl, Python, and shell scripts work:
For them, the executable is an interpreter, and the program, even if compiled 
into some intermediate code, is just data for the interpreter and isn’t 
something the UNIX kernel ever sees or cares about. 
The kernel’s customer is the interpreter. 

While the process is running, the kernel keeps track of its threads, 
each of  which is a separate flow of control through the instructions, 
all potentially  reading and writing the same parts of the process’s data.

(Each thread has its own stack, however.)
When you’re programming, you start with one thread, and that’s all you get
unless you execute a special system call to create another.

Not every version of UNIX supports multiple threads. They’re part of an optional
 feature called POSIX Threads, or “pthreads,” 
and were introduced in the mid-1990s.

Several concurrently running processes can be initialized from the same program.
There is no functional relationship, however, between these processes.
The kernel might be able to save memory by arranging for such processes to share instruction segments, but the processes involved can’t detect such sharing.

A process’s system data includes attributes such as current directory, open file
descriptors, accumulated CPU time, and so on.
A process can’t access or modify
its system data directly, since it is outside of its address space.
Instead, there are various system calls to access or modify attributes.

A process is created by the kernel on behalf of a currently executing process,
which becomes the parent of the new child process.
The child inherits most of the parent’s system-data attributes.
For example, if the parent has any files open, the
child will have them open too.

This is different from a thread creating a new thread.
Threads in the same process are equal in most respects, 
and there’s no inheritance. All threads have equal access to all data
and resources, not copies of them.

Signals
-------
The kernel can send a signal to a process.
A signal can be originated by the kernel itself, sent from a process to itself,
sent from another process, or sent on behalf of the user.

An example of a kernel-originated signal is a segmentation-violation signal, 
sent when a process attempts to access memory outside of its address space.

An example of a signal sent by a process to itself is an abort signal, 
sent by the abort function to terminate a process with a core dump.

An example of a signal sent from one process to another is a termination signal,
 sent when one of several related processes decides to terminate the whole 
family.

Finally, an example of a user-originated signal is an interrupt signal, sent to
all processes created by the user when 
he or she types Ctrl-c.

There are about 28 types of signals. a process can control what happens when it
receives the signal.
It can accept the default action, which usually
results in termination of the process;
it can ignore the signal;
or it can catch the signal and execute a function, called a signal handler, 
when the signal arrives.

The signal type ( SIGALRM , say) is passed as an argument to the handler.
There isn’t
any direct way for the handler to determine who sent the signal, however.

When the signal handler returns, the process resumes executing at the point of 
interruption.
Two signals are left unused by the kernel. These may be used by an
application for its own purposes.


Process-IDs, Process Groups, and Sessions
-----------------------------------------
Every process has a process-ID, which is a positive integer. At any instant, 
these are guaranteed to be unique. Every process has a parent.
A process’s system data also records its parent-process-ID, 
the process-ID of its parent.

If a process is orphaned because its parent terminated before it did, its
parent-process-ID is changed to 1 (or some other fixed number).

This is the process-ID of the initialization process (init), created at 
boot time, which is the ancestor of all other processes.

Sometimes programmers choose to implement a subsystem as a group of related
processes instead of as a single process.
The UNIX kernel allows these related
processes to be organized into a process group.
Process groups are further organized into sessions.

One of the session members is the session leader, and one member of each process
 group is the process-group leader.
For each process, UNIX keeps track of the
process IDs of its process-group leader and session leader.

The kernel provides a system call to send a signal to each member of a process
group. Typically, this would be used to terminate the entire group, but any 
signal can be broadcast in this way.

UNIX shells generally create one session per login.
the kernel allows more flexibility so that any number of sessions can be formed
 within a single login, and each session can have its process groups formed any way it likes

A session, and thus the processes in it, can have a controlling terminal, which is the first terminal device opened by the session leader; the session leader 
then becomes the controlling process.

Not having a controlling terminal is typical of so-called daemons (Web servers,
the cron facility, etc.) which, once started, run divorced
from whatever terminal was used to start them.

It’s possible to organize things so that only one process group in a session—the
foreground job—has access to the terminal and to move process groups back and
forth between foreground and background. This is called job control.

A back-ground process group that attempts to access the terminal is suspended 
until moved to the foreground. This prevents it from stealing input from the 
foreground job or scribbling on the foreground process group’s output.

The terminal device driver sends interrupt, quit, and hangup signals coming from
a terminal to every process in the foreground process group 
for which that terminal is the controlling terminal.
Additionally, when a session leader (controlling process) terminates for any 
reason, all processes in the foreground process group are sent a hangup signal.

In summary, there are four process-IDs associated with each process:
* process-ID: Positive integer that uniquely identifies this process.
* parent-process-ID: Process-ID of this process’s parent.
* process-group-ID: Process-ID of the process-group leader.
If equal to the process-ID, this process is the group leader.

* session-leader-ID: Process-ID of the session leader.

Permissions
-----------
A user-ID is a positive integer that is associated with a user’s login name in 
the password file (/etc/passwd).
When a user logs in, the login command makes this
ID the user-ID of the first process created, the login shell.
Processes descended from the shell inherit this user-ID.

Users are also organized into groups which have IDs too, called group-IDs.
A user’s login group-ID is made by the group-ID of his or her login shell.
Groups are defined in the group file (/etc/group).

While logged in, a user can
change to another group of which he or she is a member.
This changes the group-ID 
of the process that handles the request (normally the shell), which then is 
inherited by all descendent processes.

As a user may be a member of more than one group, a process also has a list of 
supplementary group-IDs.

These two user and group login IDs are called the real user-ID and the real 
group-ID because they are representative of the real user, the person who is 
logged in.
Two other IDs are also associated with each process: 
the effective user-ID and the effective group-ID.
The effective ID is always used to determine permissions. The real ID is used 
for accounting and user-to-user communication.
One indicates the user’s permissions; the other indicates the user’s identity.

Each file (regular, directory, socket, etc.) has, in its i-node, 
an owner user-ID (owner for short) and an owner group-ID (group for short).
The i-node also contains 
three sets of three permission bits (nine bits in all).
Each set has one bit for read permission,
one bit for write permission, and one bit for execute permission.

The permission system determines whether a given process can perform a desired
action (read, write, or execute) on a given file.
For regular files, the meaning of the actions is obvious. 
For directories, the meaning of read is obvious.
“Write” permission on a directory means the ability to issue a system call that
 would modify the directory (add or remove a link).

“Execute” permission means the ability to
use the directory in a path (sometimes called “search” permission).

For special files, read and write permissions mean the ability to execute the 
read and write system calls.
That implies is up to the designer of the device driver.
Execute permission on a special file is meaningless.

The permission system determines whether permission will be granted using this
algorithm:
1. If the effective user-ID is zero, permission is instantly granted 
(the effective user is the superuser).

2. If the process’s effective user-ID matches the file’s user-ID, then the owner
set of bits is used to see if the action will be allowed.

3. If the process’s effective group-ID or one of the supplementary group-IDs
matches the file’s group-ID, then the group set of bits is used.

4. If neither the user-IDs nor group-IDs match, then the process is an “other”
and the third set of bits is used.

The steps go in order, so if in step 3 access is denied because, say, write 
permission is denied for the group, then the process cannot write, even though
the “other” permission (step 4) might allow writing.

Let's see an example:
Consider we want a user to temporarily take on the privileges of another user.
For example, when we execute the passwd command to change our password, we
would like the effective user-ID to be that of the superuser, because only root
can write into the password file.

This is done by making root the owner of the passwd command and then turning on
another permission bit in the passwd command’s i-node, called 
the set-user-ID bit.

Executing a program with this bit on
changes the effective user-ID of the process to the owner of the file containing the program.

Since it’s the effective user-ID, rather than the real user-ID, that determines
permissions, this allows a user to temporarily take on the permissions of
someone else.
The set-group-ID bit is used in a similar way.

Since both user-IDs (real and effective) are inherited from parent process to 
child process, it is possible to use the set-user-ID feature to run with an 
effective user-ID for a very long time.
That’s what the su command does.

A process’s priority is used by the kernel scheduler. Any process can lower its
 priority via the system call nice ;

Signals are mainly used just to terminate processes or to indicate very
unusual events.

With process tracing, a parent can control the execution of its child. Since the
 parent can read and write the child’s data, the two can communicate freely. 
Process tracing is used only by debuggers, since it is far too complicated and 
unsafe for general use.

Files are the most common form of interprocess communication. For example, one
might write a file with a process running vi and then execute it with a process
 running python.

Finally, something I can actually recommend: Pipes solve the synchronization
problems of files.

A pipe is not a type of regular file; although it has an i-node, there are no 
links to it.
If the reader gets ahead of the writer, the reader blocks (stops running for a 
while) until there’s more data. If the writer gets too far ahead of the reader,
it blocks until the reader has a chance to catch up, so the kernel doesn’t have
too much data queued.

A semaphore (in the computer business) is a counter that prevents two or more
processes from accessing the same resource at the same time.

A file lock, in effect a special-purpose semaphore, prevents two or more 
processes from accessing the same segment of a file.

A message is a small amount of data (500 bytes, say) that can be sent to a 
message queue. Messages can be of different types. Any process with appropriate
permissions can receive messages from a queue.

Shared memory potentially provides the fastest interprocess communication of 
all. The same memory is mapped into the address spaces of two or more processes.

As soon as data is written to the shared memory, it is instantly available to 
the readers. A semaphore or a message is used to synchronize the reader and 
writer.

networking interprocess communication, using a group of system calls called 
sockets.
(Other system calls in this group have names like bind , connect , and accept .)










