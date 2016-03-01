#Hello, world of concurrency in C++!

One of the most significant new features in the C++11 Standard is the 
support of multithreaded programs. For the first time, the C++ Standard
will acknowledge the existence of multithreaded applications in the 
language and provide components in the library for writing 
multithreaded applications.

This will make it possible to write multithreaded C++ programs without 
relying on platform-specific extensions and thus allow writing portable
multithreaded code with guaranteed behavior. It also comes at a time 
when programmers are increasingly looking to concurrency in general, 
and multithreaded programming in particular, to improve application 
performance.

This book is about writing programs in C++ using multiple threads for 
concurrency and the C++ language features and library facilities that 
make that possible.

So, what do I mean by *concurrency* and *multithreading*?

##What is concurrency?

At the simplest and most basic level, concurrency is about two or more 
separate activities happening at the same time.

When we talk about concurrency in terms of computers, we mean a single 
system performing multiple independent activities in parallel, rather 
than sequentially, or one after the other.

It isn’t a new phenomenon: multitasking operating systems that allow
a single computer to run multiple applications at the same time through
task switching have been commonplace for many years, and high-end 
server machines with multiple processors that enable genuine 
concurrency have been available for even longer.

Whether machines have multiple processors or multiple cores within a 
processor (or both), these computers are capable of genuinely running 
more than one task in parallel. We call this **hardware concurrency**.

In order to do the interleaving, the system has to perform a **context 
switch** every time it changes from one task to another, and this takes
time. In order to perform a context switch, the OS has to save the CPU 
state and instruction pointer for the currently running task, work out
which task to switch to, and reload the CPU state for the task being 
switched to. The CPU will then potentially have to load the memory for 
the instructions and data for the new task into cache, which can 
prevent the CPU from executing any instructions, causing further delay.

Though the availability of concurrency in the hardware is most 
obvious with multiprocessor or multicore systems, some processors can 
execute multiple threads on a single core. The important factor to 
consider is really the number of **hardware threads**: the measure of 
how many independent tasks the hardware can genuinely run concurrently.

Even with a system that has genuine hardware concurrency, it’s easy to 
have more tasks than the hardware can run in parallel, so task 
switching is still used in these cases.

All the techniques, functions, and classes covered in this book can be 
used whether your application is running on a machine with one 
single-core processor or on a machine with many multicore processors 
and are not affected by whether the concurrency is achieved through 
task switching or by genuine hardware concurrency.

There are the two basic approaches to concurrency. The first approach 
is to have multiple single-threaded processes, which is similar to 
having each developer in their own office, and the second approach is 
to have multiple threads in a single process, which is like having two 
developers in the same office.

Let’s now have a brief look at these two approaches to concurrency in
an application.

##Concurrency With Mutiple Processes

The first way to make use of concurrency within an application is to 
divide the application into multiple, separate, single-threaded 
processes that are run at the same time, much as you can run your web 
browser and word processor at the same time. These separate processes 
can then pass messages to each other through all the normal 
inter-process communication channels (signals, sockets, files, pipes, 
and so on).

One downside is that such communication between processes is often
either complicated to set up or slow or both, because operating systems
typically provide a lot of protection between processes to avoid one 
process accidentally modifying data belonging to another process. 
Another downside is that there’s an inherent overhead in running
multiple processes: it takes time to start a process,  operating system
must devote internal resources to managing the process, and so forth.

Using separate processes for concurrency also has an additional 
advantage—you can run the separate processes on distinct machines 
connected over a network. Though this increases the communication 
cost, on a carefully designed system it can be a cost-effective way 
of increasing the available parallelism and improving performance.

##Concurrency With Mutiple Threads

The alternative approach to concurrency is to run multiple threads in 
a single process. Threads are much like lightweight processes: each 
thread runs independently of the others, and each thread may run a 
different sequence of instructions. But all threads in a process share
the same address space, and most of the data can be accessed directly 
from all threads—global variables remain global, and pointers or 
references to objects or data can be passed around among threads.

Having clarified what we mean by concurrency, let’s now look at why 
you would use concurrency in your applications.

##Why use concurrency?

There are two main reasons to use concurrency in an application: 
separation of concerns and performance.

##Using concurrency for separation of concerns

