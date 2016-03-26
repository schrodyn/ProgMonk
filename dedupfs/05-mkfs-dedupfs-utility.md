#mkfs-dedupfs utility


We now develop the overall on-disk organization of the data structures of the dedupfs file system. Our filesystem will be simple 
(just for now) and, hence, it will be stored very simple on the disk.

The first thing we’ll need to do is divide the disk into **blocks**; 
simple file systems use just one block size, and that’s exactly what 
we’ll do here. Let’s choose a commonly-used size of 4 KB.

Filesystem can work with blocks of different sizes. But now we will
work only with 4KB blocks. Choosing right block size for classic
filesystems is an interesting topic on computer science.

Thus, our view of the disk partition where we’re building our file 
system is simple: a series of blocks, each of size 4 KB. The blocks are
addressed from `0` to `N − 1`, in a partition of size `N` 4-KB blocks. 
Assume we have a really small disk, with just 64 blocks:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_2.png)

Let’s now think about what we need to store in these blocks to build 
a file system. Of course, the first thing that comes to mind is user 
data. In fact, most of the space in any file system is (and should be) 
user data. Let’s call the region of the disk we use for user data the 
**data region**, and, again for simplicity, reserve a fixed portion of 
the disk for these blocks, say the last 56 of 64 blocks on the disk:

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_3.png)

As we learned, the file system has to track information about each 
file. This information is a key piece of **metadata**, and tracks 
things like which data blocks (in the **data region**) comprise a file,
the size of the file, its owner and access rights, access and modify 
times, and other similar kinds of information. To store this 
information, file systems have a structure called an **inode**.

To accommodate inodes, we’ll need to reserve some space on the disk for
them as well. Let’s call this portion of the disk the **inode table**, 
which simply holds an array of on-disk inodes. Thus, our on-disk image 
now looks like this picture, assuming that we use 5 of our 64 blocks 
for inodes (denoted by I’s in the diagram):

![](/dedupfs/images/filesystem_guide_OS_three_easy_piecies_4.png)

We should note here that inodes are typically not that big, for 
example 128 or 256 bytes. Assuming 256 bytes per inode, a 4-KB block 
can hold 16 inodes, and our file system above contains 80 total inodes.

In our simple file system, built on a tiny 64-block partition, this 
number represents the maximum number of files we can have in our file 
system; however, do note that the same file system, built on a larger 
disk, could simply allocate a larger inode table and thus accommodate 
more files.

Our file system thus far has data blocks (D), and inodes (I), but a few
things are still missing. One primary component that is still needed, 
as you might have guessed, is some way to track whether inodes or 
data blocks are free or allocated. Such **allocation structures** are 
thus a requisite element in any file system.

Many allocation-tracking methods are possible, of course. For example, 
we could use a **free list** that points to the first free block, 
which then points to the next free block, and so forth. We instead 
choose a simple and popular structure known as a **bitmap**, one for 
the data region (the **data bitmap** or **blocks map**), and one for 
the inode table (the **inode bitmap**). A bitmap is a simple structure:
each bit is used to indicate whether the corresponding object/block is 
free (0) or in-use (1). And thus our new on-disk layout, with an inode 
bitmap (i) and a data bitmap (d):

Let's define `dedupfs-structures.h` header file for convinience. First 
of all, we define two important filesystem constants 

```c
const int DEDUPFS_MAGIC_NUMBER = 0x25252516;
const int DEDUPFS_DEFAULT_BLOCK_SIZE = 4 * 1024;
```

Of course, we should include `dedupfs-structures.h` in our `super.c`
file.

Now we will create an utility `mkfs-dedupfs.c` to create dedupfs on a device.
At this moment we will store only filesystem information 
(super block). So we should define in `dedupfs-structures.h` the 
structure which describes the on-disk super block

```c
struct dedupfs_super_block {
	unsigned int version;
	unsigned int magic;
	unsigned int block_size;
	unsigned int free_blocks;

	char padding [(4 * 1024) - (4 * sizeof(unsigned int))];
};
```

The source code for `mkfs-dedupfs.c` is straightforward and I won't
demonstrate it here. But we also should add compilation of the 
utility to Makefile.

 



