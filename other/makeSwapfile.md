##detect all memory info in Mb
```sh
free -m
```

##create swapfile
```sh
dd if=/dev/zero of=/root/swapfile bs=1M count=512
chmod 600 swapfile
```

##make this file as swap
```sh
mkswap swapfile
```

##"mount" our swap file
```sh
swapon swapfile
```

##deactivate swap
```sh
swapoff swapfile
```
