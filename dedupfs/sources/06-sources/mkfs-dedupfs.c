#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#include "dedupfs-structures.h"

int main(int argc, char *argv[])
{
	int fd;
	ssize_t ret;
	struct dedupfs_super_block sb;

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
	sb.free_blocks = ~0;

	ret = write(fd, (char *)&sb, sizeof(sb));

	if (ret != DEDUPFS_DEFAULT_BLOCK_SIZE) {
		fprintf(stderr,
					"bytes written [%d] are not equal to the default"
					"block size\n", (int)ret);
	}
	else
		printf("Super block written succesfully\n");

	close(fd);

	return 0;

}
