#Register/Unregister filesystem

The life of filesystem begins with registration. We can **register** 
filesystem with using `register_filesystem` function. 
We will register our filesystem in init module function 
(`dedupfs_init`). 

To **unregister** filesystem we should use `unregister_filesystem` 
function. And we will call it in `dedupfs_exit` function of our module.

Both functions takes as parameter a pointer to `file_system_type` 
structure -- it will "describe" filesystem, you can think it is like
a filesystem class. 

There are a lot of fields in this structure. Hovewer, we are interested
only in some of them:

```c
static struct file_system_type dedupfs_fs_type = {
	.owner = THIS_MODULE,
	.name = "dedupfs",
	.mount = dedupfs_mount,
	.kill_sb = dedupfs_kill_superblock,
};
```
First of all, we are interested in `name` field, it holds the name of
our filesystem, and exactly this name will be used while mounting.

`mount` and `kill_sb` -- two fields that stores pointers to functions.
`mount` function will be called while mounting, and `kill_sb` -- while
unmounting.

Finally, `owner` field is intended for organisation of **links counter** to this module. Links counter is created for module not to be
unloaded untimely. For example, if filesystem was mounted then 
unloading of module could lead to crash, so links counter prevents
unloading of the module while it's used i.e. while we don't unmount
our filesystem.

Now, let's go ahead and consider `dedupfs_mount`. It should mount
device and return the structure that describes **root directory** of
filesystem. It seems to be hard, but kernel will help us.


```c
static struct dentry *dedupfs_mount(struct file_system_type *fs_type,
									int flags, const char *dev_name,
									void *data)
{
	struct dentry *ret;

	ret = mount_bdev(fs_type, flags, dev_name, data, dedupfs_fill_superblock);

	if(IS_ERR(ret))
		printk(KERN_ERR "Error mounting dedupfs");
	else
		printk(KERN_INFO "dedupfs is succesfully mounted on [%s]",
				dev_name);

	return ret;

}
```

It's the fact that almost all work occures in `mount_bdev` function,
and we are interested only in one its parameter 
`dedupfs_fill_superblock` -- it's a pointer to function, which will
be called in `mount_bdev` to **initialize superblock**.

Before we move on let's understand the important structure in VFS layer
called `dentry`. This structure represents a part of the file path 
name. For example, if we appeal to file `/usr/bin/bash` we will have
instances of `dentry` structure representing `/` (root directory),
`bin/` and `bash`. The kernel maintains cache of this structures, that
allows quickly search `inode` by file path name. 

So, `dedupfs_mount` must return `dentry` which represents root 
directory of our filesystem. And function `dedupfs_fill_superblock` 
makes the `super_block` valid and fills filesystem specific 
information in the super block.

```c
static int dedupfs_fill_superblock(struct super_block *sb, void *data,
									int silent)
{
	struct inode *root_inode;

	/* A magic number that uniquely identifies our filesystem type */
	sb->s_magic = DEDUPFS_MAGIC_NUMBER;

	root_inode = dedupfs_get_inode(sb, NULL, S_IFDIR);
	sb->s_root = d_make_root(root_inode);
	if(!sb->s_root)
		return -ENOMEM;

	return 0;	

}
```

So what is the structure `super_block`? Usually, filesystems store
parameters of whole filesystem in the special place on disk (this
place is chosen by filesystem writer). For example, the size of block,
number of free/occupied blocks, filesystem version, "pointer" on root
dentry, magic number by which filesystem driver can check that there
is exactly this filesystem on the disk.

First of all, we allocate new inode

```c
root_inode = dedupfs_get_inode(sb, NULL, S_IFDIR);
```

and in this line

```c
sb->s_root = d_make_root(root_inode);
```

we create `dentry` linked with root `inode`.

`dedupfs_get_inode` creates, configures and returns an `inode`,
for the asked file (or) directory (differentiated by the `mode` 
param), under the directory specified by the `dir` param, managed 
by the superblock `sb` param)

```c
static struct inode *dedupfs_get_inode(struct super_block *sb,
									  const struct inode *dir, umode_t mode)
{
	struct inode *inode = new_inode(sb);

	if(inode) {
		inode->i_ino = get_next_ino();
		inode_init_owner(inode, dir, mode);

		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

		switch(mode & S_IFMT) {
			case S_IFDIR:
				/* for "." entry */
				inc_nlink(inode);
				break;
			case S_IFREG:
			case S_IFLNK:
			default:
				printk(KERN_ERR
						"dedupfs can create meaningful inode only for root directory at the moment\n");
				return NULL;
		}
	}

	return inode;
}
```

in this line we deligate setting of owner and permissions to the 
kernel 

```c
inode_init_owner(inode, dir, mode);
```

in `switch` statement the most interesting line is calling `inc_nlink`
function. In `inode` structure `i_nlink` will be initialized to 1 in 
the `inode_init_always` function (that gets called inside the 
`new_inode` function). We change it to 2 for directories, for 
covering the "." entry

```c
switch(mode & S_IFMT) {
			case S_IFDIR:
				/* for "." entry */
				inc_nlink(inode);
			case S_IFREG:
			case S_IFLINK:
			default:
				printk(KERN_ERR
						"dedupfs can create meaningful inode only for root directory at the moment\n");
				return NULL;
		}
```

`dedupfs_kill_superblock` is just a dummy function as of now. As our 
filesystem gets matured, we will do more meaningful operations here

```c
static void dedupfs_kill_superblock(struct super_block *sb)
{
	printk(KERN_INFO
			"dedupfs superblock is destroyed. Unmount succesful.\n");
}
```

Finally, we should register our filesystem when module is loaded.

```c
static int __init dedupfs_init(void)
{
	int ret;

	ret = register_filesystem(&dedupfs_fs_type);
	if(ret == 0)
		printk(KERN_INFO "Succesfully registered dedupfs\n");
	else
		printk(KERN_ERR "Failed to register dedupfs. Error:[%d]", 
		ret);

	return ret;
}
```

and unregister it while the module is unloading.

```c
static void __exit dedupfs_exit(void)
{
	int ret;

	ret = unregister_filesystem(&dedupfs_fs_type);
	if(ret == 0)
		printk(KERN_INFO "Succesfully unregistered dedupfs\n");
	else
		printk(KERN_ERR "Failed to unregister dedupfs. Error:[%d]",
				ret);
}
```








