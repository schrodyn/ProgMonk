#Microsoft Windows

Windows operating systems can be easily divided into 2 categories: 
Win9x, and WinNT.

##Windows 9x

Operating Systems based on the 9x kernel are: Windows 95, Windows 98, 
and Windows ME. The actual OS itself was a 32 bit 
extension of MS-DOS, its predecessor. An important issue with the 9x 
line is that they were all based around using the ASCII format for 
storing strings, rather than Unicode.

Development on the Win9x kernel ended with the release of Windows ME.

##Windows NT

The WinNT kernel series was originally written as enterprise-level 
server and network software. WinNT stresses stability and security 
far more than Win9x kernels did. It also handles all string operations 
internally in Unicode, giving more flexibility when using different 
languages. Operating Systems based on the WinNT kernel are: Windows NT 
(versions 3.1, 3.5, 3.51 and 4.0), Windows 2000 (NT 5.0), Windows XP 
(NT 5.1), Windows Server 2003 (NT 5.2), Windows Vista (NT 6.0), 
Windows 7 (NT 6.1), Windows 8 (NT 6.2), Windows 8.1 (NT 6.3), and 
Windows 10 (NT 10.0).

The Microsoft XBOX and and XBOX 360 also run a variant of NT, forked 
from Windows 2000. 

##Virtual Memory

32 bit WinNT allows for a maximum of 4Gb of virtual memory space, 
divided into "pages" that are 4096 bytes by default.

##System Architecture

The Windows architecture is heavily layered. Function calls that a 
programmer makes may be redirected 3 times or more before any action is
actually performed. There is an unignorable penalty for calling Win32 
functions from a user-mode application. 

The Win32 API comprises 3 modules: KERNEL, USER, and GDI. 

KERNEL is layered on top of NTDLL, and most calls to KERNEL functions 
are simply redirected into NTDLL function calls. 

USER and GDI are both based on WIN32K (a kernel-mode module, 
responsible for the Windows "look and feel"), although USER also makes 
many calls to the more-primitive functions in GDI. 

##System calls and interrupts

After filtering through different layers of subroutines, most API calls
require interaction with part of the operating system.

Services are provided via 'software interrupts', traditionally through 
the `int 0x2e` instruction. This switches control of execution to the 
NT executive / kernel, where the request is handled. 

It should be pointed out here that the stack used in kernel mode is 
different from the user mode stack. This provides an added layer of 
protection between kernel and user. Once the function completes, 
control is returned back to the user application.

##Win32 API

Both WinNT and Win9x systems utilize the Win32 API. However, the WinNT 
version of the API has more functionality and security constructs, as 
well as Unicode support. Most of the Win32 API can be broken down into 
3 separate components, each performing a separate task.

###kernel32.dll

`Kernel32.dll`, home of the KERNEL subsystem, is where non-graphical 
functions are implemented. Some of the APIs located in KERNEL are: 
The Heap API, the Virtual Memory API, File I/O API, the Thread API, 
the System Object Manager, and other similar system services. Most of 
the functionality of `kernel32.dll` is implemented in `ntdll.dll`, but in undocumented functions.

Microsoft prefers to publish documentation for `kernel32` and guarantee
that these APIs will remain unchanged, and then put most of the work in
other libraries, which are then not documented.

###gdi32.dll

`gdi32.dll` is the library that implements the GDI (Graphics Device 
Interface) subsystem, where primitive graphical operations are 
performed. GDI diverts most of its calls into WIN32K, but it does 
contain a manager for GDI objects, such 
as pens, brushes and device contexts. 

The GDI object manager and the KERNEL object manager are completely 
separate.

###user32.dll

The USER subsystem is located in the `user32.dll` library file. This 
subsystem controls the creation and manipulation of USER objects, 
which are common screen items such as windows, menus, cursors, etc... 

USER will set up the objects to be drawn, but will perform the actual 
drawing by calling on GDI (which in turn will make many calls to 
WIN32K) or sometimes even calling WIN32K directly. USER utilizes the 
GDI Object Manager.

##Native API

The native API, hereby referred to as the NTDLL subsystem, is a 
series of undocumented API function calls that handle most of the work
performed by KERNEL. Microsoft also does not guarantee that the native
API will remain the same between different versions, as Windows 
developers modify the software. This gives the risk of native API 
calls being removed or changed without warning, breaking software that
utilizes it.

###ntdll.dll

The NTDLL subsystem is located in `ntdll.dll`. This library contains 
many API function calls, that all follow a particular naming scheme. 
Each function has a prefix: `Ldr`, `Nt`, `Zw`, `Csr`, `Dbg,` etc... 
and all the functions that have a particular prefix all follow 
particular rules.

The "official" native API is usually limited only to functions whose 
prefix is `Nt` or `Zw`.

In actual implementation, the system call stubs merely load two 
registers with values required to describe a native API call, and then
execute a software interrupt.