Separation of concerns is almost always a good idea when writing 
software; by grouping related bits of code together and keeping 
unrelated bits of code apart, you can make your programs easier to 
understand and test, and thus less likely to contain bugs.

Consider a processing-intensive application with a user interface, 
such as a DVD player application for a desktop computer. Such an 
application fundamentally has two sets of responsibilities: not only 
does it have to read the data from the disk, decode the images and 
sound, and send them to the graphics and sound hardware in a timely
fashion so the DVD plays without glitches, but it must also take input
from the user, such as when the user clicks Pause or Return To Menu, 
or even Quit.

In a single thread, the application has to check for user input at 
regular intervals during the playback, thus conflating the DVD 
playback code with the user interface code. 

By using multithreading to separate these concerns, the user interface
code and DVD playback code no longer have to be so closely intertwined
; one thread can handle the user interface and another the DVD 
playback. There will have to be interaction between them, such as 
when the user clicks Pause, but now these interactions are directly
related to the task at hand.

This gives the illusion of responsiveness, because the user interface 
thread can typically respond immediately to a user request, even if 
the response is simply to display a busy cursor or Please Wait message
while the request is conveyed to the thread doing the work.

Using threads in this way generally makes the logic in each thread
much simpler, because the interactions between them can be limited to 
clearly identifiable points, rather than having to intersperse the 
logic of the different tasks.

In this case, the number of threads is independent of the number of 
CPU cores available, because the division into threads is based on the
conceptual design rather than an attempt to increase throughput.

##Using concurrency for performance

There are two ways to use concurrency for performance. The first, and 
most obvious, is to divide a single task into parts and run each in 
parallel, thus reducing the total runtime. This is **task 
parallelism**. Although this sounds straightforward, it can be quite a
complex process, because there may be many dependencies between the 
various parts. The divisions may be either in terms of processing — 
one thread performs one part of the algorithm while another thread 
performs a different part — or in terms of data — each thread performs
the same operation on different parts of the data. This latter 
approach is called **data parallelism**.

The second way to use concurrency for performance is to use the 
available parallelism to solve bigger problems; rather than processing
one file at a time, process 2 or 10 or 20, as appropriate. Although 
this is really just an application of *data parallelism*, by 
performing the same operation on multiple sets of data concurrently, 
there’s a different focus. It still takes the same amount of time to 
process one chunk of data, but now more data can be processed in the 
same amount of time. Obviously, there are limits to this approach too,
and this won’t be beneficial in all cases, but the increase in 
throughput that comes from such an approach can actually make new 
things possible — increased resolution in video processing, for 
example, if different areas of the picture can be processed in 
parallel.

##When not to use concurrency

Also, the performance gain might not be as large as expected; there’s 
an inherent overhead associated with launching a thread, because the 
OS has to allocate the associated kernel resources and stack space and
then add the new thread to the scheduler, all of which takes time. If 
the task being run on the thread is completed quickly, the actual time
taken by the task may be dwarfed by the overhead of launching the 
thread, possibly making the overall performance of the application 
worse than if the task had been executed directly by the spawning 
thread.

Furthermore, threads are a limited resource. If you have too many 
threads running at once, this consumes OS resources and may make the 
system as a whole run slower. Not only that, but using too many 
threads can exhaust the available memory or address space for a 
process, because each thread requires a separate stack space. This
is particularly a problem for 32-bit processes with a flat 
architecture where there’s a 4 GB limit in the available address 
space: if each thread has a 1 MB stack (as is typical on many 
systems), then the address space would be all used up with 4096 
threads, without allowing for any space for code or static data or 
heap data. Although 64-bit (or larger) systems don’t have this direct 
address-space limit, they still have finite resources:
if you run too many threads, this will eventually cause problems. 
Though thread pools (see chapter 9) can be used to limit the number of
threads, these are not a silver bullet, and they do have their own 
issues.

If the server side of a client/server application launches a separate 
thread for each connection, this works fine for a small number of 
connections, but can quickly exhaust system resources by launching too
many threads if the same technique is used for a high-demand server 
that has to handle many connections. In this scenario, careful use of 
thread pools can provide optimal performance (see chapter 9).

One particularly important design that’s common to many C++ class 
libraries, and that provides considerable benefit to the programmer, 
has been the use of the *Resource Acquisition Is Initialization 
( RAII )* idiom with locks to ensure that mutexes are unlocked when 
the relevant scope is exited.

