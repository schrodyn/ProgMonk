#Filesystem Basics

##Interlude: File and Directories

A persistent-storage device, such as a classic **hard disk drive** or a more modern **solid-state storage device**, stores information permanently (or at least, for a long time). Unlike memory, whose contents are lost when there is a power loss, a persistent-storage device
keeps such data intact. Thus, the OS must take extra care with such a
device: this is where users keep data that they really care about.

**How To Manage A Persistent Device**

How should the OS manage a persistent device? What are the APIs?
What are the important aspects of the implementation?

Thus, in the next few chapters, we will explore critical techniques for
managing persistent data, focusing on methods to improve performance
and reliability. We begin, however, with an overview of the API: the interfaces you’ll expect to see when interacting with a UNIX file system.

##Files and Directories

Two key abstractions have developed over time in the virtualization
of storage. The first is the **file**. A file is simply a linear array of bytes,
each of which you can read or write. Each file has some kind of **low-level name**, usually a number of some kind; often, the user is not aware of this name (as we will see). For historical reasons, the low-level name of a file is often referred to as its **inode number**. We’ll be learning a lot more
about inodes in future chapters; for now, just assume that each file has an
inode number associated with it.

In most systems, the OS does not know much about the structure of
the file (e.g., whether it is a picture, or a text file, or C code); rather, the
responsibility of the file system is simply to store such data persistently
on disk and make sure that when you request the data again, you get
what you put there in the first place. Doing so is not as simple as it seems!

The second abstraction is that of a **directory**. A directory, like a file,
also has a low-level name (i.e., an inode number), but its contents are
quite specific: it contains a list of (user-readable name, low-level name)
pairs. For example, let’s say there is a file with the low-level name “10”,
and it is referred to by the user-readable name of “foo”. The directory
“foo” resides in thus would have an entry (“foo”, “10”) that maps the
user-readable name to the low-level name. Each entry in a directory refers
to either files or other directories. By placing directories within other directories, users are able to build an arbitrary **directory tree** (or **directory
hierarchy**), under which all files and directories are stored.

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_1.png)

*An Example Directory Tree*

The directory hierarchy starts at a **root directory** (in UNIX -based systems, the root directory is simply referred to as `/`) and uses some kind of **separator** to name subsequent **sub-directories** until the desired file or
directory is named. For example, if a user created a directory `foo` in the
root directory `/`, and then created a file `bar.txt` in the directory `foo`,
we could refer to the file by its **absolute pathname**, which in this case
would be `/foo/bar.txt`. See picture above for a more complex directory
tree; valid directories in the example are `/`, `/foo`, `/bar`, `/bar/bar`,
`/bar/foo` and valid files are `/foo/bar.txt` and `/bar/foo/bar.txt`.
Directories and files can have the same name as long as they are in different locations in the file-system tree (e.g., there are two files named
`bar.txt` in the figure, `/foo/bar.txt` and `/bar/foo/bar.txt`).

You may also notice that the file name in this example often has two
parts: `bar` and `txt`, separated by a period. The first part is an arbitrary
name, whereas the second part of the file name is usually used to indicate the **type** of the file, e.g., whether it is C code (e.g., `.c`), or an image
(e.g., `.jpg`), or a music file (e.g., `.mp3`). However, this is usually just a
**convention**: there is usually no enforcement that the data contained in a
file named `main.c` is indeed C source code.

Thus, we can see one great thing provided by the file system: a conve-
nient way to **name** all the files we are interested in. Names are important
in systems as the first step to accessing any resource is being able to name
it. In UNIX systems, the file system thus provides a unified way to access
files on disk, USB stick, CD-ROM, many other devices, and in fact many
other things, all located under the single directory tree.

##The File System Interface

Let’s now discuss the file system interface in more detail. We’ll start
with the basics of creating, accessing, and deleting files. You may think
this is straightforward, but along the way we’ll discover the mysterious
call that is used to remove files, known as `unlink()`. Hopefully, by the
end of this chapter, this mystery won’t be so mysterious to you!

##Creating Files

We’ll start with the most basic of operations: creating a file. This can be
accomplished with the `open` system call; by calling `open()` and passing
it the `O_CREAT` flag, a program can create a new file. Here is some example code
to create a file called “foo” in the current working directory.

```c
int fd = open("foo", O_CREAT | O_WRONLY | O_TRUNC);
```

The routine open() takes a number of different flags. In this example, the program creates the file (`O_CREAT`), can only write to that file while opened in this manner (`O_WRONLY`), and, if the file already exists, first truncate it to a size of zero bytes thus removing any existing content (`O_TRUNC`).

###ASIDE : The `CREAT()` SYSTEM CALL

The older way of creating a file is to call `creat()`, as follows:

```c
int fd = creat("foo");
```

You can think of `creat()` as `open()` with the following flags:
`O_CREAT | O_WRONLY | O_TRUNC`. Because `open()` can create a file,
the usage of `creat()` has somewhat fallen out of favor (indeed, it could
just be implemented as a library call to `open()`); however, it does hold a
special place in UNIX lore. Specifically, when Ken Thompson was asked
what he would do differently if he were redesigning UNIX , he replied:
“I’d spell creat with an e.”


One important aspect of `open()` is what it returns: a **file descriptor**. A
file descriptor is just an integer, private per process, and is used in U NIX
systems to access files; thus, once a file is opened, you use the file descriptor to read or write the file, assuming you have permission to do so.
In this way, a file descriptor is a **capability**, i.e., an opaque handle
that gives you the power to perform certain operations. Another way to
think of a file descriptor is as a pointer to an object of type file; once you
have such an object, you can call other “methods” to access the file, like
`read()` and `write()`. We’ll see just how a file descriptor is used below.

##Reading and Writing Files

Once we have some files, of course we might like to read or write them.
Let’s start by reading an existing file. If we were typing at a command
line, we might just use the program `cat` to dump the contents of the file
to the screen.

```
prompt> echo hello > foo
prompt> cat foo
hello
prompt>
```

In this code snippet, we redirect the output of the program `echo` to
the file `foo`, which then contains the word “hello” in it. We then use `cat`
to see the contents of the file. But how does the `cat` program access the
file `foo`?

To find this out, we’ll use an incredibly useful tool to trace the system
calls made by a program. On Linux, the tool is called **strace**; other systems have similar tools (see **dtruss** on Mac OS X, or **truss** on some older UNIX variants). What `strace` does is trace every system call made by a program while it runs, and dump the trace to the screen for you to see.

###TIP : Use STRACE (AND SIMILAR TOOLS)

The `strace` tool provides an awesome way to see what programs are up
to. By running it, you can trace which system calls a program makes, see
the arguments and return codes, and generally get a very good idea of
what is going on.

The tool also takes some arguments which can be quite useful. For example, `-f` follows any fork’d children too; `-t` reports the time of day at each call; `-e trace=open,close,read,write` only traces calls to
those system calls and ignores all others. There are many more powerful
flags — read the man pages and find out how to harness this wonderful tool.

Here is an example of using `strace` to figure out what cat is doing
(some calls removed for readability):

```
prompt> strace cat foo
...
open("foo", O_RDONLY|O_LARGEFILE)       = 3
read(3, "hello\n", 4096)                = 6
write(1, "hello\n", 6)                  = 6
hello
read(3, "", 4096)                       = 0
close(3)                                = 0
...
prompt>
```

The first thing that `cat` does is open the file for reading. A couple
of things we should note about this; first, that the file is only opened for
reading (not writing), as indicated by the `O_RDONLY` flag; second, that
the 64-bit offset be used (`O_LARGEFILE`); third, that the call to `open()`
succeeds and returns a file descriptor, which has the value of 3.

Why does the first call to `open()` return 3, not 0 or perhaps 1 as you
might expect? As it turns out, each running process already has three
files open, standard input (which the process can read to receive input),
standard output (which the process can write to in order to dump information to the screen), and standard error (which the process can write
error messages to). These are represented by file descriptors 0, 1, and 2,
respectively. Thus, when you first open another file (as `cat` does above),
it will almost certainly be file descriptor 3.

After the open succeeds, `cat` uses the `read()` system call to repeatedly read some bytes from a file.
The first argument to `read()` is the file
descriptor, thus telling the file system which file to read; a process can of
course have multiple files open at once, and thus the descriptor enables
the operating system to know which file a particular read refers to. The
second argument points to a buffer where the result of the `read()` will be
placed; in the system-call trace above, strace shows the results of the read
in this spot (“hello”). The third argument is the size of the buffer, which
in this case is 4 KB. The call to `read()` returns successfully as well, here
returning the number of bytes it read (6, which includes 5 for the letters
in the word “hello” and one for an end-of-line marker).

At this point, you see another interesting result of the strace: a single
call to the `write()` system call, to the file descriptor 1. As we mentioned
above, this descriptor is known as the standard output, and thus is used
to write the word “hello” to the screen as the program `cat` is meant to
do. But does it call `write()` directly? Maybe (if it is highly optimized).
But if not, what `cat` might do is call the library routine `printf()`; internally, `printf()` figures out all the formatting details passed to it, and
eventually calls write on the standard output to print the results to the
screen.

The `cat` program then tries to read more from the file, but since there
are no bytes left in the file, the `read()` returns 0 and the program knows
that this means it has read the entire file. Thus, the program calls `close()`
to indicate that it is done with the file “foo”, passing in the corresponding
file descriptor. The file is thus closed, and the reading of it thus complete.

Writing a file is accomplished via a similar set of steps. First, a file
is opened for writing, then the `write()` system call is called, perhaps
repeatedly for larger files, and then `close()`. Use `strace` to trace writes
to a file, perhaps of a program you wrote yourself, or by tracing the `dd`
utility, e.g., `dd if=foo of=bar`.

##Reading And Writing, But Not Sequentially

Thus far, we’ve discussed how to read and write files, but all access
has been **sequential**; that is, we have either read a file from the beginning
to the end, or written a file out from beginning to end.

Sometimes, however, it is useful to be able to read or write to a specific offset within a file; for example, if you build an index over a text
document, and use it to look up a specific word, you may end up reading
from some **random** offsets within the document. To do so, we will use
the `lseek()` system call. Here is the function prototype:

```c
off_t lseek(int fildes, off_t offset, int whence);
```

The first argument is familiar (a file descriptor). The second argument is the `offset`, which positions the **file offset** to a particular location
within the file. The third argument, called `whence` for historical reasons,
determines exactly how the seek is performed. From the man page:

```
If whence is SEEK_SET, the offset is set to offset bytes.
If whence is SEEK_CUR, the offset is set to its current
location plus offset bytes.
If whence is SEEK_END, the offset is set to the size of
the file plus offset bytes.
```

As you can tell from this description, for each file a process opens, the OS tracks a “current” offset, which determines where the next read or write will begin reading from or writing to within the file. Thus, part of the abstraction of an open file is that it has a current offset, which
is updated in one of two ways. The first is when a read or write of N
bytes takes place, N is added to the current offset; thus each read or write
*implicitly* updates the offset. The second is *explicitly* with `lseek`, which
changes the offset as specified above.

Note that this call `lseek()` has nothing to do with the **seek** operation
of a disk, which moves the disk arm. The call to `lseek()` simply changes
the value of a variable within the kernel; when the I/O is performed,
depending on where the disk head is, the disk may or may not perform
an actual seek to fulfill the request.

###ASIDE : CALLING lseek() Does NOT Perform A DISK SEEK

The poorly-named system call `lseek()` confuses many a student trying to understand disks and
how the file systems atop them work. Do not confuse the two!
The `lseek()` call simply changes a variable in OS memory that tracks, for a particular process, at which offset to which its
next read or write will start. A disk seek occurs when a read or write
issued to the disk is not on the same track as the last read or write, and
thus necessitates a head movement. Making this even more confusing is
the fact that calling `lseek()` to read or write from/to random parts of a
file, and then reading/writing to those random parts, will indeed lead to
more disk seeks. Thus, calling `lseek()` can certainly lead to a seek in an
upcoming read or write, but absolutely does not cause any disk I/O to
occur itself.

##Writing Immediately with `fsync()`

Most times when a program calls `write()`, it is just telling the file
system: please write this data to persistent storage, at some point in the
future. The file system, for performance reasons, will **buffer** such writes
in memory for some time (say 5 seconds, or 30); at that later point in
time, the write(s) will actually be issued to the storage device. From the
perspective of the calling application, writes seem to complete quickly,
and only in rare cases (e.g., the machine crashes after the `write()` call
but before the write to disk) will data be lost.

However, some applications require something more than this eventual guarantee.
For example, in a database management system (DBMS),
development of a correct recovery protocol requires the ability to force
writes to disk from time to time.

To support these types of applications, most file systems provide some
additional control APIs. In the UNIX world, the interface provided to ap-
plications is known as `fsync(int fd)`. When a process calls `fsync()`
for a particular file descriptor, the file system responds by forcing all **dirty**
(i.e., not yet written) data to disk, for the file referred to by the specified
file descriptor. The `fsync()` routine returns once all of these writes are
complete.
Here is a simple example of how to use `fsync()`. The code opens the file `foo`, writes a single chunk of data to it, and then calls `fsync()` to ensure the writes are forced immediately to disk. Once the `fsync()`
returns, the application can safely move on, knowing that the data has
been persisted (if `fsync()` is correctly implemented, that is).

```c
int fd = open("foo", O_CREAT | O_WRONLY | O_TRUNC);
assert(fd > -1);
int rc = write(fd, buffer, size);
assert(rc == size);
rc = fsync(fd);
assert(rc == 0);
```

Interestingly, this sequence does not guarantee everything that you
might expect; in some cases, you also need to `fsync()` the directory that
contains the file `foo`. Adding this step ensures not only that the file itself
is on disk, but that the file, if newly created, also is durably a part of the
directory. Not surprisingly, this type of detail is often overlooked, leading
to many application-level bugs.

##Renaming Files

Once we have a file, it is sometimes useful to be able to give a file a
different name. When typing at the command line, this is accomplished
with `mv` command; in this example, the file `foo` is renamed `bar`:

```
prompt> mv foo bar
```

Using strace, we can see that mv uses the system call `rename(char *old, char *new)`, which takes precisely two arguments: the original name of the file (`old`) and the new name (`new`).

One interesting guarantee provided by the `rename()` call is that it is (usually) implemented as an **atomic** call with respect to system crashes; if the system crashes during the renaming, the file will either be named the old name or the new name, and no odd in-between state can arise.
Thus, `rename()` is critical for supporting certain kinds of applications that require an atomic update to file state.

Let’s be a little more specific here. Imagine that you are using a file editor (e.g., emacs), and you insert a line into the middle of a file. The file’s name, for the example, is `foo.txt`. The way the editor might update the file to guarantee that the new file has the original contents plus the line
inserted is as follows (ignoring error-checking for simplicity):

```c
int fd = open("foo.txt.tmp", O_WRONLY|O_CREAT|O_TRUNC);
write(fd, buffer, size); // write out new version of file
fsync(fd);
close(fd);
rename("foo.txt.tmp", "foo.txt");
```

What the editor does in this example is simple: write out the new version of the file under a temporary name (`foot.txt.tmp`), force it to disk with `fsync()`, and then, when the application is certain the new
file metadata and contents are on the disk, rename the temporary file to the original file’s name. This last step atomically swaps the new file into place, while concurrently deleting the old version of the file, and thus an atomic file update is achieved.

##Getting Information About Files

Beyond file access, we expect the file system to keep a fair amount of information about each file it is storing. We generally call such data about files **metadata**. To see the metadata for a certain file, we can use the `stat()` or `fstat()` system calls. These calls take a pathname (or file descriptor) to a file and fill in a `stat` structure as seen here:

```c
struct stat {
    dev_t       st_dev;     /* ID of device containing file */
    ino_t       st_ino;     /* inode number */
    mode_t      st_mode;    /* protection */
    nlink_t     st_nlink;   /* number of hard links */
    uid_t       st_uid;     /* user ID of owner */
    gid_t       st_gid;     /* group ID of owner */
    dev_t       st_rdev;    /* device ID (if special file) */
    off_t       st_size;    /* total size, in bytes */
    blksize_t   st_blksize; /* blocksize for filesystem I/O */
    blkcnt_t    st_blocks;  /* number of blocks allocated */
    time_t      st_atime;   /* time of last access */
    time_t      st_mtime;   /* time of last modification */
    time_t      st_ctime;   /* time of last status change */*
};
```

You can see that there is a lot of information kept about each file, including its size (in bytes), its low-level name (i.e., inode number), some ownership information, and some information about when the file was accessed or modified, among other things. To see this information, you can use the command line tool `stat`:

```
prompt> echo hello > file
prompt> stat file
    File: ‘file’
    Size: 6 Blocks: 8         IO Block: 4096    regular file
  Device: 811h/2065d Inode: 67158084    Links: 1
  Access: (0640/-rw-r-----) Uid: (30686/ remzi) Gid: (30686/ remzi)
  Access: 2011-05-03 15:50:20.157594748 -0500
  Modify: 2011-05-03 15:50:20.157594748 -0500
  Change: 2011-05-03 15:50:20.157594748 -0500
```

As it turns out, each file system usually keeps this type of information
in a structure called an **inode** (Some file systems call these structures similar, but slightly different, names, such as dnodes; the basic idea is similar however). We’ll be learning a lot more about inodes when we talk about file system implementation. For now, you should just think of an inode as a persistent data structure kept by the file system that has information like we see above inside of it.

##Removing Files

At this point, we know how to create files and access them, either sequentially or not. But how do you delete files? If you’ve used UNIX , you probably think you know: just run the program `rm`. But what system call does `rm` use to remove a file?

Let’s use our old friend `strace` again to find out. Here we remove that pesky file “foo”:

```
prompt> strace rm foo
...
unlink("foo")               = 0
...
```

We’ve removed a bunch of unrelated cruft from the traced output, leaving just a single call to the mysteriously-named system call `unlink()`. As you can see, `unlink()` just takes the name of the file to be removed, and returns zero upon success. But this leads us to a great puzzle: why
is this system call named “unlink”? Why not just “remove” or “delete”. To understand the answer to this puzzle, we must first understand more than just files, but also directories.

##Making Directories

Beyond files, a set of directory-related system calls enable you to make,
read, and delete directories. Note you can never write to a directory di-
rectly; because the format of the directory is considered file system meta-
data, you can only update a directory indirectly by, for example, creating
files, directories, or other object types within it. In this way, the file system
makes sure that the contents of the directory always are as expected.

To create a directory, a single system call, `mkdir()`, is available. The
eponymous `mkdir` program can be used to create such a directory. Let’s
take a look at what happens when we run the `mkdir` program to make a
simple directory called `foo`:

```
prompt> strace mkdir foo
...
mkdir("foo", 0777)          = 0
...
prompt>
```

When such a directory is created, it is considered “empty”, although it
does have a bare minimum of contents. Specifically, an empty directory
has two entries: one entry that refers to itself, and one entry that refers
to its parent. The former is referred to as the “.” (dot) directory, and the
latter as “..” (dot-dot). You can see these directories by passing a flag (`-a`)
to the program `ls`:

```
prompt> ls -a
./ ../
prompt> ls -al
total 8
drwxr-x--- 2 remzi remzi     6 Apr 30 16:17 ./
drwxr-x--- 26 remzi remzi 4096 Apr 30 16:17 ../
```

##Reading Directories

Now that we’ve created a directory, we might wish to read one too.
Indeed, that is exactly what the program `ls` does. Let’s write our own
little tool like `ls` and see how it is done.

Instead of just opening a directory as if it were a file, we instead use
a new set of calls. Below is an example program that prints the contents
of a directory. The program uses three calls, `opendir()`, `readdir()`,
and `closedir()`, to get the job done, and you can see how simple the
interface is; we just use a simple loop to read one directory entry at a time,
and print out the name and inode number of each file in the directory.

```c
int main(int argc, char *argv[]) {
    DIR *dp = opendir(".");
    assert(dp != NULL);
    struct dirent *d;
    while ((d = readdir(dp)) != NULL) {
        printf("%d %s\n", (int) d->d_ino, d->d_name);
    }
    closedir(dp);
    return 0;
}
```

The declaration below shows the information available within each
directory entry in the `struct dirent` data structure:

```c
struct dirent {
    char            d_name[256];    /* filename */
    ino_t           d_ino;          /* inode number */
    off_t           d_off;          /* offset to the next dirent */
    unsigned short  d_reclen;       /* length of this record */
    unsigned char   d_type;         /* type of file */
};
```

Because directories are light on information (basically, just mapping the name to the inode number, along with a few other details), a program may want to call `stat()` on each file to get more information on each,
such as its length or other detailed information. Indeed, this is exactly
what `ls` does when you pass it the `-l` flag; try `strace` on `ls` with and
without that flag to see for yourself.

##Deleting Directories

Finally, you can delete a directory with a call to `rmdir()` (which is used by the program of the same name, `rmdir`). Unlike file deletion, however, removing directories is more dangerous, as you could potentially delete a large amount of data with a single command. Thus, `rmdir()` has the requirement that the directory be empty (i.e., only has “.” and “..” entries) before it is deleted. If you try to delete a non-empty directory, the call to `rmdir()` simply will fail.

