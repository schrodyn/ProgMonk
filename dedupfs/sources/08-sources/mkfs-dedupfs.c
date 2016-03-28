#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#include "dedupfs-structures.h"

/* exit file with fd descriptor and return  */
static inline int close_and_exit(int fd, int return_code){
	close(fd);
	exit(return_code);	
}

int main(int argc, char *argv[])
{
	int fd;
	ssize_t ret;
	struct dedupfs_super_block sb;
	struct dedupfs_inode root_inode;

	if(argc != 2) {
		printf("Usage: mkfs-dedupfs <device>\n");
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		fprintf(stderr, "Error opening the device\n");
		return -1;
	}

	sb.version = 1;
	sb.magic = DEDUPFS_MAGIC_NUMBER;
	sb.block_size = DEDUPFS_DEFAULT_BLOCK_SIZE;


	/* Only the root dir will have an inode now */
	sb.inodes_count = 1;
	sb.free_blocks = ~0;

	
	ret = write(fd, (char *)&sb, sizeof(sb));

	if (ret != DEDUPFS_DEFAULT_BLOCK_SIZE) {
		fprintf(stderr,
					"bytes written [%d] are not equal to the default"
					"block size\n", (int)ret);
		close_and_exit(fd, -1);
	}
	
	
	printf("Super block written succesfully\n");


	root_inode.mode = S_IFDIR;
	root_inode.inode_no = DEDUPFS_ROOT_INODE_NUMBER;
	root_inode.data_block_number = DEDUPFS_ROOTDIR_DATABLOCK_NUMBER;
	root_inode.dir_children_count = 0;

	ret = write(fd, (char *)&root_inode, sizeof(root_inode));

	if(ret != sizeof(root_inode)) {
		printf("The inode store was not written properly. Retry your "
				"mkfs\n");
		close_and_exit(fd, -2);

	}

	close_and_exit(fd, 0);

}
