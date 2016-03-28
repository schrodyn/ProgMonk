#Inode Store

Now we will retrieve the rootdir inode from the inode store instead of
the super block. First we should slightly modify our dedupfs structures
in `dedupfs-structures.h` header, exactly `dedupfs_super_block`

instead of

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

we change it to

```c
struct dedupfs_super_block {
	__le32 version;
	__le32 magic;
	__le32 block_size;
	__le32 free_blocks;
	__le32 inodes_count;

	char padding [(4 * 1024) - (5 * sizeof(__le32))];
};
```

you may notice we've added new field `inodes_count`; it will hold the
size of inodes table for filesystem.

Now we will create `dedupfs_get_inode` function which returns 
a `dedupfs_inode` with the given `inode_no` from the inode store, if it
exists.

```c
static struct dedupfs_inode *dedupfs_get_inode(struct super block *sb,
										__le32 inode_no)
{
	struct dedupfs_super_block *dfs_sb = DEDUPFS_SB(sb);
	struct dedupfs_inode *dfs_inode = NULL;

	struct buffer_head *bh;

	bh = (struct buffer_head *)sb_bread(sb, DEDUPFS_INODESTORE_BLOCK_NUMBER);
	dfs_inode = (struct dedupfs_inode *) bh->b_data;

	for(int i = 0; i < dfs_sb->inodes_count; i++) {
		if(dfs_inode->inode_no == inode_no) {
			return dfs_inode;
		}
		dfs_inode++;
	}

	return NULL;
}
```

This function is very straightforward, we simply iterate over inode 
store and look for inode with `inode_no` number. After we find it
we return it, otherwise we return NULL.

The inode store can be read once and kept in memory permanently while 
mounting. But such a model will not be scalable in a filesystem with
millions or billions of files (inodes).

In `dedupfs_fill_superblock` we change explicit reading of zero block

```c
bh = (struct buffer_head *)sb_bread(sb, 0);
```

to

```c
bh = (struct buffer_head *)sb_bread(sb, DEDUPFS_SUPERBLOCK_BLOCK_NUMBER);
```

and since we don't hold root inode in our super block anymore we 
change

```c
	root_inode->i_private = &(sb_disk->root_inode);
```

to

```c
root_inode->i_private = dedupfs_get_inode(sb, DEDUPFS_ROOT_INODE_NUMBER);
```

Similarly, in `mkfs-dedupfs.c` we add

```c
struct dedupfs_inode root_inode;
```

and replace

```c
	sb.root_inode.mode = S_IFDIR;
	sb.root_inode.inode_no = DEDUPFS_ROOT_INODE_NUMBER;
	sb.root_inode.data_block_number = DEDUPFS_ROOTDIR_DATABLOCK_NUMBER;
	sb.root_inode.dir_children_count = 0;
```

to 

```c
	root_inode.mode = S_IFDIR;
	root_inode.inode_no = DEDUPFS_ROOT_INODE_NUMBER;
	root_inode.data_block_number = DEDUPFS_ROOTDIR_DATABLOCK_NUMBER;
	root_inode.dir_children_count = 0;
```

Also we should fill `inodes_count` field of super block

```c
	/* Only the root dir will have an inode now */
	sb.inodes_count = 1;
	sb.free_blocks = ~0;
```

and since we now don't hold `root_inode` in `sb` structure, then
we should write `root_inode` on the disk

```c
ret = write(fd, (char *)&root_inode, sizeof(root_inode));
```

Finally I've added convenience inline function for exiting and closing
file descriptor

```c
/* exit file with fd descriptor and return  */
static inline int close_and_exit(int fd, int return_code){
	close(fd);
	exit(return_code);	
}
```















