#What is Assembly Language?

##Exercise 1.1

Hello students! Welcome to exercise 1.1. The topic was what is assembly
language. Let's go ahead and create lab setup. 

**My notes** I recomend to download VDI image of Ubuntu 32-bit edition
from [osboxes.org](http://www.osboxes.org/).

As you installed Ubuntu in Virtualbox. Check if you have network 
connection. Just for ensure `ifconfig; ping google.com`.

Now there is a couple of things you should do:

```
sudo apt-get install nasm build-essential
```

Now we need to upload our source code and other exercises for all 
upcoming videos.

Change **NAT** to **Bridged Adapter** in VirtualBox settings.

**Arch Linux note**

The following modules are optional but are recommended if you do not 
want to be bothered in some advanced configurations (precised here 
after): `vboxnetadp`, `vboxnetflt`.

`vboxnetadp` and `vboxnetflt` are both needed when you intend to use 
the **bridged** or **host-only** networking feature. More precisely, 
`vboxnetadp` is needed to create the host interface in the VirtualBox 
global preferences, and `vboxnetflt` is needed to launch a virtual 
machine using that network interface.

Finally, if you use the aforementioned "Host-only" or "bridge 
networking" feature, make sure `net-tools` is installed. VirtualBox 
actually uses `ifconfig` and `route` to assign the IP and route to the host interface configured with `VBoxManage hostonlyif` or via the GUI 
in Settings > Network > Host-only Networks > Edit host-only network 
(space) > Adapter.

Now we need to install `sshd` daemon in our VM box

```
sudo apt-cache search sshd
```

here we see `openssh-server` and install it

```
sudo apt-get install openssh-server
```

After that connect from our host to guest VM:

```
ssh osboxes@192.168.1.148
```

and use `scp` to copy `SLAE-Code` directory in VM or use 
`sftp` to make it 

```
$ sftp osboxes@192.168.1.148
osboxes@192.168.1.148's password: 
Connected to 192.168.1.148.
sftp> 
```

**My note**: I prefer `scp`.

So this is all your exercise files which you need to go ahead then.

