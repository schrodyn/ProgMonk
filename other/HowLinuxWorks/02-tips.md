Bourne Shell: `/bin/sh`
A shell is a program that runs commands.

Linux uses an enhanced version of the Bourne shell called bash or
the “Bourne-again” shell.

`chsh` -- change login shell

`cat` -- outputs the contents of one or more files
cat file1 file2 ...

The command is called cat because it performs concatenation when it
prints the contents of more than one file.

Unix processes use I/O streams to read and write data.
Processes read data from input streams and write data to output streams.

Pressing `CTRL-D` on an empty line stops the current standard input entry from the terminal (and often terminates a program).

Don’t confuse this with `CTRL-C`, which terminates a program
regardless of its input or output.

`rm -rf dir` -- The -r option specifies recursive delete to repeatedly
delete everything inside dir, and -f forces the delete operation.

##Shell Globbing (Wildcards)

`echo *` -- prints a list of files in the current directory

Another shell glob character, the question mark (?), instructs the shell to match
exactly one arbitrary character.

If you don’t want the shell to expand a glob in a command, enclose the glob in
single quotes ('').

Two of the most important grep options are -i (for case-insensitive matches)
and -v (which inverts the search, that is, prints all lines that don’t match).

When running less, you’ll see the contents of the file one screenful at a time.
Press the spacebar to go forward in the file and the b key to skip back one
screenful. To quit, type q.

to search forward for a word, type /word, and to search backward, use ?word.
When you find a match, press n to continue searching.

symbolic links can sometimes obscure the true
full path of the current working directory. You’ll use pwd -P to eliminate this
confusion.

`diff file1 file2` -- differences between two text files

the shell expands globs before running commands.

Most systems also have a locate command for finding files. Rather than searching for a file in real time, locate searches an index that the system builds
periodically.

`head /etc/passwd` -- shows the first 10 lines of the password file

`head -5 /etc/passwd` -- 5 is the number of lines you want to see

`tail /etc/passwd` -- shows the last 10 lines.

To print lines starting at line n, use `tail +n`.

The sort command quickly puts the lines of a text file in alphanumeric order.

If the file’s lines start with numbers and you want to sort in numerical order,
use the -n option.

The -r option reverses the order of the sort.

The shell can store temporary variables, called shell variables, containing the
values of text strings.

`STUFF=blahann` -- assign a value to a shell variable

An environment variable is like a shell variable, but it’s not specific to the
shell.

All processes on Unix systems have environment variable storage.

The main difference between environment and shell variables is that the
operating system passes all of your shell’s environment variables to programs t
hat the shell runs,
whereas shell variables cannot be accessed in the commands that you run.

Assign an environment variable with the shell’s export command:
```
STUFF=blah
export STUFF
```

Keystroke | Action
----------|-------
CTRL-B	 |   Move the cursor left
CTRL-F	 |   Move the cursor right
CTRL-P	 |   View the previous command (or move the cursor up)
CTRL-N	 |   View the next command (or move the cursor down)
CTRL-A	 |   Move the cursor to the beginning of the line
CTRL-E	 |   Move the cursor to the end of the line

To search for a manual page by keyword, use the -k option:
`man -k` keyword

manual pages have different sections for other purposes:

Section  |	Description
---------|--------------
1	|	User commands
2	|	System calls
3	|	Higher-level Unix programming library documentation
4	|	Device interface and driver information
5	|	File descriptions (system configuration files)
6	|	Games
7	|	File formats, conventions, and encodings (ASCII, suffixes, and so on)
8	|	System commands and servers

You can select a manual page by section (by default, man displays the first manual
page that it finds when matching a particular search term):

For example, to read the `/etc/passwd` file description:

```
man 5 passwd (as opposed to the passwd command)
```

To access an info manual, use info with the command name:
info command

To send the output of command to a file instead of the terminal,
use the > redirection character:
```
command > file
```

The shell creates file if it does not already exist. If file exists, the shell
erases (clobbers) the original file first.

You can append the output to the file instead of overwriting it with
the `>>` redirection syntax:

```
command >> file
```

```
head /proc/cpuinfo -- CPU info :)
```

standard error (stderr); it’s an additional output stream for diagnostics and debugging.

You can redirect the standard error if you like.
For example, to send standard output to f and standard error to e,
use the 2> syntax, like this:

```
ls /fffffffff > f 2> e
```

The number 2 specifies the stream ID that the shell modifies.
Stream ID 1 is standard output (the default), and 2 is standard error.

You can also send the standard error to the same place as stdout
with the >& notation. For example, to send
both standard output and standard error to the file named f, try this command:

```
ls /fffffffff > f 2>&1
```

To channel a file to a program’s standard input, use the < operator:

```
head < /proc/cpuinfo
```

A process is a running program. Each process on the system has a numeric process
ID (PID).

For a quick listing of running processes, just run ps on the command line:
ps

