#Create file support

Here we will add create file support to our filesystem. Since it's
our first data-modify operation we should carefully manage it.

As you will see we will modify super block, create new inode in 
inode storage, and also modify free-blocks table. So for any critical 
section operation we should use some kind of data protection. The
simplest form is `mutex`.

To use `mutex` structure let's include `<linux/mutex.h>`.

This is the most trivial case: you initialize the mutex. Then you can 
call `mutex_lock_interruptible()` to grab the mutex, and 
`mutex_unlock()` to release it. There is also a `mutex_lock()`, which 
should be avoided, because it will not return if a signal is received.

```c
static DEFINE_MUTEX(simplefs_sb_lock);
static DEFINE_MUTEX(simplefs_inodes_mgmt_lock);
```

`simplefs_sb_lock` will control access to super block, and
`simplefs_inodes_mgmt_lock` -- access to the inode store. 

`DEFINE_MUTEX` is simply a macros which definition you can see [here](http://lxr.free-electrons.com/source/include/linux/mutex.h#L116).

Let's add convenience function `dedupfs_sb_sync` which will synchronize
super block in memory with super block on disk.

```c
void dedupfs_sb_sync(struct super_block *vsb)
{
	struct buffer_head *bh;
	struct dedupfs_super_block *sb = DEDUPFS_SB(vsb);

	bh = (struct buffer_head*)sb_bread(vsb, 
									DEDUPFS_SUPERBLOCK_BLOCK_NUMBER);
	bh->b_data = (char *)sb;
	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
}
```

Another convenient function we will add is `dedupfs_inode_add` which
simply add new inode to the inode store (exactly to the end of 
it).

```c
void dedupfs_inode_add(struct super_block *vsb, 
						struct dedupfs_inode *inode)
{
	struct dedupfs_super_block *sb = DEDUPFS_SB(vsb);
	struct buffer_head *bh;
	struct dedupfs_inode *inode_iterator;

	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return;
	}

	bh = (struct buffer_head *)sb_bread(vsb,
									DEDUPFS_INODESTORE_BLOCK_NUMBER);

	inode_iterator = (struct dedupfs_inode *)bh->b_data;

	if(mutex_lock_interruptible(&dedupfs_sb_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
							__FILE__, __LINE__);	
		return;
	}

	/* Append the new inode in the end in the inode store */
	inode_iterator += sb->inodes_count;

	memcpy(inode_iterator, inode, sizeof(struct dedupfs_inode));
	sb->inodes_count++;

	mark_buffer_dirty(bh);
	dedupfs_sb_sync(vsb);

	mutex_unlock(&dedupfs_sb_lock);
	mutex_unlock(&dedupfs_inodes_mgmt_lock);

}
```

Here we set lock on the inode store

```c
	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return;
	}
```

After acquiring lock we retrieve first inode from the inode store

```c
	bh = (struct buffer_head *)sb_bread(vsb,
									DEDUPFS_INODESTORE_BLOCK_NUMBER);

	inode_iterator = (struct dedupfs_inode *)bh->b_data;
```

Next we go ahead and get mutex on super block, go to the end of
inode store, add new inode and increment number of inodes in super
block structure

```c
	if(mutex_lock_interruptible(&dedupfs_sb_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
							__FILE__, __LINE__);	
		return;
	}

	/* Append the new inode in the end in the inode store */
	inode_iterator += sb->inodes_count;

	memcpy(inode_iterator, inode, sizeof(dedupfs_inode));
	sb->inodes_count++;

	mark_buffer_dirty(bh);
	dedupfs_sb_sync(vsb);

	mutex_unlock(&dedupfs_sb_lock);
	mutex_unlock(&dedupfs_inodes_mgmt_lock);
```

The next convenience function `dedupfs_sb_get_a_freeblock` returns
a block number which is free. The block will be removed from the
freeblock list. The code is very straightforward.

```c
int dedupfs_sb_get_a_freeblock(struct super_block *vsb, __le32 *out)
{
	struct dedupfs_super_block *sb = DEDUPFS_SB(vsb);
	int ret = 0;

	if(mutex_lock_interruptible(&dedupfs_sb_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		ret = -EINTR;
		goto end;
	}


	/* Loop until we find a free block. */
	int i;
	for(i = 0; i < DEDUPFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED; i++) 
	{
		if(sb->free_blocks & (1<<i)) {
			break;
		}
	}

	if(i == DEDUPFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED) {
		printk(KERN_ERR "No more free blocks available");
		ret = -ENOSPC;
		goto end;
	}

	*out = i;

	/* Remove the identified block from the free list */
	sb->free_blocks &= ~(1 << i);

	dedupfs_sb_sync(vsb);

end:
	mutex_unlock(&dedupfs_sb_lock);
	return ret;
}
```

The last convenience function we will use is 
`dedupfs_sb_get_objects_count` which simply return current number of
inodes in the inode store

```c
static int dedupfs_sb_get_objects_count(struct super_block *vsb,
										__le32 *out)
{
	struct dedupfs_super_block *sb = DEDUPFS_SB(vsb);

	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n"
					__FILE__, __LINE__);
		return -EINTR;
	}
	*out = sb->inodes_count;
	mutex_unlock(&dedupfs_inodes_mgmt_lock);

	return 0;
}
```

Now let's go ahead and add `create` function to `inode_operations`
structure. The VFS calls this function from the `creat()` and `open()`
system calls to create a new inode associated with the given dentry 
object with the specified initial access mode. It has the following
prototype

```c
int create(struct inode *dir, struct dentry *dentry, umode_t mode,
			bool excl);
```

as defined [here](http://lxr.free-electrons.com/source/include/linux/fs.h#L1667).

First of all, we acquire mutex for updating directory children and
retrieve super block

```c
	if(mutex_lock_interruptible(&dedupfs_directory_children_update_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}
	sb = dir->i_sb;
```

Then we get current count of inodes in the inode store and check
if we can add yet another inode.

```c
	ret = dedupfs_sb_get_objects_count(sb, &count);
	if(ret < 0) {
		return ret;
	}

	if(count >= DEDUPFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED) {
		printk(KERN_ERR
				"Maximum number of objects supported "
				"by dedupfs is already reached\n");
		return -ENOSPC;
	}
```

Now check if this file is a directory or a file.

```c
	if(!S_ISDIR(mode) && !S_ISREG(mode)) {
		printk(KERN_ERR
				"Creation request for neither a file nor a directory\n");
		return -EINVAL;
	}
```

If it is, then allocate new inode for it and find new inode number

```c
	inode = new_inode(sb);
	if(!inode)
		return -ENOMEM;
	
	inode->i_sb = sb;
	inode->i_op = &dedupfs_inode_ops;
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	inode->i_ino = 11;

	/* Loop until we get an unique inode number */
	while(dedupfs_get_inode(sb, inode->i_ino)) {
		printk(KERN_INFO "inode [%d] already exists\n", inode->i_ino);
		inode->i_ino++;
	}
	printk(KERN_INFO "Got new unique inode number [%d]\n", inode->i_ino);
```

Now we go ahead and allocate new our custom inode for this file 

```c
	dfs_inode = kmalloc(sizeof(struct dedupfs_inode), GFP_KERNEL);
	dfs_inode->inode_no = inode->i_ino;
	inode->i_private = &dfs_inode;

	if(S_ISDIR(mode)) {
		printk(KERN_INFO "New directory creation request\n");
		dfs_inode->dir_children_count = 0;
		dfs_inode->mode = S_IFDIR;
		inode->i_fop = &dedupfs_dir_operations;
	}
	else if(S_ISREG(mode)) {
		printk(KERN_INFO "New file creation request\n");
		dfs_inode->file_size = 0;
		dfs_inode->mode = S_IFREG;
		inode->i_fop = &dedupfs_file_operations;
	}
```

The `kmalloc()` function is declared in `<linux/slab.h>`.

Then we get a free block and update the free map

```c
	ret = dedupfs_sb_get_a_freeblock(sb, &dfs_inode->data_block_number);
	if(ret < 0) {
		printk(KERN_ERR "dedupfs could not get a freeblock\n");
		return ret;
	}
```

After that add it to the inode store block and update the sb 
`inodes_count`

```c
	dedupfs_inode_add(sb, dfs_inode);
```

Then update the parent directory's datablock with the new 
child.

```c
	record = kmalloc(sizeof(struct dedupfs_dir_record), GFP_KERNEL);
	record->inode_no = dfs_inode->inode_no;
	strcpy(record->filename, dentry->d_name.name);

	parent_dir_inode = DEDUPFS_INODE(dir);
	bh = sb_bread(sb, parent_dir_inode->data_block_number);
	dir_contents_datablock = (struct dedupfs_dir_record *)bh->b_data;

	/* Navigate to the last record in the directory contents */	
	dir_contents_datablock += parent_dir_inode->dir_children_count;

	memcpy(dir_contents_datablock, record,
				sizeof(struct dedupfs_file_record));

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);

```

and also parent directory's inode

```c
	parent_dir_inode->dir_children_count++;

	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
				__FILE__, __LINE__);
		return -EINTR;
	}

	bh = (struct buffer_head *) sb_bread(sb,
									DEDUPFS_INODESTORE_BLOCK_NUMBER);
	
	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);
```

Finally, we release mutexes and assign mode (owner, permissions, etc.)
to the inode, and associate dentry with this inode

```c
	mutex_unlock(&dedupfs_inodes_mgmt_lock);
	mutex_unlock(&dedupfs_directory_children_update_lock);

	printk(KERN_INFO "Returning success after creating the file\n");

	inode_init_owner(inode, dir, mode);
	d_add(dentry, inode);

	/* This sleep is necessary to update the dentry cache */
	msleep(5);
	return 0;
```

to use `msleep()` we need to include `<linux/delay.h>` header.

Also we need to add our `dedupfs_create` function to the 
`inode_operations` structure

```c
static int dedupfs_create(struct inode *dir, struct dentry *dentry,
						umode_t mode, bool excl);

static struct inode_operations dedupfs_inode_ops = {
	.create = dedupfs_create,
	.lookup = dedupfs_lookup,
};
```

The last thing we should add is the constant 
`DEDUPFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED` to `dedupfs-structures.h`

```c
const int DEDUPFS_MAX_FILESYSTEM_OBJECTS_SUPPORTED = 64;
```

and in `mkfs-dedupfs.c` mark `WELCOMEFILE_DATABLOCK_NUMBER` position 
in free blocks as non-free

```c
	sb.free_blocks = ~0;
	sb.free_blocks &= ~(1 << WELCOMEFILE_DATABLOCK_NUMBER);
```