##Hard Links

We now come back to the mystery of why removing a file is performed via `unlink()`, by understanding a new way to make an entry in the file system tree, through a system call known as `link()`. The `link()`
system call takes two arguments, an old pathname and a new one; when you “link” a new file name to an old one, you essentially create another way to refer to the same file. The command-line program `ln` is used to
do this, as we see in this example:

```
prompt> echo hello > file
prompt> cat file
hello
prompt> ln file file2
prompt> cat file2
hello
```

Here we created a file with the word “hello” in it, and called the file
`file`. We then create a hard link to that file using the `ln` program. After
this, we can examine the file by either opening `file` or `file2`.

The way `link` works is that it simply creates another name in the directory you are creating the link to, and refers it to the *same* inode number (i.e., low-level name) of the original file. The file is not copied in any way; rather, you now just have two human names (`file` and `file2`) that both
refer to the same file. We can even see this in the directory itself, by printing out the inode number of each file:

```
prompt> ls -i file file2
67158084 file
67158084 file2
prompt>
```

By passing the `-i` flag to `ls`, it prints out the inode number of each file
(as well as the file name). And thus you can see what link really has done:
just make a new reference to the same exact inode number (67158084 in
this example).

By now you might be starting to see why `unlink()` is called `unlink()`. When you create a file, you are really doing *two* things. First, you are making a structure (the inode) that will track virtually all relevant information about the file, including its size, where its blocks are on disk, and so forth. Second, you are *linking* a human-readable name to that file, and putting that link into a directory.

After creating a hard link to a file, to the file system, there is no dif-
ference between the original file name (`file`) and the newly created file
name (`file2`); indeed, they are both just links to the underlying meta-
data about the file, which is found in inode number 67158084.

Thus, to remove a file from the file system, we call `unlink()`. In the
example above, we could for example remove the file named `file`, and
still access the file without difficulty:

```
prompt> rm file
removed ‘file’
prompt> cat file2
hello
```

The reason this works is because when the file system unlinks file, it checks a **reference count** within the inode number. This reference count (sometimes called the **link count**) allows the file system to track how many different file names have been linked to this particular inode. When `unlink()` is called, it removes the “link” between the human-readable name (the file that is being deleted) to the given inode number, and decrements the reference count; only when the reference count reaches zero
does the file system also free the inode and related data blocks, and thus truly “delete” the file.

You can see the reference count of a file using `stat()` of course. Let’s see what it is when we create and delete hard links to a file. In this example, we’ll create three links to the same file, and then delete them. Watch the link count!

```
prompt> echo hello > file
prompt> stat file
... Inode: 67158084     Links: 1 ...
prompt> ln file file2
prompt> stat file
... Inode: 67158084     Links: 2 ...
prompt> stat file2
... Inode: 67158084     Links: 2 ...
prompt> ln file2 file3
prompt> stat file
... Inode: 67158084     Links: 3 ...
prompt> rm file
prompt> stat file2
... Inode: 67158084     Links: 2 ...
prompt> rm file2
prompt> stat file3
... Inode: 67158084     Links: 1 ...
prompt> rm file3
```

##Symbolic Links

There is one other type of link that is really useful, and it is called a **symbolic link** or sometimes a **soft link**. As it turns out, hard links are somewhat limited: you can’t create one to a directory (for fear that you will create a cycle in the directory tree); you can’t hard link to files in other disk partitions (because inode numbers are only unique within a particular file system, not across file systems); etc. Thus, a new type of link called the symbolic link was created.

To create such a link, you can use the same program `ln`, but with the `-s` flag. Here is an example:

```
prompt> echo hello > file
prompt> ln -s file file2
prompt> cat file2
hello
```

As you can see, creating a soft link looks much the same, and the original file can now be accessed through the file name `file` as well as the symbolic link name `file2`.

However, beyond this surface similarity, symbolic links are actually
quite different from hard links. The first difference is that a symbolic
link is actually a file itself, of a different type. We’ve already talked about
regular files and directories; symbolic links are a third type the file system
knows about. A `stat` on the symlink reveals all:

```
prompt> stat file
... regular file ...
prompt> stat file2
... symbolic link ...
```

Running `ls` also reveals this fact. If you look closely at the first character of the long-form of the output from `ls`, you can see that the first character in the left-most column is a `-` for regular files, a `d` for directories, and an `l` for soft links. You can also see the size of the symbolic link
(4 bytes in this case), as well as what the link points to (the file named `file`).

```
prompt> ls -al
drwxr-x--- 2 remzi remzi    29 May  3   19:10   ./
drwxr-x--- 27 remzi remzi 4096 May  3   15:14   ../
-rw-r----- 1 remzi remzi     6 May  3   19:10   file
lrwxrwxrwx 1 remzi remzi     4 May  3   19:10   file2 -> file
```

The reason that `file2` is 4 bytes is because the way a symbolic link is formed is by holding the pathname of the linked-to file as the data of the link file. Because we’ve linked to a file named `file`, our link file `file2` is small (4 bytes). If we link to a longer pathname, our link file would be bigger:

```
prompt> echo hello > alongerfilename
prompt> ln -s alongerfilename file3
prompt> ls -al alongerfilename file3
-rw-r----- 1 remzi remzi 6 May 3 19:17 alongerfilename
lrwxrwxrwx 1 remzi remzi 15 May 3 19:17 file3 -> alongerfilename
```

Finally, because of the way symbolic links are created, they leave the
possibility for what is known as a **dangling reference**:

```
prompt> echo hello > file
prompt> ln -s file file2
prompt> cat file2
hello
prompt> rm file
prompt> cat file2
cat: file2: No such file or directory
```

As you can see in this example, quite unlike hard links, removing the original file named `file` causes the link to point to a pathname that no longer exists.

##Making and Mounting a File System

We’ve now toured the basic interfaces to access files, directories, and certain types of special types of links. But there is one more topic we should discuss: how to assemble a full directory tree from many underlying file systems. This task is accomplished via first making file systems, and then mounting them to make their contents accessible.

To make a file system, most file systems provide a tool, usually referred to as `mkfs` (pronounced “make fs”), that performs exactly this task. The idea is as follows: give the tool, as input, a device (such as a disk partition, e.g., `/dev/sda1`) a file system type (e.g., ext3), and it simply
writes an empty file system, starting with a root directory, onto that disk
partition. And mkfs said, let there be a file system!

However, once such a file system is created, it needs to be made accessible within the uniform file-system tree. This task is achieved via the `mount` program (which makes the underlying system call `mount()` to do the real work). What mount does, quite simply is take an existing directory as a target **mount point** and essentially paste a new file system onto
the directory tree at that point.

An example here might be useful. Imagine we have an unmounted ext3 file system, stored in device partition `/dev/sda1`, that has the following contents: a root directory which contains two sub-directories, a
and b, each of which in turn holds a single file named `foo`. Let’s say we wish to mount this file system at the mount point `/home/users`. We would type something like this:

```
prompt> mount -t ext3 /dev/sda1 /home/users
```

If successful, the mount would thus make this new file system available. However, note how the new file system is now accessed. To look at the contents of the root directory, we would use `ls` like this:

```
prompt> ls /home/users/
a b
```

As you can see, the pathname `/home/users/` now refers to the root of the newly-mounted directory. Similarly, we could access files `a` and `b` with the pathnames `/home/users/a` and `/home/users/b`. Finally, the files named `foo` could be accessed via `/home/users/a/foo` and `/home/users/b/foo`. And thus the beauty of mount: instead of having a number of separate file systems, mount unifies all file systems into one tree, making naming uniform and convenient.

To see what is mounted on your system, and at which points, simply
run the `mount` program. You’ll see something like this:

```
/dev/sda1 on / type ext3 (rw)
proc on /proc type proc (rw)
sysfs on /sys type sysfs (rw)
/dev/sda5 on /tmp type ext3 (rw)
/dev/sda7 on /var/vice/cache type ext3 (rw)
tmpfs on /dev/shm type tmpfs (rw)
AFS on /afs type afs (rw)
```

This crazy mix shows that a whole number of different file systems,
including ext3 (a standard disk-based file system), the proc file system (a
file system for accessing information about current processes), tmpfs (a
file system just for temporary files), and AFS (a distributed file system)
are all glued together onto this one machine’s file-system tree.


#The Virtual Filesystem

The _Virtual Filesystem_ (commonly simply _VFS_) is the subsystem of the kernel that implements
the file and filesystem-related interfaces provided to user-space programs. All filesystems rely on the _VFS_ to enable them not only to coexist, but also to interoperate. This enables programs to use standard _Unix system calls_ to read and write to different filesystems, even on different media, as
shown in picture

![](/dedupfs/images/filesystem_guide_vfs_1.png)

*The VFS in action: Using the cp(1) utility to move
data from a hard disk mounted as ext3 to a removable disk mounted
as ext2. Two different filesystems, two different media, one VFS.*

For example, let's assume you want to copy data from a hard disk mounted as _ext3_ to a removable
disk mounted as _ext2_. Without _VFS_ programs ought to know all specifics of filesystems implementations. It's not good, as you may notice. To illuminate this issue _VFS_ was created. Think it like a NAT that forward all requests (system calls) to specific hosts ( FS file operations).

##Common Filesystem Interface

The VFS is the glue that enables system calls such as `open()` , `read()` , and `write()` to
work regardless of the filesystem or underlying physical medium.These days, that might
not sound novel—we have long been taking such a feature for granted—but it is a non-trivial feat for such generic system calls to work across many diverse filesystems and varying media. More so, the system calls work *between* these different filesystems and media — we can use standard system calls to copy or move files from one filesystem to another. In older operating systems, such as DOS, this would never have worked; any access to a non-native filesystem required special tools. It is only because modern operating systems, such as Linux, abstract access to the filesystems via a virtual interface that such interoperation
and generic access is possible.

New filesystems and new varieties of storage media can find their way into Linux, and
programs need not be rewritten or even recompiled. In this chapter, we will discuss the
VFS, which provides the abstraction allowing myriad filesystems to behave as one. In the
next chapter, we will discuss the block I/O layer, which allows various storage devices—
CD to Blu-ray discs to hard drives to CompactFlash.Together, the VFS and the block I/O
layer provide the abstractions, interfaces, and glue that allow user-space programs to issue
generic system calls to access files via a uniform naming policy on any filesystem, which
itself exists on any storage medium.

##Filesystem Abstraction Layer

Such a generic interface for any type of filesystem is feasible only because the kernel
implements an abstraction layer around its low-level filesystem interface.This abstraction
layer enables Linux to support different filesystems, even if they differ in supported features or behavior.This is possible because the VFS provides a common file model that can
represent any filesystem’s general feature set and behavior. Of course, it is biased toward
Unix-style filesystems. (You see what constitutes a Unix-style filesystem later in this chapter.) Regardless, wildly differing filesystem types are still supportable in Linux, from
DOS’s FAT to Windows’s NTFS to many Unix-style and Linux-specific filesystems.

The abstraction layer works by defining the basic conceptual interfaces and data structures that all filesystems support.The filesystems mold their view of concepts such as *“this
is how I open files”* and *“this is what a directory is to me”* to match the expectations of the
VFS.The actual filesystem code hides the implementation details.To the VFS layer and the
rest of the kernel, however, each filesystem looks the same.They all support notions such
as files and directories, and they all support operations such as creating and deleting files.

The result is a general abstraction layer that enables the kernel to support many types
of filesystems easily and cleanly.The filesystems are programmed to provide the abstracted
interfaces and data structures the VFS expects; in turn, the kernel easily works with any
filesystem and the exported user-space interface seamlessly works on any filesystem.

In fact, nothing in the kernel needs to understand the underlying details of the filesystems, except the filesystems themselves. For example, consider a simple user-space program that does

```c
ret = write (fd, buf, len);
```

This system call writes the `len` bytes pointed to by `buf` into the current position in the
file represented by the file descriptor `fd` .This system call is first handled by a generic
`sys_write()` system call that determines the actual file writing method for the filesystem
on which `fd` resides.The generic write system call then invokes this method, which is part
of the filesystem implementation, to write the data to the media (or whatever this filesystem does on write). Figure below shows the flow from user-space’s `write()` call through
the data arriving on the physical media. On one side of the system call is the genericVFS
interface, providing the frontend to user-space; on the other side of the system call is the
filesystem-specific backend, dealing with the implementation details.The rest of this chapter looks at how the VFS achieves this abstraction and provides its interfaces.

![](/dedupfs/images/filesystem_guide_vfs_2.png)

The flow of data from user-space issuing a `write()` call, through the
VFS’s generic system call, into the filesystem’s specific write method, and finally
arriving at the physical media.

##Unix Filesystems

Historically, Unix has provided four basic filesystem-related abstractions: files, directory
entries, inodes, and mount points.

A *filesystem* is a hierarchical storage of data adhering to a specific structure. Filesystems
contain files, directories, and associated control information.Typical operations performed
on filesystems are creation, deletion, and mounting. In Unix, filesystems are mounted at a
specific mount point in a global hierarchy known as a *namespace*. Recently, Linux has made this hierarchy per-process, to give a unique namespace to each process. Because each process inherits its parent’s namespace (unless you specify otherwise), there is seemingly one global namespace.

 This enables all mounted filesystems to appear as entries in a single tree. Contrast this single, unified tree with the behavior of DOS and Windows, which break the file namespace up into drive
letters, such as `C:`.This breaks the namespace up among device and partition boundaries,
“leaking” hardware details into the filesystem abstraction.As this delineation may be arbitrary and even confusing to the user, it is inferior to Linux’s unified namespace.

A *file* is an ordered string of bytes.The first byte marks the beginning of the file, and
the last byte marks the end of the file. Each file is assigned a human-readable name for
identification by both the system and the user.Typical file operations are read, write,
create, and delete.The Unix concept of the file is in stark contrast to record-oriented
filesystems, such as OpenVMS’s Files-11. Record-oriented filesystems provide a richer,
more structured representation of files than Unix’s simple byte-stream abstraction, at the
cost of simplicity and flexibility.

Files are organized in directories.A *directory* is analogous to a folder and usually contains related files. Directories can also contain other directories, called subdirectories. In
this fashion, directories may be nested to form paths. Each component of a path is called a
*directory entry*.A path example is `/home/wolfman/butter` —the root directory `/` , the directories `home` and `wolfman` , and the file `butter` are all directory entries, called *dentries*. In
Unix, directories are actually normal files that simply list the files contained therein.
Because a directory is a file to the VFS, the same operations performed on files can be
performed on directories.

Unix systems separate the concept of a file from any associated information about it,
such as access permissions, size, owner, creation time, and so on.This information is some-
times called *file metadata* (that is, data about the file’s data) and is stored in a separate data
structure from the file, called the *inode*.This name is short for *index node*, although these
days the term *inode* is much more ubiquitous.

All this information is tied together with the filesystem’s own control information,
which is stored in the *superblock*.The superblock is a data structure containing information
about the filesystem as a whole. Sometimes the collective data is referred to as *filesystem
metadata*. Filesystem metadata includes information about both the individual files and the
filesystem as a whole.

Traditionally, Unix filesystems implement these notions as part of their physical on-disk layout. For example, file information is stored as an inode in a separate block on the
disk; directories are files; control information is stored centrally in a superblock, and so
on.The Unix file concepts are *physically mapped* on to the storage medium.The Linux
VFS is designed to work with filesystems that understand and implement such concepts.
Non-Unix filesystems, such as FAT or NTFS, still work in Linux, but their filesystem
code must provide the appearance of these concepts. For example, even if a filesystem
does not support distinct inodes, it must assemble the inode data structure in memory as
if it did. Or if a filesystem treats directories as a special object, to the VFS they must repre-
sent directories as mere files. Often, this involves some special processing done on-the-fly
by the non-Unix filesystems to cope with the Unix paradigm and the requirements of
the VFS. Such filesystems still work, however, and the overhead is not unreasonable.

##VFS Objects and Their Data Structures

The VFS is object-oriented. People often miss this, or even deny it, but there are many examples of object-oriented programming in the kernel. Although the kernel developers may shun C++ and other explicitly object-oriented languages, thinking in terms of objects is often useful. The VFS is a good example of how to do clean and efficient OOP in C, which is a language that lacks any OOP constructs.
 A family of data structures represents the common file model.These data structures are akin to objects. Because the kernel is programmed strictly in C, without the benefit of a language directly supporting object-oriented paradigms, the data structures are represented as C structures.The structures contain both data and pointers to filesystem-implemented functions that operate on the data.

The four primary object types of the VFS are

  * The *superblock* object, which represents a specific mounted filesystem.

  * The *inode* object, which represents a specific file.

  * The *dentry* object, which represents a directory entry, which is a single component
      of a path.

  * The *file* object, which represents an open file as associated with a process.

Note that because the VFS treats directories as normal files, there is not a specific
directory object. Recall from earlier in this chapter that a dentry represents a component
in a path, which might include a regular file. In other words, a dentry is not the same as a
directory, but a directory is just another kind of file. Got it?

An *operations* object is contained within each of these primary objects.These objects
describe the methods that the kernel invokes against the primary objects:

  * The `super_operations` object, which contains the methods that the kernel can
    invoke on a specific filesystem, such as `write_inode()` and `sync_fs()`

  * The `inode_operations` object, which contains the methods that the kernel can
    invoke on a specific file, such as `create()` and `link()`

  * The `dentry_operations` object, which contains the methods that the kernel can
    invoke on a specific directory entry, such as `d_compare()` and `d_delete()`

  * The `file_operations` object, which contains the methods that a process can
    invoke on an open file, such as `read()` and `write()`

The operations objects are implemented as a structure of pointers to functions that
operate on the parent object. For many methods, the objects can inherit a generic function if basic functionality is sufficient. Otherwise, the specific instance of the particular
filesystem fills in the pointers with its own filesystem-specific methods.

Again, note that *objects* refer to structures—not explicit class types, such as those in
C++ or Java.These structures, however, represent specific instances of an object, their
associated data, and methods to operate on themselves.They are very much objects.

The VFS loves structures, and it is comprised of a couple more than the primary
objects previously discussed. Each registered filesystem is represented by a
`file_system_type` structure.This object describes the filesystem and its capabilities. Fur-
thermore, each mount point is represented by the `vfsmount` structure.This structure con-
tains information about the mount point, such as its location and mount flags.

Finally, two per-process structures describe the filesystem and files associated with a
process.They are, respectively, the `fs_struct` structure and the `file` structure.

The rest of this chapter discusses these objects and the role they play in implementing
the VFS layer.

##The Superblock Object

The superblock object is implemented by each filesystem and is used to
store information describing that specific filesystem. This object
usually corresponds to the *filesystem superblock* or the *filesystem
control block*, which is stored in a special sector on disk (hence the
object’s name). Filesystems that are not disk-based (a virtual
memory–based filesystem, such as *sysfs*, for example) generate the
superblock on-the-fly and store it in memory.

The superblock object is represented by struct `super_block` and defined
in `<linux/fs.h>`. Here is what it looks like, with comments describing
each entry:

```c
struct super_block {
	struct list_head       s_list;				/* list of all superblocks */
	dev_t                  s_dev;				/* identifier */
	unsigned long          s_blocksize; 		/* block size in bytes */
	unsigned char          s_blocksize_bits; 	/* block size in bits */
	unsigned char          s_dirt; 			/* dirty flag */
	unsigned long long     s_maxbytes; 		/* max file size */
	struct file_system_type s_type; 			/* filesystem type */
	struct super_operations s_op; 				/* superblock methods */
	struct dquot_operations *dq_op; 			/* quota methods */
	struct quotactl_ops 	*s_qcop; 			/* quota control methods */
	struct export_operations *s_export_op; 		/* export methods */
	unsigned long          s_flags; 			/* mount flags */
	unsigned long          s_magic; 			/* filesystem’s magic number */
	struct dentry          *s_root; 			/* directory mount point */
	struct rw_semaphore    s_umount; 			/* unmount semaphore */
	struct semaphore       s_lock; 			/* superblock semaphore */
	int                    s_count; 			/* superblock ref count */
	int                    s_need_sync; 		/* not-yet-synced flag */
	atomic_t               s_active; 			/* active reference count */
	void                   *s_security; 		/* security module */
	struct xattr_handler   **s_xattr; 			/* extended attribute handlers */
	struct list_head       s_inodes;			/* list of inodes */
	struct list_head       s_dirty;			/* list of dirty inodes */
	struct list_head       s_io;				/* list of writebacks */
	struct list_head       s_more_io;			/* list of more writeback */
	struct hlist_head      s_anon;				/* anonymous dentries */
	struct list_head       s_files;			/* list of assigned files */
	struct list_head       s_dentry_lru;		/* list of unused dentries */
	int                    s_nr_dentry_unused; /* number of dentries on list */
	struct block_device    *s_bdev;			/* associated block device */
	struct mtd_info        *s_mtd;				/* memory disk information */
	struct list_head       s_instances;		/* instances of this fs */
	struct quota_info      s_dquot;			/* quota-specific options */
	int                    s_frozen;			/* frozen status */
	wait_queue_head_t      s_wait_unfrozen; 	/* wait queue on freeze */
	char                   s_id[32];			/* text name */
	void                   *s_fs_info;			/* filesystem-specific info */
	fmode_t                s_mode;				/* mount permissions */
	struct semaphore       s_vfs_rename_sem; 	/* rename semaphore */
	u32                    s_time_gran;		/* granularity of timestamps */
	char                   *s_subtype;			/* subtype name */
	char                   *s_options;			/* saved mount options */

};
```

