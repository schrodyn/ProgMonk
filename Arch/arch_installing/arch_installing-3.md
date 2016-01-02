##Step \#2 Connect to the Internet

Plug in the newly created USB drive and start the system. Choose Arch Linux from the boot menu as its shown in the image below.

![](/Arch/arch_installing/images/arch-linux-install-001.jpg)

You will be greeted by the login prompt. The entire installation will be done over the Internet,
so I suggest use the Ethernet cable to connect to the Internet. Using WiFi means performing
additional steps to get the wireless to work. If you do need to use the wireless
(can’t connect using the Ethernet cable), then do ensure that your wifi chip is supported by Linux.
 Run the following command and see if Arch can detect your wireless network. If it does, then you are in luck:

```
# wifi-menu
```

It will open this box where you can choose the network you want to connect to.

![](/Arch/arch_installing/images/arch-linux-install-002.png)

Hit OK and the next window will show you the name of the selected network. Hit OK again and provide
it with the password for the network. It will offer the option to save the current profile,
say ‘yes’ and you should be connected.

Now let’s see if the system is connected to the Internet:

```
ping -c 3 www.google.com
```

If you can see the pings, it’s time to move ahead.

You can read more about network management [here](https://www.garron.me/en/bits/ip-addr-add-address-linux.html) and [here](https://wiki.archlinux.org/index.php/Network_Debugging).

##Step \#3 Prepare the hard drives

This step may seem complicated, since it involves formatting your hard drive, but I have tried my best to make it as simple as possible. I looked at it from a new user’s point of view to simplify things without dumbing them down.

If you don’t want to ‘create’ partitions then you can also use live USB of other distros like Ubuntu to format the drive using a GUI tool. If you are dual booting with some other OS, then I heavily recommend that use the other, installed, OS to create partition for Arch root.

If your HDD/SSD is already partitioned skip these steps. In this chapter we are assuming this is a new hard drive with no other OSes or partitioned on it.

If you are comfortable with partitioning the drive, and in this case we are assuming this is a new hard drive with no other OSes or partitions on it.

###GPT or MBR?

There are two kind of partitioning tables – GPT and MBR. You should choose the appropriate table for your system. GPT is modern and it has more features, but if you are going to dual boot with Windows or you are planning to use legacy grub, then MBR is the only option. Check out [the Arch Wiki page](https://wiki.archlinux.org/index.php/partitioning#Partition_table) to understand the difference between the two, which will help you in making the right decision about GPT vs MBR.

In this tutorial I will talk about both MBR and GPT. We use *cgdisk* tool to create GPT and *cfdisk* for creating MBR partitioning table.

First let’s find the device node of the SDD/HDD we are going to install Arch on. Run *lsblk* command which will print the name of connected storage devices. In this case I connected my HDD.

```
sda      8:0    0 119.2G  0 disk
├─sda1   8:1    0  119.2G  0 part
sde      8:64   0 931.5G  0 disk
└─sde1   8:65   0 931.5G  0 part
```

The HDD is printed out as sda, lets create some partitions on this device.

##Prepare the hard drive using cgdisk for GPT

Run this command:

```
cgdisk /dev/sda
```

If this is a brand new system then you will see all the free space

![](/Arch/arch_installing/images/arch-linux-install-003.jpg)

GPT needs a ‘boot partition’ of 1007KiB size.
