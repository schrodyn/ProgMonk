#include "dedupfs-structures.h"

static inline struct dedupfs_super_block *DEDUPFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct dedupfs_inode *DEDUPFS_INODE(struct inode *inode)
{
	return inode->i_private;
}
