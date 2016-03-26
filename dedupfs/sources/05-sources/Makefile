obj-m := dedupfs.o
dedupfs-objs := super.o

PWD := $(shell pwd)

all: ko mkfs-dedupfs

ko:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

mkfs-dedupfs_SOURCES:
	mkfs-dedupfs.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
