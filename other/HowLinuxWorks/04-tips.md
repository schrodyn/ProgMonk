disk devices have names like /dev/sda, the first SCSI subsystem disk.

there are many different components and layers inside a disk.

```
Partition Table
---------------
 |	    |
 v	    v
Partition   Partition
---------
    |
    v
Filesystem Data
Structures
    |
    v
File Data
```

Partitions are subdivisions of the whole disk.
On Linux, they’re denoted with a number after the whole block device,
and therefore have device names such as /dev/sda1 and /dev/sdb3.

The kernel presents each partition as a block device, just as it would an entire
disk.
Partitions are defined on a small area of the disk called a partition table.

The next layer after the partition is the filesystem, the database of files and
directories that you’re accustomed to interacting with in user space.

if you want to access the data in a file, you need to get the appropriate
partition location from the partition table and then
search the filesystem database on that partition for the desired file data.

you can work with the disk through the filesystem as well as directly through the
disk devices.

There are many kinds of partition tables.
The traditional table is the one found inside the Master Boot Record (MBR).
A newer standard starting to gain traction is
the Globally Unique Identifier Partition Table (GPT).

There is a critical difference between partitioning and filesystem manipulation.

The partition table defines simple boundaries on the disk,
whereas a filesystem is a much more involved data system. For this reason, we’ll
use parted for partitioning but use separate utilities for creating filesystems.

```
parted -l -- view your system’s partition table

Model: ATA ST1000LM024 HN-M (scsi)
Disk /dev/sda: 1000GB
Sector size (logical/physical): 512B/4096B
Partition Table: gpt

Number  Start   End     Size    File system     Name                  Flags
 1      1049kB  316MB   315MB   fat32           EFI System Partition  msftdata
 2      316MB   395GB   395GB   ext4
 3      395GB   408GB   12,3GB  linux-swap(v1)
 4      408GB   1000GB  593GB   ext4
```

The MBR table may contain primary, extended, and logical partitions.

A primary partition is a normal subdivision of the disk.
The basic MBR has a limit of four primary partitions, so if you want more than
four, you designate one partition as an extended partition.

Next, you subdivide the extended partition into logical partitions
that the operating system can use as it would any other partition.

`dmesg` -- Linux kernel debugging output

When initially reading the MBR table, the Linux kernel may produce the following debugging output:

```
sda: sda1 sda2 < sda5 >
```

The sda2 < sda5 > output indicates that `/dev/sda2` is an extended partition
containing one logical partition, `/dev/sda5`.

You’ll normally ignore extended partitions because you’ll typically want to access only the logical partitions inside.

There are two major Linux partitioning command-line tools: parted and fdsik

`gparted` -- GUI version of parted

With fdisk, you design your new partition table before making the actual changes
to the disk; fdisk only makes the changes as you exit the program.

with parted, partitions are created, modified, and removed as you issue the
commands.

Both fdisk and parted modify the partitions entirely in user space;
there is no need to provide kernel support for rewriting a partition table
because user space can read and modify all of a block device.

The kernel must read the partition table in order to present the partitions as block devices.

The fdisk utility uses a relatively simple method:
After modifying the partition table, fdisk issues a single system call on the disk to tell the kernel that it should reread the partition table.

The kernel then generates debugging output that you can view with dmesg.

The parted tools do not use this disk-wide system call.
Instead, they signal the kernel when individual partitions are altered.

There are a few ways to see the partition changes:

  1. Use udevadm to watch the kernel event changes.
  For example,
  `udevadm monitor --kernel`
  will show the old partition devices being removed and the new ones being added.

  2. Check `/proc/partitions` for full partition information.

  3. Check `/sys/block/device/` for altered partition system interfaces or
  `/dev` for altered partition devices.

To force the kernel to reload the partition table on `/dev/sdf`, run this:
```
blockdev --rereadpt /dev/sdf
```

#Disk and Partition Geometry

The traditional addressing scheme that uses CHS doesn’t scale with modern disk
hardware, nor does it account for the fact that you can put more data into outer
cylinders than inner cylinders.

Disk hardware supports Logical Block Addressing (LBA) to simply address a location on the disk by a block number, but remnants of CHS remain.