The code for creating, managing, and destroying superblock objects lives
in `fs/super.c`. A superblock object is created and initialized via the
`alloc_super()` function. When mounted, a filesystem invokes this
function, reads its superblock off of the disk, and fills in its
superblock object.

### Superblock Operations

The most important item in the superblock object is `s_op`, which is a
pointer to the superblock operations table. The superblock operations
table is represented by `struct super_operations` and is defined in
`<linux/fs.h>`. It looks like this:

```c
struct super_operations {
	struct inode *(*alloc_inode)(struct super_block *sb);
	void (*destroy_inode)(struct inode *);
	void (*dirty_inode) (struct inode *);
	int (*write_inode) (struct inode *, int);
	void (*drop_inode) (struct inode *);
	void (*delete_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	void (*write_super) (struct super_block *);
	int (*sync_fs)(struct super_block *sb, int wait);
	int (*freeze_fs) (struct super_block *);
	int (*unfreeze_fs) (struct super_block *);
	int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	void (*clear_inode) (struct inode *);
	void (*umount_begin) (struct super_block *);
	int (*show_options)(struct seq_file *, struct vfsmount *);
	int (*show_stats)(struct seq_file *, struct vfsmount *);
	ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);
	ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);
	int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
};
```
 
Each item in this structure is a pointer to a function that operates on
a superblock object. The superblock operations perform low-level
operations on the filesystem and its inodes.

When a filesystem needs to perform an operation on its superblock, it
follows the pointers from its superblock object to the desired method.
For example, if a filesystem wanted to write to its superblock, it would
invoke

```c
sb->s_op->write_super(sb);
```

In this call, `sb` is a pointer to the filesystem’s superblock.
Following that pointer into `s_op` yields the superblock operations
table and ultimately the desired `write_super()` function, which is then
invoked. Note how the `write_super()` call must be passed a superblock,
despite the method being associated with one. This is because of the
lack of object-oriented support in C. In C++, a call such as the
following would suffice:

```c
sb.write_super();
```

In C, there is no way for the method to easily obtain its parent, so you
have to pass it.

Let’s take a look at some of the superblock operations that are
specified by `super_operations`:

  * `struct inode * alloc_inode(struct super_block *sb)`
    Creates and initializes a new inode object under the given superblock.

  * `void destroy_inode(struct inode *inode)`
    Deallocates the given inode.

  * `void dirty_inode(struct inode *inode)`
  Invoked by the VFS when an inode is dirtied (modified). Journaling filesystems such
  as ext3 and ext4 use this function to perform journal updates.

  * `void write_inode(struct inode *inode, int wait)`
  Writes the given inode to disk.The wait parameter specifies whether the operation
  should be synchronous.

  * `void drop_inode(struct inode *inode)`
    Called by the VFS when the last reference to an inode is dropped. Normal Unix
    filesystems do not define this function, in which case the VFS simply deletes the
    inode.

  * `void delete_inode(struct inode *inode)`
  Deletes the given inode from the disk.

  * `void put_super(struct super_block *sb)`
  Called by the VFS on unmount to release the given superblock object.The caller
  must hold the s_lock lock.

  * `void write_super(struct super_block *sb)`
  Updates the on-disk superblock with the specified superblock.The VFS uses this
  function to synchronize a modified in-memory superblock with the disk.The caller
  must hold the s_lock lock.

  * `int sync_fs(struct super_block *sb, int wait)`
  Synchronizes filesystem metadata with the on-disk filesystem.The wait parameter
  specifies whether the operation is synchronous.

  * `void write_super_lockfs(struct super_block *sb)`
  Prevents changes to the filesystem, and then updates the on-disk superblock with
  the specified superblock. It is currently used by LVM (the LogicalVolume Manager).

  * `void unlockfs(struct super_block *sb)`
  Unlocks the filesystem against changes as done by write_super_lockfs() .

  * `int statfs(struct super_block *sb, struct statfs *statfs)`
  Called by the VFS to obtain filesystem statistics.The statistics related to the given
  filesystem are placed in statfs .

  * `int remount_fs(struct super_block *sb, int *flags, char *data)`
  Called by the VFS when the filesystem is remounted with new mount options.The
  caller must hold the s_lock lock.

  * `void clear_inode(struct inode *inode)`
  Called by the VFS to release the inode and clear any pages containing related data.

  * `void umount_begin(struct super_block *sb)`
  Called by the VFS to interrupt a mount operation. It is used by network filesystems,
  such as NFS.

All these functions are invoked by the VFS, in process context. All
except `dirty_inode()` may all block if needed.

Some of these functions are optional; a specific filesystem can then set
its value in the superblock operations structure to `NULL`. If the
associated pointer is `NULL`, the VFS either calls a generic function or
does nothing, depending on the operation.

## The Inode Object

The inode object represents all the information needed by the kernel to
manipulate a file or directory. For Unix-style filesystems, this
information is simply read from the on-disk inode. If a filesystem does
not have inodes, however, the filesystem must obtain the information
from wherever it is stored on the disk. Filesystems without inodes
generally store file-specific information as part of the file; unlike
Unix-style filesystems, they do not separate file data from its control
information. Some modern filesystems do neither and store file metadata
as part of an on-disk database. Whatever the case, the inode object is
constructed in memory in whatever manner is applicable to the
filesystem.
 
The inode object is represented by `struct inode` and is defined in
`<linux/fs.h>`. Here is the structure, with comments describing each
entry:

```c
struct inode {                                            
   struct hlist_node          i_hash;                    /* hash list */
   struct list_head           i_list;                    /* list of inodes */
   struct list_head           i_sb_list;                /* list of superblocks */
   struct list_head           i_dentry;                  /* list of dentries */
   unsigned long               i_ino;                     /* inode number */
   atomic_t                   i_count;                   /* reference counter */
   unsigned int                i_nlink;                   /* number of hard links */
   uid_t                      i_uid;                     /* user id of owner */
   gid_t                      i_gid;                     /* group id of owner */
   kdev_t                     i_rdev;                    /* real device node */
   u64                         i_version;                 /* versioning number */
   loff_t                     i_size;                    /* file size in bytes */
   seqcount_t                 i_size_seqcount;          /* serializer for i_size */
   struct timespec             i_atime;                   /* last access time */
   struct timespec             i_mtime;                   /* last modify time */
   struct timespec             i_ctime;                   /* last change time */
   unsigned int                i_blkbits;                 /* block size in bits */
   blkcnt_t                   i_blocks;                  /* file size in blocks */
   unsigned short              i_bytes;                   /* bytes consumed */
   umode_t                    i_mode;                    /* access permissions */
   spinlock_t                 i_lock;                    /* spinlock */
   struct rw_semaphore        i_alloc_sem;              /* nests inside of i_sem */
   struct semaphore            i_sem;                     /* inode semaphore */
   struct inode_operations    *i_op;                    /* inode ops table */
   struct file_operations     *i_fop;                   /* default inode ops */
   struct super_block         *i_sb;                    /* associated superblock */
   struct file_lock           *i_flock;                 /* file lock list */
   struct address_space       *i_mapping;               /* associated mapping */
   struct address_space       i_data;                    /* mapping for device */
   struct dquot                *i_dquot[MAXQUOTAS];    /* disk quotas for inode */
   struct list_head           i_devices;                 /* list of block devices */
   union {
     struct pipe_inode_info *i_pipe;						/* pipe information */   
   struct block_device      *i_bdev;                  /* block device driver */         
   struct cdev              *i_cdev;                  /* character device driver */     
   };                                                                                                                                                    
   unsigned long   			  i_dnotify_mask;           /* directory notify mask */       
   struct dnotify_struct      *i_dnotify;               /* dnotify */                     
   struct list_head           inotify_watches;          /* inotify watches */             
  struct mutex               inotify_mutex;             /* protects inotify_watches */   
   unsigned long             i_state;                   /* state flags */                 
   unsigned long             dirtied_when;              /* first dirtying time */         
   unsigned int              i_flags;                   /* filesystem flags */            
   atomic_t                  i_writecount;              /* count of writers */            
   void                      *i_security;              /* security module */             
   void                      *i_private;               /* fs private pointer */    
};
```

An inode represents each file on a filesystem, but the inode object is
constructed in memory only as files are accessed. This includes special
files, such as device files or pipes. Consequently, some of the entries
in `struct inode` are related to these special files. For example, the
`i_pipe` entry points to a named pipe data structure, `i_bdev` points to
a block device structure, and `i_cdev` points to a character device
structure. These three pointers are stored in a union because a given
inode can represent only one of these (or none of them) at a time.

It might occur that a given filesystem does not support a property
represented in the inode object. For example, some filesystems might not
record an access timestamp. In that case, the filesystem is free to
implement the feature however it sees fit; it can store zero for
`i_atime`, make `i_atime` equal to `i_mtime`, update `i_atime` in memory
but never flush it back to disk, or whatever else the filesystem
implementer decides.

## Inode Operations

As with the superblock operations, the `inode_operations` member is
important. It describes the filesystem’s implemented functions that the
VFS can invoke on an inode. As with the superblock, inode operations are
invoked via

```c
i->i_op->truncate(i)
```
 
In this call, `i` is a reference to a particular inode. In this case,
the `truncate()` operation defined by the filesystem on which `i` exists
is called on the given inode. The `inode_operations` structure is
defined in `<linux/fs.h>`:

```c
struct inode_operations {
    int (*create) (struct inode *,struct dentry *,int, struct nameidata *);
    struct dentry * (*lookup) (struct inode *,struct dentry *, struct nameidata *);
    int (*link) (struct dentry *,struct inode *,struct dentry *);
    int (*unlink) (struct inode *,struct dentry *);
    int (*symlink) (struct inode *,struct dentry *,const char *);
    int (*mkdir) (struct inode *,struct dentry *,int);
    int (*rmdir) (struct inode *,struct dentry *);
    int (*mknod) (struct inode *,struct dentry *,int,dev_t);
    int (*rename) (struct inode *, struct dentry *,
                    struct inode *, struct dentry *);
    int (*readlink) (struct dentry *, char __user *,int);
    void * (*follow_link) (struct dentry *, struct nameidata *);
    void (*put_link) (struct dentry *, struct nameidata *, void *);
    void (*truncate) (struct inode *);
    int (*permission) (struct inode *, int);
    int (*setattr) (struct dentry *, struct iattr *);
    int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);
    int (*setxattr) (struct dentry *, const char *,const void *,size_t,int);
    ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);
    ssize_t (*listxattr) (struct dentry *, char *, size_t);
    int (*removexattr) (struct dentry *, const char *);
    void (*truncate_range)(struct inode *, loff_t, loff_t);
    long (*fallocate)(struct inode *inode, int mode, loff_t offset,
                    loff_t len);
    int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
                u64 len);
};
```

The following interfaces constitute the various functions that the VFS
may perform, or ask a specific filesystem to perform, on a given inode:

  * `int create(struct inode *dir, struct dentry *dentry, int mode)`
    TheVFS calls this function from the `creat()` and `open()` system calls to create a new
    inode associated with the given dentry object with the specified initial access mode.

  * `struct dentry * lookup(struct inode *dir, struct dentry *dentry)`
    This function searches a directory for an inode corresponding to a filename specified in the given dentry.

  * `int link(struct dentry *old_dentry, struct inode *dir, struct dentry *dentry)`
    Invoked by the `link()` system call to create a hard link of the file `old_dentry` in
    the directory `dir` with the new filename `dentry` .

  * `int unlink(struct inode *dir, struct dentry *dentry)`
    Called from the `unlink()` system call to remove the inode specified by the directory entry dentry from the directory dir.

  * ```
      int symlink(struct inode *dir,
				struct dentry *dentry,
				const char *symname)
	```

	  Called from the `symlink()` system call to create a symbolic link named `symname` to
	  the file represented by `dentry` in the directory `dir`.


  * ```
	  int mkdir(struct inode *dir,
				struct dentry *dentry,
				int mode)
	 ```

	Called from the mkdir() system call to create a new directory with the given initial mode.

  * ```
	  int rmdir(struct inode *dir,
				struct dentry *dentry)
	```

	Called by the `rmdir()` system call to remove the directory referenced by `dentry`
	from the directory `dir`.

  * ```
	  int mknod(struct inode *dir,
				struct dentry *dentry,
				int mode, dev_t rdev)
	```

	Called by the `mknod()` system call to create a special file (device file, named pipe, or
	socket).The file is referenced by the device `rdev` and the directory entry `dentry` in
	the directory `dir` .The initial permissions are given via mode .

  * ```
	 int rename(struct inode *old_dir,
				struct 	dentry *old_dentry,
				struct 	inode *new_dir,
				struct 	dentry *new_dentry)
	  ```

	Called by the VFS to move the file specified by old_dentry from the old_dir
	directory to the directory new_dir , with the filename specified by new_dentry .

  * ```
	  int readlink(struct dentry *dentry,
					char *buffer, int buflen)
	```

	Called by the `readlink()` system call to copy at most `buflen` bytes of the full path
	associated with the symbolic link specified by dentry into the specified buffer.

  * ```
	  int follow_link(struct dentry *dentry,
					  struct nameidata *nd)
	```

	Called by the VFS to translate a symbolic link to the inode to which it points.The
	link pointed at by `dentry` is translated, and the result is stored in the `nameidata`
	structure pointed at by `nd`.

  * ```
	  int put_link(struct dentry *dentry,
				   struct nameidata *nd)
	```

	Called by the VFS to clean up after a call to `follow_link()`.

	- `void truncate(struct inode *inode)`

	Called by the VFS to modify the size of the given file. Before invocation, the inode’s
	`i_size` field must be set to the desired new size.

	- `int permission(struct inode *inode, int mask)`

	Checks whether the specified access mode is allowed for the file referenced by
	inode .This function returns `zero` if the access is allowed and a negative error code
	otherwise. Most filesystems set this field to `NULL` and use the generic VFS method,
	which simply compares the mode bits in the inode’s objects to the given mask.

	More complicated filesystems, such as those supporting access control lists (ACLs),
	have a specific `permission()` method.

  * ```
	  int setattr(struct dentry *dentry,
				  struct iattr *attr)
	```

	Called from `notify_change()` to notify a “change event” after an inode has been
	modified.

  * ```
	  int getattr(struct vfsmount *mnt,
				  struct dentry *dentry,
				  struct kstat *stat)
	```

	Invoked by the VFS upon noticing that an inode needs to be refreshed from disk.
	Extended attributes allow the association of key/values pairs with files.

  * ```
	   int setxattr(struct dentry *dentry,
				   	const char *name,
				   	const void *value,
					size_t size, int flags)
	```

	Used by the VFS to set the extended attribute `name` to the value `value` on the file
	referenced by `dentry`.

  * ```
	   ssize_t getxattr(struct dentry *dentry,
						const char *name,
						void *value, size_t size)
	```

	Used by the VFS to copy into value the value of the extended attribute `name` for
	the specified file.

  * ```
	  ssize_t listxattr(struct dentry *dentry,
						char *list, size_t size)
	```

	Copies the list of all attributes for the specified file into the buffer `list`.

  * ```
		int removexattr(struct dentry *dentry,
						const char *name)
	```

	Removes the given attribute from the given file.


## The Dentry Object

As discussed, the VFS treats directories as a type of file. In the path
`/bin/vi`, both `bin` and `vi` are files—`bin` being the special
directory file and `vi` being a regular file. An inode object represents
each of these components. Despite this useful unification, the VFS often
needs to perform directory-specific operations, such as path name
lookup. Path name lookup involves translating each component of a path,
ensuring it is valid, and following it to the next component.


To facilitate this, the VFS employs the concept of a directory entry
(dentry). A *dentry* is a specific component in a path. Using the
previous example, `/`, `bin`, and `vi` are all dentry objects. The first
two are directories and the last is a regular file. This is an important
point: Dentry objects are *all* components in a path, including files.
Resolving a path and walking its components is a nontrivial exercise,
time-consuming and heavy on string operations, which are expensive to
execute and cumbersome to code. The dentry object makes the whole
process easier.


Dentries might also include mount points. In the path `/mnt/cdrom/foo`,
the components `/`, `mnt`, `cdrom`, and `foo` are all dentry objects.
The VFS constructs dentry objects on-the-fly, as needed, when performing
directory operations.

Dentry objects are represented by `struct dentry` and defined in
`<linux/dcache.h>`. Here is the structure, with comments describing each
member:

```c
struct dentry {                                                                           
   atomic_t                    d_count;         /* usage count */
   unsigned int                 d_flags;        /* dentry flags */
   spinlock_t                  d_lock;          /* per-dentry lock */
   int                          d_mounted;      /* is this a mount point? */
   struct inode                 *d_inode;       /* associated inode */
   struct hlist_node           d_hash;          /* list of hash table entries */
   struct dentry                *d_parent;      /* dentry object of parent */
   struct qstr                  d_name;         /* dentry name */
   struct list_head            d_lru;           /* unused list */
   union {                                                                                 
   struct list_head            d_child;         /* list of dentries within */
   struct rcu_head             d_rcu;           /* RCU locking */
   } d_u;                                                                                 
   struct list_head            d_subdirs;       /* subdirectories */
   struct list_head            d_alias;         /* list of alias inodes */
   unsigned long                d_time;         /* revalidate time */
   struct dentry_operations    *d_op;           /* dentry operations table */
   struct super_block          *d_sb;           /* superblock of file */
   void                         *d_fsdata;      /* filesystem-specific data */
   unsigned char                d_iname[DNAME_INLINE_LEN_MIN]; /* short name */
 };  
```

Unlike the previous two objects, the dentry object does not correspond
to any sort of on-disk data structure. The VFS creates it on-the-fly
from a string representation of a path name. Because the dentry object
is not physically stored on the disk, no flag in `struct dentry`
specifies whether the object is modified (that is, whether it is dirty
and needs to be written back to disk).

### Dentry State

A valid dentry object can be in one of three states: used, unused, or
negative.

A used dentry corresponds to a valid inode (`d_inode` points to an
associated inode) and indicates that there are one or more users of the
object (`d_count` is positive). A used dentry is in use by the VFS and
points to valid data and, thus, cannot be discarded.


An unused dentry corresponds to a valid inode (`d_inode` points to an
inode), but the VFS is not currently using the dentry object (`d_count`
is zero). Because the dentry object still points to a valid object, the
dentry is kept around—cached—in case it is needed again. Because the
dentry has not been destroyed prematurely, the dentry need not be
re-created if it is needed in the future, and path name lookups can
complete quicker than if the dentry was not cached. If it is necessary
to reclaim memory, however, the dentry can be discarded because it is
not in active use.

 
A negative dentry is not associated with a valid inode (`d_inode` is
`NULL`) because either the inode was deleted or the path name was never
correct to begin with. The dentry is kept around, however, so that
future lookups are resolved quickly. For example, consider a daemon that
continually tries to open and read a config file that is not present.
The `open()` system calls continually returns `ENOENT`, but not until
after the kernel constructs the path, walks the on-disk directory
structure, and verifies the file’s inexistence. Because even this failed
lookup is expensive, caching the “negative” results are worthwhile.
Although a negative dentry is useful, it can be destroyed if memory is
at a premium because nothing is actually using it.


A dentry object can also be freed, sitting in the slab object cache, as
discussed in the previous chapter. In that case, there is no valid
reference to the dentry object in any VFS or any filesystem code.

### The Dentry Cache

After the VFS layer goes through the trouble of resolving each element
in a path name into a dentry object and arriving at the end of the path,
it would be quite wasteful to throw away all that work. Instead, the
kernel caches dentry objects in the dentry cache or, simply, the
*dcache*.

The dentry cache consists of three parts:

  * Lists of “used” dentries linked off their associated inode via the
  `i_dentry` field of the inode object. Because a given inode can have
  multiple links, there might be multiple dentry objects; consequently,
  a list is used.

  * A doubly linked “least recently used” list of unused and negative
  dentry objects. The list is inserted at the head, such that entries
  toward the head of the list are newer than entries toward the tail.
  When the kernel must remove entries to reclaim memory, the entries are
  removed from the tail; those are the oldest and presumably have the
  least chance of being used in the near future.

  * A hash table and hashing function used to quickly resolve a given
  path into the associated dentry object. 

The hash table is represented by the `dentry_hashtable` array. Each
element is a pointer to a list of dentries that hash to the same value.
The size of this array depends on the amount of physical RAM in the
system.