Most of the other prefixes are obscure, but the known ones are:

  * `Rtl` stands for "Run Time Library", calls which help 
  functionality at runtime (such as `RtlAllocateHeap`)

  * `Csr` is for "Client Server Runtime", which represents the 
  interface to the win32 subsystem located in `csrss.exe`

  * `Dbg` functions are present to enable debugging routines and 
  operations

  * `Ldr` provides the ability to load, manipulate and retrieve data 
  from DLLs and other module resources

##User Mode Versus Kernel Mode

Many functions, especially Run-time Library routines, are shared 
between `ntdll.dll` and `ntoskrnl.exe`. Most Native API functions, as 
well as other kernel-mode only functions exported from the kernel are 
useful for driver writers. As such, Microsoft provides documentation 
on many of the native API functions with the Microsoft Server 2003 
Platform DDK.

The DDK (Driver Development Kit) is available as a free download.

###ntoskrnl.exe

This module is the Windows NT "'Executive'", providing all the 
functionality required by the native API, as well as the kernel 
itself, which is responsible for maintaining the machine state. By 
default, all interrupts and kernel calls are channeled through 
`ntoskrnl` in some way, making it the single most important program in
Windows itself.

###Win32K.sys

This module is the "Win32 Kernel" that sits on top of the lower-level,
more primitive NTOSKRNL. WIN32K is responsible for the "look and feel"
of windows, and many portions of this code have remained largely 
unchanged since the Win9x versions.

This module provides many of the specific instructions that cause 
USER and GDI to act the way they do. It's responsible for translating 
the API calls from the USER and GDI libraries into the pictures you 
see on the monitor.

##Win64 API

With the advent of 64-bit processors, 64-bit software is a necessity. 
As a result, the Win64 API was created to utilize the new hardware. It
is important to note that the format of many of the function calls are
identical in Win32 and Win64, except for the size of pointers, and 
other data types that are specific to 64-bit address space.

#Linux

The **GNU/Linux operating system** is open source, but at the same 
time there is so much that constitutes "GNU/Linux".

##System Architecture

The concept of "GNU/Linux" is mostly a collection of a large number of
software components that are based off the GNU tools and the Linux 
kernel. Linux kernels are generally based off the philosophy that 
system configuration details should be stored in aptly-named, 
human-readable (and therefore human-editable) configuration files.

The Linux kernel implements much of the core API, but certainly not 
all of it. Much API code is stored in external modules (although users
have the option of compiling all these modules together into a 
"Monolithic Kernel").

On top of the kernel generally runs one or more **shells**. Bash is 
one of the more popular shells, but many users prefer other shells, 
especially for different tasks.

##File Analyzers

  * **strings**

  Finds printable strings in a file. When, for example, a password is 
  stored in the binary itself (defined statically in the source), the 
  string can then be extracted from the binary without ever needing to
  execute it.

  * **file**

  Determines a file type, useful for determining whether an executable
  has been stripped and whether it's been dynamically (or statically) 
  linked.

  * **objdump**

  Disassembles object files, executables and libraries. Can list 
  internal file structure and disassemble specific sections. Supports 
  both Intel and AT&T syntax

  * **nm** 

  Lists symbols from executable files. Doesn't work on stripped 
  binaries. Used mostly on debugging version of executables.

#Linux Executable Files

##ELF Files

The **ELF file format** (short for Executable and Linking Format) was 
developed by Unix System Laboratories to be a successor to previous 
file formats such as COFF and a.out.

The ELF format has widely become the standard on Linux, Solaris, IRIX,
and FreeBSD (although the FreeBSD-derived Mac OS X uses the Mach-O 
format instead).

Historically, Linux has not always used ELF; Red Hat Linux 4 was the 
first time that distribution used ELF; previous versions had used the 
a.out format.

ELF Objects are broken down into different **segments** and/or 
**sections**. These can be located by using the **ELF header** found 
at the first byte of the object.

The ELF header provides the location for both the **program header** 
and the **section header**. Using these data structures the rest of 
the ELF objects contents can be found, this includes `.text` and 
`.data` segments which contain code and data respectively.

The GNU `readelf` utility, from the binutils package, is a common tool
for parsing ELF objects.

##File Format

Each ELF file is made up of one ELF header, followed by file data. 
The file data can include:

  * Program header table, describing zero or more segments
  * Section header table, describing zero or more sections
  * Data referred to by entries in the program or section header 
  table

[here image]

An ELF file has two views: the program header shows the *segments* 
used at run-time, while the section header lists the set of *sections*
of the binary.

The **segments** contain information that is necessary for *runtime 
execution* of the file, while **sections** contain important data for 
*linking* and *relocation*. Each byte in the entire file is taken by 
**no more than one** section at a time, but there can be orphan bytes,
which are not covered by a section.

##Relocatable ELF Files

Relocatable ELF files are created by compilers. They need to be linked
before running.

Those files are often found in `.a` archives with a `.o` extension.




