
The FAT (File Allocation Table) file system has its origins in the late 1970s and
early 1980s and was the file system supported by the Microsoft MS-DOS operating
system.

It was originally developed as a simple file system suitable for floppy disk
drives less than 500K in size. Over time it has been enhanced to support larger and larger media.
Currently there are three FAT file system types: FAT12, FAT16 and FAT32.

The basic difference in these FAT sub types, and the reason for the names,
is the size, in bits, of the entries in the actual FAT structure on the disk.
There are 12 bits in a FAT12 FAT entry, 16 bits in a FAT16 FAT entry and 32 bits
in a FAT32 FAT entry.

Beginning
---------

The first sector of the drive is called the Master Boot Record (MBR). You can read
 it with LBA = 0.

For any new projects, you should not even worry about accessing a drive
in CHS mode, as LBA just numbers the sectors sequentially starting at zero, which
is much simpler.

for remainder, LBA(c,h,s) = (c × HPC + h) × SPT + (s - 1)
where
HPC is the maximum number of heads per cylinder (reported by disk drive,
typically 16 for 28-bit LBA)

SPT is the maximum number of sectors per track (reported by disk drive, typically
63 for 28-bit LBA)

All IDE drives support accessing sectors using LBA.
Also, all IDE drives use sectors that are 512 bytes.

Recent Microsoft operating systems refer to using larger sectors, but the drives
still use 512 bytes per sector and MS is just treating multiple sectors as if they
were one sector.

The first 446 bytes of the MBR are code that boots the computer. This is followed
by a 64 byte partition table, and the last two bytes are always 0x55 and 0xAA.
You should always check these last two bytes, as a simple "sanity check" that the
MBR is ok.

The MBR can only represent four partitions.(each 16bytes)
A technique called "extended" partitioning is used to allow more than four, and
often times it is used when there are more than two partitions.

All we're going to say about extended partitions is that they appear in this table
 just like a normal partition, and their first sector has another partition table
that describes the partitions within its space.
But for the sake of simply getting some code to work, we're going to not worry
about extended partitions
The most common scenario is only one partition using the whole drive, with
partitions 2, 3 and 4 blank.

Each partition description is just 16 bytes. The fifth byte is a Type Code
that tells what type of filesystem is supposed to be contained within the
partition.
and the ninth through twelth bytes indicate the LBA Begin address where that
partition begins on the disk.

```
	----------------------------------------------------------------------
	| Boot | CHS Begin | Type | CHS End  | LBA Begin | Number of Sectors |
	| Flag | (3bytes)  | Code | (3bytes) | (4bytes)  |      (4bytes)     |
	----------------------------------------------------------------------
```

Normally you only need to check the Type Code of each entry, looking for either
0x0B or 0x0C (the two that are used for FAT32), and then read the LBA Begin to
learn where the FAT32 filesystem is located on the disk.

The Number of Sectors field can be checked to make sure you do not access
(particularly write) beyond the end of the space that is allocated for the
parition.
However, the FAT32 filesystem itself contains information about its size, so this
Number of Sectors field is redundant.

Several of Microsoft's operating systems ignore it and instead rely on the size in
formation embedded within the first sector of the filesystem.
Linux checks the Number of Sectors field and properly prevents access beyond the
allocated space.

The Boot Flag, CHS Begin, and CHS End fields should be ignored.

FAT32 Reading
-------------

The first step to reading the FAT32 filesystem is the read its first sector,
called the Volume ID.
The Volume ID is read using the LBA Begin address found from the partition table.

From this sector, you will extract information that tells you everything you need to know about the physical layout of the FAT32 filesystem.

Microsoft's specification lists many variables in FAT32 Volume ID, but
most of the information is not needed for simple code.
Only four variables are required, and three others should be checked to make sure
 they have the expected values.

Field	 |		Microsoft's Name |	Offset	Size |	Value
---------|-----------------------|---------------|-------------------------------
Bytes Per Sector |	BPB_BytsPerSec	|	0x0B  |	16 Bits	Always 512 Bytes
Sectors Per Cluster |	BPB_SecPerClus |		0x0D |	8 Bits	1,2,4,8,16,32,64,
Number of Reserved Sectors |	BPB_RsvdSecCnt |		0x0E |	16 Bits	Usually 0x20
Number of FATs	|	BPB_NumFATs	 |	0x10 |	8 Bits	Always 2
Sectors Per FAT	 |	BPB_FATSz32	 |	0x24 |	32 Bits	Depends on disk size
Root Directory First |	BPB_RootClus	|	0x2C |	32 Bits	Usually 0x00000002
Cluster Signature	|	(none)		|	0x1FE |	16 Bits	Always 	0xAA55

After checking the three fields to make sure the filesystem is:
using 512 byte sectors, 2 FATs, and has a correct signature
you may "drop" these variables read from the MBR and Volume ID into just
four simple numbers that are needed for accessing the FAT32:

```c
(unsigned long)fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;
(unsigned long)cluster_begin_lba = Partition_LBA_Begin +
		Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_FAT);
(unsigned char)sectors_per_cluster = BPB_SecPerClus;
(unsigned long)root_dir_first_cluster = BPB_RootClus;
```

So most of the information is needed only to learn the location of the first
cluster and the FAT.
You will need to remember the size of the clusters and where the root directory is
 located.

Microsoft's formulas do not show the "Partition_LBA_Begin" term.
Their formulas are all relative to the beginning of the filesystem, which they
don't explicitly state very well.

You must add the "Partition_LBA_Begin" term found from the MBR to compute correct
LBA addresses for the IDE interface, because to the drive the MBR is at zero, not
the Volume ID.

How The FAT32 Filesystem Is Arranged
------------------------------------

