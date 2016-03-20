#cd and ls support

We will add `cd` support to the mountpoint and do `ls` there.

##The Inode Object

The inode object represents all the information needed by the kernel to
manipulate a file or directory. For Unix-style filesystems, this 
information is simply read from the on-disk inode.

If a filesystem does not have inodes, however, the filesystem must 
obtain the information from wherever it is stored on the disk. 
Filesystems without inodes generally store file-specific information as
part of the file; unlike Unix-style filesystems, they do not separate 
file data from its control information. Some modern filesystems do 
neither and store file metadata as part of an on-disk database.
Whatever the case, the inode object is constructed in memory in 
whatever manner is applicable to the filesystem.

The inode object is represented by `struct inode` and is defined in 
`<linux/fs.h>`. There are a lot of fields in this structure but now we
need only two of them

```c
struct inode_operations    *i_op;                    
struct file_operations     *i_fop;
```

An inode represents each file on a filesystem, but the inode object is 
constructed in memory only as files are accessed. This includes 
special files, such as device files or pipes.

##Inode Operations

As with the superblock operations, the `inode_operations` member is 
important. It describes the filesystem’s implemented functions that the
VFS can invoke on an inode. As with the superblock, inode operations 
are invoked via

```c
i->i_op->truncate(i)
```

In this call, `i` is a reference to a particular inode. In this case, 
the `truncate()` operation defined by the filesystem on which `i` 
exists is called on the given inode. The `inode_operations` structure 
is defined in `<linux/fs.h>`. 

At this moment we need to define only `lookup` function.

```c
struct dentry *dedupfs_lookup(struct inode *parent_inode,
							  struct dentry *child_dentry, 
							  unsigned int flags)
{
	return NULL;
}

static struct inode_operations dedupfs_inode_ops = {
	.lookup = dedupfs_lookup,
};
```

The `lookup` function is used for `dentry` association. This function 
searches a directory for an `inode` corresponding to a filename 
specified in the `child_dentry`. As of now, we don't deal with 
dentries in dedupfs. So we will keep this simple for 
now and revisit later.

##File Operations

As with all the other VFS objects, the file operations table is quite 
important. The operations associated with `struct file` are the 
familiar system calls that form the basis of the standard Unix system 
calls.

The file object methods are specified in `file_operations` and defined 
in `<linux/fs.h>`. Also at this moment we need to implement only
one function:

```c
static int dedupfs_readdir(struct file *filp, 
						   void *dirent,
						   filldir_t filldir)
{
	return 0;
}

static struct file_operations dedupfs_dir_operations = {
	.owner = THIS_MODULE,
	.readdir = dedupfs_readdir,
};
```

The `readdir` function returns the next directory in a directory 
listing. This function is called by the `readdir()` system call.
`ls` will list nothing as of now. It's just a basic skeleton code to 
make `ls` (`readdir`) work for dedupfs.

The last thing we should do is to assign this operation tables to our
root inode in `dedupfs_fill_superblock`

```c
root_inode->i_op = &dedupfs_inode_ops;
root_inode->i_fop = &dedupfs_dir_operations;
```

```c
static int dedupfs_fill_superblock(struct super_block *sb, void *data,
									int silent)
{
	struct inode *root_inode;

	/* A magic number that uniquely identifies our filesystem type */
	sb->s_magic = DEDUPFS_MAGIC_NUMBER;

	root_inode = dedupfs_get_inode(sb, NULL, S_IFDIR);
	root_inode->i_op = &dedupfs_inode_ops;
	root_inode->i_fop = &dedupfs_dir_operations;

	sb->s_root = d_make_root(root_inode);
	if(!sb->s_root) {
		printk(KERN_ERR "root creation failed\n");
		return -ENOMEM;
	}

	return 0;	

}
```

**Note**

I've tested this on Linux Kernel version 3.13 and had compilation
error

```
error: unknown field ‘readdir’ specified in initialiser
```

So you have two ways: first is for more portability use macros
defined in `<linux/version.h>` like this

```c
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
static int dedupfs_iterate(struct file *filp, 
							struct dir_context *ctx)
#else
static int dedupfs_readdir(struct file *filp, 
							void *dirent, 
							filldir_t filldir)
#endif
{
	return 0;
}


static struct file_operations simplefs_dir_operations = {
	.owner = THIS_MODULE,
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
	.iterate = simplefs_iterate,
#else
	.readdir = simplefs_readdir,
#endif
};
```

And the second is just to write for the specified kernel version. 
I prefer this variant since you concentrate only on filesystem
development and not on portability issues. So I will write code now 
only for kernel version >= 3.11.

So I have

```c
static int dedupfs_iterate(struct file *filp,
						   struct dir_context *ctx)
{
	return 0;
}

static struct file_operations dedupfs_dir_operations = {
	.owner = THIS_MODULE,
	.iterate = dedupfs_iterate,
};
```
