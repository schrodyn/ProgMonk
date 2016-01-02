##Create BIOS boot partition for GPT

BIOS-GPT requires BIOS Boot Partition at the beginning of the disk. Follow this step:

```
New -> Enter
First Sector -> Enter
Size in Sector -> 1007KiB -> Enter
Hex Code of GUID (L to show pres, Enter = 8300) -> ef02 ->Enter
Enter partition name – > Enter
```

You should see a 1007KiB boot partition created.

##Create Root partition (using cgdisk – for GPT)

Now select the free space and repeat the same process for creating the root partition.

```
New -> Enter
First Sector -> Enter
Size in Sector -> 40GB -> Enter (give as much space as you want to give to root + home)
Hex Code of GUID (L to show pres, Enter = 8300) -> Enter
Enter partition name – > Enter
```

Note: I don’t create separate `/home` partition anymore. When you create a user later, the home directory for that user will be created withing root. This allows root and home to share the whole space on the drive offering more flexibility.

##It’s time to create swap partition (GPT/cgdisk)

Repeat the same procedure as above. Choose the free space and follow these steps. If you are curious how much swap does your system need, [read this article](http://www.cyberciti.biz/tips/linux-swap-space.html).

```
Hit New -> Enter
First Sector -> Enter
Size in Sector -> 2GB -> Enter (as much as you need)
Hex Code of GUID (L to show pres, Enter = 8300) -> 8200 -> Enter
Enter partition name - > swap
```

Swap has been created.

**Note**: Don't use a GUID hex code of '8300' for a swap partition. Use '8200' instead.
Your partition type should then be defined as "linux swap" instead of a "linux filesystem".

If you want to create other partitions for your data, just use the same procedure for each partition.

```
Hit New -> Enter
First Sector -> Enter
Size in Sector -> 400GB -> Enter
Hex Code of GUID (L to show pres, Enter = 8300) -> Enter
Enter partition name - > data (lable for the partition)
```

Once you are done with all partitions, select [write]. It will ask you to confirm as it will write this data to the partition table. Type ‘yes’.