The layout of a FAT32 filesystem is simple.
The first sector is always the Volume ID, which is followed by some unused space
called the reserved sectors.

Following the reserved sectors are two copies of the FAT (File Allocation Table).

The remainder of the filesystem is data arranged in "clusters".

The vast majority of the disk space is the clusters section, which is used to hold
 all the files and directories.
The clusters begin their numbering at 2, so there is no cluster #0 or cluster #1

To access any particular cluster, you need to use this formula to turn the cluster
 number into the LBA address for the IDE drive:

lba_addr = cluster_begin_lba + (cluster_number - 2) * sectors_per_cluster;

Normally clusters are at least 4k (8 sectors), and sizes of 8k, 16k and 32k are
also widely used.
The FAT32 specification from Microsoft states that 32k is the maximum cluster
size.

Now If Only We Knew Where The Files Were....
--------------------------------------------

When you begin, you only know the first cluster of the root directory.
Reading the directory will reveal the names and first cluster location of other
files and subdirectories.

A key point is that directories only tell you how to find the first cluster number of their files and subdirectories.
You also obtain a variety of other info from the directory such as
the file's length, modification time, attribute bits, etc, but a directory only
tells you where the files begin.

To access more than the first cluster, you will need to use the FAT.
But first we need to be able to find where those files start.

Directory data is organized in 32 byte records.
This is nice, because any sector holds exactly 16 records, and no directory record
 will ever cross a sector boundry.
There are four types of 32-byte directory records:

  1. Normal record with short filename - Attrib is normal
  2. Long filename text - Attrib has all four type bits set
  3. Unused - First byte is 0xE5
  4. End of directory - First byte is zero

Unused directory records are a result of deleting files.
The first byte is overwritten with 0xE5, and later when a new file is created it
can be reused.

At the end of the directory is a record that begins with zero. All other records
will be non-zero in their first byte, so this is an easy way to determine when you
 have reached the end of the directory.

Records that do not begin with 0xE5 or zero are actual directory data, and the
format can be determined by checking the Attrib byte.

In FAT32, all files and subdirectories have short names, even if the user gave
the file a longer name, so you can access all files without needing to decode the
long filename records (as long as your code simply ignores them).

Here is the format of a normal directory record:

```
		Short Filename    Attrib	    Cluster High    Cluster Low Size
	-----------------------------------------------------------------------------
	|	11 bytes	|  1   |	        |   2   | 4       |   2   | 4
	|			    | byte | 8 bytes... | bytes | bytes.. | bytes | bytes
	-----------------------------------------------------------------------------
```

Field	|	Microsoft's Name  |	Offset |	Size
--------|---------------------|--------|-----------------
Short Filename |	DIR_Name	|	0x00  |	11 Bytes
Attrib Byte |	DIR_Attr |		0x0B |	8 Bits
High First Cluster |	DIR_FstClusHI	|	0x14 |	16 Bits
Low First Cluster |	DIR_FstClusLO	|	0x1A |	16 Bits
File Size |	DIR_FileSize	| 	0x1C |	32 Bits


The Attrib byte has six bits defined, as shown in the table below.
Most simple firmware will check the Attrib byte to determine if the 32 bytes are a normal record or
long filename data, and to determine if it is a normal file or a subdirectory.

Long filename records have all four of the least significant bits set. Normal files rarely have any
of these four bits set.


Attrib Bit |	Function |	LFN |	Comment
-----------|-------------|------|-------------------------------
0 (LSB)	|	Read Only	1 |	Should not allow writing
1	|	Hidden	|	1 |	Should not show in dir listing
2	|	System	|	1 |	File is operating system
3	|	Volume ID |	1 |	Filename is Volume ID
4	|	Directory |	x |	Is a subdirectory (32-byte records)
5	|	Archive	|	x |	Has been changed since last backup
6	|	Ununsed	|	0 |	Should be zero
7 (MSB)	 |	Ununsed	|	0 |	Should be zero


The remaining fields are relatively simple and straigforward.
The first 11 bytes are the short filename. The extension is always the last three bytes.
If the file's name is shorter than 8 bytes, the unused bytes are filled
with spaces (0x20).

The starting cluster number is found as two 16 bit sections, and the file size (in bytes) is found
in the last four bytes of the record.
In both, the least significant byte is first.

The first cluster number tells you where the file's data begins on the drive,
and the size field tells you how long the file is actual space allocated on the disk will be an
integer number of clusters,
so the file size lets you know how much of the last cluster is the file's data.


File Allocation Table - Following Cluster Chains
------------------------------------------------

The directory entries tell you where the first cluster of each file (or subdirectory) is located
on the disk, and of course you find the first cluster of the root directory from the volume ID sector.

For tiny files and directories (that fit inside just one cluster), the only information you obtain
from the FAT is that there are no more clusters for that file.
To access all the other clusters of a file beyond the first one, you need to use the File Allocation
 Table.

The name FAT32 refers to this table, and the fact that each entry of the table is 32 bits.
In FAT16 and FAT12, the entries are 16 and 12 bits.
FAT16 and FAT12 work the same way as FAT32. We're only going to look at FAT32.

The File Allocation Table is a big array of 32 bit integers, where each one's position in the array
corresponds to a cluster number,
and the value stored indicates the next cluster in that file.

The purpose of the FAT is to tell you where the next cluster of a file is located on the disk, when
you know where the current cluster is at.

Every sector of the FAT holds 128 of these 32 bit integers, so looking up the next cluster of a file
is relatively easy.
Bits 7-31 of the current cluster tell you which sectors to read from the FAT,
and bits 0-6 tell you which of the 128 integers in that sector contain is the number of the next
cluster of your file (or if all ones, that the current cluster is the last).
