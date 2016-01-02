##Prepare hard drive using cfdisk for MBR

```
cfdisk /dev/sda
```

![](/Arch/arch_installing/images/arch-linux-install-004.png)

If this is a brand new system then you will see all the free space
(in this case I am testing it on a 14 GB SSD).

Let’s first create the root partition:

```
New -> Enter
Partition Size -> 40G
Primary or Extended -> Select Primary
Bootable -> Enter
Write -> Enter -> Yes
```

##It’s time to create swap partition (cfdisk/MBR)

Repeat the same procedure as above. Choose the free space and follow these steps. If you are curious how much swap does your system need, [read this article](http://www.cyberciti.biz/tips/linux-swap-space.html). The only difference is that you won’t have to make the partition bootable.

```
New -> Enter
Partition Size -> 2G
Primary or Extended -> Select Primary (or extended, if you are going to
create more than 3 partitions on this hard drive)
Write -> Enter -> Yes
```

If you want to create more partitions use the same procedure (just don’t select any of these partitions as bootable):

```
New -> Enter
Partition Size -> 400G (allocate the size you want for the partition)
Primary or Extended -> Select Primary
Write -> Enter -> Yes
```

Once you are done with all partitions, select [write]. Before writing the changes to the hard drive,
it would seek your confirmation – type ‘yes’.

Before we move ahead, check the status of the partitions by running the good old, *lsblk*, command.
Note down the name/device node of each partition, as we will need them to mount those partitions in the following steps.
