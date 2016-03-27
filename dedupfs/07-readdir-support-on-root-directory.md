#readdir support on root directory

First of all, we will use fixed POSIX datatypes instead of 
platform dependent types. However, it's not enough to fix type size, we
should also fix the byte order. 

The following types can be used for endian dependent variables after 
including the `<linux/types.h>` header file.

```
__le16
__le32
__le64
 
__be16
__be32
__be64
```

I will use little endian `__le32` since it has advantage on `x86` and
`x64` microarchitectures.

**dedupfs-structures.h**

```c
struct dedupfs_super_block {
	__le32 version;
	__le32 magic;
	__le32 block_size;
	__le32 free_blocks;

	char padding [(4 * 1024) - (4 * sizeof(__le32))];
};
```

Let's define convenience inline function in `super.h` header for
retrieving our custom super block structure which will be stored
in `s_fs_info` field of `super_block` structure. It has just `void*`
type as described [here](http://lxr.free-electrons.com/source/include/linux/fs.h#L1303).

```c
#include "dedupfs-structures.h"

static inline struct dedupfs_super_block *DEDUPFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}
```

Now, we go ahead and replace 

```c
const int DEDUPFS_MAGIC_NUMBER = 0x25252516;
const int DEDUPFS_DEFAULT_BLOCK_SIZE = 4 * 1024;
```

to

```c
#define SIMPLEFS_MAGIC 0x10032013
#define SIMPLEFS_DEFAULT_BLOCK_SIZE 4096
```

we also add `SIMPLEFS_FILENAME_MAXLEN` parameter

```c
#define SIMPLEFS_FILENAME_MAXLEN 255
```


and our custom `dedupfs_inode`

```c
struct dedupfs_inode {
	mode_t mode;
	__le32 inode_no;
	__le32 data_block_number;

	union {
		__le32 file_size;
		__le32 dir_children_count;
	};
};
```

after that we include `root_inode` in `dedupfs_super_block` structure

```c
struct dedupfs_super_block {
	__le32 version;
	__le32 magic;
	__le32 block_size;
	__le32 free_blocks;

	struct dedupfs_inode root_inode;

	char padding [(4 * 1024) - (4 * sizeof(__le32)) 
								- sizeof(struct dedupfs_inode)];
};
```

Now, we change slightly interface for `dedupfs_get_inode` from

```c
static struct inode *dedupfs_get_inode(struct super_block *sb, 
									  const struct inode *dir, umode_t mode)
```

to 

```c
static struct inode *dedupfs_allocate_inode(struct super_block *sb, 
										int new_inode_number,
									  const struct inode *dir, umode_t mode)
```

and here change inode number initialization

```c
	if(inode) {
		inode->i_ino = get_next_ino();
```	

to

```c
	if(inode) {
		inode->i_ino = new_inode_number;
```

After that we go ahead and modify `dedupfs_fill_superblock` slightly.
We set filesystem private info field in `super_block` structure to
our custom filesystem super block

```c
	/* A magic number that uniquely identifies our filesystem type */
	sb->s_magic = DEDUPFS_MAGIC_NUMBER;

	/* For all practical purposes, we will be using this s_fs_info as the super block */
	sb->s_fs_info = sb_disk;

	root_inode = dedupfs_allocate_inode(sb, DEDUPFS_ROOT_INODE_NUMBER,
									NULL, S_IFDIR);
```	

and also in `root_inode->i_private` field we save pointer to our 
custom filesystem inode structure (here for root)

```c
	root_inode = dedupfs_allocate_inode(sb, DEDUPFS_ROOT_INODE_NUMBER,
									NULL, S_IFDIR);
	root_inode->i_op = &dedupfs_inode_ops;
	root_inode->i_fop = &dedupfs_dir_operations;

	root_inode->i_private = &(sb_disk->root_inode);
```

which is defined in `dedupfs-structures.h` as 

```c
struct dedupfs_inode {
	mode_t mode;
	__le32 inode_no;
	__le32 data_block_number;

	union {
		__le32 file_size;
		__le32 dir_children_count;
	};
};
```

in linux sources `i_private` field of `inode` structure is defined as
 
```c
void                    *i_private; /* fs or device private pointer */
```

You can see it [here](http://lxr.free-electrons.com/source/include/linux/fs.h#L586).

Now we will implement `readdir` ( or `lookup`) support. But before we
do it let's define `DEDUPFS_INODE` convenience function in `super.h`
which will retrieve from `inode` our custom filesystem inode.

```c
#include "dedupfs-structures.h"

static inline struct dedupfs_super_block *DEDUPFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct dedupfs_inode *DEDUPFS_INODE(struct inode *inode)
{
	return inode->i_private;
}
```

```c
static int dedupfs_iterate(struct file *filp,
						   struct dir_context *ctx)
{
	loff_t pos;
	struct inode *inode;
	struct super_block *sb;
	struct buffer_head *bh;
	struct dedupfs_inode *dfs_inode;
	struct dedupfs_file_record *record;

	pos = ctx->pos;

	inode = file_inode(filp);
	sb = inode->i_sb;

	printk(KERN_INFO
		"We are inside iterate. The pos[%d], inode_number[%d]\n",
		pos, inode->i_ino);


	dfs_inode = DEDUPFS_INODE(inode);

	if(!S_ISDIR(dfs_inode->mode)) {
		printk(KERN_ERR
			   "inode [%d] is not a directory\n",
			   dfs_inode->inode_no);
		return -ENOTDIR;
	}

	bh = (struct buffer_head *)sb_bread(sb, dfs_inode->data_block_number);

	record = (struct dedupfs_file_record *)bh->b_data;
	for(int i = 0; i < dfs_inode->dir_children_count; i++) {
		printk(KERN_INFO "Got filename: %s\n", record->filename);
		dir_emit(ctx, record->filename, DEDUPFS_FILENAME_MAXLEN,
				record->inode_no, DT_UNKNOWN);
		ctx->pos += sizeof(struct dedupfs_file_record);
		record++;
	}
	brelse(bh);

	return 0;

}
```

Let's understand what `dedupfs_iterate` makes. First of all,
`loff_t pos;` will simply contain current offset while reading context
of directory `struct dir_context *ctx`.

After that we retrieve inode of directory and super block

```c
	inode = file_inode(filp);
	sb = inode->i_sb;
```

Now we go ahead and retrieve our custom inode and check if this inode 
is really associated with directory

```c
	dfs_inode = DEDUPFS_INODE(inode);

	if(!S_ISDIR(dfs_inode->mode)) {
		printk(KERN_ERR
			   "inode [%d] is not a directory\n",
			   dfs_inode->inode_no);
		return -ENOTDIR;
	}
```

Finally, we simply iterate over data block which is associated with 
this directory (`dfs_inode->data_block_number`) and fill directory 
context `ctx` with pairs `(record->filename, record->inode_no)`

```c
	bh = (struct buffer_head *)sb_bread(sb, dfs_inode->data_block_number);

	record = (struct dedupfs_file_record *)bh->b_data;
	for(int i = 0; i < dfs_inode->dir_children_count; i++) {
		printk(KERN_INFO "Got filename: %s\n", record->filename);
		dir_emit(ctx, record->filename, DEDUPFS_FILENAME_MAXLEN,
				record->inode_no, DT_UNKNOWN);
		ctx->pos += sizeof(struct dedupfs_file_record);
		record++;
	}
	brelse(bh);
```


Also we should fill `root_inode` on disk with valid information
in `mkfs-dedupfs.c`

```c
	sb.root_inode.mode = S_IFDIR;
	sb.root_inode.inode_no = DEDUPFS_ROOT_INODE_NUMBER;
	sb.root_inode.data_block_number = DEDUPFS_ROOTDIR_DATABLOCK_NUMBER;
	sb.root_inode.dir_children_count = 0;
```

where these constants are defined in `dedupfs-structures.h`

```c
const int DEDUPFS_ROOT_INODE_NUMBER = 1;
const int DEDUPFS_ROOTDIR_DATABLOCK_NUMBER = 2;
```

That's all. Only one detail we should take into account: it's `for`
loop which allowed only in `c99` mode. So we should add this line to 
Makefile

```
ccflags-y := -std=gnu99
```

**Makefile**

```
obj-m := dedupfs.o
dedupfs-objs := super.o

ccflags-y := -std=gnu99

PWD := $(shell pwd)

all: ko mkfs-dedupfs

ko:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

mkfs-dedupfs_SOURCES:
	mkfs-dedupfs.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```