For example, the MBR partition table contains CHS information
as well as LBA equivalents,
and some boot loaders are still dumb enough to believe the CHS values
(don’t worry—most Linux boot loaders use the LBA values).

One of the most significant factors affecting the performance of SSDs is partition
alignment.

When you read data from an SSD, you read it in chunks — typically 4096 bytes
at a time — and the read must begin at a multiple of that same size.

So if your partition and its data do not lie on a 4096-byte boundary,
you may have to do two reads instead of one for small, common operations, such as
 reading the contents of a directory.

You don't need to worry about it because partition utilities put newly created
partitions at the proper offsets from the beginning of the disks

However, If you want to know where your partitions begin and make sure that they
begin on a boundary:

you can easily find this information by looking in `/sys/block`

for example, for a partition `/dev/sda1`:

```
cat /sys/block/sda/sda1/start

2048
```

This partition starts at 2048 bytes from the beginning of the disk.

Because this number is not divisible by 4,096, the partition would not be
attaining optimal performance if it were on SSD.

the filesystem is a form of database;
it supplies the structure to transform a simple block device into the
sophisticated hierarchy of files and subdirectories that users can understand.

The Second Extended filesystem (ext2) was a longtime default for Linux systems
inspired by traditional Unix filesystems such as the Unix File System (UFS) and
the Fast File System (FFS).

The Third Extended filesystem (ext3) added a journal feature (a small cache
outside the normal filesystem data structure) to
enhance data integrity and hasten booting.

The ext4 filesystem is an incremental improvement with support for larger files
than ext2 or ext3 support and a greater number of subdirectories.

There is a certain amount of backward compatibility:
For example, you can mount ext2 and ext3 filesystems as each other.
you can mount ext2 and ext3 filesystems as ext4,
but you cannot mount ext4 as ext2 or ext3.

ISO 9660 (iso9660) is a CD-ROM standard. Most CD-ROMs use some variety of the ISO 9660 standard.

FAT filesystems (msdos, vfat, umsdos) pertain to Microsoft systems.
For most modern Windows filesystems, you should use the vfat filesystem
in order to get full access from Linux.

HFS+ (hfsplus) is an Apple standard used on most Macintosh systems.

#Creating a Filesystem

Once you’re done with the partitioning process, you’re ready to
create filesystems.

As with partitioning, you’ll do this in user space because a user-space process
can directly access and manipulate a block device.

The mkfs utility can create many kinds of filesystems.

For example,
you can create an ext4 partition on /dev/sda2 with this command:
mkfs -t ext4 /dev/sda2

The mkfs program automatically determines the number of blocks in a device and
sets some reasonable defaults.

mkfs is only a frontend for a series of filesystem creation programs, mkfs.fs,
where fs is a filesystem type.

So when you run mkfs -t ext4, mkfs in turn runs mkfs.ext4.

```
ls -l /sbin/mkfs.*

-rwxr-xr-x 1 root root 18736 февр. 12 21:53 /sbin/mkfs.bfs
-rwxr-xr-x 1 root root 31216 февр. 12 21:53 /sbin/mkfs.cramfs
lrwxrwxrwx 1 root root     6 февр. 16 22:25 /sbin/mkfs.ext2 -> mke2fs
lrwxrwxrwx 1 root root     6 февр. 16 22:25 /sbin/mkfs.ext3 -> mke2fs
lrwxrwxrwx 1 root root     6 февр. 16 22:25 /sbin/mkfs.ext4 -> mke2fs
lrwxrwxrwx 1 root root     6 февр. 16 22:25 /sbin/mkfs.ext4dev -> mke2fs
-rwxr-xr-x 1 root root 27608 марта 18  2014 /sbin/mkfs.fat
-rwxr-xr-x 1 root root 27144 февр. 12 21:53 /sbin/mkfs.minix
lrwxrwxrwx 1 root root     8 окт.  29  2014 /sbin/mkfs.msdos -> mkfs.fat
lrwxrwxrwx 1 root root     6 окт.  29  2014 /sbin/mkfs.ntfs -> mkntfs
lrwxrwxrwx 1 root root     8 окт.  29  2014 /sbin/mkfs.vfat -> mkfs.fat
```

#Mounting a Filesystem

On Unix, the process of attaching a filesystem is called mounting.

