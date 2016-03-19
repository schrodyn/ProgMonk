#Build Instructions

##Mounting

```
$ make
# insmod dedupfs.ko
# mount -o loop -t dedupfs image mount_directory
$ dmesg
```

##Unmounting

```
# umount mount_directory
$ dmesg
# rmmod dedupfs.ko 
$ dmesg
```


