##\#1 Step One – Grab Arch ISO and prepare the installation media

[Download the latest ISO image of Arch Linux](https://www.archlinux.org/download/).
Once you have downloaded the image it’s time
to create a bootable USB drive using the *dd* command. Plug in the USB device and run *lsblk* command
to print a list of connected storage devices. Note down the name of the USB drive.

```
swapnil@kubuntu:~$ lsblk
NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sda      8:0    0 119.2G  0 disk
├─sda1   8:1    0  1007K  0 part
├─sda2   8:2    0    40G  0 part /media/swapnil/2050288e-aafc-4453-
sde      8:64   0 931.5G  0 disk
└─sde1   8:65   0 931.5G  0 part
sr0     11:0    1  1024M  0 rom  
```

In the above output, `sde` is the USB drive. Keep in mind that in the dd command we will be using the name of the device and not the partition, which are shown with numbers such as `sda1`, `sda2` or `sde1`. So we will use `sde` and not `sde1`.

Now run the `dd` command:

```
sudo dd if=/path_to_arch_.iso of=/device-name
```

Example:

```
sudo dd if=/path_to_arch_.iso of=/dev/sde
```

Exchange ‘sde’ with the device node from your output.
