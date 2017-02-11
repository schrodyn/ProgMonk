
ISO guest image:

```
/usr/lib/virtualbox/additions/VBoxGuestAdditions.iso
```

mounting: 

```
mount -t iso9660 /dev/cdrom /media/cdrom
```

```
cd /media/cdrom
./VBoxLinuxAdditions-x86.run
```

reboot

mount shared directory

```
mkdir shared
mount -t vboxsf shared_folder shared
```

where **shared_folder** -- path on host os

## Can't do chown/chgrp/chmod on shared dir

(Optionally) change umask:

```
umask 077
```

The formula to calculate your file permission:

```
default_mode & ~umask
```

Also specified by POSIX, the default mode for file is **S_IROTH | 
S_IWOTH | S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR** or **666**, the 
default mode for directory is **S_IRWXU | S_IRWXG | S_IRWXO** or 
**777** if it was not specified by application.