The actual hash value is determined by `d_hash()`. This enables
filesystems to provide a unique hashing function.

Hash table lookup is performed `via d_lookup()`. If a matching dentry
object is found in the dcache, it is returned. On failure, `NULL` is
returned.

As an example, assume that you are editing a source file in your home
directory, `/home/dracula/src/the_sun_sucks.c`. Each time this file is
accessed (for example, when you first open it, later save it, compile
it, and so on), the VFS must follow each directory entry to resolve the
full path: `/`, `home`, `dracula`, `src`, and finally `the_sun_sucks.c`.
To avoid this time-consuming operation each time this path name is
accessed, the VFS can first try to look up the path name in the dentry
cache. If the lookup succeeds, the required final dentry object is
obtained without serious effort. Conversely, if the dentry is not in the
dentry cache, the VFS must manually resolve the path by walking the
filesystem for each component of the path. After this task is completed,
the kernel adds the dentry objects to the dcache to speed up any future
lookups.

 
The dcache also provides the front end to an inode cache, the *icache*.
Inode objects that are associated with dentry objects are not freed
because the dentry maintains a positive usage count over the inode. This
enables dentry objects to pin inodes in memory. As long as the dentry is
cached, the corresponding inodes are cached, too. Consequently, when a
path name lookup succeeds from cache, as in the previous example, the
associated inodes are already cached in memory.


Caching dentries and inodes is beneficial because file access exhibits
both spatial and temporal locality. File access is temporal in that
programs tend to access and reaccess the same files over and over. Thus
when a file is accessed, there is a high probability that caching the
associated dentries and inodes will result in a cache hit in the near
future. File access is spatial in that programs tend to access multiple
files in the same directory. Thus caching directories entries for one
file have a high probability of a cache hit, as a related file is likely
manipulated next.

## Dentry Operations

The `dentry_operations` structure specifies the methods that the VFS
invokes on directory entries on a given filesystem.

The `dentry_operations` structure is defined in `<linux/dcache.h>`:

```c
struct dentry_operations {
	int (*d_revalidate) (struct dentry *, struct nameidata *);
	int (*d_hash) (struct dentry *, struct qstr *);
	int (*d_compare) (struct dentry *, struct qstr *, struct qstr *);
	int (*d_delete) (struct dentry *);
	void (*d_release) (struct dentry *);
	void (*d_iput) (struct dentry *, struct inode *);
	char *(*d_dname) (struct dentry *, char *, int);
};
```

The methods are as follows:

  * ```
  	int d_revalidate(struct dentry *dentry,
					struct nameidata *)
	```

	Determines whether the given dentry object is valid.The VFS calls this function
	whenever it is preparing to use a dentry from the dcache. Most filesystems set this
	method to NULL because their dentry objects in the dcache are always valid.

  * ```
  	int d_hash(struct dentry *dentry,
			   struct qstr *name)
	```

	Creates a hash value from the given dentry.The VFS calls this function whenever it
	adds a dentry to the hash table.

  * ```
  	int d_compare(struct dentry *dentry,
					struct qstr *name1,
					struct qstr *name2)
	```

	Called by the VFS to compare two filenames, name1 and name2 . Most filesystems
	leave this at the VFS default, which is a simple string compare. For some filesystems,
	such as FAT, a simple string compare is insufficient.The FAT filesystem is not case-
	sensitive and therefore needs to implement a comparison function that disregards
	case.This function requires the dcache_lock .

  * `int d_delete (struct dentry *dentry)`

	Called by the VFS when the specified dentry object’s d_count reaches zero.This
	function requires the dcache_lock and the dentry’s d_lock .

  * `void d_release(struct dentry *dentry)`

	Called by the VFS when the specified dentry is going to be freed.The default function does nothing.

  * ```
  	void d_iput(struct dentry *dentry,
				struct inode *inode)
	```

	Called by the VFS when a dentry object loses its associated inode (say, because the
	entry was deleted from the disk). By default, the VFS simply calls the `iput()` function to release the inode. If a filesystem overrides this function, it must also call
	`iput()` in addition to performing whatever filesystem-specific work it requires.


## The File Object

The final primary VFS object that we shall look at is the file object.
The file object is used to represent a file opened by a process. When we
think of the VFS from the perspective of user-space, the file object is
what readily comes to mind. Processes deal directly with files, not
superblocks, inodes, or dentries. It is not surprising that the
information in the file object is the most familiar (data such as access
mode and current offset) or that the file operations are familiar system
calls such as `read()` and `write()`.

 
The file object is the in-memory representation of an open file. The
object (but not the physical file) is created in response to the
`open()` system call and destroyed in response to the `close()` system
call. All these file-related calls are actually methods defined in the
file operations table. Because multiple processes can open and
manipulate a file at the same time, there can be multiple file objects
in existence for the same file. The file object merely represents a
process’s view of an open file. The object points back to the dentry
(which in turn points back to the inode) that actually represents the
open file. The inode and dentry objects, of course, are unique.

 
The file object is represented by `struct file` and is defined in
`<linux/fs.h>`. Let’s look at the structure, again with comments added
to describe each entry:

```c
 struct file {                                    
   union {                                        
   struct list_head          fu_list;           /* list of file objects */
   struct rcu_head           fu_rcuhead;        /* RCU list after freeing */
   } f_u;                                        
   struct path                f_path;            /* contains the dentry */
   struct file_operations    *f_op;            /* file operations table */
   spinlock_t                f_lock;            /* per-file struct lock */
   atomic_t                  f_count;           /* file object’s usage count */
   unsigned int               f_flags;           /* flags specified on open */
   mode_t                    f_mode;            /* file access mode */
   loff_t                    f_pos;             /* file offset (file pointer) */
   struct fown_struct        f_owner;           /* owner data for signals */
   const struct cred          *f_cred;          /* file credentials */
   struct file_ra_state     f_ra;              /* read-ahead state */
   u64                        f_version;         /* version number */
   void                       *f_security;      /* security module */
   void                       *private_data;    /* tty driver hook */
   struct list_head				f_ep_links;		/* list of epoll links */
   spinlock_t					f_ep_lock;		/* epoll lock */
   struct address_space			*f_mapping;		/* page cache mapping */
   unsigned long 				f_mnt_write_state; /* debugging state */
};
```



Similar to the dentry object, the file object does not actually
correspond to any on-disk data. Therefore, no flag in the object
represents whether the object is dirty and needs to be written back to
disk. The file object does point to its associated dentry object via the
`f_dentry` pointer. The dentry in turn points to the associated inode,
which reflects whether the file itself is dirty.


## File Operations

As with all the other VFS objects, the file operations table is quite
important. The operations associated with `struct file` are the familiar
system calls that form the basis of the standard Unix system calls.


The file object methods are specified in `file_operations` and defined
in `<linux/fs.h>`:

```c
struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);

 	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
 	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
 	ssize_t (*aio_read) (struct kiocb *, const struct iovec *,
 							unsigned long, loff_t);
 	ssize_t (*aio_write) (struct kiocb *, const struct iovec *,
 							unsigned long, loff_t);
 	int (*readdir) (struct file *, void *, filldir_t);
 	unsigned int (*poll) (struct file *, struct poll_table_struct *);
 	int (*ioctl) (struct inode *, struct file *, unsigned int,
				unsigned long);
 long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
 long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
 int (*mmap) (struct file *, struct vm_area_struct *);

 int (*open) (struct inode *, struct file *);
 int (*flush) (struct file *, fl_owner_t id);
 int (*release) (struct inode *, struct file *);

 int (*fsync) (struct file *, struct dentry *, int datasync);
 int (*aio_fsync) (struct kiocb *, int datasync);

 int (*fasync) (int, struct file *, int);

 int (*lock) (struct file *, int, struct file_lock *);
 ssize_t (*sendpage) (struct file *, struct page *,
 						int, size_t, loff_t *, int);
 unsigned long (*get_unmapped_area) (struct file *, unsigned long,
 													unsigned long,
 													unsigned long,
 													unsigned long);

 int (*check_flags) (int);

 int (*flock) (struct file *, int, struct file_lock *);
 ssize_t (*splice_write) (struct pipe_inode_info *,
							struct file *,
							loff_t *,
							size_t,
							unsigned int);

 ssize_t (*splice_read) (struct file *,
 							loff_t *,
							struct pipe_inode_info *,
							size_t,
							unsigned int);

 int (*setlease) (struct file *, long, struct file_lock **);

};
```

Filesystems can implement unique functions for each of these operations,
or they can use a generic method if one exists. The generic methods tend
to work fine on normal Unix-based filesystems. A filesystem is under no
obligation to implement all these methods—although not implementing the
basics is silly—and can simply set the method to `NULL` if not
interested.

Here are the individual operations:

  * ```
  	loff_t llseek(struct file *file,
					loff_t offset, int origin)
	```

	Updates the file pointer to the given offset. It is called via the `llseek()` system call.

  * ```
   ssize_t read(struct file *file,
				char *buf, size_t count,
				loff_t *offset)
	```

	Reads `count` bytes from the given file at position `offset` into `buf`. The file pointer
	is then updated.This function is called by the `read()` system call.

  * ```
  ssize_t aio_read(struct kiocb *iocb,
					char *buf, size_t count,
					loff_t offset)
	```

	Begins an asynchronous read of `count` bytes into `buf` of the file described in `iocb`.
	This function is called by the `aio_read()` system call.

  * ```
  	ssize_t write(struct file *file,
				const char *buf, size_t count,
				loff_t *offset)
	```

	Writes `count` bytes from `buf` into the given file at position `offset`.The file pointer
	is then updated.This function is called by the `write()` system call.

  * ```
  ssize_t aio_write(struct kiocb *iocb,
					const char *buf,
					size_t count, loff_t offset)
	```

	Begins an asynchronous write of `count` bytes into `buf` of the file described in `iocb`.
	This function is called by the `aio_write()` system call.

  * ```
  	int readdir(struct file *file, void *dirent,
				filldir_t filldir)
	```

	Returns the next directory in a directory listing. This function is called by the
	`readdir()` system call.

  * ```
  unsigned int poll(struct file *file,
					struct poll_table_struct *poll_table)
	```

	Sleeps, waiting for activity on the given file. It is called by the `poll()` system call.

  * ```
  int ioctl(struct inode *inode,
			struct file *file,
			unsigned int cmd,
			unsigned long arg)
	```

	Sends a command and argument pair to a device. It is used when the file is an open
	device node.This function is called from the `ioctl()` system call. Callers must hold
	the BKL.

  * ```
  int unlocked_ioctl(struct file *file,
					unsigned int cmd,
					unsigned long arg)
	```

	Implements the same functionality as `ioctl()` but without needing to hold the
	BKL.The VFS calls `unlocked_ioctl()` if it exists in lieu of `ioctl()` when user-
	space invokes the `ioctl()` system call.Thus filesystems need implement only one,
	preferably `unlocked_ioctl()`.

  * ```
  int compat_ioctl(struct file *file,
					unsigned int cmd,
					unsigned long arg)
	```

	Implements a portable variant of `ioctl()` for use on 64-bit systems by 32-bit
	applications.This function is designed to be 32-bit safe even on 64-bit architectures, performing any necessary size conversions. New drivers should design their
	ioctl commands such that all are portable, and thus enable `compat_ioctl()` and
	`unlocked_ioctl()` to point to the same function. Like `unlocked_ioctl()` ,
	`compat_ioctl()` does not hold the BKL.

  * ```
  int mmap(struct file *file,
			struct vm_area_struct *vma)
	```

	Memory maps the given file onto the given address space and is called by the
	`mmap()` system call.

  * ```
  	int open(struct inode *inode,
			struct file *file)
	```

	Creates a new file object and links it to the corresponding inode object. It is called
	by the `open()` system call.

  * `int flush(struct file *file)`

	Called by the VFS whenever the reference count of an open file decreases. Its purpose is filesystem-dependent.

  * ```
  	int release(struct inode *inode,
				struct file *file)
	```

	Called by the VFS when the last remaining reference to the file is destroyed—for
	example, when the last process sharing a file descriptor calls `close()` or exits. Its
	purpose is filesystem-dependent.

  * ```
  int fsync(struct file *file,
			struct dentry *dentry,
			int datasync)
	```

	Called by the `fsync()` system call to write all cached data for the file to disk.

  * ```
  int aio_fsync(struct kiocb *iocb,
				int datasync)
	```

	Called by the `aio_fsync()` system call to write all cached data for the file associated with `iocb` to disk.

  * `int fasync(int fd, struct file *file, int on)`

	Enables or disables signal notification of asynchronous I/O.

  * ```
  int lock(struct file *file, int cmd,
		   struct file_lock *lock)
	```

	Manipulates a file lock on the given file.

  * ```
  ssize_t readv(struct file *file,
				const struct iovec *vector,
				unsigned long count,
				loff_t *offset)
	```

	Called by the `readv()` system call to read from the given file and put the results
	into the `count` buffers described by `vector` .The file offset is then incremented.

  * ```
  ssize_t writev(struct file *file,
				const struct iovec *vector,
				unsigned long count,
				loff_t *offset)
	```

	Called by the `writev()` system call to write from the `count` buffers described by
	`vector` into the file specified by file .The file offset is then incremented.

  * ```
  ssize_t sendfile(struct file *file,
				   	loff_t *offset,
					size_t size,
					read_actor_t actor,
					void *target)
	```

	Called by the `sendfile()` system call to copy data from one file to another. It performs the copy entirely in the kernel and avoids an extraneous copy to user-space.

  * ```
  ssize_t sendpage(struct file *file,
					struct page *page,
					int offset, size_t size,
					loff_t *pos, int more)
	```

	Used to send data from one file to another.

  * ```
  unsigned long get_unmapped_area(struct file *file,
								unsigned long addr,
								unsigned long len,
								unsigned long offset,
								unsigned long flags)
	```

	Gets unused address space to map the given file.

  * `int check_flags(int flags)`

	Used to check the validity of the flags passed to the `fcntl()` system call when the
	`SETFL` command is given. As with many VFS operations, filesystems need not
	implement `check_flags()` ; currently, only NFS does so.This function enables
	filesystems to restrict invalid `SETFL` flags otherwise enabled by the generic `fcntl()`
	function. In the case of NFS, combining `O_APPEND` and `O_DIRECT` is not enabled.

  * ```
  int flock(struct file *filp,
			int cmd,
			struct file_lock *fl)
	```

	Used to implement the `flock()` system call, which provides advisory locking.



###So Many Ioctls!

Not long ago, there existed only a single ioctl method. Today, there
are three methods. `unlocked_ioctl()` is the same as `ioctl()`,
except it is called without the Big Kernel Lock (BKL). It is thus up
to the author of that function to ensure proper synchronization.
Because the BKL is a coarse-grained, inefficient lock, drivers
should implement `unlocked_ioctl()` and not `ioctl()`.

`compat_ioctl()` is also called without the BKL, but its purpose is
to provide a 32-bit compatible ioctl method for 64-bit systems. How
you implement it depends on your existing ioctl commands. Older
drivers with implicitly sized types (such as `long`) should
implement a `compat_ioctl()` method that works appropriately with
32-bit applications. This generally means translating the 32-bit
values to the appropriate types for a 64-bit kernel. New drivers
that have the luxury of designing their ioctl commands from scratch
should ensure all their arguments and data are explicitly sized,
safe for 32-bit apps on a 32-bit system, 32-bit apps on a 64-bit
system, and 64-bit apps on a 64-bit system. These drivers can then
point the `compat_ioctl()` function pointer at the same function as
`unlocked_ioctl()`.

## Data Structures Associated with Filesystems

In addition to the fundamental VFS objects, the kernel uses other
standard data structures to manage data related to filesystems. The
first object is used to describe a specific variant of a filesystem,
such as ext3, ext4, or UDF. The second data structure describes a
mounted instance of a filesystem.


Because Linux supports so many different filesystems, the kernel must
have a special structure for describing the capabilities and behavior of
each filesystem. The `file_system_type` structure, defined in
`<linux/fs.h>`, accomplishes this:

```c
```
struct file_system_type {                        
	const char                   *name;      /*    filesystem’s name */
   	int                          fs_flags;   /*    filesystem type flags */

   	/* the following is used to read the superblock off the disk */
 	struct super_block *(*get_sb) (struct file_system_type *, int,
								char *, void *);

 	/* the following is used to read the superblock off the disk */
 	struct super_block *(*get_sb) (struct file_system_type *, int,
 									char *, void *);

  	/* the following is used to terminate access to the superblock */
  	void    				(*kill_sb) (struct super_block *);   

  	struct module   		*owner;    /*  module owning the filesystem */
  	struct file_system_type *next;     /* next file_system_type in list */
  	struct list_head        fs_supers; /* list of superblock objects */

 	/* the remaining fields are used for runtime lock validation */
 	struct lock_class_key s_lock_key;
 	struct lock_class_key s_umount_key;
 	struct lock_class_key i_lock_key;
 	struct lock_class_key i_mutex_key;
 	struct lock_class_key i_mutex_dir_key;
 	struct lock_class_key i_alloc_sem_key;
};
```
```

The `get_sb()` function reads the superblock from the disk and populates
the superblock object when the filesystem is loaded. The remaining
functions describe the filesystem’s properties.


There is only one `file_system_type` per filesystem, regardless of how
many instances of the filesystem are mounted on the system, or whether
the filesystem is even mounted at all.

 
Things get more interesting when the filesystem is actually mounted, at
which point the `vfsmount` structure is created. This structure
represents a specific instance of a filesystem—in other words, a mount
point.

 
The `vfsmount` structure is defined in `<linux/mount.h>`. Here it is:


```c
struct vfsmount {

 struct list_head 		mnt_hash; 			/* hash table list */
 struct vfsmount  		*mnt_parent;		/* parent filesystem */
 struct dentry    		*mnt_mountpoint;	/* dentry of this mount point */
struct dentry      		*mnt_root;			/* dentry of root of this fs */
struct super_block  	*mnt_sb;			/* superblock of this filesystem */
struct list_head       	mnt_mounts;			/* list of children */
struct list_head      	mnt_child;			/* list of children */
int                     mnt_flags;			/* mount flags */
char                    *mnt_devname;		/* device file name */
struct list_head       	mnt_list;			/* list of descriptors */
struct list_head     	mnt_expire;			/* entry in expiry list */
struct list_head     	mnt_share;			/* entry in shared mounts list */
struct list_head  		mnt_slave_list;		/* list of slave mounts */
struct list_head   		mnt_slave;			/* entry in slave list */
struct vfsmount   		*mnt_master;		/* slave’s master */
struct mnt_namespace 	*mnt_namespace; 	/* associated namespace */
int                 	mnt_id;				/* mount identifier */
int                 	mnt_group_id;		/* peer group identifier */
atomic_t           		mnt_count;			/* usage count */
int              		mnt_expiry_mark;	/* is marked for expiration */
int             		mnt_pinned;			/* pinned count */
int          			mnt_ghosts;			/* ghosts count */
atomic_t     			__mnt_writers;		/* writers count */*
};                                        
```

The complicated part of maintaining the list of all mount points is the
relation between the filesystem and all the other mount points. The
various linked lists in `vfsmount` keep track of this information.

 
The `vfsmount` structure also stores the flags, if any, specified on
mount in the `mnt_flags` field. Table below is a list of the standard
mount flags.

| Flag 	   |  Description  |
|-----------|-------------|
| MNT_NOSUID | 	Forbids setuid and setgid flags on binaries on this filesystem |
| MNT_NODEV  |	Forbids access to device files on this filesystem |
| MNT_NOEXEC |	Forbids execution of binaries on this filesystem |

 
Standard Mount Flags

These flags are most useful on removable devices that the administrator
does not trust. They are defined in `<linux/mount.h>` along with other,
lesser used, flags.


## Data Structures Associated with a Process

Each process on the system has its own list of open files, root
filesystem, current working directory, mount points, and so on. Three
data structures tie together the VFS layer and the processes on the
system: `files_struct`, `fs_struct`, and `namespace`.


The `files_struct` is defined in `<linux/fdtable.h>`. This table’s
address is pointed to by the `files` entry in the processor descriptor.
All per-process information about open files and file descriptors is
contained therein. Here it is, with comments:

```c
struct files_struct {                                                                                                                                                     
   atomic_t   				count;    			/*  usage count */     
   struct fdtable   		*fdt;      			/*  pointer  to other fd table */
   struct fdtable   		fdtab;      		/*  base fd   table */   
   spinlock_t      			file_lock;  		/*  per-file lock */   
   int   					next_fd;           	/*  cache of next available fd */
   struct embedded_fd_set 	close_on_exec_init; /*  list of close-on-exec fds */
   struct embedded_fd_set 	open_fds_init       /*  list of  open fds   */
   struct file   			*fd_array[NR_OPEN_DEFAULT]; /* base files array */*
  };            
 ```