When the system boots, the kernel reads
some configuration data and mounts root (/) based on the configuration data.

to mount a filesystem, you must know the following:

  1. The filesystem’s device (such as a disk partition; where the actual file-system data resides).
  2. The filesystem type
  3. The mount point -- that is, the place in the current system’s directory
  hierarchy where the filesystem will be attached.
  The mount point is always a normal directory.

For instance, you could use `/cdrom` as a mount point for CD-ROM devices.
The mount point may be anywhere on the system.

When mounting a filesystem, the common terminology is
“mount a device on a mount point.”

To learn the
current filesystem status of your system, run `mount`:

```
/dev/sda2 on / type ext4 (rw,errors=remount-ro)
proc on /proc type proc (rw,noexec,nosuid,nodev)
sysfs on /sys type sysfs (rw,noexec,nosuid,nodev)
none on /sys/fs/cgroup type tmpfs (rw)
none on /sys/fs/fuse/connections type fusectl (rw)
none on /sys/kernel/debug type debugfs (rw)
none on /sys/kernel/security type securityfs (rw)
none on /sys/firmware/efi/efivars type efivarfs (rw)
udev on /dev type devtmpfs (rw,mode=0755)
devpts on /dev/pts type devpts (rw,noexec,nosuid,gid=5,mode=0620)
tmpfs on /run type tmpfs (rw,noexec,nosuid,size=10%,mode=0755)
none on /run/lock type tmpfs (rw,noexec,nosuid,nodev,size=5242880)
none on /run/shm type tmpfs (rw,nosuid,nodev)
none on /run/user type tmpfs (rw,noexec,nosuid,nodev,size=104857600,mode=0755)
none on /sys/fs/pstore type pstore (rw)
/dev/sda1 on /boot/efi type vfat (rw)
/dev/sda4 on /home type ext4 (rw)
```

Each line corresponds to one currently mounted filesystem,
with items in this order:

  1. The device, such as /dev/sda3. Notice that some of these aren’t real devices
  (proc, for example) but are stand-ins for real device names because these
  special-purpose filesystems do not need devices.
  2. The word on
  3. The mount point
  4. The word type
  5. The filesystem type
  6. Mount options in parentheses

To mount a filesystem, use the mount command as follows:
```
mount -t type device mountpoint
```

For example, to mount the Fourth Extended filesystem `/dev/sdf2` on `/home/extra`,
use this command:

```
mount -t ext4 /dev/sdf2 /home/extra
```

To unmount (detach) a filesystem, use the umount command:
```
umount mountpoint
```

You can also unmount a filesystem with its device instead of its mount point.

#Filesystem UUID

The method of mounting filesystems discussed in the preceding section depends on
device names.
However,
device names can change because they depend on the order in which the kernel finds the devices. To solve this problem, you can identify and mount filesystems by
their Universally Unique Identifier (UUID)

UUID -- type of serial number, and each one should be different.

Filesystem creation programs like mke2fs generate a UUID identifier when
initializing the filesystem data structure.

To view a list of devices and the corresponding filesystems and UUIDs on your
system:

```
blkid

/dev/sda1: UUID="48DC-1AA1" TYPE="vfat"
/dev/sda2: UUID="2a8cf10f-ba21-40d0-8fec-b18f835d519a" TYPE="ext4"
/dev/sda3: UUID="69f7e49d-bcba-4c93-ac0b-d473db52a643" TYPE="swap"
/dev/sda4: UUID="9b86fa97-5406-4449-a577-3e94dddfbb03" TYPE="ext4"
```

To mount a filesystem by its UUID, use the UUID= syntax.
For example,to mount the first filesystem
from the preceding list on /home/extra, enter:

```
mount UUID=48DC-1AA1 /home/extra
```

Still, it’s important to understand UUIDs.

In addition, many distributions use the UUID as a mount point when you insert
removable media. In the preceding example, the FAT filesystem is on a flash
media card.

An Ubuntu system with someone logged in will mount this partition at
`/media/3762-6138` upon insertion.

The udevd daemon handles the initial event for the device insertion.

You can change the UUID of a filesystem if necessary (for example, if you copied
the complete filesystem from somewhere else and now need to distinguish it from
the original).

