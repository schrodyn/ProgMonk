#detect all memory info in Mb
free -m
#create swapfile
dd if=/dev/zero of=/root/swapfile bs=1M count=512
chmod 600 swapfile
#make this file as swap
mkswap swapfile
#"mount" our swap file
swapon swapfile
#deactivate swap
#swapoff swapfile
