#Create a welcome file

Now we will create one welcome file on our disk image and try to
list it. Let's start with creating a file in `mkfs-dedupfs.c`

First of all, we must create `dedupfs_inode` structure for our
new file

```c
struct dedupfs_inode welcomefile_inode;
```

and define welcomefile name, body and also inode number and data block
number for this file

```c
	char welcomefile_name[]="readme";
	char welcomefile_body[] = "The mind is everything.\nWhat you think \nyou become.";
	const __le32 WELCOMEFILE_INODE_NUMBER = 2;
	const __le32 WELCOMEFILE_DATABLOCK_NUMBER = 3;
```

apart from this we should also define

```c
char *block_padding;
```

for padding block to filesystem block size, and

```c
	struct dedupfs_file_record record;
```

for holding file record pair (name and inode number) for welcomefile

Now, let's go ahead and modify 

```c
	/* Only the root dir will have an inode now */
	sb.inodes_count = 1;
```	

to 

```c
	/* One inode for rootdirectory and another for a welcome file that we are going to create */
	sb.inodes_count = 2;
```	

since we have already two inodes: one for root directory and one for
a welcomefile. Surely, we change

```c
root_inode.dir_children_count = 0;
```

to

```c
root_inode.dir_children_count = 1;
```

Now, it's time to fill welcomefile inode with valid information and
write it to the appropriate place on the disk.

```c
	welcomefile_inode.mode = S_IFREG;
	welcomefile_inode.inode_no = WELCOMEFILE_INODE_NUMBER;
	welcomefile_inode.data_block_number = WELCOMEFILE_DATABLOCK_NUMBER;
	welcomefile_inode.file_size = sizeof(welcomefile_inode);
	ret = write(fd, (char *)&welcomefile_inode, sizeof(welcomefile_inode));
```

after writing `welcomefile_inode` to inode store we should pad inode
store block with approprite number of bytes

```c
	nbytes = DEDUPFS_DEFAULT_BLOCK_SIZE - sizeof(root_inode) 
										- sizeof(welcomefile_inode);
	block_padding = malloc(nbytes);

	ret = write(fd, block_padding, nbytes);
```

Now, It's time to fill rootdirectory datablock with welcomefile record
(name+inode pair) 

```c
 	strcpy(record.filename, welcomefile_name);
	record.inode_no = WELCOMEFILE_INODE_NUMBER;
	nbytes = sizeof(record);

	ret = write(fd, (char *)&record, nbytes);
```	

Similarly, we should pad this block to filesystem block size

```c
	nbytes = DEDUPFS_DEFAULT_BLOCK_SIZE - sizeof(record);
	block_padding = realloc(block_padding, nbytes);

	ret = write(fd, block_padding, nbytes);
```

Finally, we write welcomefile content to the welcomefile data block

```c
	nbytes = sizeof(welcomefile_body);
	ret = write(fd, welcomefile_body, nbytes);
```	

Ok, we have written file on the disk.

Now at the beginning of `dedupfs_iterate` let's add one check 

```c
	if (pos) {
		printk(KERN_INFO "pos seem to be non-zero which means "
					"we have already filled in all the details\n");
		return 0;
	}
```

as `printk` says if `pos` is non-zero it seems that we have already 
filled directory context, so all work has already been done and we 
can just return from this function.




