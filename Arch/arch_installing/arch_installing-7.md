##Step \#5 Mount the root partition

Now it’s time to install Arch Linux base system. First we need to mount the ‘root’ partition.

```
# mount /dev/sda1 /mnt
```

##Step \#6 Choose mirror

Before we start downloading packages, let’s choose the mirror closest to your location
so you get the fastest speed while downloading the packages.

Edit the mirror list using your favorite editor

```
# vim /etc/pacman.d/mirrorlist
```

##Step \#7  Install the base packages

It’s time for us to install the base packages. Run this command:

```
# pacstrap -i /mnt base base-devel
```

##Step \#8 Configure fstab

Now we have to generate the *fstab* file to keep an eye on the mounted partitions:

```
# genfstab -U -p /mnt >> /mnt/etc/fstab
```

Run this command only once, even if there are any mistakes. In case you made any mistake, you need to edit the ‘fstab’ file to correct it. Don’t re-run the command. Check the ‘*fstab*‘ entry to ensure everything
is correct:

```
# vim /mnt/etc/fstab
```

If there is an entry for the *root* partition that means everything is fine.

Now Chroot into your newly installed system to configure it.

```
# arch-chroot /mnt
```
