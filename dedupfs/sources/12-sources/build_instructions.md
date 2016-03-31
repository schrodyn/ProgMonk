#Build Instructions

```
dd bs=1M count=100 if=/dev/zero of=image
mkdir mount
```

##Creating Filesystem

```
./mkfs-dedupfs ~/image
```


##Mounting

```
# make
# insmod dedupfs.ko
# mount -o loop -t dedupfs image mount/
$ dmesg
```

##Unmounting

```
# umount mount/
$ dmesg
# rmmod dedupfs.ko 
$ dmesg
```