##Concurrency support in the new standard

All this changes with the release of the new C++11 Standard. Not only 
is there a brand-new thread-aware memory model, but the C++ Standard 
Library has been extended to include classes for managing threads 
(see chapter 2), protecting shared data (see chapter 3), synchronizing
operations between threads (see chapter 4), and low-level atomic 
operations (see chapter 5).

The support for atomic operations directly in C++ enables programmers 
to write efficient code with defined semantics without the need for 
platform-specific assembly language. This is a real boon for those 
trying to write efficient, portable code; not only does the compiler 
take care of the platform specifics, but the optimizer can be written
to take into account the semantics of the operations, thus enabling 
better optimization of the program as a whole.

##Platform-specific facilities

Although the C++ Thread Library provides reasonably comprehensive 
facilities for multithreading and concurrency, on any given platform 
there will be platform-specific facilities that go beyond what’s 
offered. In order to gain easy access to those facilities without 
giving up the benefits of using the Standard C++ Thread Library, the 
types in the C++ Thread Library may offer a `native_handle()` member 
function that allows the underlying implementation to be directly 
manipulated using a platform-specific API . By its very nature, any 
operations performed using the `native_handle()` are entirely platform
dependent and out of the scope of this book (and the Standard C++
Library itself).

Of course, before even considering using platform-specific facilities,
it’s important to understand what the Standard Library provides, so 
let’s get started with an example.

#Getting started

OK, so you have a nice, shiny C++11-compatible compiler. What next? 
What does a multithreaded C++ program look like? It looks pretty much 
like any other C++ program, with the usual mix of variables, classes, 
and functions. The only real distinction is that some functions might 
be running concurrently, so you need to ensure that shared data is 
safe for concurrent access, as described in chapter 3. Of course, in
order to run functions concurrently, specific functions and objects 
must be used to manage the different threads.

##Hello, Concurrent World

Let’s start with a classic example: a program to print “Hello World.” 
A really simple Hello, World program that runs in a single thread is 
shown here, to serve as a baseline

```cpp
#include <iostream>
int main()
{
	std::cout<<"Hello World\n";
}
```

All this program does is write “Hello World” to the standard output 
stream. Let’s compare it to the simple Hello, Concurrent World program
shown in the following listing, which starts a separate thread to 
display the message.

```cpp
#include <iostream>
#include <thread>

void hello()
{
    std::cout << "Hello Fast Proxy Concurrent World\n";
}

int main()
{
    std::thread t(hello);
    t.join(); 
}
```

The first difference is the extra `#include <thread>`. The 
declarations for the multithreading support in the Standard C++ 
Library are in new headers: the functions and classes for managing 
threads are declared in `<thread>`, whereas those for protecting
shared data are declared in other headers.

Second, the code for writing the message has been moved to a separate function. This is because every thread has to have an **initial 
function**, which is where the new thread of execution begins. For 
the initial thread in an application, this is `main()`, but for every 
other thread it’s specified in the constructor of a `std::thread` 
object — in this case, the `std::thread` object named `t` has the new 
function `hello()` as its initial function.

This is the next difference: rather than just writing directly to 
standard output or calling `hello()` from `main()`, this program 
launches a whole new thread to do it, bringing the thread count to 
two — the initial thread that starts at `main()` and the new thread 
that starts at `hello()`.

After the new thread has been launched, the initial thread 
**continues** execution. If it didn’t wait for the new thread to 
finish, it would merrily continue to the end of `main()` and thus end 
the program — possibly before the new thread had had a chance to run. 
This is why the call to `join()` is there — as described in chapter 2,
this causes the calling thread (in `main()` ) to wait for the thread 
associated with the `std::thread` object, in this case, `t`.

**My note** to compile it on Linux system run

```
g++ -o hello hello.cpp -std=c++11 -pthread
```

If you run `g++ -v` it will give you a bunch of information about how 
it was configured. One of those things will generally be a line that 
looks like

```
Thread model: posix
```

which means that it was configured to use pthreads for its threading 
library (`std::thread` in libstdc++), and which means you also need to
use any flags that might be required for pthreads on your system 
(`-pthread` on Linux).

After the taster example, it’s time for something with a bit more
substance. In chapter 2 we’ll look at the classes and functions 
available for managing threads.