The array `fd_array` points to the list of open file objects. Because
`NR_OPEN_DEFAULT` is equal to `BITS_PER_LONG`, which is 64 on a 64-bit
architecture; this includes room for 64 file objects. If a process opens
more than 64 file objects, the kernel allocates a new array and points
the `fdt` pointer at it. In this fashion, access to a reasonable number
of file objects is quick, taking place in a static array. If a process
opens an abnormal number of files, the kernel can create a new array. If
the majority of processes on a system opens more than 64 files, for
optimum performance the administrator can increase the `NR_OPEN_DEFAULT`
preprocessor macro to match.


The second process-related structure is `fs_struct`, which contains
filesystem information related to a process and is pointed at by the
`fs` field in the process descriptor. The structure is defined in
`<linux/fs_struct.h>`. Here it is, with comments:

```c
struct fs_struct {                 
   int          	users;     	/* user count */
   rwlock_t        	lock;     	/* per-structure lock */
   int           	umask;   	/* umask */
   int            	in_exec;  	/* currently executing a file */
   struct path     	root;     	/* root directory */
   struct path    	pwd;      	/* current working directory */*
};       
```

This structure holds the current working directory (`pwd`) and root
directory of the current process.


The third and final structure is the `namespace` structure, which is
defined in `<linux/mnt_namespace.h>` and pointed at by the
`mnt_namespace` field in the process descriptor. Per-process namespaces
were added to the 2.4 Linux kernel. They enable each process to have a
unique view of the mounted filesystems on the system—not just a unique
root directory, but an entirely unique filesystem hierarchy. Here is the
structure, with the usual comments:

```c
struct mnt_namespace {                                       
   atomic_t           		count; /* usage count */   
   struct vfsmount          *root; /* root directory */
   struct list_head       	list;  /* list of mount points */
   wait_queue_head_t    	poll;  /* polling waitqueue */
   int                     	event; /* event count */*
};
```

The `list` member specifies a doubly linked list of the mounted
filesystems that make up the namespace.


These data structures are linked from each process descriptor. For most
processes, the process descriptor points to unique `files_struct` and
`fs_struct` structures. For processes created with the clone flag
`CLONE_FILES` or `CLONE_FS`, however, these structures are
shared. Threads usually specify `CLONE_FILES` and `CLONE_FS` and, thus, share a single
`files_struct` and `fs_struct` among themselves. Normal processes, on the other hand, do not specify these flags and consequently have their own filesystems information and open files tables. Consequently, multiple process descriptors might point to the same `files_struct` or
`fs_struct` structure. The `count` member of each structure provides a
reference count to prevent destruction while a process is still using
the structure.

The `namespace` structure works the other way around. By default, all
processes share the same namespace. (That is, they all see the same
filesystem hierarchy from the same mount table.) Only when the
`CLONE_NEWNS` flag is specified during `clone()` is the process given a
unique copy of the `namespace` structure. Because most processes do
*not* provide this flag, all the processes inherit their parents’
namespaces. Consequently, on many systems there is only one namespace,
although the functionality is but a single `CLONE_NEWNS` flag away.

### Conclusion

Linux supports a wide range of filesystems, from native filesystems,
such as ext3 and ext4, to networked filesystems, such as NFS and
Coda—more than 60 filesystems alone in the official kernel. The VFS
layer provides these disparate filesystems with both a framework for
their implementation and an interface for working with the standard
system calls. The VFS layer, thus, both makes it clean to implement new
filesystems in Linux and enables those filesystems to automatically
interoperate via the standard Unix system calls.


#Building and Running Modules

It’s almost time to begin programming. This chapter introduces all the essential concepts about modules and kernel programming. In these few pages, we build and run
a complete (if relatively useless) module, and look at some of the basic code shared
by all modules.

##The Hello World Module

Many programming books begin with a “hello world” example as a way of showing
the simplest possible program. This book deals in kernel modules rather than programs; so, for the impatient reader, the following code is a complete “hello world”
module:

```c
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
```

This module defines two functions, one to be invoked when the module is loaded
into the kernel (*hello_init*) and one for when the module is removed (*hello_exit*). The
*module_init* and *module_exit* lines use special kernel macros to indicate the role of
these two functions. Another special macro (`MODULE_LICENSE`) is used to tell the
kernel that this module bears a free license; without such a declaration, the kernel
complains when the module is loaded.

The *printk* function is defined in the Linux kernel and made available
to modules; it behaves similarly to the standard C library function
*printf*. The kernel needs its own printing function because it runs by
itself, without the help of the C library. The module can call *printk*
because, after *insmod* has loaded it, the module is linked to the
kernel and can access the kernel’s public symbols (functions and
variables, as detailed in the next section). The string `KERN\_ALERT` is
the priority of the message. We’ve specified a high priority in this
module, because a message with the default priority might not show up
anywhere useful, depending on the kernel version you are running, the
version of the *klogd* daemon, and your configuration.

You can test the module with the insmod and rmmod utilities, as shown below. Note
that only the superuser can load and unload a module.

```
$ make
make[1]: Entering directory `/usr/src/linux-2.6.10'
  CC [M] /home/ldd3/src/misc-modules/hello.o
  Building modules, stage 2.
  MODPOST
  CC 		/home/ldd3/src/misc-modules/hello.mod.o
  LD [M] 	/home/ldd3/src/misc-modules/hello.ko
make[1]: Leaving directory `/usr/src/linux-2.6.10'
$ su
root# insmod ./hello.ko
Hello, world
root# rmmod hello
Goodbye cruel world
root#
```

Please note once again that, for the above sequence of commands to work, you must
have a properly configured and built kernel tree in a place where the makefile is able
to find it (*/usr/src/linux-2.6.10* in the example shown). We get into the details of how
modules are built in the section “Compiling and Loading.”

According to the mechanism your system uses to deliver the message lines, your out-
put may be different. In particular, the previous screen dump was taken from a text
console; if you are running *insmod* and *rmmod* from a terminal emulator running
under the window system, you won’t see anything on your screen. The message goes
to one of the system log files, such as */var/log/messages* (the name of the actual file
varies between Linux distributions). The mechanism used to deliver kernel messages
is described in Chapter 4.

As you can see, writing a module is not as difficult as you might expect—at least, as
long as the module is not required to do anything worthwhile. The hard part is
understanding your device and how to maximize performance. We go deeper into
modularization throughout this chapter and leave device-specific issues for later
chapters.

##Kernel Modules Versus Applications

Before we go further, it’s worth underlining the various differences between a kernel
module and an application.

While most small and medium-sized applications perform a single task from begin-
ning to end, every kernel module just registers itself in order to serve future requests,
and its initialization function terminates immediately. In other words, the task of the
module’s initialization function is to prepare for later invocation of the module’s
functions; it’s as though the module were saying, “Here I am, and this is what I can
do.” The module’s exit function (*hello_exit* in the example) gets invoked just before
the module is unloaded. It should tell the kernel, “I’m not there anymore; don’t ask
me to do anything else.” This kind of approach to programming is similar to event-
driven programming, but while not all applications are event-driven, each and every
kernel module is. Another major difference between event-driven applications and
kernel code is in the exit function: whereas an application that terminates can be lazy
in releasing resources or avoids clean up altogether, the exit function of a module
must carefully undo everything the *init* function built up, or the pieces remain
around until the system is rebooted.

Incidentally, the ability to unload a module is one of the features of modularization
that you’ll most appreciate, because it helps cut down development time; you can
test successive versions of your new driver without going through the lengthy shut-
down/reboot cycle each time.

As a programmer, you know that an application can call functions it doesn’t define:
the linking stage resolves external references using the appropriate library of functions.
*printf* is one of those callable functions and is defined in *libc*. A module, on the
other hand, is linked only to the kernel, and the only functions it can call are the
ones exported by the kernel; there are no libraries to link to. The *printk* function
used in *hello.c* earlier, for example, is the version of *printf* defined within the kernel
and exported to modules. It behaves similarly to the original function, with a few
minor differences, the main one being lack of floating-point support.

Figure below shows how function calls and function pointers are used in a module to
add new functionality to a running kernel.

![](/dedupfs/images/filesystem_guide_kernel_modules_1.png)

*Linking a module to the kernel*

Because no library is linked to modules, source files should never include the usual
header files, `<stdarg.h>` and very special situations being the only exceptions. Only
functions that are actually part of the kernel itself may be used in kernel modules.
Anything related to the kernel is declared in headers found in the kernel source tree
you have set up and configured; most of the relevant headers live in `include/linux` and
`include/asm`, but other subdirectories of `include` have been added to host material
associated to specific kernel subsystems.

Another important difference between kernel programming and application programming is in
how each environment handles faults: whereas a segmentation fault
is harmless during application development and a debugger can always be used to
trace the error to the problem in the source code, a kernel fault kills the current process at least, if not the whole system.

##User Space and Kernel Space

A module runs in *kernel space*, whereas applications run in *user space*. This concept
is at the base of operating systems theory.

The role of the operating system, in practice, is to provide programs with a consistent
view of the computer’s hardware. In addition, the operating system must
account for independent operation of programs and protection against unauthorized
access to resources. This nontrivial task is possible only if the CPU enforces protection
of system software from the applications.

Every modern processor is able to enforce this behavior. The chosen approach is to
implement different operating modalities (or levels) in the CPU itself. The levels have
different roles, and some operations are disallowed at the lower levels; program code
can switch from one level to another only through a limited number of gates. Unix
systems are designed to take advantage of this hardware feature, using two such levels.
All current processors have at least two protection levels, and some, like the x86
family, have more levels; when several levels exist, the highest and lowest levels are
used. Under Unix, the kernel executes in the highest level (also called *supervisor
mode*), where everything is allowed, whereas applications execute in the lowest level
(the so-called *user mode*), where the processor regulates direct access to hardware
and unauthorized access to memory.

We usually refer to the execution modes as *kernel space* and *user space*. These terms
encompass not only the different privilege levels inherent in the two modes, but also
the fact that each mode can have its own memory mapping—its own address
space—as well.

Unix transfers execution from user space to kernel space whenever an application
issues a system call or is suspended by a hardware interrupt. Kernel code executing a
system call is working in the context of a process—it operates on behalf of the calling process and is able to access data in the process’s address space. Code that handles interrupts, on the other hand, is asynchronous with respect to processes and is
not related to any particular process.

The role of a module is to extend kernel functionality; modularized code runs in kernel space. Usually a driver performs both the tasks outlined previously: some functions in the module are executed as part of system calls, and some are in charge of interrupt handling.


##Concurrency in the Kernel

One way in which kernel programming differs greatly from conventional application
programming is the issue of concurrency. Most applications, with the notable exception
of multithreading applications, typically run sequentially, from the beginning to
the end, without any need to worry about what else might be happening to change
their environment. Kernel code does not run in such a simple world, and even the
simplest kernel modules must be written with the idea that many things can be happening
at once.

There are a few sources of concurrency in kernel programming. Naturally, Linux systems
run multiple processes, more than one of which can be trying to use your driver
at the same time. Most devices are capable of interrupting the processor; interrupt
handlers run asynchronously and can be invoked at the same time that your driver is
trying to do something else. Several software abstractions (such as kernel timers,
introduced in Chapter 7) run asynchronously as well. Moreover, of course, Linux
can run on symmetric multiprocessor (SMP) systems, with the result that your driver
could be executing concurrently on more than one CPU. Finally, in 2.6, kernel code
has been made preemptible; this change causes even uniprocessor systems to have
many of the same concurrency issues as multiprocessor systems.

As a result, Linux kernel code, including driver code, must be *reentrant*—it must be
capable of running in more than one context at the same time. Data structures must
be carefully designed to keep multiple threads of execution separate, and the code
must take care to access shared data in ways that prevent corruption of the data.
Writing code that handles concurrency and avoids race conditions (situations in
which an unfortunate order of execution causes undesirable behavior) requires
thought and can be tricky. Proper management of concurrency is required to write
correct kernel code; for that reason, every sample driver in this book has been writ-
ten with concurrency in mind. The techniques used are explained as we come to
them; Chapter 5 has also been dedicated to this issue and the kernel primitives avail-
able for concurrency management.

A common mistake made by driver programmers is to assume that concurrency is
not a problem as long as a particular segment of code does not go to sleep (or
“block”). Even in previous kernels (which were not preemptive), this assumption
was not valid on multiprocessor systems. In 2.6, kernel code can (almost) never
assume that it can hold the processor over a given stretch of code. If you do not write
your code with concurrency in mind, it will be subject to catastrophic failures that
can be exceedingly difficult to debug.

##The Current Process

Although kernel modules don’t execute sequentially as applications do, most actions
performed by the kernel are done on behalf of a specific process. Kernel code can
refer to the current process by accessing the global item current , defined in
*<asm/current.h>*, which yields a pointer to `struct task_struct`, defined by
*<linux/sched.h>*.
The `current` pointer refers to the process that is currently executing. During the
execution of a system call, such as *open* or *read*, the current process is the one that
invoked the call. Kernel code can use process-specific information by using `current`,
if it needs to do so. An example of this technique is presented in Chapter 6.

Actually, `current` is not truly a global variable. The need to support SMP systems
forced the kernel developers to develop a mechanism that finds the current process on
the relevant CPU. This mechanism must also be fast, since references to current hap-
pen frequently. The result is an architecture-dependent mechanism that, usually,
hides a pointer to the `task_struct` structure on the kernel stack. The details of the
implementation remain hidden to other kernel subsystems though, and a device
driver can just include *<linux/sched.h>* and refer to the current process. For example,
the following statement prints the process ID and the command name of the current
process by accessing certain fields in `struct task_struct`:

```c
printk(KERN_INFO "The process is \"%s\" (pid %i)\n",
		current->comm, current->pid);
```

The command name stored in `current->comm` is the base name of the program file
(trimmed to 15 characters if need be) that is being executed by the current process.

##A Few Other Details

Kernel programming differs from user-space programming in many ways. We’ll
point things out as we get to them over the course of the book, but there are a few
fundamental issues which, while not warranting a section of their own, are worth a
mention. So, as you dig into the kernel, the following issues should be kept in mind.

Applications are laid out in virtual memory with a very large stack area. The stack, of
course, is used to hold the function call history and all automatic variables created by
currently active functions. The kernel, instead, has a very small stack; it can be as
small as a single, 4096-byte page. Your functions must share that stack with the
entire kernel-space call chain. Thus, it is never a good idea to declare large automatic variables; if you need larger structures, you should allocate them dynamically
at call time.

Often, as you look at the kernel API, you will encounter function names starting with
a double underscore ( `__` ). Functions so marked are generally a low-level component
of the interface and should be used with caution. Essentially, the double underscore
says to the programmer: “If you call this function, be sure you know what you are
doing.”

Kernel code cannot do floating point arithmetic. Enabling floating point would
require that the kernel save and restore the floating point processor’s state on each
entry to, and exit from, kernel space—at least, on some architectures. Given that
there really is no need for floating point in kernel code, the extra overhead is not
worthwhile.

##Compiling and Loading

The “hello world” example at the beginning of this chapter included a brief demonstration
of building a module and loading it into the system. There is, of course, a lot
more to that whole process than we have seen so far. This section provides more
detail on how a module author turns source code into an executing subsystem within
the kernel.

###Compiling Modules

As the first step, we need to look a bit at how modules must be built. The build process
for modules differs significantly from that used for user-space applications; the
kernel is a large, standalone program with detailed and explicit requirements on how
its pieces are put together. The build process also differs from how things were done
with previous versions of the kernel; the new build system is simpler to use and pro-
duces more correct results, but it looks very different from what came before. The
kernel build system is a complex beast, and we just look at a tiny piece of it. The files
found in the *Documentation/kbuild* directory in the kernel source are required reading for anybody wanting to understand all that is really going on beneath the surface.

There are some prerequisites that you must get out of the way before you can build
kernel modules. The first is to ensure that you have sufficiently current versions of the
compiler, module utilities, and other necessary tools. The file *Documentation/Changes*
in the kernel documentation directory always lists the required tool versions; you
should consult it before going any further. Trying to build a kernel (and its modules)
with the wrong tool versions can lead to no end of subtle, difficult problems. Note
that, occasionally, a version of the compiler that is too new can be just as problematic
as one that is too old; the kernel source makes a great many assumptions about the
compiler, and new releases can sometimes break things for a while.

If you still do not have a kernel tree handy, or have not yet configured and built that
kernel, now is the time to go do it. You cannot build loadable modules for a 2.6 kernel without this tree on your filesystem. It is also helpful (though not required) to be
actually running the kernel that you are building for.

Once you have everything set up, creating a makefile for your module is straightfor-
ward. In fact, for the “hello world” example shown earlier in this chapter, a single
line will suffice:

```
obj-m := hello.o
```

Readers who are familiar with *make*, but not with the 2.6 kernel build system, are
likely to be wondering how this makefile works. The above line is not how a traditional makefile looks, after all. The answer, of course, is that the kernel build system
handles the rest. The assignment above (which takes advantage of the extended syn-
tax provided by GNU *make*) states that there is one module to be built from the
object file hello.o. The resulting module is named *hello.ko* after being built from the
object file.

If, instead, you have a module called *module.ko* that is generated from two source
files (called, say, *file1.c* and *file2.c*), the correct incantation would be:

```
obj-m := module.o
module-objs := file1.o file2.o
```

For a makefile like those shown above to work, it must be invoked within the con-
text of the larger kernel build system. If your kernel source tree is located in, say,
your *~/kernel-2.6* directory, the *make* command required to build your module
(typed in the directory containing the module source and makefile) would be:

```
make -C ~/kernel-2.6 M=`pwd` modules
```

This command starts by changing its directory to the one provided with the `-C`
option (that is, your kernel source directory). There it finds the kernel’s top-level
makefile. The `M=` option causes that makefile to move back into your module source
directory before trying to build the `modules` target. This target, in turn, refers to the list of modules found in the `obj-m` variable, which we’ve set to *module.o* in our examples.

Typing the previous *make* command can get tiresome after a while, so the kernel
developers have developed a sort of makefile idiom, which makes life easier for those
building modules outside of the kernel tree. The trick is to write your makefile as follows:

```
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := hello.o

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else

	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif
```

Once again, we are seeing the extended GNU *make* syntax in action. This makefile is
read twice on a typical build. When the makefile is invoked from the command line,
it notices that the `KERNELRELEASE` variable has not been set. It locates the kernel source
directory by taking advantage of the fact that the symbolic link *build* in the installed
modules directory points back at the kernel build tree. If you are not actually running
the kernel that you are building for, you can supply a `KERNELDIR=` option on the
command line, set the `KERNELDIR` environment variable, or rewrite the line that sets
`KERNELDIR` in the makefile. Once the kernel source tree has been found, the makefile
invokes the default: target, which runs a second *make* command (parameterized in
the makefile as `$(MAKE)` ) to invoke the kernel build system as described previously.
On the second reading, the makefile sets `obj-m` , and the kernel makefiles take care of
actually building the module.

This mechanism for building modules may strike you as a bit unwieldy and obscure.
Once you get used to it, however, you will likely appreciate the capabilities that have
been programmed into the kernel build system. Do note that the above is not a complete makefile; a real makefile includes the usual sort of targets for cleaning up
unneeded files, installing modules, etc. See the makefiles in the example source
directory for a complete example.

###Loading and Unloading Modules

After the module is built, the next step is loading it into the kernel. As we’ve already
pointed out, *insmod* does the job for you. The program loads the module code and
data into the kernel, which, in turn, performs a function similar to that of *ld*, in that
it links any unresolved symbol in the module to the symbol table of the kernel.
Unlike the linker, however, the kernel doesn’t modify the module’s disk file, but
rather an in-memory copy. *insmod* accepts a number of command-line options (for
details, see the manpage), and it can assign values to parameters in your module
before linking it to the current kernel. Thus, if a module is correctly designed, it can
be configured at load time; load-time configuration gives the user more flexibility
than compile-time configuration, which is still used sometimes. Load-time configura-
tion is explained in the section “Module Parameters,” later in this chapter.

Interested readers may want to look at how the kernel supports *insmod*: it relies on a
system call defined in *kernel/module.c*. The function `sys_init_module` allocates kernel
memory to hold a module (this memory is allocated with `vmalloc`; see the section
“vmalloc and Friends” in Chapter 8); it then copies the module text into that mem-
ory region, resolves kernel references in the module via the kernel symbol table, and
calls the module’s initialization function to get everything going.

If you actually look in the kernel source, you’ll find that the names of the system calls
are prefixed with `sys_` . This is true for all system calls and no other functions; it’s
useful to keep this in mind when grepping for the system calls in the sources.

The *modprobe* utility is worth a quick mention. *modprobe*, like *insmod*, loads a
module into the kernel. It differs in that it will look at the module to be loaded to see
whether it references any symbols that are not currently defined in the kernel. If any
such references are found, *modprobe* looks for other modules in the current module
search path that define the relevant symbols. When *modprobe* finds those modules
(which are needed by the module being loaded), it loads them into the kernel as well.
If you use *insmod* in this situation instead, the command fails with an “unresolved
symbols” message left in the system logfile.

