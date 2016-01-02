A simplified view of the boot process looks like this:

  1. The machine’s BIOS or boot firmware loads and runs a boot loader.
  2. The boot loader finds the kernel image on disk, loads it into memory,
  and starts it.

  3. The kernel initializes the devices and its drivers.
  4. The kernel mounts the root filesystem.
  5. The kernel starts a program called init with a process ID of 1.
  This point is the user space start.

  6. init sets the rest of the system processes in motion.
  7. At some point, init starts a process allowing you to log in, usually at the end or near the end of the boot.

There are two ways to view the kernel’s boot and runtime diagnostic messages.
You can:

  1. Look at the kernel system log file. You’ll often find this in
  `/var/log/kern.log`; it may be in /var/log/messages

  2. Use the dmesg command, but be sure to pipe the output to less.

Upon startup, the Linux kernel initializes in this general order:

  1. CPU inspection
  2. Memory inspection
  3. Device bus discovery
  4. Device discovery
  5. Auxiliary kernel subsystem setup (networking, and so on)
  6. Root filesystem mount
  7. User space start

When running the Linux kernel, the boot loader passes in a set of text-based
kernel parameters that tell the kernel how it should start.

You can view the kernel parameters from your system’s boot by looking at the
`/proc/cmdline` file:

```
cat /proc/cmdline

BOOT_IMAGE=/boot/vmlinuz-3.13.0-49-generic.efi.signed root=UUID=2a8cf10f-ba21-40d0-8fec-b18f835d519a ro quiet splash vt.handoff=7
```

```
splash flag for displaying a splash screen
root parameter -- location of the root filesystem; without it, the
kernel cannot find init and therefore cannot perform the user space start.
```

The ro parameter -- instructs the kernel to mount the root filesystem in read-only mode upon user space start.

(Read-only mode ensures that fsck can check the root filesystem safely; after the
check, the bootup process remounts the root filesystem in read-write mode.)

Upon encountering a parameter that it does not understand, the Linux kernel saves the parameter. The kernel later passes the parameter to init when performing the
user space start.

For example, if you add `-s` to the kernel parameters, the kernel passes the `-s` to
the init program to indicate that it should start in single-user mode.


The task of a boot loader sounds simple:
It loads the kernel into memory, and then starts the kernel with a set of kernel
parameters.

On PCs, boot loaders use the Basic Input/Output System (BIOS) or Unified
Extensible Firmware Interface (UEFI) to access disks.

Nearly all disk hardware has firmware that allows the
BIOS to access attached storage hardware with Linear Block Addressing (LBA).

Boot loaders are often the only programs to use the BIOS for disk access;
the kernel uses its own high-performance drivers.

#GRUB Introduction

GRUB stands for Grand Unified Boot Loader.

One of GRUB’s most important capabilities is filesystem navigation that allows for
much easier kernel image and configuration selection.

To access the GRUB menu, press and hold SHIFT when your BIOS or firmware startup
screen first appears.

Try the following to explore the boot loader:

  1. Reboot or power on your Linux system.
  2. Hold down SHIFT during the BIOS/Firmware self-test and/or splash screen to get
  the GRUB menu.
  3. Press e to view the boot loader configuration commands for the default boot
  option.

GRUB has its own “kernel” and its own insmod command to dynamically
load GRUB modules, completely independent of the Linux kernel.

Many GRUB commands are similar to Unix shell commands; there’s even an ls command to list files.

The GRUB “root” is the filesystem where GRUB searches for kernel and RAM
filesystem image files.

the GRUB root is first set to a GRUB-specific device (hd0,msdos1).
Then in the following command, GRUB searches for a particular UUID on a partition:

```
search --no-floppy --fs-uuid --set=root bd60fc55-f774-4df8-a3d2-5800cfa227da
```

the linux command’s first argument (`/boot/vmlinuz-...`) is the location of the
Linux kernel image file. GRUB loads this file from the GRUB root.
The initrd command is similar,
specifying the file for the initial RAM filesystem.

You can edit this configuration inside GRUB; doing so is usually the easiest way
to temporarily fix an erroneous boot.


let’s go one step deeper and examine some GRUB internals with the command-
line interface.

As you can see, GRUB has its own device-addressing scheme:
For example, the first hard disk found is hd0, followed by hd1, and so on.

Fortunately, GRUB can search all partitions for a UUID in order to find the one
where the kernel resides, as you just saw with the search command.

To access the GRUB command line by pressing C at the boot menu or config editor:

```
grub>
```

to get started, try a diagnostic command instead: ls. With no arguments,
the output is a list of devices known to GRUB:

```
grub> ls

(hd0) (hd0,msdos1) (hd0,msdos5)
```

In this case, there is one main disk device denoted by (hd0) and the partitions
(hd0,msdos1) and (hd0,msdos5).

The msdos prefix on the partitions tells you that
the disk contains an MBR partition table; it would begin with gpt for GPT.

