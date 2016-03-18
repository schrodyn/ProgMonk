#The Hello World Module

In the previous part we saw simple example of "hello world" module.
There is my version with which I will start.

```c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Shalymov");

static int dedupfs_init(void)
{
	printk(KERN_ALERT, "Hello man!\n");
	return 0;
}


static void dedupfs_exit(void)
{
	printk(KERN_ALERT, "Goodbye, man!\n");
}

module_init(dedupfs_init);
module_exit(dedupfs_exit);
```

This module defines two functions, one to be invoked when the module is
loaded into the kernel (`dedupfs_init`) and one for when the module is 
removed (`dedupfs_exit`). The `module_init` and `module_exit` lines use
special kernel macros to indicate the role of these two functions. 
Another special macro (`MODULE_LICENSE`) is used to tell the kernel 
that this module bears a free license; without such a declaration, the 
kernel complains when the module is loaded.

The `printk` function is defined in the Linux kernel and made available
to modules; it behaves similarly to the standard C library function 
`printf`. The kernel needs its own printing function because it runs by
itself, without the help of the C library. The module can call `printk`
because, after `insmod` has loaded it, the module is linked to the 
kernel and can access the kernel’s public symbols (functions and 
variables). The string `KERN_ALERT` is the priority of the message.

##Compiling Module

As the first step, we need to look a bit at how modules must be built. 
The build process for modules differs significantly from that used for 
user-space applications; the kernel is a large, standalone program with
detailed and explicit requirements on how its pieces are put together. 

Once you have everything set up, creating a makefile for your module is
straightforward. 

```
obj-m := dedupfs.o
dedupfs-objs := super.o

PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

who are familiar with `make`, but not with the kernel build system, are
likely to be wondering how this makefile works. The above line is not 
how a traditional makefile looks, after all. 

The assignment

```
obj-m := dedupfs.o
```

states that there is one module to be built from the object file 
`dedupfs.o`. The resulting module is named `dedupfs.ko` after being 
built from the object file.

If, instead, you have a module called `module.ko` that is generated 
from two source files (called, say, `file1.c` and `file2.c`), the 
correct incantation would be:

```
obj-m := module.o
module-objs := file1.o file2.o
```

For a makefile like those shown above to work, it must be invoked 
within the context of the larger kernel build system. If your kernel 
source tree is located in, say, your `~/kernel-2.6` directory, 
the `make` command required to build your module (typed in the 
directory containing the module source and makefile) would be:

```
make -C ~/kernel-2.6 M=`pwd` modules
```

This command starts by changing its directory to the one provided withthe `-C` option (that is, your kernel source directory). There it 
finds the kernel’s top-level makefile. The `M=` option causes that 
makefile to move back into your module source directory before trying 
to build the `modules` target. This target, in turn, refers to the 
list of modules found in the `obj-m` variable, which we’ve set to 
`module.o` in our examples.

To build module you should use 

```
$ make
```

After that you should go ahead and load module

```
$ sudo insmod ./dedupfs.ko
```

To ensure that module has been loaded 

```
$ lsmod | grep dedupfs
```

To watch system log we should use `dmesg` command and there we see
message from our module.

To unload module

```
$ sudo rmmod dedupfs
```

So we have in system log

```
$ sudo insmod ./dedupfs.ko
sudo rmmod dedupfs

...
[   14.805762] 00:00:00.000463 main     Process ID: 1046
[   14.805762] 00:00:00.000465 main     Package type: LINUX_32BITS_GENERIC
[   14.806908] 00:00:00.001613 main     5.0.14 r105127 started. Verbose level = 0
[   57.850133] audit_printk_skb: 126 callbacks suppressed
[   57.850139] type=1400 audit(1458333427.284:69): apparmor="DENIED" operation="open" profile="/usr/lib/telepathy/mission-control-5" name="/usr/share/gvfs/remote-volume-monitors/" pid=2108 comm="mission-control" requested_mask="r" denied_mask="r" fsuid=1000 ouid=0
[  369.896169] type=1400 audit(1458333739.332:70): apparmor="DENIED" operation="capable" profile="/usr/sbin/cupsd" pid=790 comm="cupsd" capability=36  capname="block_suspend"
[ 1123.547057] Hello man!
[ 1134.883668] Goodbye, man!
```