As mentioned before, modules may be removed from the kernel with the *rmmod* utility.
Note that module removal fails if the kernel believes that the module is still in
use (e.g., a program still has an open file for a device exported by the modules), or if
the kernel has been configured to disallow module removal. It is possible to config-
ure the kernel to allow “forced” removal of modules, even when they appear to be
busy. If you reach a point where you are considering using this option, however,
things are likely to have gone wrong badly enough that a reboot may well be the bet-
ter course of action.

The *lsmod* program produces a list of the modules currently loaded in the kernel.
Some other information, such as any other modules making use of a specific mod-
ule, is also provided. *lsmod* works by reading the */proc/modules* virtual file.
Information on currently loaded modules can also be found in the sysfs virtual filesystem
under */sys/module*.

##Preliminaries

We are getting closer to looking at some actual module code. But first, we need to
look at some other things that need to appear in your module source files. The kernel
is a unique environment, and it imposes its own requirements on code that
would interface with it.

Most kernel code ends up including a fairly large number of header files to get
definitions of functions, data types, and variables. We’ll examine these files as we
come to them, but there are a few that are specific to modules, and must appear in
every loadable module. Thus, just about all module code has the following:

```
#include <linux/module.h>
#include <linux/init.h>
```

*module.h* contains a great many definitions of symbols and functions needed by loadable modules. You need *init.h* to specify your initialization and cleanup functions, as
we saw in the “hello world” example above, and which we revisit in the next sec-
tion. Most modules also include *moduleparam.h* to enable the passing of parameters
to the module at load time; we will get to that shortly.

It is not strictly necessary, but your module really should specify which license
applies to its code. Doing so is just a matter of including a `MODULE_LICENSE` line:

```c
MODULE_LICENSE("GPL");
```

The specific licenses recognized by the kernel are “GPL” (for any version of the GNU
General Public License), “GPL v2” (for GPL version two only), “GPL and additional
rights,” “Dual BSD/GPL,” “Dual MPL/GPL,” and “Proprietary.” Unless your mod-
ule is explicitly marked as being under a free license recognized by the kernel, it is
assumed to be proprietary, and the kernel is “tainted” when the module is loaded. As
we mentioned in the section “License Terms” in Chapter 1, kernel developers tend to
be unenthusiastic about helping users who experience problems after loading propri-
etary modules.

Other descriptive definitions that can be contained within a module include
`MODULE_AUTHOR` (stating who wrote the module), `MODULE_DESCRIPTION` (a human-readable statement of what the module does), `MODULE_VERSION` (for a code revision num-
ber; see the comments in *<linux/module.h>* for the conventions to use in creating
version strings), `MODULE_ALIAS` (another name by which this module can be known),
and `MODULE_DEVICE_TABLE` (to tell user space about which devices the module supports).

The various `MODULE_` declarations can appear anywhere within your source file outside of
a function. A relatively recent convention in kernel code, however, is to put
these declarations at the end of the file.

##Initialization and Shutdown

As already mentioned, the module initialization function registers any facility offered
by the module. By *facility*, we mean a new functionality, be it a whole driver or a new
software abstraction, that can be accessed by an application. The actual definition of
the initialization function always looks like:

```c
static int __init initialization_function(void)
{
	/* Initialization code here  */__
}
module_init(initialization_function);
```

Initialization functions should be declared static , since they are not meant to be visi-
ble outside the specific file; there is no hard rule about this, though, as no function is
exported to the rest of the kernel unless explicitly requested. The `__init` token in the
definition may look a little strange; it is a hint to the kernel that the given function is
used only at initialization time. The module loader drops the initialization function
after the module is loaded, making its memory available for other uses. There is
a similar tag ( `__initdata` ) for data used only during initialization. Use of `__init` and
`__initdata` is optional, but it is worth the trouble. Just be sure not to use them for
any function (or data structure) you will be using after initialization completes. You
may also encounter `__devinit` and `__devinitdata` in the kernel source; these translate
to `__init` and `__initdata` only if the kernel has not been configured for hotpluggable devices. We will look at hotplug support in Chapter 14.


The use of *module_init* is mandatory. This macro adds a special section to the module’s object code stating where the module’s initialization function is to be found.
Without this definition, your initialization function is never called.

Modules can register many different types of facilities, including different kinds of
devices, filesystems, cryptographic transforms, and more. For each facility, there is a
specific kernel function that accomplishes this registration. The arguments passed to
the kernel registration functions are usually pointers to data structures describing the
new facility and the name of the facility being registered. The data structure usually
contains pointers to module functions, which is how functions in the module body
get called.

The items that can be registered go beyond the list of device types mentioned in
Chapter 1. They include, among others, serial ports, miscellaneous devices, sysfs
entries, */proc* files, executable domains, and line disciplines. Many of those registra-
ble items support functions that aren’t directly related to hardware but remain in the
“software abstractions” field. Those items can be registered, because they are inte-
grated into the driver’s functionality anyway (like */proc* files and line disciplines for
example).

There are other facilities that can be registered as add-ons for certain drivers, but
their use is so specific that it’s not worth talking about them; they use the stacking
technique, as described in the section “The Kernel Symbol Table.” If you want to
probe further, you can grep for `EXPORT_SYMBOL` in the kernel sources, and find the
entry points offered by different drivers. Most registration functions are prefixed with
`register_` , so another possible way to find them is to grep for `register_` in the kernel source.

###The Cleanup Function

Every nontrivial module also requires a cleanup function, which unregisters inter-
faces and returns all resources to the system before the module is removed. This
function is defined as:

```c
static void __exit cleanup_function(void)
{
	/* Cleanup code here */__
}
module_exit(cleanup_function);
```

The cleanup function has no value to return, so it is declared void . The `__exit`
modifier marks the code as being for module unload only (by causing the compiler to
place it in a special ELF section). If your module is built directly into the kernel,
or if your kernel is configured to disallow the unloading of modules, functions
marked `__exit` are simply discarded. For this reason, a function marked `__exit` can
be called *only* at module unload or system shutdown time; any other use is an error.
Once again, the *module\_exit* declaration is necessary to enable to kernel to find your
cleanup function.

If your module does not define a cleanup function, the kernel does not allow it to be
unloaded.

###Error Handling During Initialization

One thing you must always bear in mind when registering facilities with the kernel
is that the registration could fail. Even the simplest action often requires memory
allocation, and the required memory may not be available. So module code must
always check return values, and be sure that the requested operations have actually
succeeded.

If any errors occur when you register utilities, the first order of business is to decide
whether the module can continue initializing itself anyway. Often, the module can
continue to operate after a registration failure, with degraded functionality if necessary. Whenever possible, your module should press forward and provide what capabilities it can after things fail.

If it turns out that your module simply cannot load after a particular type of failure,
you must undo any registration activities performed before the failure. Linux doesn’t
keep a per-module registry of facilities that have been registered, so the module must
back out of everything itself if initialization fails at some point. If you ever fail to
unregister what you obtained, the kernel is left in an unstable state; it contains internal pointers to code that no longer exists. In such situations, the only recourse, usually,
is to reboot the system. You really do want to take care to do the right thing
when an initialization error occurs.

Error recovery is sometimes best handled with the goto statement. We normally hate
to use `goto`, but in our opinion, this is one situation where it is useful. Careful use of
goto in error situations can eliminate a great deal of complicated, highly-indented,
“structured” logic. Thus, in the kernel, goto is often used as shown here to deal with
errors.

The following sample code (using fictitious registration and unregistration functions) behaves correctly if initialization fails at any point:

```c
int __init my_init_function(void)
{
	int err;


	/* registration takes a pointer and a name */
	err = register_this(ptr1, "skull");
	if (err) goto fail_this;
	err = register_that(ptr2, "skull");
	if (err) goto fail_that;
	err = register_those(ptr3, "skull");
	if (err) goto fail_those;

	return 0; /* success */

	fail_those: unregister_that(ptr2, "skull");
	fail_that: unregister_this(ptr1, "skull");
	fail_this: return err; /* propagate the error */__
}
```

This code attempts to register three (fictitious) facilities. The goto statement is used
in case of failure to cause the unregistration of only the facilities that had been successfully registered before things went bad.

Another option, requiring no hairy goto statements, is keeping track of what has
been successfully registered and calling your module’s cleanup function in case of
any error. The cleanup function unrolls only the steps that have been successfully
accomplished. This alternative, however, requires more code and more CPU time, so
in fast paths you still resort to `goto` as the best error-recovery tool.

The return value of *my\_init\_function*, err , is an error code. In the Linux kernel, error
codes are negative numbers belonging to the set defined in *<linux/errno.h>*. If you
want to generate your own error codes instead of returning what you get from other
functions, you should include *<linux/errno.h>* in order to use symbolic values such
as `-ENODEV` , `-ENOMEM` , and so on. It is always good practice to return appropriate error
codes, because user programs can turn them to meaningful strings using *perror* or
similar means.

Obviously, the module cleanup function must undo any registration performed by
the initialization function, and it is customary (but not usually mandatory) to unreg-
ister facilities in the reverse order used to register them:

```c
void __exit my_cleanup_function(void)
{
	unregister_those(ptr3, "skull");
	unregister_that(ptr2, "skull");
	unregister_this(ptr1, "skull");
	return;
}//__
```

If your initialization and cleanup are more complex than dealing with a few items,
the goto approach may become difficult to manage, because all the cleanup code
must be repeated within the initialization function, with several labels intermixed.
Sometimes, therefore, a different layout of the code proves more successful.

What you’d do to minimize code duplication and keep everything streamlined is to
call the cleanup function from within the initialization whenever an error occurs.
The cleanup function then must check the status of each item before undoing its
registration. In its simplest form, the code looks like the following:

```c
struct something *item1;
struct somethingelse *item2;
int stuff_ok;

void my_cleanup(void)
{
	if (item1)
		release_thing(item1);
	if (item2)
		release_thing2(item2);
	if (stuff_ok)
		unregister_stuff( );
	return;
}

int __init my_init(void)
{
	int err = -ENOMEM;

	item1 = allocate_thing(arguments);
	item2 = allocate_thing2(arguments2);
	if (!item2 || !item2)
		goto fail;

	err = register_stuff(item1, item2);
	if (!err)
		stuff_ok = 1;
	else
		goto fail;
	return 0; /* success */*

	fail:
		my_cleanup( );
		return err;
}
```

As shown in this code, you may or may not need external flags to mark success of the
initialization step, depending on the semantics of the registration/allocation function
you call. Whether or not flags are needed, this kind of initialization scales well to a
large number of items and is often better than the technique shown earlier. Note,
however, that the cleanup function cannot be marked `__exit` when it is called by
nonexit code, as in the previous example.

###Module-Loading Races

Thus far, our discussion has skated over an important aspect of module loading: race
conditions. If you are not careful in how you write your initialization function, you
can create situations that can compromise the stability of the system as a whole. We
will discuss race conditions later in this book; for now, a couple of quick points will
have to suffice.

The first is that you should always remember that some other part of the kernel can
make use of any facility you register immediately after that registration has completed.
It is entirely possible, in other words, that the kernel will make calls into your
module while your initialization function is still running. So your code must be prepared
to be called as soon as it completes its first registration. Do not register any
facility until all of your internal initialization needed to support that facility has been
completed.

You must also consider what happens if your initialization function decides to fail,
but some part of the kernel is already making use of a facility your module has registered. If this situation is possible for your module, you should seriously consider not
failing the initialization at all. After all, the module has clearly succeeded in exporting something useful. If initialization must fail, it must carefully step around any possible operations going on elsewhere in the kernel until those operations have
completed.

##Module Parameters

Several parameters that a driver needs to know can change from system to system.
These can vary from the device number to use (as we’ll see in the next chapter) to
numerous aspects of how the driver should operate. For example, drivers for SCSI
adapters often have options controlling the use of tagged command queuing, and the
Integrated Device Electronics (IDE) drivers allow user control of DMA operations. If
your driver controls older hardware, it may also need to be told explicitly where to
find that hardware’s I/O ports or I/O memory addresses. The kernel supports these
needs by making it possible for a driver to designate parameters that may be changed
when the driver’s module is loaded.

These parameter values can be assigned at load time by *insmod* or *modprobe*; the latter
can also read parameter assignment from its configuration file (*/etc/modprobe.conf*).
The commands accept the specification of several types of values on the com-
mand line. As a way of demonstrating this capability, imagine a much-needed
enhancement to the “hello world” module (called *hellop*) shown at the beginning of
this chapter. We add two parameters: an integer value called `howmany` and a character
string called `whom`. Our vastly more functional module then, at load time, greets `whom`
not just once, but `howmany` times. Such a module could then be loaded with a command line such as:

```
insmod hellop howmany=10 whom="Mom"
```

Upon being loaded that way, *hellop* would say “Hello, Mom” 10 times.

However, before *insmod* can change module parameters, the module must make
them available. Parameters are declared with the `module_param` macro, which is
defined in *moduleparam.h*. `module_param` takes three parameters: the name of the
variable, its type, and a permissions mask to be used for an accompanying sysfs
entry. The macro should be placed outside of any function and is typically found
near the head of the source file. So *hellop* would declare its parameters and make
them available to *insmod* as follows:

```c
static char *whom = "world";//*
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);
```

Numerous types are supported for module parameters:

  * bool, invbool

	A boolean (true or false) value (the associated variable should be of type int ).
	The invbool type inverts the value, so that true values become false and vice
	versa.

  * charp
	A char pointer value. Memory is allocated for user-provided strings, and the
	pointer is set accordingly.

  * int

	long

    short

	uint

	ulong

	ushort

	Basic integer values of various lengths. The versions starting with `u` are for
	unsigned values.

Array parameters, where the values are supplied as a comma-separated list, are also
supported by the module loader. To declare an array parameter, use:

```c
module_param_array(name,type,num,perm);
```

Where name is the name of your array (and of the parameter), `type` is the type of the
array elements, num is an integer variable, and `perm` is the usual permissions value. If
the array parameter is set at load time, `num` is set to the number of values supplied.
The module loader refuses to accept more values than will fit in the array.

If you really need a type that does not appear in the list above, there are hooks in the
module code that allow you to define them; see *moduleparam.h* for details on how to
do that. All module parameters should be given a default value; *insmod* changes the
value only if explicitly told to by the user. The module can check for explicit parameters
by testing parameters against their default values.

The final *module_param* field is a permission value; you should use the definitions
found in *<linux/stat.h>*. This value controls who can access the representation of the
module parameter in sysfs. If perm is set to 0 , there is no sysfs entry at all; otherwise,
it appears under */sys/module* with the given set of permissions. Use `S_IRUGO` for a
parameter that can be read by the world but cannot be changed; `S_IRUGO|S_IWUSR`
allows root to change the parameter. Note that if a parameter is changed by sysfs, the
value of that parameter as seen by your module changes, but your module is not
notified in any other way. You should probably not make module parameters writable,
unless you are prepared to detect the change and react accordingly.


#File System Implementation

In this chapter, we introduce a simple file system implementation, known
as **vsfs** (the **Very Simple File System**). This file system is a simplified
version of a typical U NIX file system and thus serves to introduce some
of the basic on-disk structures, access methods, and various policies that
you will find in many file systems today.

The file system is pure software; unlike our development of CPU and
memory virtualization, we will not be adding hardware features to make
some aspect of the file system work better (though we will want to pay attention
to device characteristics to make sure the file system works well).
Because of the great flexibility we have in building a file system, many
different ones have been built, literally from AFS (the Andrew File System)
[H+88] to ZFS (Sun’s Zettabyte File System) [B07]. All of these file
systems have different data structures and do some things better or worse
than their peers. Thus, the way we will be learning about file systems is
through case studies: first, a simple file system (vsfs) in this chapter to
introduce most concepts, and then a series of studies of real file systems
to understand how they can differ in practice.

**How To Implement A Simple Filesystem**

How can we build a simple file system? What structures are needed
on the disk? What do they need to track? How are they accessed?

##The Way To Think

To think about file systems, we usually suggest thinking about two
different aspects of them; if you understand both of these aspects, you
probably understand how the file system basically works.

The first is the **data structures** of the file system. In other words, what
types of on-disk structures are utilized by the file system to organize its
data and metadata? The first file systems we’ll see (including vsfs below)
employ simple structures, like arrays of blocks or other objects, whereas
more sophisticated file systems, like SGI’s XFS, use more complicated
tree-based structures [S+96].

**MENTAL MODELS OF FILE SYSTEMS**

mental models are what you are really trying
to develop when learning about systems. For file systems, your mental
model should eventually include answers to questions like: what on-disk
structures store the file system’s data and metadata? What happens when
a process opens a file? Which on-disk structures are accessed during a
read or write? By working on and improving your mental model, you
develop an abstract understanding of what is going on, instead of just
trying to understand the specifics of some file-system code (though that
is also useful, of course!).

The second aspect of a file system is its **access methods**. How does
it map the calls made by a process, such as `open()`, `read()`, `write()`,
etc., onto its structures? Which structures are read during the execution
of a particular system call? Which are written? How efficiently are all of
these steps performed?

If you understand the data structures and access methods of a file system,
you have developed a good mental model of how it truly works, a
key part of the systems mindset. Try to work on developing your mental
model as we delve into our first implementation.

##Overall Organization

We now develop the overall on-disk organization of the data struc-
tures of the vsfs file system. The first thing we’ll need to do is divide the
disk into **blocks**; simple file systems use just one block size, and that’s
exactly what we’ll do here. Let’s choose a commonly-used size of 4 KB.

Thus, our view of the disk partition where we’re building our file system is simple:
a series of blocks, each of size 4 KB. The blocks are addressed from `0` to `N − 1`, in a partition of size `N` 4-KB blocks. Assume we have a really small disk, with just 64 blocks:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_2.png)

Let’s now think about what we need to store in these blocks to build
a file system. Of course, the first thing that comes to mind is user data.
In fact, most of the space in any file system is (and should be) user data.
Let’s call the region of the disk we use for user data the **data region**, and,
again for simplicity, reserve a fixed portion of the disk for these blocks,
say the last 56 of 64 blocks on the disk:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_3.png)

As we learned about (a little) last chapter, the file system has to track
information about each file. This information is a key piece of **metadata**,
and tracks things like which data blocks (in the data region) comprise a
file, the size of the file, its owner and access rights, access and modify
times, and other similar kinds of information. To store this information,
file systems usually have a structure called an **inode** (we’ll read more
about inodes below).

To accommodate inodes, we’ll need to reserve some space on the disk
for them as well. Let’s call this portion of the disk the **inode table**, which
simply holds an array of on-disk inodes. Thus, our on-disk image now
looks like this picture, assuming that we use 5 of our 64 blocks for inodes
(denoted by I’s in the diagram):

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_4.png)

We should note here that inodes are typically not that big, for example
128 or 256 bytes. Assuming 256 bytes per inode, a 4-KB block can hold 16
inodes, and our file system above contains 80 total inodes. In our simple
file system, built on a tiny 64-block partition, this number represents the
maximum number of files we can have in our file system; however, do
note that the same file system, built on a larger disk, could simply allocate
a larger inode table and thus accommodate more files.

Our file system thus far has data blocks (D), and inodes (I), but a few
things are still missing. One primary component that is still needed, as
you might have guessed, is some way to track whether inodes or data
blocks are free or allocated. Such **allocation structures** are thus a requisite
element in any file system.

Many allocation-tracking methods are possible, of course. For exam-
ple, we could use a **free list** that points to the first free block, which then
points to the next free block, and so forth. We instead choose a simple and
popular structure known as a **bitmap**, one for the data region (the **data
bitmap**), and one for the inode table (the **inode bitmap**). A bitmap is a
simple structure: each bit is used to indicate whether the corresponding
object/block is free (0) or in-use (1). And thus our new on-disk layout,
with an inode bitmap (i) and a data bitmap (d):

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_5.png)

You may notice that it is a bit of overkill to use an entire 4-KB block for
these bitmaps; such a bitmap can track whether 32K objects are allocated,
and yet we only have 80 inodes and 56 data blocks. However, we just use
an entire 4-KB block for each of these bitmaps for simplicity.

The careful reader (i.e., the reader who is still awake) may have no-
ticed there is one block left in the design of the on-disk structure of our
very simple file system. We reserve this for the **superblock**, denoted by
an S in the diagram below. The superblock contains information about
this particular file system, including, for example, how many inodes and
data blocks are in the file system (80 and 56, respectively in this instance),
where the inode table begins (block 3), and so forth. It will likely also
include a magic number of some kind to identify the file system type (in
this case, vsfs).

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_6.png)

Thus, when mounting a file system, the operating system will read
the superblock first, to initialize various parameters, and then attach the
volume to the file-system tree. When files within the volume are accessed,
the system will thus know exactly where to look for the needed on-disk
structures.

##File Organization: The Inode

One of the most important on-disk structures of a file system is the
**inode**; virtually all file systems have a structure similar to this. The name
inode is short for **index node**, the historical name given to it by UNIX inventor Ken Thompson [RT74], used because these nodes were originally
arranged in an array, and the array *indexed* into when accessing a particular inode.

**ASIDE: DATA STRUCTURE — THE INODE**

