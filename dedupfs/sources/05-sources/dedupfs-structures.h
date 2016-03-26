
const int DEDUPFS_MAGIC_NUMBER = 0x25252516;
const int DEDUPFS_DEFAULT_BLOCK_SIZE = 4 * 1024;

struct dedupfs_super_block {
	unsigned int version;
	unsigned int magic;
	unsigned int block_size;
	unsigned int free_blocks;

	char padding [(4 * 1024) - (4 * sizeof(unsigned int))];
};



