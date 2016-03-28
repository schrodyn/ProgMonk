#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>

#include "super.h"

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
		"We are inside iterate. The pos[%lu], inode_number[%lu]\n",
		(long unsigned int)pos, inode->i_ino);

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

static struct file_operations dedupfs_dir_operations = {
	.owner = THIS_MODULE,
	.iterate = dedupfs_iterate,
};

struct dentry *dedupfs_lookup(struct inode *parent_inode,
							  struct dentry *child_dentry, 
							  unsigned int flags)
{
	return NULL;
}

static struct inode_operations dedupfs_inode_ops = {
	.lookup = dedupfs_lookup,
};

static struct inode *dedupfs_allocate_inode(struct super_block *sb, 
										int new_inode_number,
									  const struct inode *dir, umode_t mode)
{
	struct inode *inode = new_inode(sb);

	if(inode) {
		inode->i_ino = new_inode_number;
		inode_init_owner(inode, dir, mode);

		inode->i_sb = sb;
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

static struct dedupfs_inode *dedupfs_get_inode(struct super_block *sb,
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

static int dedupfs_fill_superblock(struct super_block *sb, void *data,
									int silent)
{
	struct inode *root_inode;
	struct buffer_head *bh;
	struct dedupfs_super_block *sb_disk;

	bh = (struct buffer_head *)sb_bread(sb, DEDUPFS_SUPERBLOCK_BLOCK_NUMBER);

	sb_disk = (struct dedupfs_super_block *)bh->b_data;

	brelse(bh);

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

	/* For all practical purposes, we will be using this s_fs_info as the super block */
	sb->s_fs_info = sb_disk;

	root_inode = dedupfs_allocate_inode(sb, DEDUPFS_ROOT_INODE_NUMBER,
										NULL, S_IFDIR);
	root_inode->i_op = &dedupfs_inode_ops;
	root_inode->i_fop = &dedupfs_dir_operations;

	root_inode->i_private = dedupfs_get_inode(sb, DEDUPFS_ROOT_INODE_NUMBER);

	sb->s_root = d_make_root(root_inode);
	if (!sb->s_root) {
		printk(KERN_ERR "root creation failed\n");
		return -ENOMEM;
	}

	return 0;	

}

static struct dentry *dedupfs_mount(struct file_system_type *fs_type,
									int flags, const char *dev_name,
									void *data)
{
	struct dentry *ret;

	ret = mount_bdev(fs_type, flags, dev_name, data, dedupfs_fill_superblock);

	if(IS_ERR(ret))
		printk(KERN_ERR "Error mounting dedupfs\n");
	else
		printk(KERN_INFO "dedupfs is succesfully mounted on [%s]\n",
				dev_name);

	return ret;

}

static void dedupfs_kill_superblock(struct super_block *sb)
{
	printk(KERN_INFO
			"dedupfs superblock is destroyed. Unmount succesful.\n");
}

static struct file_system_type dedupfs_fs_type = {
	.owner = THIS_MODULE,
	.name = "dedupfs",
	.mount = dedupfs_mount,
	.kill_sb = dedupfs_kill_superblock,
};

static int __init dedupfs_init(void)
{
	int ret;

	ret = register_filesystem(&dedupfs_fs_type);
	if(ret == 0)
		printk(KERN_INFO "Succesfully registered dedupfs\n");
	else
		printk(KERN_ERR "Failed to register dedupfs. Error:[%d]\n", 
				ret);

	return ret;
}


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

module_init(dedupfs_init);
module_exit(dedupfs_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Shalymov");