The **inode** is the generic name that is used in many file systems to describe
the structure that holds the metadata for a given file, such as its
length, permissions, and the location of its constituent blocks. The name
goes back at least as far as UNIX (and probably further back to Multics
if not earlier systems); it is short for **index node**, as the inode number is
used to index into an array of on-disk inodes in order to find the inode
of that number. As we’ll see, design of the inode is one key part of file
system design. Most modern systems have some kind of structure like
this for every file they track, but perhaps call them different things (such
as dnodes, fnodes, etc.).

Each inode is implicitly referred to by a number (called the **inumber**),
which we’ve earlier called the **low-level name** of the file. In vsfs (and
other simple file systems), given an i-number, you should directly be able
to calculate where on the disk the corresponding inode is located. For ex-
ample, take the inode table of vsfs as above: 20-KB in size (5 4-KB blocks)
and thus consisting of 80 inodes (assuming each inode is 256 bytes); fur-
ther assume that the inode region starts at 12KB (i.e, the superblock starts
at 0KB, the inode bitmap is at address 4KB, the data bitmap at 8KB, and
thus the inode table comes right after). In vsfs, we thus have the following
layout for the beginning of the file system partition (in closeup view):

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_7.png)

To read inode number 32, the file system would first calculate the offset into
the inode region (`32 · sizeof (inode)` or 8192), add it to the start
address of the inode table on disk (`inodeStartAddr` = 12KB), and thus
arrive upon the correct byte address of the desired block of inodes: 20KB.
Recall that disks are not byte addressable, but rather consist of a large
number of addressable sectors, usually 512 bytes. Thus, to fetch the block
of inodes that contains inode 32, the file system would issue a read to sector
`20×1024/512`, or 40, to fetch the desired inode block. More generally, the
sector address `iaddr` of the inode block can be calculated as follows:

```c
blk = (inumber * sizeof(inode_t)) / blockSize;
sector = ((blk * blockSize) + inodeStartAddr) / sectorSize;
```

Inside each inode is virtually all of the information you need about a
file: its *type* (e.g., regular file, directory, etc.), its *size*, the number of
*blocks* allocated to it, *protection information* (such as who owns the file, as well
as who can access it), some *time* information, including when the file was
created, modified, or last accessed, as well as information about where its
data blocks reside on disk (e.g., pointers of some kind). We refer to all
such information about a file as **metadata**; in fact, any information inside
the file system that isn’t pure user data is often referred to as such. An
example inode from ext2 [P09] is shown in Figure below.


| Size  |  Name   |  What is this inode field for? |
------|--------------|-------------------------------------
|  2   | mode         | can this file be read/written/executed? |
|  2   | uid          | who owns this file? |
|  4   | size         | how many bytes are in this file? |
|  4   | time         | what time was this file last accessed? |
|  4   | ctime        | what time was this file created? |
|  4   | mtime        | what time was this file last modified? |
|  4   | dtime        | what time was this inode deleted? |
|  2   | gid          | which group does this file belong to? |
|  2   | links count  | how many hard links are there to this file? |
|  4   | blocks       | how many blocks have been allocated to this file? |
|  4   | flags        | how should ext2 use this inode? |
|  4   | osd1         | an OS-dependent field |
|  60  | block        | a set of disk pointers (15 total) |
|  4   | generation   | file version (used by NFS) |
|  4   | file acl     | a new permissions model beyond mode bits |
|  4   | dir acl      | called access control lists |
|  4   | faddr        | an unsupported field |
|  12  | i\_osd2       | another OS-dependent field |


*The Ext2 Inode*

One of the most important decisions in the design of the inode is how
it refers to where data blocks are. One simple approach would be to
have one or more **direct pointers** (disk addresses) inside the inode; each
pointer refers to one disk block that belongs to the file. Such an approach
is limited: for example, if you want to have a file that is really big (e.g.,
bigger than the size of a block multiplied by the number of direct point-
ers), you are out of luck.

##The Multi-Level Index

To support bigger files, file system designers have had to introduce dif-
ferent structures within inodes. One common idea is to have a special
pointer known as an **indirect pointer**. Instead of pointing to a block that
contains user data, it points to a block that contains more pointers, each
of which point to user data. Thus, an inode may have some fixed number
of direct pointers (e.g., 12), and a single indirect pointer. If a file grows
large enough, an indirect block is allocated (from the data-block region of
the disk), and the inode’s slot for an indirect pointer is set to point to it.
Assuming 4-KB blocks and 4-byte disk addresses, that adds another 1024
pointers; the file can grow to be (12 + 1024) · 4K or 4144KB.

**TIP : CONSIDER EXTENT - BASED APPROACHES**

A different approach is to use **extents** instead of pointers. An extent is
simply a disk pointer plus a length (in blocks); thus, instead of requiring
a pointer for every block of a file, all one needs is a pointer and a length
to specify the on-disk location of a file. Just a single extent is limiting, as
one may have trouble finding a contiguous chunk of on-disk free space
when allocating a file. Thus, extent-based file systems often allow for
more than one extent, thus giving more freedom to the file system during
file allocation.

In comparing the two approaches, pointer-based approaches are the most
flexible but use a large amount of metadata per file (particularly for large
files). Extent-based approaches are less flexible but more compact; in par-
ticular, they work well when there is enough free space on the disk and
files can be laid out contiguously (which is the goal for virtually any file
allocation policy anyhow).

**end tip**

Not surprisingly, in such an approach, you might want to support
even larger files. To do so, just add another pointer to the inode:
the **double indirect pointer**. This pointer refers to a block that contains pointers
to indirect blocks, each of which contain pointers to data blocks. A dou-
ble indirect block thus adds the possibility to grow files with an additional
1024 · 1024 or 1-million 4KB blocks, in other words supporting files that
are over 4GB in size. You may want even more, though, and we bet you
know where this is headed: the **triple indirect pointer**.

Overall, this imbalanced tree is referred to as the multi-level index ap-
proach to pointing to file blocks. Let’s examine an example with twelve
direct pointers, as well as both a single and a double indirect block. As-
suming a block size of 4 KB, and 4-byte pointers, this structure can accom-
modate a file of just over 4 GB in size (i.e., (12 + 1024 + 1024^2^ ) × 4 KB).
Can you figure out how big of a file can be handled with the addition of
a triple-indirect block? (hint: pretty big)

Many file systems use a multi-level index, including commonly-used
file systems such as Linux ext2 [P09] and ext3, NetApp’s WAFL, as well as
the original UNIX file system. Other file systems, including SGI XFS and
Linux ext4, use **extents** instead of simple pointers; see the earlier aside for
details on how extent-based schemes work (they are akin to segments in
the discussion of virtual memory).

You might be wondering: why use an imbalanced tree like this? Why
not a different approach? Well, as it turns out, many researchers have
studied file systems and how they are used, and virtually every time they
find certain “truths” that hold across the decades. One such finding is
that *most files are small*. This imbalanced design reflects such a reality; if
most files are indeed small, it makes sense to optimize for this case. Thus,
with a small number of direct pointers (12 is a typical number), an inode
can directly point to 48 KB of data, needing one (or more) indirect blocks
for larger files. See Agrawal et. al [A+07] for a recent study; Figure 40.2
summarizes those results.

Of course, in the space of inode design, many other possibilities exist;
after all, the inode is just a data structure, and any data structure that
stores the relevant information, and can query it effectively, is sufficient.
As file system software is readily changed, you should be willing to ex-
plore different designs should workloads or technologies change.


|            Statement            |               Description
----------------------------------|----------------------------------------
|  **Most files are small**                   | Roughly 2K is the most common size |
|  **Average file size is growing**           | Almost 200K is the average |
|  **Most bytes are stored in large files**   | A few big files use most of the space |
|  **File systems contains lots of files**    | Almost 100K on average |
|  **File systems are roughly half full**     | Even as disks grow, file systems remain ˜50% full |
|  **Directories are typically small**        | Many have few entries; most have 20 or fewer |
----------------------------------|----------------------------------------------

*File System Measurement Summary*

**ASIDE : LINKED-BASED APPROACHES**

Another simpler approach in designing inodes is to use a **linked list**.
Thus, inside an inode, instead of having multiple pointers, you just need
one, to point to the first block of the file. To handle larger files, add an-
other pointer at the end of that data block, and so on, and thus you can
support large files.

As you might have guessed, linked file allocation performs poorly for
some workloads; think about reading the last block of a file, for example,
or just doing random access. Thus, to make linked allocation work better,
some systems will keep an in-memory table of link information, instead
of storing the next pointers with the data blocks themselves. The table
is indexed by the address of a data block D; the content of an entry is
simply D’s next pointer, i.e., the address of the next block in a file which
follows D. A null-value could be there too (indicating an end-of-file), or
some other marker to indicate that a particular block is free. Having such
a table of next pointers makes it so that a linked allocation scheme can
effectively do random file accesses, simply by first scanning through the
(in memory) table to find the desired block, and then accessing (on disk)
it directly.

Does such a table sound familiar? What we have described is the basic
structure of what is known as the **file allocation table**, or **FAT** file system.
Yes, this classic old Windows file system, before NTFS [C94], is based on a
simple linked-based allocation scheme. There are other differences from
a standard UNIX file system too; for example, there are no inodes per se,
but rather directory entries which store metadata about a file and refer
directly to the first block of said file, which makes creating hard links
impossible. See Brouwer [B02] for more of the inelegant details.

##Directory Organization

In vsfs (as in many file systems), directories have a simple organiza-
tion; a directory basically just contains a list of (entry name, inode num-
ber) pairs. For each file or directory in a given directory, there is a string
and a number in the data block(s) of the directory. For each string, there
may also be a length (assuming variable-sized names).

For example, assume a directory `dir` (inode number 5) has three files
in it (`foo`, `bar`, and `foobar`), and their inode numbers are 12, 13, and 24
respectively. The on-disk data for `dir` might look like this:

| inum | reclen | strlen | name |
-----|--------|--------|-----
  5  |  4 |  2   | .
  2  |  4 |  3   | ..
  12 |  4 |  4   | foo
  13 |  4 |  4   | bar
  24 |  8 |  7   | foobar

In this example, each entry has an inode number, record length (the
total bytes for the name plus any left over space), string length (the actual
length of the name), and finally the name of the entry. Note that each di-
rectory has two extra entries, . “dot” and .. “dot-dot”; the dot directory
is just the current directory (in this example, `dir`), whereas dot-dot is the
parent directory (in this case, the root).

Deleting a file (e.g., calling `unlink()`) can leave an empty space in
the middle of the directory, and hence there should be some way to mark
that as well (e.g., with a reserved inode number such as zero). Such a
delete is one reason the record length is used: a new entry may reuse an
old, bigger entry and thus have extra space within.

You might be wondering where exactly directories are stored. Often,
file systems treat directories as a special type of file. Thus, a directory has
an inode, somewhere in the inode table (with the type field of the inode
marked as “directory” instead of “regular file”). The directory has data
blocks pointed to by the inode (and perhaps, indirect blocks); these data
blocks live in the data block region of our simple file system. Our on-disk
structure thus remains unchanged.

We should also note again that this simple linear list of directory en-
tries is not the only way to store such information. As before, any data
structure is possible. For example, XFS [S+96] stores directories in B-tree
form, making file create operations (which have to ensure that a file name
has not been used before creating it) faster than systems with simple lists
that must be scanned in their entirety.

##Free Space Management

A file system must track which inodes and data blocks are free, and
which are not, so that when a new file or directory is allocated, it can find
space for it. Thus **free space management** is important for all file systems.
In vsfs, we have two simple bitmaps for this task.

For example, when we create a file, we will have to allocate an inode
for that file. The file system will thus search through the bitmap for an in-
ode that is free, and allocate it to the file; the file system will have to mark
the inode as used (with a 1) and eventually update the on-disk bitmap
with the correct information. A similar set of activities take place when a
data block is allocated.

Some other considerations might also come into play when allocating
data blocks for a new file. For example, some Linux file systems, such
as ext2 and ext3, will look for a sequence of blocks (say 8) that are free
when a new file is created and needs data blocks; by finding such a se-
quence of free blocks, and then allocating them to the newly-created file,
the file system guarantees that a portion of the file will be on the disk and
contiguous, thus improving performance. Such a **pre-allocation** policy is
thus a commonly-used heuristic when allocating space for data blocks.

**ASIDE : FREE SPACE MANAGEMENT**

There are many ways to manage free space; bitmaps are just one way.
Some early file systems used **free lists**, where a single pointer in the super
block was kept to point to the first free block; inside that block the next
free pointer was kept, thus forming a list through the free blocks of the
system. When a block was needed, the head block was used and the list
updated accordingly.

Modern file systems use more sophisticated data structures. For example,
SGI’s XFS [S+96] uses some form of a **B-tree** to compactly represent which
chunks of the disk are free. As with any data structure, different timespace trade-offs
are possible.

##Access Paths: Reading and Writing

Now that we have some idea of how files and directories are stored on
disk, we should be able to follow the flow of operation during the activity
of reading or writing a file. Understanding what happens on this **access
path** is thus the second key in developing an understanding of how a file
system works; pay attention!

For the following examples, let us assume that the file system has been
mounted and thus that the superblock is already in memory. Everything
else (i.e., inodes, directories) is still on the disk.

**Reading A File From Disk**

In this simple example, let us first assume that you want to simply open
a file (e.g., `/foo/bar`, read it, and then close it. For this simple example,
let’s assume the file is just 4KB in size (i.e., 1 block).

When you issue an `open("/foo/bar", O RDONLY)` call, the file system first needs
to find the inode for the file bar, to obtain some basic information about the file
(permissions information, file size, etc.). To do so,
the file system must be able to find the inode, but all it has right now is
the full pathname. The file system must **traverse** the pathname and thus
locate the desired inode.

All traversals begin at the root of the file system, in the **root directory**
which is simply called /. Thus, the first thing the FS will read from disk
is the inode of the root directory. But where is this inode? To find an
inode, we must know its i-number. Usually, we find the i-number of a file
or directory in its parent directory; the root has no parent (by definition).
Thus, the root inode number must be “well known”; the FS must know
what it is when the file system is mounted. In most U NIX file systems,
the root inode number is 2. Thus, to begin the process, the FS reads in the
block that contains inode number 2 (the first inode block).

Once the inode is read in, the FS can look inside of it to find pointers to
data blocks, which contain the contents of the root directory. The FS will
thus use these on-disk pointers to read through the directory, in this case
looking for an entry for `foo`. By reading in one or more directory data
blocks, it will find the entry for foo; once found, the FS will also have
found the inode number of `foo` (say it is 44) which it will need next.

The next step is to recursively traverse the pathname until the desired
inode is found. In this example, the FS reads the block containing the
inode of `foo` and then its directory data, finally finding the inode number
of bar. The final step of `open()` is to read bar’s inode into memory; the
FS then does a final permissions check, allocates a file descriptor for this
process in the per-process open-file table, and returns it to the user.

Once open, the program can then issue a `read()` system call to read
from the file. The first read (at offset 0 unless `lseek()` has been called)
will thus read in the first block of the file, consulting the inode to find
the location of such a block; it may also update the inode with a new last-
accessed time. The read will further update the in-memory open file table
for this file descriptor, updating the file offset such that the next read will
read the second file block, etc.

At some point, the file will be closed. There is much less work to be
done here; clearly, the file descriptor should be deallocated, but for now,
that is all the FS really needs to do. No disk I/Os take place.

A depiction of this entire process is found in Figure below (time increases
downward).

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_8.png)

*File Read Timeline (Time Increasing Downward)*

In the figure, the open causes numerous reads to take place
in order to finally locate the inode of the file. Afterwards, reading each
block requires the file system to first consult the inode, then read the
block, and then update the inode’s last-accessed-time field with a write.
Spend some time and try to understand what is going on.

Also note that the amount of I/O generated by the open is proportional to the
length of the pathname. For each additional directory in the
path, we have to read its inode as well as its data. Making this worse
would be the presence of large directories; here, we only have to read one
block to get the contents of a directory, whereas with a large directory, we
might have to read many data blocks to find the desired entry. Yes, life
can get pretty bad when reading a file; as you’re about to find out, writing
out a file (and especially, creating a new one) is even worse.

**ASIDE : READS DON’T ACCESS ALLOCATION STRUCTURES**

We’ve seen many students get confused by allocation structures such
as bitmaps. In particular, many often think that when you are simply
reading a file, and not allocating any new blocks, that the bitmap will still
be consulted. This is not true! Allocation structures, such as bitmaps,
are only accessed when allocation is needed. The inodes, directories, and
indirect blocks have all the information they need to complete a read request;
there is no need to make sure a block is allocated when the inode
already points to it.

##Writing to Disk

Writing to a file is a similar process. First, the file must be opened (as
above). Then, the application can issue `write()` calls to update the file
with new contents. Finally, the file is closed.

Unlike reading, writing to the file may also **allocate** a block (unless
the block is being overwritten, for example). When writing out a new
file, each write not only has to write data to disk but has to first decide
which block to allocate to the file and thus update other structures of the
disk accordingly (e.g., the data bitmap and inode). Thus, each write to a
file logically generates five I/Os: one to read the data bitmap (which is
then updated to mark the newly-allocated block as used), one to write the
bitmap (to reflect its new state to disk), two more to read and then write
the inode (which is updated with the new block’s location), and finally
one to write the actual block itself.

The amount of write traffic is even worse when one considers a sim-
ple and common operation such as file creation. To create a file, the file
system must not only allocate an inode, but also allocate space within
the directory containing the new file. The total amount of I/O traffic to
do so is quite high: one read to the inode bitmap (to find a free inode),
one write to the inode bitmap (to mark it allocated), one write to the new
inode itself (to initialize it), one to the data of the directory (to link the
high-level name of the file to its inode number), and one read and write
to the directory inode to update it. If the directory needs to grow to ac-
commodate the new entry, additional I/Os (i.e., to the data bitmap, and
the new directory block) will be needed too. All that just to create a file!

Let’s look at a specific example, where the file `/foo/bar` is created,
and three blocks are written to it. Figure below shows what happens during
the `open()` (which creates the file) and during each of three 4KB writes.

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_9.png)

*File Creation Timeline (Time Increasing Downward)*

In the figure, reads and writes to the disk are grouped under which
system call caused them to occur, and the rough ordering they might take
place in goes from top to bottom of the figure. You can see how much
work it is to create the file: 10 I/Os in this case, to walk the pathname
and then finally create the file. You can also see that each allocating write
costs 5 I/Os: a pair to read and update the inode, another pair to read
and update the data bitmap, and then finally the write of the data itself.
How can a file system accomplish any of this with reasonable efficiency?


**HOW TO REDUCE FILE SYSTEM I/O COSTS**

Even the simplest of operations like opening, reading, or writing a file
incurs a huge number of I/O operations, scattered over the disk. What
can a file system do to reduce the high costs of doing so many I/Os?**

##Caching and Buffering

As the examples above show, reading and writing files can be expen-
sive, incurring many I/Os to the (slow) disk. To remedy what would
clearly be a huge performance problem, most file systems aggressively
use system memory (DRAM) to cache important blocks.

Imagine the open example above: without caching, every file open
would require at least two reads for every level in the directory hierarchy
(one to read the inode of the directory in question, and at least one to read
its data). With a long pathname (e.g., `/1/2/3/ ... /100/file.txt`), the file
system would literally perform hundreds of reads just to open the file!

Early file systems thus introduced a **fixed-size cache** to hold popular
blocks. As in our discussion of virtual memory, strategies such as **LRU**
and different variants would decide which blocks to keep in cache. This
fixed-size cache would usually be allocated at boot time to be roughly
10% of total memory.

This **static partitioning** of memory, however, can be wasteful; what
if the file system doesn’t need 10% of memory at a given point in time?
With the fixed-size approach described above, unused pages in the file
cache cannot be re-purposed for some other use, and thus go to waste.

Modern systems, in contrast, employ a **dynamic partitioning** approach.
Specifically, many modern operating systems integrate virtual memory
pages and file system pages into a **unified page cache** [S00]. In this way,
memory can be allocated more flexibly across virtual memory and file
system, depending on which needs more memory at a given time.

Now imagine the file open example with caching. The first open may
generate a lot of I/O traffic to read in directory inode and data, but sub-
sequent file opens of that same file (or files in the same directory) will
mostly hit in the cache and thus no I/O is needed.

Let us also consider the effect of caching on writes. Whereas read I/O
can be avoided altogether with a sufficiently large cache, write traffic has
to go to disk in order to become persistent. Thus, a cache does not serve
as the same kind of filter on write traffic that it does for reads. That said,
**write buffering** (as it is sometimes called) certainly has a number of
performance benefits. First, by delaying writes, the file system can **batch**
some updates into a smaller set of I/Os; for example, if an inode bitmap
is updated when one file is created and then updated moments later as
another file is created, the file system saves an I/O by delaying the write
after the first update. Second, by buffering a number of writes in memory,
the system can then **schedule** the subsequent I/Os and thus increase per-
formance. Finally, some writes are avoided altogether by delaying them;
for example, if an application creates a file and then deletes it, delaying
the writes to reflect the file creation to disk **avoids** them entirely. In this
case, laziness (in writing blocks to disk) is a virtue.

