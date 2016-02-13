#The UNIX System Interface

The UNIX operating system provides its services through a set of 
*system calls*, which are in effect functions within the operating 
system that may be called by user programs.

This chapter describes how to use some of the most important system 
calls from C programs.

#File Descriptors

In the UNIX operating system, all input and output is done by reading 
or writing files, because all peripheral devices, even keyboard and 
screen, are files in the file system. 

In the most general case, before you read and write a file, you must 
inform the system of your intent to do so, a process called *opening* 
the file. If you are going to write on a file it may also be necessary 
to create it or to discard its previous contents. The system checks 
your right to do so (Does the file exist? Do you have permission to 
access it?) and if all is well, returns to the program a small 
non-negative integer called a *file descriptor*. Whenever input or 
output is to be done on the file, the file descriptor is used instead 
of the name to identify the file.

When the command interpreter (the "shell") runs a program, three files 
are open, with file descriptors 0, 1, and 2, called the standard input,
the standard output, and the standard error. If a program reads 0 and 
writes 1 and 2, it can do input and output without worrying about 
opening files.

The user of a program can redirect I/O to and from files with `<` and 
`>`:

```c
prog <infile >outfile
```

#Low Level I/O - Read and Write

Input and output uses the `read` and `write` system calls, which are 
accessed from C programs through two functions called `read` and 
`write`. For both, the first argument is a *file descriptor*. The 
second argument is a *character array* in your program where the data 
is to go to or to come from. The third argument is the number is the 
*number of bytes* to be transferred.

```c
int n_read = read(int fd, char *buf, int n);
int n_written = write(int fd, char *buf, int n);
```

Each call returns a count of the number of bytes transferred.

On reading, the number of bytes returned may be less than the number 
requested. A return value of *zero* bytes implies end of file, and 
`-1` indicates an error of some sort.

For writing, the return value is the number of bytes written.

Any number of bytes can be read or written in one call. The most common
values are 1, which means one character at a time ("unbuffered"), and a
number like `1024` or `4096` that corresponds to a physical block size 
on a peripheral device. Larger sizes will be more efficient because 
fewer system calls will be made.

Putting these facts together, we can write a simple program to copy its
input to its output

```c
#include <unistd.h>

main()  /* copy input to output */
{
	char buf[BUFSIZ];
	int n;

	while ((n = read(0, buf, BUFSIZ)) > 0)
		write(1, buf, n);
	return 0;
}
```

If the file size is not a multiple of `BUFSIZ`, some `read` will return
a smaller number of bytes to be written by `write`; the next call to 
`read` after that will return zero.

It is instructive to see how `read` and `write` can be used to 
construct higher-level routines like `getchar`, `putchar`, etc. For 
example, here is a version of `getchar` that does unbuffered input, by 
reading the standard input one character at a time.

```c
#include <unistd.h>

/* getchar:  unbuffered single character input */
int getchar(void)
{
	char c;

	return (read(0, &c, 1) == 1) ? (unsigned char) c : EOF;
}
```

`c` must be a `char`, because `read` needs a character pointer. 
Casting `c` to `unsigned char` in the return statement eliminates any 
problem of sign extension.

The second version of `getchar` does input in big chunks, and hands 
out the characters one at a time.

```c
#include <unistd.h>

/* getchar:  simple buffered version */
int getchar(void)
{
	static char buf[BUFSIZ];
	static char *bufp = buf;
	static int n = 0;

	if (n == 0) {  /* buffer is empty */
		n = read(0, buf, sizeof buf);
		bufp = buf;
	}
	return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}
```

If these versions of `getchar` were to be compiled with `<stdio.h>` 
included, it would be necessary to `#undef` the name `getchar` in case
it is implemented as a macro.

#Open, Creat, Close, Unlink

Other than the default standard input, output and error, you must 
explicitly open files in order to read or write them. There are two 
system calls for this, `open` and `creat`.

`open` is rather like the `fopen`, except that instead of returning a 
file pointer, it returns a file descriptor, which is just an `int`.
`open` returns `-1` if any error occurs.

```c
#include <fcntl.h>

int fd;
int open(char *name, int flags, int perms);

fd = open(name, flags, perms);
```

As with `fopen`, the `name` argument is a character string containing 
the filename. The second argument, `flags`, is an `int` that specifies
how the file is to be opened; the main values are

`O_RDONLY` -- open for reading only

`O_WRONLY` -- open for writing only

`O_RDWR` -- open for both reading and writing

These constants are defined in `<fcntl.h>` on System V UNIX systems, 
and in `<sys/file.h>` on Berkeley (BSD) versions.

To open an existing file for reading,

```c
fd = open(name, O_RDONLY,0);
```

It is an error to try to open a file that does not exist. The system 
call `creat` is provided to create new files, or to re-write old ones.

```c
int creat(char *name, int perms);

fd = creat(name, perms);
```

returns a file descriptor if it was able to create the file, and `-1` 
if not. If the file already exists, `creat` will truncate it to zero 
length, thereby discarding its previous contents; it is not an error 
to `creat` a file that already exists.

If the file does not already exist, `creat` creates it with the 
permissions specified by the perms argument.

The function `close(int fd)` breaks the connection between a file 
descriptor and an open file, and frees the file descriptor for use 
with some other file; it corresponds to `fclose` in the standard 
library except that there is no buffer to flush. Termination of a 
program via `exit` or `return` from the main program closes all open 
files.

The function `unlink(char *name)` removes the file name from the file 
system. It corresponds to the standard library function `remove`.

#Random Access - Lseek

Input and output are normally sequential: each `read` or `write` takes
place at a position in the file right after the previous one. When 
necessary, however, a file can be read or written in any arbitrary 
order. The system call `lseek` provides a way to move around in a file
without reading or writing any data:

```c
long lseek(int fd, long offset, int origin);
```

sets the current position in the file whose descriptor is `fd` to 
`offset`, which is taken relative to the location specified by 
`origin`. 

`origin` can be 0, 1, or 2 to specify that `offset` is to be measured 
from the beginning, from the current position, or from the end of the 
file respectively.

With `lseek`, it is possible to treat files more or less like arrays, 
at the price of slower access. For example, the following function 
reads any number of bytes from any arbitrary place in a file. It 
returns the number read, or `-1` on error.

```c
#include <unistd.h>

/*get:  read n bytes from position pos */
int get(int fd, long pos, char *buf, int n)
{
	if (lseek(fd, pos, 0) >= 0) /* get to pos */
		return read(fd, buf, n);
	else
		return -1;
}
```

The return value from `lseek` is a long that gives the new position in
the file, or `-1` if an error occurs. The standard library function 
`fseek` is similar to `lseek` except that the first argument is a 
`FILE *` and the return is non-zero if an error occurred.



