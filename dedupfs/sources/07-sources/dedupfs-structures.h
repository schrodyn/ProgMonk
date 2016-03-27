#include <linux/types.h>

#define DEDUPFS_MAGIC_NUMBER  0x25252516
#define DEDUPFS_DEFAULT_BLOCK_SIZE 4096
#define DEDUPFS_FILENAME_MAXLEN 255

/* Hard-coded inode number for the root directory */
const int DEDUPFS_ROOT_INODE_NUMBER = 1;

/* The disk block where super block is stored */
const int DEDUPFS_SUPERBLOCK_BLOCK_NUMBER = 0;

/* The disk block where the inodes are stored */
const int DEDUPFS_INODESTORE_BLOCK_NUMBER = 1;

/* The disk block where the name+inode_number pairs of the
 * contents of the root directory are stored */
const int DEDUPFS_ROOTDIR_DATABLOCK_NUMBER = 2;

/* The name+inode_number pair for each file in a directory.
 * This gets stored as the data for a directory */
struct dedupfs_file_record {
	char filename[DEDUPFS_FILENAME_MAXLEN];
	__le32 inode_no;
};

struct dedupfs_inode {
	mode_t mode;
	__le32 inode_no;
	__le32 data_block_number;

	union {
		__le32 file_size;
		__le32 dir_children_count;
	};
};

struct dedupfs_super_block {
	__le32 version;
	__le32 magic;
	__le32 block_size;
	__le32 free_blocks;

	struct dedupfs_inode root_inode;

	char padding [(4 * 1024) - (4 * sizeof(__le32)) 
								- sizeof(struct dedupfs_inode)];
};