**TIP : UNDERSTAND STATIC VS. DYNAMIC PARTITIONING**

When dividing a resource among different clients/users, you can use
either **static partitioning** or **dynamic partitioning**. The static approach
simply divides the resource into fixed proportions once; for example, if
there are two possible users of memory, you can give some fixed fraction
of memory to one user, and the rest to the other. The dynamic approach
is more flexible, giving out differing amounts of the resource over time;
for example, one user may get a higher percentage of disk bandwidth for
a period of time, but then later, the system may switch and decide to give
a different user a larger fraction of available disk bandwidth.

Each approach has its advantages. Static partitioning ensures each user
some share of the resource, usually delivers more predictable perfor-
mance, and is often easier to implement. Dynamic partitioning can
achieve better utilization (by letting resource-hungry users consume oth-
erwise idle resources), but can be more complex to implement, and can
lead to worse performance for users whose idle resources get consumed
by others and then take a long time to reclaim when needed. As is of-
ten the case, there is no best method; rather, you should think about the
problem at hand and decide which approach is most suitable. Indeed,
shouldn’t you always be doing that?

**end tip**

For the reasons above, most modern file systems buffer writes in mem-
ory for anywhere between five and thirty seconds, representing yet an-
other trade-off: if the system crashes before the updates have been prop-
agated to disk, the updates are lost; however, by keeping writes in mem-
ory longer, performance can be improved by batching, scheduling, and
even avoiding writes.

Some applications (such as databases) don’t enjoy this trade-off. Thus,
to avoid unexpected data loss due to write buffering, they simply force
writes to disk, by calling `fsync()`, by using **direct I/O** interfaces that
work around the cache, or by using the **raw disk** interface and avoiding
the file system altogether 1 . While most applications live with the trade-
offs made by the file system, there are enough controls in place to get the
system to do what you want it to, should the default not be satisfying.

**TIP : UNDERSTAND THE DURABILITY / PERFORMANCE TRADE-OFF**

Storage systems often present a durability/performance trade-off to
users. If the user wishes data that is written to be immediately durable,
the system must go through the full effort of committing the newly-
written data to disk, and thus the write is slow (but safe). However, if
the user can tolerate the loss of a little data, the system can buffer writes
in memory for some time and write them later to the disk (in the back-
ground). Doing so makes writes appear to complete quickly, thus im-
proving perceived performance; however, if a crash occurs, writes not
yet committed to disk will be lost, and hence the trade-off. To understand
how to make this trade-off properly, it is best to understand what the ap-
plication using the storage system requires; for example, while it may be
tolerable to lose the last few images downloaded by your web browser,
losing part of a database transaction that is adding money to your bank
account may be less tolerable. Unless you’re rich, of course; in that case,
why do you care so much about hoarding every last penny?

##Summary

We have seen the basic machinery required in building a file system.
There needs to be some information about each file (metadata), usually
stored in a structure called an inode. Directories are just a specific type
of file that store name→inode-number mappings. And other structures
are needed too; for example, file systems often use a structure such as a
bitmap to track which inodes or data blocks are free or allocated.

The terrific aspect of file system design is its freedom; the file systems
we explore in the coming chapters each take advantage of this freedom
to optimize some aspect of the file system. There are also clearly many
policy decisions we have left unexplored. For example, when a new file
is created, where should it be placed on disk? This policy and others will
also be the subject of future chapters. Or will they?

#Locality and The Fast File System

When the UNIX operating system was first introduced, the UNIX wizard
himself Ken Thompson wrote the first file system. Let’s call that the “old
UNIX file system”, and it was really simple. Basically, its data structures
looked like this on the disk:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_10.png)

The super block (S) contained information about the entire file system:
how big the volume is, how many inodes there are, a pointer to the head
of a free list of blocks, and so forth. The inode region of the disk contained
all the inodes for the file system. Finally, most of the disk was taken up
by data blocks.

The good thing about the old file system was that it was simple, and
supported the basic abstractions the file system was trying to deliver: files
and the directory hierarchy. This easy-to-use system was a real step for-
ward from the clumsy, record-based storage systems of the past, and the
directory hierarchy was a true advance over simpler, one-level hierarchies
provided by earlier systems.

##The Problem: Poor Performance

The problem: performance was terrible. As measured by Kirk McKusick and his
colleagues at Berkeley [MJLF84], performance started off bad
and got worse over time, to the point where the file system was delivering
only 2% of overall disk bandwidth!

The main issue was that the old U NIX file system treated the disk like it
was a random-access memory; data was spread all over the place without
regard to the fact that the medium holding the data was a disk, and thus
had real and expensive positioning costs. For example, the data blocks of
a file were often very far away from its inode, thus inducing an expensive
seek whenever one first read the inode and then the data blocks of a file
(a pretty common operation).

Worse, the file system would end up getting quite **fragmented**, as the
free space was not carefully managed. The free list would end up point-
ing to a bunch of blocks spread across the disk, and as files got allocated,
they would simply take the next free block. The result was that a logi-
cally contiguous file would be accessed by going back and forth across
the disk, thus reducing performance dramatically.

For example, imagine the following data block region, which contains
four files (A, B, C, and D), each of size 2 blocks:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_11.png)

If B and D are deleted, the resulting layout is:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_12.png)

As you can see, the free space is fragmented into two chunks of two
blocks, instead of one nice contiguous chunk of four. Let’s say you now
wish to allocate a file E, of size four blocks:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_13.png)

You can see what happens: E gets spread across the disk, and as a
result, when accessing E, you don’t get peak (sequential) performance
from the disk. Rather, you first read E1 and E2, then seek, then read E3
and E4. This fragmentation problem happened all the time in the old
UNIX file system, and it hurt performance. (A side note: this problem is
exactly what disk defragmentation tools help with; they will reorganize
on-disk data to place files contiguously and make free space one or a few
contiguous regions, moving data around and then rewriting inodes and
such to reflect the changes)

One other problem: the original block size was too small (512 bytes).
Thus, transferring data from the disk was inherently inefficient. Smaller
blocks were good because they minimized **internal fragmentation** (waste
within the block), but bad for transfer as each block might require a positioning
overhead to reach it. Thus, the problem:

**HOW TO ORGANIZE ON-DISK DATA TO IMPROVE PERFORMANCE**

How can we organize file system data structures so as to improve per-
formance? What types of allocation policies do we need on top of those
data structures? How do we make the file system “disk aware”?

##FFS: Disk Awareness Is The Solution

A group at Berkeley decided to build a better, faster file system, which
they cleverly called the **Fast File System (FFS)**. The idea was to design
the file system structures and allocation policies to be “disk aware” and
thus improve performance, which is exactly what they did. FFS thus ush-
ered in a new era of file system research; by keeping the same interface
to the file system (the same APIs, including `open()`, `read()`, `write()`,
`close()`, and other file system calls) but changing the internal *implementation*,
the authors paved the path for new file system construction, work
that continues today. Virtually all modern file systems adhere to the ex-
isting interface (and thus preserve compatibility with applications) while
changing their internals for performance, reliability, or other reasons.

##Organizing Structure: The Cylinder Group

The first step was to change the on-disk structures. FFS divides the
disk into a bunch of groups known as **cylinder groups** (some modern file
systems like Linux ext2 and ext3 just call them **block groups**). Imagine a
disk with ten cylinder groups:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_14.png)

These groups are the central mechanism that FFS uses to improve per-
formance; by placing two files within the same group, FFS can ensure that
accessing one after the other will not result in long seeks across the disk.

Thus, FFS needs to have the ability to allocate files and directories
within each of these groups. Each group looks like this:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_15.png)

Let’s now examine the components of a cylinder group. A copy of the
**super block** (S) is found in each group for reliability reasons (e.g., if one
gets corrupted or scratched, you can still mount and access the file system
by using one of the others).

Within each group, FFS needs to track whether the inodes and data
blocks of the group are allocated. A per-group **inode bitmap** (ib) and
**data bitmap** (db) serve this role for inodes and data blocks in each group.
Bitmaps are an excellent way to manage free space in a file system be-
cause it is easy to find a large chunk of free space and allocate it to a file,
perhaps avoiding some of the fragmentation problems of the free list in
the old file system.

Finally, the inode and data block regions are just like those in the previous
very-simple file system (VSFS). Most of each cylinder group, as usual,
is comprised of data blocks.

**ASIDE : FFS FILE CREATION**

As an example, think about what data structures must be updated when
a file is created; assume, for this example, that the user creates a new file
`/foo/bar.txt` and that the file is one block long (4KB). The file is new,
and thus needs a new inode; thus, both the inode bitmap and the newly-allocated
inode will be written to disk. The file also has data in it and
thus it too must be allocated; the data bitmap and a data block will thus
(eventually) be written to disk. Hence, at least four writes to the current
cylinder group will take place (recall that these writes may be buffered
in memory for a while before they take place). But this is not all! In
particular, when creating a new file, you must also place the file in the
file-system hierarchy, i.e., the directory must be updated. Specifically, the
parent directory `foo` must be updated to add the entry for `bar.txt`; this
update may fit in an existing data block of `foo` or require a new block to
be allocated (with associated data bitmap). The inode of `foo` must also
be updated, both to reflect the new length of the directory as well as to
update time fields (such as last-modified-time). Overall, it is a lot of work
just to create a new file! Perhaps next time you do so, you should be more
thankful, or at least surprised that it all works so well.

##Policies: How To Allocate Files and Directories

With this group structure in place, FFS now has to decide how to place
files and directories and associated metadata on disk to improve performance.
The basic mantra is simple: *keep related stuff together* (and its corollary,
keep unrelated stuff far apart).

Thus, to obey the mantra, FFS has to decide what is “related” and
place it within the same block group; conversely, unrelated items should
be placed into different block groups. To achieve this end, FFS makes use
of a few simple placement heuristics.

The first is the placement of directories. FFS employs a simple approach:
find the cylinder group with a low number of allocated directories
(to balance directories across groups) and a high number of free
inodes (to subsequently be able to allocate a bunch of files), and put the
directory data and inode in that group. Of course, other heuristics could
be used here (e.g., taking into account the number of free data blocks).

For files, FFS does two things. First, it makes sure (in the general case)
to allocate the data blocks of a file in the same group as its inode, thus
preventing long seeks between inode and data (as in the old file system).
Second, it places all files that are in the same directory in the cylin-
der group of the directory they are in. Thus, if a user creates four files,
`/dir1/1.txt`, `/dir1/2.txt`, `/dir1/3.txt`, and `/dir99/4.txt`, FFS
would try to place the first three near one another (same group) and the
fourth far away (in some other group).

These heuristics are not based on extensive studies of file-system traffic or
anything particularly nuanced; rather, they are based on good old-
fashioned common sense (isn’t that what CS stands for after all?). Files in
a directory *are* often accessed together (imagine compiling a bunch of files
and then linking them into a single executable). Because they are, FFS will
often improve performance, making sure that seeks between related files
are short.

##Measuring File Locality

To understand better whether these heuristics make sense, let’s analyze some traces
of file system access and see if indeed there is namespace
locality. For some reason, there doesn’t seem to be a good study of this
topic in the literature.

Specifically, we’ll use the SEER traces [K94] and analyze how “far
away” file accesses were from one another in the directory tree. For example,
if file `f` is opened, and then re-opened next in the trace (before
any other files are opened), the distance between these two opens in the
directory tree is zero (as they are the same file). If a file `f` in directory
`dir` (i.e., `dir/f`) is opened, and followed by an open of file `g` in the same
directory (i.e., `dir/g`), the distance between the two file accesses is one,
as they share the same directory but are not the same file. Our distance
metric, in other words, measures how far up the directory tree you have
to travel to find the *common ancestor* of two files; the closer they are in the
tree, the lower the metric.

Figure below shows the locality observed in the SEER traces over all
workstations in the SEER cluster over the entirety of all traces. The graph
plots the difference metric along the x-axis, and shows the cumulative
percentage of file opens that were of that difference along the y-axis.
Specifically, for the SEER traces (marked “Trace” in the graph), you can
see that about 7% of file accesses were to the file that was opened previously,
and that nearly 40% of file accesses were to either the same file or
to one in the same directory (i.e., a difference of zero or one). Thus, the
FFS locality assumption seems to make sense (at least for these traces).

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_16.png)

Interestingly, another 25% or so of file accesses were to files that had a
distance of two. This type of locality occurs when the user has structured
a set of related directories in a multi-level fashion and consistently jumps
between them. For example, if a user has a src directory and builds
object files (`.o` files) into an `obj` directory, and both of these directories
are sub-directories of a main `proj` directory, a common access pattern
will be `proj/src/foo.c` followed by `proj/obj/foo.o`. The distance
between these two accesses is two, as `proj` is the common ancestor. FFS
does *not* capture this type of locality in its policies, and thus more seeking
will occur between such accesses.

For comparison, the graph also shows locality for a “Random” trace.
The random trace was generated by selecting files from within an existing
SEER trace in random order, and calculating the distance metric between
these randomly-ordered accesses. As you can see, there is less namespace
locality in the random traces, as expected. However, because eventually
every file shares a common ancestor (e.g., the root), there is some locality,
and thus random is useful as a comparison point.

##The Large-File Exception

In FFS, there is one important exception to the general policy of file
placement, and it arises for large files. Without a different rule, a large
file would entirely fill the block group it is first placed within (and maybe
others). Filling a block group in this manner is undesirable, as it prevents
subsequent “related” files from being placed within this block group, and
thus may hurt file-access locality.

Thus, for large files, FFS does the following. After some number of
blocks are allocated into the first block group (e.g., 12 blocks, or the num-
ber of direct pointers available within an inode), FFS places the next “large”
chunk of the file (e.g., those pointed to by the first indirect block) in an-
other block group (perhaps chosen for its low utilization). Then, the next
chunk of the file is placed in yet another different block group, and so on.

Let’s look at some pictures to understand this policy better. Without
the large-file exception, a single large file would place all of its blocks into
one part of the disk. Let’s use a small example of a file with 10 blocks to
illustrate the behavior visually.

Here is the depiction of FFS without the large-file exception:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_17.png)

With the large-file exception, you might see something more like this,
with the file spread across the disk in chunks:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_18.png)

The astute reader will note that spreading blocks of a file across the
disk will hurt performance, particularly in the relatively common case
of sequential file access (e.g., when a user or application reads chunks 0
through 9 in order). And you are right! But you can address this problem
by choosing chunk size carefully.

Specifically, if the chunk size is large enough, the file system will spend
most of its time transferring data from disk and just a (relatively) little
time seeking between chunks of the block. This process of reducing an
overhead by doing more work per overhead paid is called **amortization**
and is a common technique in computer systems.

Let’s do an example: assume that the average positioning time (i.e.,
seek and rotation) for a disk is 10 ms. Assume further that the disk trans-
fers data at 40 MB/s. If our goal was to spend half our time seeking be-
tween chunks and half our time transferring data (and thus achieve 50%
of peak disk performance), we would thus need to spend 10 ms transfer-
ring data for every 10 ms positioning. So the question becomes: how big
does a chunk have to be in order to spend 10 ms in transfer? Easy, just
use our old friend, math, in particular the dimensional analysis we spoke
of in the chapter on disks:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_19.png)

Basically, what this equation says is this: if you transfer data at 40
MB/s, you need to transfer only 409.6KB every time you seek in order to
spend half your time seeking and half your time transferring. Similarly,
you can compute the size of the chunk you would need to achieve 90%
of peak bandwidth (turns out it is about 3.69MB), or even 99% of peak
bandwidth (40.6MB!). As you can see, the closer you want to get to peak,
the bigger these chunks get (see Figure below for a plot of these values).

FFS did not use this type of calculation in order to spread large files
across groups, however. Instead, it took a simple approach, based on the
structure of the inode itself. The first twelve direct blocks were placed
in the same group as the inode; each subsequent indirect block, and all
the blocks it pointed to, was placed in a different group. With a block
size of 4KB, and 32-bit disk addresses, this strategy implies that every
1024 blocks of the file (4MB) were placed in separate groups, the lone
exception being the first 48KB of the file as pointed to by direct pointers.

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_20.png)

**Amortization: How Big Do Chunks Have To Be?**

Note that the trend in disk drives is that transfer rate improves fairly
rapidly, as disk manufacturers are good at cramming more bits into the
same surface, but the mechanical aspects of drives related to seeks (disk
arm speed and the rate of rotation) improve rather slowly [P98]. The
implication is that over time, mechanical costs become relatively more
expensive, and thus, to amortize said costs, you have to transfer more
data between seeks.

##A Few Other Things About FFS

FFS introduced a few other innovations too. In particular, the design-
ers were extremely worried about accommodating small files; as it turned
out, many files were 2KB or so in size back then, and using 4KB blocks,
while good for transferring data, was not so good for space efficiency.
This **internal fragmentation** could thus lead to roughly half the disk being
wasted for a typical file system.

The solution the FFS designers hit upon was simple and solved the
problem. They decided to introduce **sub-blocks**, which were 512-byte
little blocks that the file system could allocate to files. Thus, if you created
a small file (say 1KB in size), it would occupy two sub-blocks and thus not
waste an entire 4KB block. As the file grew, the file system will continue
allocating 512-byte blocks to it until it acquires a full 4KB of data. At that
point, FFS will find a 4KB block, *copy* the sub-blocks into it, and free the
sub-blocks for future use.

You might observe that this process is inefficient, requiring a lot of ex-
tra work for the file system (in particular, a lot of extra I/O to perform the
copy). And you’d be right again! Thus, FFS generally avoided this pes-
simal behavior by modifying the `libc` library; the library would buffer
writes and then issue them in 4KB chunks to the file system, thus avoid-
ing the sub-block specialization entirely in most cases.

A second neat thing that FFS introduced was a disk layout that was
optimized for performance. In those times (before SCSI and other more
modern device interfaces), disks were much less sophisticated and re-
quired the host CPU to control their operation in a more hands-on way.
A problem arose in FFS when a file was placed on consecutive sectors of
the disk, as on the left in Figure below.

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_21.png)

**FFS: Standard Versus Parameterized Placement**

In particular, the problem arose during sequential reads. FFS would
first issue a read to block 0; by the time the read was complete, and FFS
issued a read to block 1, it was too late: block 1 had rotated under the
head and now the read to block 1 would incur a full rotation.

FFS solved this problem with a different layout, as you can see on the
right in Figure 41.3. By skipping over every other block (in the example),
FFS has enough time to request the next block before it went past the
disk head. In fact, FFS was smart enough to figure out for a particular
disk *how many* blocks it should skip in doing layout in order to avoid the
extra rotations; this technique was called **parameterization**, as FFS would
figure out the specific performance parameters of the disk and use those
to decide on the exact staggered layout scheme.

You might be thinking: this scheme isn’t so great after all. In fact, you
will only get 50% of peak bandwidth with this type of layout, because
you have to go around each track twice just to read each block once.
Fortunately, modern disks are much smarter: they internally read the entire
track in and buffer it in an internal disk cache (often called a **track buffer**
for this very reason). Then, on subsequent reads to the track, the disk will
just return the desired data from its cache. File systems thus no longer
have to worry about these incredibly low-level details. Abstraction and
higher-level interfaces can be a good thing, when designed properly.

Some other usability improvements were added as well. FFS was one
of the first file systems to allow for **long file names**, thus enabling more
expressive names in the file system instead of the traditional fixed-size
approach (e.g., 8 characters). Further, a new concept was introduced
called a **symbolic link**. As discussed in a previous chapter, hard links are
limited in that they both could not point to directories (for fear of intro-
ducing loops in the file system hierarchy) and that they can only point to
files within the same volume (i.e., the inode number must still be mean-
ingful). Symbolic links allow the user to create an “alias” to any other
file or directory on a system and thus are much more flexible. FFS also
introduced an atomic `rename()` operation for renaming files. Usabil-
ity improvements, beyond the basic technology, also likely gained FFS a
stronger user base.

**TIP : MAKE THE SYSTEM USABLE**

Probably the most basic lesson from FFS is that not only did it intro-
duce the conceptually good idea of disk-aware layout, but it also added
a number of features that simply made the system more usable. Long file
names, symbolic links, and a rename operation that worked atomically
all improved the utility of a system; while hard to write a research pa-
per about (imagine trying to read a 14-pager about “The Symbolic Link:
Hard Link’s Long Lost Cousin”), such small features made FFS more use-
ful and thus likely increased its chances for adoption. Making a system
usable is often as or more important than its deep technical innovations.

##Summary

The introduction of FFS was a watershed moment in file system history,
as it made clear that the problem of file management was one of the
most interesting issues within an operating system, and showed how one
might begin to deal with that most important of devices, the hard disk.
Since that time, hundreds of new file systems have developed, but still
today many file systems take cues from FFS (e.g., Linux ext2 and ext3 are
obvious intellectual descendants). Certainly all modern systems account
for the main lesson of FFS: treat the disk like it’s a disk.
