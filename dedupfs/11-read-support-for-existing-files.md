#Read support

Now we are going to add read support for existing files. For this 
purpose we should implement `read` function which has the following
function prototype

```c
ssize_t read(struct file *file, char *buf, size_t count, loff_t *offset)
```

It reads `count` bytes from the given file at position `offset` into
`buf`. The file pointer is then updated. This function is called by
the `read()` system call. Let's see listing

```c
ssize_t dedupfs_read(struct file *filp, char __user *buf, size_t len,
					loff_t *ppos)
{
	struct dedupfs_inode *inode;
	struct buffer_head *bh;
	char *buffer;
	int nbytes;
	
	inode = DEDUPFS_INODE(filp->f_inode);

	if(*ppos >= inode->file_size) {
		printk(KERN_INFO
					"Read request with offset beyond the filesize\n");
		return 0;
	}

	bh = (struct buffer_head *) sb_bread(filp->f_inode->i_sb,
										inode->data_block_number);

	if(!bh) {
		printk(KERN_ERR "Reading the block number [%d] failed.\n",
								inode->data_block_number);
		return 0;
	}

	buffer = (char *) bh->b_data;
	nbytes = min(strlen(buffer), len);

	if(copy_to_user(buf, buffer, nbytes)) {
		brelse(bh);
		printk(KERN_ERR
				"Error copying file contents to the userspace "
					"buffer\n");
		return -EFAULT;
	}

	brelse(bh);

	*ppos += nbytes;

	return nbytes;

}
```

First of all, we check if we try to read position beyond the filesize

```c
	inode = DEDUPFS_INODE(filp->f_inode);

	if(*ppos >= inode->file_size) {
		printk(KERN_INFO
					"Read request with offset beyond the filesize\n");
		return 0;
	}
```

After that we start reading data block associated with this inode

```c
	bh = (struct buffer_head *) sb_bread(filp->f_inode->i_sb,
										inode->data_block_number);

	if(!bh) {
		printk(KERN_ERR "Reading the block number [%d] failed.\n",
								inode->data_block_number);
		return 0;
	}
```

Finally, we determine what number of bytes we need to copy

```c
	buffer = (char *) bh->b_data;
	nbytes = min(strlen(buffer), len);
```

and apply copying from one `buffer` to another buffer `buf` 

```c
	if(copy_to_user(buf, buffer, nbytes)) {
		brelse(bh);
		printk(KERN_ERR
				"Error copying file contents to the userspace "
					"buffer\n");
		return -EFAULT;
	}

	brelse(bh);

	*ppos += nbytes;

	return nbytes;
```

You may ask what is `__user` and what is `loff_t` ? Ok, `__user`
macros tells kernel developers that they're possibly using an 
untrusted pointer (or a pointer that may be invalid in the current 
virtual address mapping) improperly. 

`__user` marks user space pointers and tells the developer/system not 
to trust it. If user gives you "invalid" pointer, then kernel tries to
reference it (note that kernel can reference everywhere) and it can 
corrupt it's own space.

For example in "read" should provide you a (`__user`) buffer to write the result to. So you have to use `copy_to_user`, but not `memcopy`, 
`strcpy` or anything like this.

`loff_t` is just a typedef which you can see [here](http://lxr.free-electrons.com/source/include/linux/types.h#L45)

```c
typedef __kernel_loff_t         loff_t;
```

and `__kernel_loff_t` is defined [here](http://lxr.free-electrons.com/source/include/uapi/asm-generic/posix_types.h#L87) as

```c
typedef long long       __kernel_loff_t;
```

So now we have implemented `read` functionality. Let's add it to our 
filesystem. First define new `file_operations` structure which will
be used only for file operations

```c
const struct file_operations dedupfs_file_operations = {
	.read = dedupfs_read,	
};
```

The last thing we should make for full read support is to distinguish
`file_operations` for files and directories inodes while associating
dentry with inode and our custom on-disk inode in `dedupfs_lookup`
function.

It means instead of 

```c
			inode->i_fop = &dedupfs_dir_operations;
```

we should add checking type

```c
			if(S_ISDIR(inode->i_mode))
				inode->i_fop = &dedupfs_dir_operations;
			else if (S_ISREG(inode->i_mode))
				inode->i_fop = &dedupfs_file_operations;
			else
				printk(KERN_ERR
						"Unknown inode type. Neither a directory "
						"nor a file");
```

