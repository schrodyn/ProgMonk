##Step \#4 Create file system

The *lsblk* command gave us the following output. Here ‘sda’ is the SSD/HDD and partitions
on the drive are denoted by number such as sda1 and sda2. The output may wary, depending on whether
you used cgdisk or cfdisk for partition. Cgdisk will create an addition ‘boot partition’
with 1007KiB space. Below is the example of MBR partition table:

```
sda      8:0    0 119.2G  0 disk
├─sda1   8:1    0  109G  0 part
├─sda2   8:2    0    8G  0 part
sde      8:64   0 931.5G  0 disk
└─sde1   8:65   0 931.5G  0 part
```

As you can see from the size of both partitions, sda1 is going to become root and sda2 will be used
for swap. Be extra careful with the output on your system.

Let’s format the root partition as ext4 file system (if you created GPT, don’t format the ‘boot partition’):

```
# mkfs.ext4 /dev/sda1
```

The mkfs (make file system) command formatted the partition as a ext4 file system.

Now let’s format SWAP:

```
# mkswap /dev/sda2
# swapon /dev/sda2
```

We will now mount these partitions.
