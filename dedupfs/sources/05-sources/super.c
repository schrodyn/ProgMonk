#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#include "dedupfs-structures.h"

static int dedupfs_iterate(struct file *filp,
						   struct dir_context *ctx)
{
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