See the tune2fs(8) manual page for how to do this on an ext2/ext3/ext4 filesystem.

#Disk Buffering, Caching, and Filesystems

Linux, like other versions of Unix, buffers writes to the disk.

This means that the kernel usually doesn’t immediately write changes to
filesystems when processes request changes.

Instead it stores the changes in RAM until the kernel can conveniently make
the actual change to the disk.
This buffering system is transparent to the user and improves performance.

When you unmount a filesystem with umount, the kernel automatically synchronizes
with the disk.

you can force the kernel to write the changes in its buffer to the disk by running the sync command.


Filesystem Mount Options:

`-r` The -r option mounts the filesystem in read-only mode. This has a number of
uses, from write protection to bootstrapping.
You don’t need to specify this option when accessing a read-only device such
as a CD-ROM;

`-t` The -t type option specifies the filesystem type.

Long Options:

To use long options with mount on the command line, start with -o and supply some
 keywords.
for example:

```
mount -t vfat /dev/hda1 /dos -o ro,conv=auto
```

The ro option specifies read-only mode and is the same as the -r short option.
The conv=auto option tells the kernel to automatically convert certain text files
from the DOS newline format to the Unix style

The most useful long options are these:
```
exec, noexec -- Enables or disables execution of programs on the filesystem.
suid, nosuid -- Enables or disables setuid programs.
ro -- Mounts the filesystem in read-only mode
rw -- Mounts the filesystem in read-write mode
```

conv=rule (FAT-based filesystems) Converts the newline characters in files based
on rule, which can be binary, text, or auto. The default is binary, which disables any character translation.

To treat all files as text, use text.
The auto setting converts files based on their extension.

Be careful with this option because it can damage files.
Consider using it in read-only mode.

to reattach a currently mounted filesystem at the same mount point
when you need to change mount options:

```
mount -n -o remount /
```

The most common such situation is when you need to make a read-
only file-system writable during crash recovery.

This command assumes that the correct device listing for / is in /etc/fstab
If it is not, you must specify the device.

#/etc/fstab Filesystem Table

To mount filesystems at boot time, Linux systems keep a permanent list of
filesystems and options in `/etc/fstab`.

This is a plaintext file in a very simple format

Each line corresponds to one filesystem, each of which is broken into six fields. These fields are as follows, in order from left to right:

  1. The device or UUID.
  Most current Linux systems no longer use the device in `/etc/fstab`, preferring
  the UUID.

  2. The mount point. Indicates where to attach the filesystem.

  3. The filesystem type.

  4. Options. Use long options separated by commas.

  5. Backup information for use by the dump command.
  You should always use a 0 in this field.

  6. The filesystem integrity test order.
  To ensure that fsck always runs on the root first, always set this to 1
  for the root filesystem and 2 for any other filesystems on a hard disk.

Use 0 to disable the bootup check for everything else, including CD-ROM drives,
swap, and the `/proc` file-system.

Alternatives to `/etc/fstab`:

`/etc/fstab.d` directory that contains individual filesystem
configuration files (one file for each filesystem).

To view the size and utilization of your currently mounted filesystems, use the
df command:

```
df

Filesystem     1K-blocks      Used Available Use% Mounted on
/dev/sda2      379605408  14818632 345480844   5% /
none                   4         0         4   0% /sys/fs/cgroup
udev             3633028         8   3633020   1% /dev
tmpfs             728768      1208    727560   1% /run
none                5120         0      5120   0% /run/lock
none             3643824     29224   3614600   1% /run/shm
none              102400        52    102348   1% /run/user
/dev/sda1         306584      3428    303156   2% /boot/efi
/dev/sda4      569475040 148205164 392319092  28% /home
```

```
Filesystem -- The filesystem device
1K-blocks -- The total capacity of the filesystem in blocks of 1024 bytes
Used -- The number of occupied blocks
Available -- The number of free blocks
Capacity -- The percentage of blocks in use
Mounted on -- The mount point
```

In both cases, 5 percent of the total capacity is unaccounted for.
In fact, the space is there, but it is hidden in reserved blocks.
Therefore, only the superuser can use the full filesystem space if the rest of the partition fills up.

`du` command -- prints the disk usage of every directory in the directory hierarchy