PID -- The process ID

TTY -- The terminal device where the process is running

STAT -- The process status, that is, what the process is doing and where its memory resides. For example, S means sleeping and R means running.

TIME -- The amount of CPU time in minutes and seconds that the process has used so far.

ps x -- Show all of your running processes.

ps ax -- Show all processes on the system, not just the ones you own.

ps u -- Include more detailed information on processes.

ps w -- Show full command names, not just what fits on one line.

To check on a specific process, add its PID to the argument list of the
ps command.

to inspect the current shell process,
you could use ps u `$$`, because
`$$` is a shell variable that evaluates to the current shell’s PID.

you can detach a process from the shell and put it in the “background” with the
ampersand (`&`)

```
gunzip file.gz &
```

CTRL-L -- redraw the entire screen.

Use the groups command to see what group you’re in.

Some executable files have an s in the user permissions listing instead of an x.
This indicates that theexecutable is setuid, meaning that when you execute the
program, it runs as though the file owner is the user instead of you.
Many programs use this setuid bit to run as root in order to get the privileges
they need to change system files.
One example is the passwd program, which needs to change the /etc/passwd file.

Directories also have permissions.

You can list the contents of a directory if it’s readable,
but you can only access a file in a directory if the directory is executable.

Symbolic links are
simply names that point to other names.

To create a symbolic link:
```
ln -s target linkname
```

```
gunzip file.gz -- uncompress <file>.gz
gzip file -- to compress it again
```

To create an archive, use tar instead:
```
tar cvf archive.tar file1 file2 ...
```

The `c` flag activates create mode.
The `f` flag denotes the file option.

To use standard input or output, enter a dash (`-`) instead of the
filename.

To unpack a `.tar` file:
```
tar xvf archive.tar
```
The `x` flag puts tar into extract (unpack) mode.

You can extract individual parts of the archive by entering the names of
the parts at the end of the command line.

To check the contents of a .tar file with the table-of-contents mode
by using the t flag instead of the x flag.

When unpacking, consider using the `p` option to preserve permissions.

use the following to verify a compressed archive:

```
tar ztvf file.tar.gz
```

A `.tgz` file is the same as a `.tar.gz` file. The suffix is meant to fit into FAT
(MS-DOS-based) filesystems.

#Linux Directory Hierarchy Essentials

`/bin` -- Contains ready-to-run programs (also known as an executables), including most of the basic Unix commands such as ls and cp.

Most of the programs in /bin are in binary format, having been created by a
C compiler, but some are shell scripts in modern systems.

`/dev` -- Contains device files.

`/etc` -- This core system configuration directory
`/etc/X11` -- contains graphics card and window system configurations.

`/home` -- Holds personal directories for regular users
`/lib` -- An abbreviation for library, this directory holds library files containing
code that executables can use.

There are two types of libraries: static and shared.

The `/lib` directory should contain only shared libraries.
but other lib directories, such as `/usr/lib`, contain both varieties as well as
other auxiliary files.

`/proc` -- Provides system statistics through a browsable directory-and-file
interface.

The `/proc` directory contains information about currently running processes as well
 as some kernel parameters.

`/sys` -- This directory is similar to /proc in that it provides a device and system
 interface.

`/sbin` -- The place for system executables.

`/tmp` -- A storage area for smaller, temporary files that you don’t care much about
. Any user may read to and write from /tmp, but the user may not have permission
to access another user’s files there.

If something is extremely important, don’t put it in /tmp because most
distributions clear /tmp when the machine boots and some even remove its old files
 periodically.

`/usr` -- it contains a large directory hierarchy; Many of the directory names in
`/usr` are the same as those in the root directory (like `/usr/bin` and `/usr/lib`), and
 they hold the same type of files.

`/var` -- The variable subdirectory, where programs record runtime information.
System logging, user tracking, caches, and other files that system programs create
 and manage are here.

`/boot` -- Contains kernel boot loader files.
These files pertain only to the very first stage of the Linux startup procedure.

`/media` -- A base attachment point for removable media such as flash drives that is found in many distributions.

`/opt` -- This may contain additional third-party software.

`/usr/include` -- Holds header files used by the C compiler.
`/usr/local` -- Is where administrators can install their own software.
`/usr/share` -- Contains files that should work on other kinds of Unix machines with
 no loss of functionality.

In the past, networks of machines would share this directory, but a true `/share`
directory is becoming rare because there are no space issues on modern disks.

On Linux systems, the kernel is normally in `/vmlinuz` or `/boot/vmlinuz`.

A boot loader loads this file into
memory and sets it in motion when the system boots.

`/lib/modules` -- loadable kernel modules

`/etc/sudoers` -- the system doesn’t let just any user run commands as the superuser; you must configure the privileged users in your /etc/sudoers file.

Use the `visudo` command to edit `/etc/sudoers`. This command checks for file syntax
errors after you save the file.
