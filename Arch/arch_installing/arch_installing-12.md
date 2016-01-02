##Step \#13 Install boot loader

We have to now install grub and configure the boot loader. My PC has BIOS, if you are using a PC with UEFI motherboard then [check the Arch Wiki Page](https://wiki.archlinux.org/index.php/Unified_Extensible_Firmware_Interface).

Install grub:

```
# pacman -S grub
# grub-install --target=i386-pc --recheck /dev/sda
```

If you are going on install other distros on this system, or if you have other distro
pre-installed then you should also install ‘os-prober’ which will keep a check on them
and add them to the boot menu. I suggest that even if you don’t have other distros,
do install this package.

```
pacman -S os-prober
```

Run this command so Arch knows about other distros:

```
# grub-mkconfig -o /boot/grub/grub.cfg
```

We are almost done with the base install of the system and we need to reboot it. But before we do
that let’s ensure that the Ethernet will start with the system reboot.
Let’s first find the name of the network. Run this command:

```
# ip link
```

This command will print an output similar to the below:

```
[swapnil@arch ~]$ ip link  
1: lo:  mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group d
efault  
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eno1:  mtu 1500 qdisc fq_codel state UP mode DEFAULT
 group default qlen 1000
    link/ether f4:6d:04:19:2b:bf brd ff:ff:ff:ff:ff:ff
[swapnil@arch ~]$
```

If you are using wireless, then you will also see an entry for the wireless device.Name of the Ethernet device starts with ‘e’ whereas name of the wireless devices start with ‘w’. So in this case ‘eno1’ is my network device.

Now comes the mighty systemd at play. We have to configure the network to start with system boot.

```
# systemctl enable dhcpcd@NAME-OF-THE-NETWORK.service
```

Exchange ‘NAME-OF-THE-NETWORK’ with the name the ip link command provided.

So in my case it would be:

```
# systemctl enable dhcpcd@eno1.service
```

You can read more about `systemd` and `systemctl` [here](https://wiki.archlinux.org/index.php/Systemd).

It’s not over yet, we have to reboot our system to complete the installation.
While the base system is ready we still have to install the Display Manager (X server),
the desktop environment, graphics drivers, etc,.

First exit from the chroot environment:

```
# exit
```

Then unmout the root

```
# umount -R /mnt
```

And reboot:

```
# reboot
```

Remove the USB drive which you used for installation, so that it doesn’t boot into it again.
