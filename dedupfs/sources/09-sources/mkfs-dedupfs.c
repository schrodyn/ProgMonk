#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <strings.h>

#include "dedupfs-structures.h"

/* exit file with fd descriptor and return  */
static inline int close_and_exit(int fd, int return_code){
	close(fd);
	exit(return_code);	
}

int main(int argc, char *argv[])
{
	int fd;
	int nbytes;
	ssize_t ret;
	struct dedupfs_super_block sb;
	struct dedupfs_inode root_inode;
	struct dedupfs_inode welcomefile_inode;

	char welcomefile_name[]="readme";
	char welcomefile_body[] = "The mind is everything.\nWhat you think \nyou become.";
	const __le32 WELCOMEFILE_INODE_NUMBER = 2;
	const __le32 WELCOMEFILE_DATABLOCK_NUMBER = 3;

	char *block_padding;

	struct dedupfs_file_record record;

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


	/* One inode for rootdirectory and another for a welcome file that we are going to create */
	sb.inodes_count = 2;
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
	root_inode.dir_children_count = 1;

	ret = write(fd, (char *)&root_inode, sizeof(root_inode));

	if(ret != sizeof(root_inode)) {
		printf("The root inode was not written properly to inode "
		"store . Retry your mkfs\n");
		close_and_exit(fd, -2);

	}

	printf("The root inode was written succesfully\n");

	welcomefile_inode.mode = S_IFREG;
	welcomefile_inode.inode_no = WELCOMEFILE_INODE_NUMBER;
	welcomefile_inode.data_block_number = WELCOMEFILE_DATABLOCK_NUMBER;
	welcomefile_inode.file_size = sizeof(welcomefile_inode);
	ret = write(fd, (char *)&welcomefile_inode, sizeof(welcomefile_inode));

	if(ret != sizeof(root_inode)) {
		printf("The welcome file inode was not written properly.\n");
		close_and_exit(fd, -2);
	}

	printf("welcomefile inode written succesfully\n");

	nbytes = DEDUPFS_DEFAULT_BLOCK_SIZE - sizeof(root_inode) 
										- sizeof(welcomefile_inode);
	block_padding = malloc(nbytes);

	ret = write(fd, block_padding, nbytes);

	if(ret != nbytes) {
		printf("The padding bytes are not written properly.\n");
		close_and_exit(fd, -1);
	}

	printf("inode store padding bytes written succesfully\n");

	strcpy(record.filename, welcomefile_name);
	record.inode_no = WELCOMEFILE_INODE_NUMBER;
	nbytes = sizeof(record);

	ret = write(fd, (char *)&record, nbytes);
	if(ret != nbytes) {
		printf("writing the rootdirectory datablock has failed\n");
		close_and_exit(fd, -1);
	}

	printf("root directory datablocks written succesfully\n");

	nbytes = DEDUPFS_DEFAULT_BLOCK_SIZE - sizeof(record);
	block_padding = realloc(block_padding, nbytes);

	ret = write(fd, block_padding, nbytes);
	if(ret != nbytes) {
		printf("writing the padding for rootdirectory datablock"
					" has failed\n");
		close_and_exit(fd, -1);
	}

	printf("padding for rootdirectory datablock written succesfully\n");

	nbytes = sizeof(welcomefile_body);
	ret = write(fd, welcomefile_body, nbytes);
	if(ret != nbytes) {
		printf("writing welcomefile body has failed\n");
		close_and_exit(fd, -1);
	}

	printf("welcomefile body has been written succesfully\n");

	close_and_exit(fd, 0);

}