To get more detailed information, use `ls -l`

This command can be particularly useful because it displays
any UUIDs of the partitions on the disk.

Now determine the GRUB root with the echo command
(recall that this is where GRUB expects to find the kernel):

```
grub> echo $root

hd0,msdos1
```

To use GRUB’s ls command to list the files and directories in that root:

```
grub> ls (hd0,msdos1)/
```

or even better:

```
grub> ls ($root)/
```

The output is a short list of file and directory names
on that partition’s filesystem, such as etc/, bin/, and dev/.

You can take a deeper look into the files and directories on a partition in a
similar manner, for example:

```
grub> ls ($root)/boot
```

You can also view all currently set GRUB variables with the set command:

```
grub> set
```

One of the most important of these variables is $prefix,
the filesystem and directory where GRUB expects to find its configuration and
auxiliary support.

Once you’ve finished with the GRUB command-line interface, enter the boot command to boot your current configuration:

```
grub> boot
```

or just press ESC to return to the GRUB menu

#GRUB Configuration

The GRUB configuration directory contains
the central configuration file (grub.cfg) and numerous loadable modules
with a .mod suffix.
The directory is usually `/boot/grub` or `/boot/grub2`.

We won’t modify `grub.cfg` directly; instead, we’ll use the `grub-mkconfig` command

First, take a quick look at grub.cfg to see how GRUB initializes its menu and
kernel options. You’ll see that the grub.cfg file consists of GRUB commands

If you want to make changes to your GRUB configuration, you won’t edit your
grub.cfg file directly because it’s automatically generated and the system
occasionally overwrites it.

You’ll add your new configuration elsewhere, then run grub-mkconfig to generate
the new configuration.

To see how the configuration generation works, look at the very beginning of
grub.cfg. There should be comment lines such as this:

```
### BEGIN /etc/grub.d/00_header ###
```

So every file in `/etc/grub.d` is a shell script that produces a piece of the
`grub.cfg` file.

The `grub-mkconfig` command itself is a shell script that runs everything in
`/etc/grub.d`.

What if you want to add menu entries and other commands to the GRUB configuration? The short answer is that you should put your customizations into a new
`custom.cfg` file in your GRUB configuration directory,
such as `/boot/grub/custom.cfg`.

long answer: The `/etc/grub.d` configuration directory gives you two options:
`40_custom` and `41_custom`.

`40_custom`, is a script that you can edit yourself, but it’s probably the
least stable; a package upgrade is likely to destroy any changes you make.

`41_custom` script is simpler;
it’s just a series of commands that load `custom.cfg` when GRUB starts.

To write and install a newly generated GRUB configuration file, you can write
the configuration to your GRUB directory with the -o option to grub-mkconfig,
like this:

```
grub-mkconfig -o /boot/grub/grub.cfg
```

#UEFI Secure Boot

When active, this mechanism in UEFI requires boot loaders to be digitally signed
by a trusted authority in order to run.

The result is that if you try to
install an unsigned boot loader (which is most current Linux distributions),
it will not load.

UEFI makes it relatively easy to support loading other operating systems because
you can install multiple boot loaders in the EFI partition.

However, the older MBR style doesn’t support it, and even if you do have UEFI,
you may still have an individual partition with an MBR-style boot loader that you want to use.

You can get GRUB to load and run a different boot loader on a specific partition
on your disk by chainloading.

To understand how boot loaders like GRUB work, let’s first survey how a PC boots
when you turn it on.
there are several variations, but there are two main schemes: MBR and UEFI.

#MBR Boot

The Master Boot Record (MBR) includes a small area (441 bytes) that the PC BIOS
loads and executes after its Power-On Self-Test (POST).

Unfortunately, this is too little storage to house almost any boot loader,
so additional space is necessary, resulting in a multi-stage boot loader.

In this case the initial piece of code
in the MBR does nothing other than load the rest of the boot loader code.

The remaining pieces of the boot loader are usually stuffed into the space
between the MBR and the first partition on the disk.

Of course, this isn’t terribly secure because anything can overwrite the code
there, but most boot loaders do it, including most GRUB installations.

#UEFI Boot

PC manufacturers and software companies realized that the traditional PC BIOS is
severely limited, so they decided to develop a replacement called
Extensible Firmware Interface (EFI)

The current standard is Unified EFI (UEFI), which includes features
such as a built-in shell and the ability to read partition tables and
navigate filesystems.

The GPT partitioning scheme is part of the UEFI standard.

Booting is radically different on UEFI systems and much easier to understand.
Rather than executable boot code residing outside of a filesystem, there is always a special filesystem called the EFI System Partition (ESP),
which contains a directory named efi.

Each boot loader has its own identifier and a
corresponding subdirectory, such as efi/microsoft, efi/apple, or efi/grub.

A boot loader file has an .efi extension
and resides in one of these subdirectories, along with other supporting files.

when using
GRUB, you have to install the UEFI version of GRUB rather than the BIOS version.
