#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/delay.h>

#include "super.h"

static DEFINE_MUTEX(dedupfs_sb_lock);
static DEFINE_MUTEX(dedupfs_inodes_mgmt_lock);
static DEFINE_MUTEX(dedupfs_directory_children_update_lock);

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

int dedupfs_sb_get_a_freeblock(struct super_block *vsb, __le32 *out)
{
	struct dedupfs_super_block *sb = DEDUPFS_SB(vsb);
	int ret = 0;
	int i;

	if(mutex_lock_interruptible(&dedupfs_sb_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		ret = -EINTR;
		goto end;
	}


	/* Loop until we find a free block. */
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

static int dedupfs_sb_get_objects_count(struct super_block *vsb,
										__le32 *out)
{
	struct dedupfs_super_block *sb = DEDUPFS_SB(vsb);

	if(mutex_lock_interruptible(&dedupfs_inodes_mgmt_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}
	*out = sb->inodes_count;
	mutex_unlock(&dedupfs_inodes_mgmt_lock);

	return 0;
}

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

	if (pos) {
		printk(KERN_INFO "pos seem to be non-zero which means "
					"we have already filled in all the details\n");
		return 0;
	}

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

const struct file_operations dedupfs_file_operations = {
	.read = dedupfs_read,	
};


struct dentry *dedupfs_lookup(struct inode *parent_inode,
							  struct dentry *child_dentry, 
							  unsigned int flags);


static int dedupfs_create(struct inode *dir, struct dentry *dentry,
						umode_t mode, bool excl);

static struct inode_operations dedupfs_inode_ops = {
	.create = dedupfs_create,
	.lookup = dedupfs_lookup,
};

static int dedupfs_create(struct inode *dir, struct dentry *dentry,
						umode_t mode, bool excl)
{
	struct inode *inode;
	struct dedupfs_inode *dfs_inode;
	struct super_block *sb;
	struct dedupfs_file_record *record;
	struct dedupfs_inode *parent_dir_inode;
	struct buffer_head *bh;
	struct dedupfs_file_record *dir_contents_datablock;
	__le32 count;
	int ret;

	if(mutex_lock_interruptible(&dedupfs_directory_children_update_lock)) {
		printk(KERN_ERR "Failed to acquire mutex lock %s +%d\n",
					__FILE__, __LINE__);
		return -EINTR;
	}
	sb = dir->i_sb;

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

	if(!S_ISDIR(mode) && !S_ISREG(mode)) {
		printk(KERN_ERR
				"Creation request for neither a file nor a directory\n");
		return -EINVAL;
	}

	inode = new_inode(sb);
	if(!inode)
		return -ENOMEM;
	
	inode->i_sb = sb;
	inode->i_op = &dedupfs_inode_ops;
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	inode->i_ino = 11;

	/* Loop until we get an unique inode number */
	while(dedupfs_get_inode(sb, inode->i_ino)) {
		printk(KERN_INFO "inode [%lu] already exists\n", inode->i_ino);
		inode->i_ino++;
	}
	printk(KERN_INFO "Got new unique inode number [%lu]\n", inode->i_ino);

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

	ret = dedupfs_sb_get_a_freeblock(sb, &dfs_inode->data_block_number);
	if(ret < 0) {
		printk(KERN_ERR "dedupfs could not get a freeblock\n");
		return ret;
	}

	dedupfs_inode_add(sb, dfs_inode);

	record = kmalloc(sizeof(struct dedupfs_file_record), GFP_KERNEL);
	record->inode_no = dfs_inode->inode_no;
	strcpy(record->filename, dentry->d_name.name);

	printk(KERN_INFO "New filename is %s\n", record->filename);

	parent_dir_inode = DEDUPFS_INODE(dir);
	bh = sb_bread(sb, parent_dir_inode->data_block_number);
	dir_contents_datablock = (struct dedupfs_file_record *)bh->b_data;

	/* Navigate to the last record in the directory contents */	
	dir_contents_datablock += parent_dir_inode->dir_children_count;

	memcpy(dir_contents_datablock, record,
				sizeof(struct dedupfs_file_record));

	mark_buffer_dirty(bh);
	sync_dirty_buffer(bh);

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

	mutex_unlock(&dedupfs_inodes_mgmt_lock);
	mutex_unlock(&dedupfs_directory_children_update_lock);

	printk(KERN_INFO "Returning success after creating the file\n");

	inode_init_owner(inode, dir, mode);
	d_add(dentry, inode);

	/* This sleep is necessary to update the dentry cache */
	msleep(5);
	return 0;
}


struct dentry *dedupfs_lookup(struct inode *parent_inode,
							  struct dentry *child_dentry, 
							  unsigned int flags)
{
	struct dedupfs_inode *parent = DEDUPFS_INODE(parent_inode);
	struct super_block *sb = parent_inode->i_sb;
	struct buffer_head *bh;
	struct dedupfs_file_record *record;

	bh = (struct buffer_head *)sb_bread(sb, parent->data_block_number);

	record = (struct dedupfs_file_record *) bh->b_data;
	for(int i = 0; i < parent->dir_children_count; i++) {
		if(!strcmp(record->filename, child_dentry->d_name.name)) {
			
			struct inode *inode;
			struct dedupfs_inode *dfs_inode;

			dfs_inode = dedupfs_get_inode(sb, record->inode_no);

			inode = new_inode(sb);
			inode->i_ino = record->inode_no;
			inode_init_owner(inode, parent_inode, dfs_inode->mode);
			inode->i_sb = sb;
			inode->i_op = &dedupfs_inode_ops;

			if(S_ISDIR(inode->i_mode))
				inode->i_fop = &dedupfs_dir_operations;
			else if (S_ISREG(inode->i_mode))
				inode->i_fop = &dedupfs_file_operations;
			else
				printk(KERN_ERR
						"Unknown inode type. Neither a directory "
						"nor a file");

			/* FIXME: we should store these times to disk and 
			retrieve them */
			inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

			inode->i_private = dfs_inode;

			d_add(child_dentry, inode);
			return NULL;
		}
		record++;
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