The `du -s` command turns on summary mode to print only the grand total.

The POSIX standard defines a block size of 512 bytes. However, this size is harder to read, so by default,
the df and du output in most Linux distributions is in 1024-byte blocks.

To explicitly specify 1024-byte blocks, use the -k option
The df program also has a -m option to list capacities in 1MB blocks and a -h
option to take a best guess at what a person can read.

The tool to check a filesystem is `fsck`.

As with the mkfs program, there is a different version of fsck for
each filesystem type that Linux supports.

To run fsck in interactive manual mode, give the device or the mount point
(as listed in `/etc/fstab`) as the argument:

```
fsck /dev/sdb1
```

**WARNING**: You should never use fsck on a mounted filesystem !!! because the kernel
may alter the disk data as you run the check, causing runtime mismatches that can
crash your system and corrupt files.

When fsck asks you about reconnecting an inode, it has found a file that doesn’t
appear to have a name.
When reconnecting such a file, fsck places the file in the lost+found directory in the filesystem, with a number as the filename. If this happens, you need to guess the name based on the content of the file; the original name is probably gone.

Fortunately, `e2fsck(fsck ->)` has a `-p` option that automatically
fixes ordinary problems without asking and aborts when there’s a serious error.
In fact, Linux distributions run some variant of fsck -p at boot time.

If you think that something really bad has happened, try running fsck -n to check
the filesystem without modifying anything.

If you suspect that only the superblock is corrupt (for example, because someone
wrote to the beginning of the disk partition), use fsck -b num to replace the
corrupted superblock with an alternate at block num and hope for the best. :)

you might be able to recover the filesystem with one of the superblock backups
that mkfs creates.

To find a backup superblock, you may be able to run mkfs -n on the device to view
a list of superblock backup numbers without destroying your data.

Again, make sure that you’re using -n, or you’ll really tear up the filesystem!!!

You normally do not need to check ext3 and ext4 filesystems manually because
the journal ensures data integrity.

you may wish to mount a broken ext3 or ext4 filesystem in ext2 mode because the
kernel will not mount an ext3 or ext4 filesystem with a nonempty journal.

To flush the journal in an ext3 or ext4 filesystem to
the regular filesystem database, run e2fsck as follows:

```
e2fsck –fy /dev/disk_device
```

#Special-Purpose Filesystems

Not all filesystems represent storage on physical media.

Specifically, most versions of Unix have filesystems
that serve as system interfaces.

That is, rather than serving only as a means to store data on a device, a
filesystem can represent system information such as process IDs and
kernel diagnostics.

The special filesystem types in common use on Linux include the following:

  1. proc

  Mounted on /proc. The name proc is actually an abbreviation for process.
  Each numbered directory
  inside /proc is actually the process ID of a current process on the system.

  other kernel and hardware information like `/proc/cpuinfo`

  2. sysfs

  Mounted on `/sys`.

  3. tmpfs
  Mounted on `/run` and other locations.
  With tmpfs, you can use your physical memory and swap space as temporary storage.

#swap space

If you run out of real memory, the Linux virtual memory system can automatically
move pieces of memory to and from a disk storage.

The free command’s output includes the current swap usage in kilobytes as follows:
```
free

             total       used       free     shared    buffers     cached
Mem:       7287652    4030412    3257240      75700     675420    1116648
-/+ buffers/cache:    2238344    5049308
Swap:     11974652          0   11974652
```

#Using a File as Swap Space

Use these commands to create an empty file, initialize it as swap, and add it to
the swap pool:
```
dd if=/dev/zero of=swap_file bs=1024k count=num_mb
mkswap swap_file
swapon swap_file
```

```
swap_file -- name of the new swap file
num_mb -- desired size in megabytes
```
To remove a swap partition or file from the kernel’s active pool, use the swapoff
command.

#Inside a Traditional Filesystem

A traditional Unix filesystem has two primary components:
a pool of data blocks where you can store data and
a database system that manages the data pool.

The database is centered around inode structure.

An inode is a set of data that describes a particular file, including its type,
permissions, and—perhaps most importantly—where in the data pool the file data
resides.

Inodes are identified by numbers listed in an inode table.

Filenames and directories are also implemented as inodes.

A directory inode contains a list of filenames and corresponding links to other
inodes.

