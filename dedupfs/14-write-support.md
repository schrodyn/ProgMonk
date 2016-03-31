#write support

First of all, we should release `buffer_head` objects when we already
don't need them using `brelse(bh)` function.

Now we replace 

```c
	nbytes = min(strlen(buffer), len);
```

to the following in `dedupfs_read` function

```c
	nbytes = min((size_t) inode->file_size, len);
```

After that let's go ahead and add write support. To accomplish this we 
should implement `write` function for `file_operations` structure. 
It has the following prototype

```c
ssize_t write(struct file *file,
            const char *buf, size_t count,
            loff_t *offset)
```

Writes `count` bytes from `buf` into the given file at position 
`offset`.The file pointer is then updated. This function is called by 
the `write()` system call.


```c
ssize_t dedupfs_write(struct file *filp, const char __user *buf,
					size_t len, loff_t *ppos)
{
	struct inode *inode;
	struct dedupfs_inode *dfs_inode;
	struct dedupfs_inode *inode_iterator;
	struct buffer_head *bh;
	struct super_block *sb;

	char *buffer;
	int count;

	printk(KERN_INFO "file write begins\n");

	inode = filp->f_inode;
	dfs_inode =DEDUPFS_INODE(inode);
	sb = inode->i_sb;

	if(*ppos + len >= DEDUPFS_DEFAULT_BLOCK_SIZE) {
		printk(KERN_ERR "File size write will exceed a block\n");
		return -ENOSPC;
	}

	bh = (struct buffer_head *)sb_bread(sb, 
								dfs_inode->data_block_number);

	if(!bh) {
		printk(KERN_ERR "Reading the block number [%d] failed.\n",
					dfs_inode->data_block_number);
		return 0;
	}
	buffer = (char *)bh->b_data;

	/* Move the pointer until the required byte offset */	
	buffer += *ppos;

	if(copy_from_user(buffer, buf, len)) {
		brelse(bh);
		printk(KERN_ERR
				"Error copying file contents from userspace to "
				"kernel space\n");
		return -EFAULT;
	}
	*ppos += len;

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
	brelse(bh);

	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}	

	bh = (struct buffer_head *)sb_bread(sb,
									DEDUPFS_INODESTORE_BLOCK_NUMBER);
	
	/* FIXME: What to do if someone writes only some parts in 
	 * between ? The above code will also fail in case a file is 
	 * overwritten with a shorter buffer */
	dfs_inode->file_size = *ppos;

	inode_iterator = (struct dedupfs_inode *)bh->b_data;

	if(mutex_lock_interruptible(&dedupfs_sb_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}

	count = 0;
	while(inode_iterator->inode_no != dfs_inode->inode_no
			&& count < DEDUPFS_SB(sb)->inodes_count) {
		count++;
		inode_iterator++;
	}

	if(count < DEDUPFS_SB(sb)->inodes_count) {
		inode_iterator->file_size = dfs_inode->file_size;
		printk(KERN_INFO
				"The new filesize that is written is [%d] "
				"and len was: [%lu]\n",
				dfs_inode->file_size, len);

		mark_buffer_dirty(bh);
		sync_dirty_buffer(bh);
	}
	else {
		printk(KERN_ERR
			"The new filesize could not be stored to the inode\n");
		len = -EIO;
	}

	brelse(bh);

	mutex_unlock(&dedupfs_sb_lock);
	mutex_unlock(&dedupfs_inodes_mgmt_lock);

	return len;
}
```

At the beginning we retrieve inode, our custom inode assciated with
it and also super block

```c
	inode = filp->f_inode;
	dfs_inode =DEDUPFS_INODE(inode);
	sb = inode->i_sb;
```

The first interesting part is here

```c
	if(*ppos + len >= DEDUPFS_DEFAULT_BLOCK_SIZE) {
		printk(KERN_ERR "File size write will exceed a block\n");
		return -ENOSPC;
	}
```

We assume now (for simplicity) that every file can not exceed one 
data block. So we check this condition.

After that we set buffere head to the data block associated with 
this inode and point `buffer` to the data in it

```c
	bh = (struct buffer_head *)sb_bread(sb, 
								dfs_inode->data_block_number);

	if(!bh) {
		printk(KERN_ERR "Reading the block number [%d] failed.\n",
					dfs_inode->data_block_number);
		return 0;
	}
	buffer = (char *)bh->b_data;
```

Now let's go ahead and move `buffer` to the required byte offset
and copy `len` bytes from `buf` to the `buffer` 

```c
	/* Move the pointer until the required byte offset */	
	buffer += *ppos;

	if(copy_from_user(buffer, buf, len)) {
		brelse(bh);
		printk(KERN_ERR
				"Error copying file contents from userspace to "
				"kernel space\n");
		return -EFAULT;
	}
	*ppos += len;

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
	brelse(bh);
```

Finally, we want to update the inode store on the disk, exactly 
`file_size` field of our custom inode since we update it

```c
	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}	

	bh = (struct buffer_head *)sb_bread(sb,
									DEDUPFS_INODESTORE_BLOCK_NUMBER);
	
	/* FIXME: What to do if someone writes only some parts in 
	 * between ? The above code will also fail in case a file is 
	 * overwritten with a shorter buffer */
	dfs_inode->file_size = *ppos;

	inode_iterator = (struct dedupfs_inode *)bh->b_data;

	if(mutex_lock_interruptible(&dedupfs_sb_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}

	count = 0;
	while(inode_iterator->inode_no != dfs_inode->inode_no
			&& count < DEDUPFS_SB(sb)->inodes_count) {
		count++;
		inode_iterator++;
	}

	if(count < DEDUPFS_SB(sb)->inodes_count) {
		inode_iterator->file_size = dfs_inode->file_size;
		printk(KERN_INFO
				"The new filesize that is written is [%d] "
				"and len was: [%lu]\n",
				dfs_inode->file_size, len);

		mark_buffer_dirty(bh);
		sync_dirty_buffer(bh);
	}
	else {
		printk(KERN_ERR
			"The new filesize could not be stored to the inode\n");
		len = -EIO;
	}

	brelse(bh);
```

After that we release acquired mutexes and return number `len` bytes 
that was written.

```c
	mutex_unlock(&dedupfs_sb_lock);
	mutex_unlock(&dedupfs_inodes_mgmt_lock);

	return len;
```









