#Load superblock from disk

First of all, to read from disk we will use linux `buffer_head`
structure. As said [here](http://lxr.free-electrons.com/source/include/linux/buffer_head.h?v=3.13#L49), `buffer_head` structure  was used 
as the unit of I/O through the filesystem and block layers.
And there we also see its definition

```c
struct buffer_head {
	unsigned long b_state;          /* buffer state bitmap (see above) */
	struct buffer_head *b_this_page;/* circular list of page's buffers */
	struct page *b_page;            /* the page this bh is mapped to */

	sector_t b_blocknr;             /* start block number */
	size_t b_size;                  /* size of mapping */
	char *b_data;                   /* pointer to data within the page */

	struct block_device *b_bdev;
	bh_end_io_t *b_end_io;          /* I/O completion */
	void *b_private;                /* reserved for b_end_io */
	struct list_head b_assoc_buffers; /* associated with another mapping */
	struct address_space *b_assoc_map;      /* mapping this buffer is
	associated with */
	atomic_t b_count;               /* users using this buffer_head */
};
```

To read from the underlying block device. Following is a typical way 
of doing it:

```c
struct buffer_head *bh;

bh = sb_bread(sb, block); /* sb is the struct super_block pointer */
// bh->b_data contains the data
// Once done, bh should be released using:
brelse(bh);
```

`sb_bread` has following declaration

```c
static inline struct buffer_head * sb_bread(struct super_block * sb, sector_t block);
```

which calls 

```c
__bread(sb->_bdev,block,sb->s_blocksize);
```

To use it we need to include `<linux/buffer_head.h>` header so we use 
this function in `super_block` initialization

```c
static int dedupfs_fill_superblock(struct super_block *sb, void *data,
									int silent)
{
	struct inode *root_inode;
	struct buffer_head *bh;
	struct dedupfs_super_block *sb_disk;

	bh = (struct buffer_head *)sb_bread(sb, 0);

	sb_disk = (struct dedupfs_super_block *)bh->b_data;

	printk(KERN_INFO "The magic number obtained in disk is: [%d]\n",
				sb_disk->magic);

	if (sb_disk->magic != DEDUPFS_MAGIC_NUMBER) {
		printk(KERN_ERR
			"The filesystem that you try to mount is not of "
				"type dedupfs\n");
		return -EPERM;
	}

	if (sb_disk->block_size != DEDUPFS_DEFAULT_BLOCK_SIZE) {
		printk(KERN_ERR
			"dedupfs was formatted using a non-standard block size\n"
		);
		return -EPERM;
	}

	/* A magic number that uniquely identifies our filesystem type */
	sb->s_magic = DEDUPFS_MAGIC_NUMBER;

	root_inode = dedupfs_get_inode(sb, NULL, S_IFDIR);
	root_inode->i_op = &dedupfs_inode_ops;
	root_inode->i_fop = &dedupfs_dir_operations;

	sb->s_root = d_make_root(root_inode);
	if (!sb->s_root) {
		printk(KERN_ERR "root creation failed\n");
		return -ENOMEM;
	}

	return 0;	

}
```

The code is very simple, the most interesting part is `return`
constants which are defined in `include/asm-generic/errno-base.h`:

  * `EPERM` -- Operation not permitted
  * `ENOMEM` -- Out of memory