(See Figure in chapter).

For any ext2/3/4 filesystem, you start at inode number 2 -- the root inode.

This system, of inodes pointing to directory data structures and directory data
structures pointing to inodes,
allows you to create the filesystem hierarchy that you’re used to.

In addition, notice that the directory inodes contain entries for . (the current
directory) and .. (the parent directory, except for the root directory). This
makes it easy to get a point of reference and to navigate back down the directory structure.

#Viewing Inode Details

To view the inode numbers for any directory, use the ls -i command.

For more detailed inode information, use the stat command.

for example:

```
stat Beginning_C/

  File: ‘Beginning_C/’
  Size: 4096      	Blocks: 8          IO Block: 4096   directory
Device: 804h/2052d	Inode: 8915162     Links: 3
Access: (0775/drwxrwxr-x)  Uid: ( 1000/   roman)   Gid: ( 1000/   roman)
Access: 2015-08-03 10:15:05.412075932 +0300
Modify: 2015-08-03 10:14:35.256075645 +0300
Change: 2015-08-03 10:14:35.256075645 +0300
 Birth: -
```

The link count field is the number of total directory entries (across
all directories) that point to an inode.

A hard link is just a manually created entry in a directory to an inode that
already exists.

The `ln` command (without the -s option) allows you to manually create new links.

This is also why removing a file is sometimes called unlinking.

If you run `rm dir_1/file_2`, the kernel searches for an entry named file_2 in
inode 12’s directory entries.

Upon finding that file_2 corresponds to inode 14, the kernel removes the directory entry and then subtracts 1 from inode 14’s link count.

As a result,
inode 14’s link count will be 0, and the kernel will know that there are no longer
any names linking to the inode.
Therefore, it can now delete the inode and any data associated with it.

However, if you run `rm dir_1/file_3`, the end result is that the link count of
inode 15 goes from 2 to 1 (because `dir_2/file_5` still points there), and the
kernel knows not to remove the inode.

Link counts work much the same for directories.
inode 12’s link count is 2, because there are two inode links there:
one for dir_1 in the directory entries for inode 2 and
the second a self-reference (.) in its own directory entries.

If you create a new directory dir_1/dir_3,
the link count for inode 12 would go to 3 because the new directory would include
a parent (..) entry that links back to  inode 12, much as inode 12’s parent link
points to inode 2.

There is one small exception. The root inode 2 has a link count of 4.
The “fourth” link is in the filesystem’s superblock because the superblock tells
you where to find the root inode.


When allocating data pool blocks for a new file, how does the filesystem
know which blocks are in use and which are available?

One of the most basic ways is with an additional management data structure called
 a block bitmap.

In this scheme, the filesystem reserves a series of bytes,
with each bit corresponding to one block in the data pool.

A value of 0 means that the block is free, and a 1 means that it’s in use.

Thus, allocating and deallocating blocks is a matter of flipping bits.

Problems in a filesystem arise when:
the inode table data doesn’t match the block allocation data or
when the link counts are incorrect;
this can happen when you don’t cleanly shut down a system

Therefore, when you check a filesystem,
the fsck program walks through the inode table and directory structure to generate new link counts and a new block allocation map (such as the block bitmap),
and then it compares the newly generated data with the filesystem on the disk.

If there are mismatches, fsck must fix the link counts and determine what to do
with any inodes and/or data that didn’t come up when it traversed
the directory structure.

You’re expected to access the contents of files and directories of a
mounted file-system through kernel system calls.
the `stat()` system call returns inode numbers and link counts.

Not all filesystems available in Linux have these filesystem internals.
The Virtual File System (VFS) interface layer ensures that system calls always
return inode numbers and link counts, but those numbers may not necessarily
mean anything.

Remember, specific filesystem support does not necessarily
need to be in the kernel.
In user-space filesystems, the kernel only needs to act as a conduit for
system calls.


Inodes are associated with precisely one directory entry at a time.
However, with hard links it is possible to associate multiple directory entries
with a single inode.

#Hard link vs. Soft link

Hard links cannot link directories.
symbolic links allows to create links between directories.

These links behave differently when the source of the link is moved or removed:

Symbolic links are not updated.
Hard links always refer to the source, even if moved or removed.
