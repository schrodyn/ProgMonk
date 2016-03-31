#mkdir support

To add `mkdir` support we should implement `mkdir` function in
`inode_operations` structure. It's called from the `mkdir()` system 
call to create a new directory with the given initial mode.
It has the following prototype

```c
int mkdir(struct inode *dir,
          struct dentry *dentry,
          umode_t mode)
```

So we go ahead and implement our own `dedupfs_mkdir` function

```c
static int dedupfs_create(struct inode *dir, struct dentry *dentry,
						umode_t mode, bool excl);

static int dedupfs_mkdir(struct inode *dir, struct dentry *dentry,
						umode_t mode);

static struct inode_operations dedupfs_inode_ops = {
	.create = dedupfs_create,
	.lookup = dedupfs_lookup,
	.mkdir = dedupfs_mkdir,
};
```

for this purpose we will rename `dedupfs_create` function

```c
static int dedupfs_create(struct inode *dir, struct dentry *dentry,
						umode_t mode, bool excl)
```

to

```c
static int dedupfs_create_fs_object(struct inode *dir, 
									struct dentry *dentry,
									umode_t mode)
```

Also we add `mutex_unlock` call on 
`dedupfs_directory_children_update_lock` mutex if some kind of error
 occured.

After that we slghtly modify `mode` initialisation to our `dfs_inode` 
while we are creating it.

```c
	dfs_inode->mode = mode;
	inode->i_private = dfs_inode;
```

instead of explicitly setting `mode` like

```c
	dfs_inode->mode = S_IFDIR;
```		

and

```c
	dfs_inode->mode = S_IFREG;
```
		
Now we go ahead and implement `dedupfs_mkdir` and new `dedupfs_create`
functions like the following

```c
static int dedupfs_mkdir(struct inode *dir, struct dentry *dentry,
						umode_t mode)
{
	/* this is a bug in the kernel, for some reason, the mkdir 
	callback does not get the S_IFDIR flag set. Even ext2 sets is 
	explicitly */
	return dedupfs_create_fs_object(dir, dentry, S_IFDIR | mode);
}

static int dedupfs_create(struct inode *dir, struct dentry *dentry,
						umode_t mode, bool excl)
{
	return dedupfs_create_fs_object(dir, dentry, mode);
}
```

