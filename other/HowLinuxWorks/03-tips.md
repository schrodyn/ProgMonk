kernel presents many of the device I/O interfaces to user processes as files.
These device files are sometimes called device nodes.

Device files are in the `/dev` directory

```
echo blah blah > /dev/null
```

kernel decides what to do with any data written to this device.
In the case of /dev/null, the kernel simply ignores the input and throws away the
data.

```
ls -l
```
If first character of line is b, c, p or s, the file is a device.
These letters stand for block, character, pipe, and socket, respectively.

Block device -- Programs access data from a block device in fixed chunks.
The sda1 is a disk device, a type of block device.

Disks can be easily split up into blocks of data.
Because a block device’s total size is fixed and easy to index,  processes have
random access to any block in the device with the help of the kernel.

Character device -- Character devices work with data streams.
You can only read characters from or write characters to
character devices.

Character devices don’t have a size.
Printers, for example, are represented by character devices.

Pipe device -- are like character devices, with another process at the other end
of the I/O stream instead of a kernel driver.

Socket device -- are special-purpose interfaces that are frequently used for
interprocess communication.

Network interfaces don’t have device files.

Kernel assigns devices in the order in which they are found, so a device may have
a different name between reboots.

To provide a uniform view for attached devices based on their actual hardware
attributes, the Linux kernel offers the sysfs interface through a system of files
and directories. The base path for devices is /sys/devices.

The `/dev` file is there so that user processes can use the device,
whereas the /sys/devices path is used to view information and manage the device.

`/sys/block` should contain all of the block devices available on a system.
Those contains just symbolic links.

To find the sysfs location of a device in /dev. Use the udevadm command to show
the path and other attributes:

```
udevadm info --query=all --name=/dev/sda
```

The program dd is extremely useful when working with block and character devices.
Its main function to read from an input file or stream and write to an output file
or stream, possibly doing some encoding conversion on the way.

dd copies data in blocks of a fixed size.

```
dd if=/dev/zero of=new_file bs=1024 count=1
```

To abbreviate large chunks
of data, you can use b and k to signify 512 and 1024 bytes, respectively.

`/dev/zero` -- a continuous stream of zero bytes

To find the name of a device:

  1. Query `udevd` using `udevadm`
  2. Look for the device in the `/sys` directory.
  3. Run `cat /proc/devices` to see the block and character devices for which your
  system currently has drivers.

Most hard disks attached to current Linux systems correspond to device names with
an sd prefix, such as `/dev/sda`, `/dev/sdb`, and so on

These devices represent entire disks; the kernel makes separate device files,
such as `/dev/sda1` and `/dev/sda2`, for the partitions on a disk.

The sd portion of the name stands for SCSI disk.
Small Computer System Interface (SCSI) was originally developed as a hardware and
protocol standard for communication between devices
such as disks and other peripherals.

traditional SCSI hardware isn’t used in most modern machines,
the SCSI protocol is everywhere due to its adaptability.

For example, USB storage devices use it to communicate.
The story on SATA disks is a little more complicated, but the Linux
kernel still uses SCSI commands at a certain point when talking to them.

To list the SCSI devices on your system:

```
lsscsi

[0:0:0:0]    disk    ATA      ST1000LM024 HN-M 2AR2  /dev/sda
[1:0:0:0]    cd/dvd  PLDS     DVD-RW DU8A5SH   NL61  /dev/sr0
[2:0:0:0]    disk    Generic- xD/SD/M.S.       1.00  /dev/sdb
```

The first column identifies the address of the device on the system
the second describes what kind of device it is
the last indicates where to find the device file

most modern Linux systems use the Universally Unique Identifier
(UUID) for persistent disk device access.

Linux recognizes most optical storage drives as the SCSI devices `/dev/sr0`,
`/dev/sr1`, and so on.

The `/dev/sr*` devices are read only, and they are used only for reading from discs.
For the write and rewrite capabilities of optical
devices, you’ll use the “generic” SCSI devices such as `/dev/sg0`.

Terminals are devices for moving characters between a user process and an I/O
device, usually for text output to a terminal screen.

Pseudoterminal devices are emulated terminals that understand the I/O features of
real terminals. But rather than talk to a real piece of hardware, the kernel
presents the I/O interface to a piece of software, such as the shell terminal
window.

Two common terminal devices are `/dev/tty1` (the first virtual console) and
`/dev/pts/0` (the first pseudoterminal device)

The `/dev/tty` device is the controlling terminal of the current process.

#Display Modes

Linux has two primary display modes: text mode and an X Window System server.

When in text mode, you can switch between consoles with an ALT-Function key
combination—for example, ALT-F1 takes you to /dev/tty1, ALT-F2 goes to /dev/tty2,
and so on.

If you want to see your text console after your system boots, press CTRL-ALT-
F1. To return to the X11 session, press ALT-F2, ALT-F3, and so on, until you get to the X session.

to force the system to change consoles with chvt command
For example, to switch to tty1, run the following as root:

```
chvt 1
```

Linux has two sets of audio devices.
There are separate devices for the Advanced Linux Sound Architecture
(ALSA) system interface and the older Open Sound System (OSS).

The ALSA devices are in the `/dev/snd` directory.

#Creating Device Files

The `mknod` command creates one device.

For example, creating `/dev/sda1` is a matter of using the following command:
```
mknod /dev/sda1 b 8 2
```

The b 8 2 specifies a block device with a major number 8 and a minor number 2.
For character or named pipe devices, use c or p instead of b
(omit the major and minor numbers for named pipes)

If you issue the `ls -l` command, you'll see two numbers (separated by a comma) in
the device file entries before the date of the last modification, where the file
length normally appears.

These numbers are the major and minor device number for the particular device.

Traditionally, the major number identifies the driver associated with the device.
For example, `/dev/null` and `/dev/zero` are both managed by driver 1,
whereas virtual consoles and serial terminals are managed by driver 4;

The minor number is used by the kernel to determine exactly which device is being
referred to.

#udev

The Linux kernel can send notifications to a user-space process (called udevd)
upon detecting a new device on the system (for example, when someone attaches a
USB flash drive).

The user-space process on the other end examines the new device’s characteristics,
creates a device file, and then performs any device initialization.

That was the theory. Unfortunately, in practice, there is a problem with this approach—device files are necessary early in the boot procedure, so udevd must start
early.

#devtmpfs

The devtmpfs filesystem was developed in response to the problem of device
availability during boot.

The kernel creates device files as necessary, but it also notifies udevd that a
new device is available. Upon receiving this signal, udevd does not create
the device files, but it does perform device initialization and
process notification.
Additionally, it creates a number of symbolic links in /dev to further identify
devices.

You can find examples in the directory `/dev/disk/by-id`, where each attached disk has one or more entries:

```
lrwxrwxrwx 1 root root   9 авг.   1 10:12 ata-PLDS_DVD-RW_DU8A5SH_S0C19800ZVJ82000E2EQ -> ../../sr0
lrwxrwxrwx 1 root root   9 авг.   1 10:13 ata-ST1000LM024_HN-M101MBB_S2SMJ9EDA15511 -> ../../sda
lrwxrwxrwx 1 root root  10 авг.   1 10:13 ata-ST1000LM024_HN-M101MBB_S2SMJ9EDA15511-part1 -> ../../sda1
lrwxrwxrwx 1 root root  10 авг.   1 10:13 ata-ST1000LM024_HN-M101MBB_S2SMJ9EDA15511-part2 -> ../../sda2
lrwxrwxrwx 1 root root  10 авг.   1 10:13 ata-ST1000LM024_HN-M101MBB_S2SMJ9EDA15511-part3 -> ../../sda3
```

udevd names the links by interface type, and then by manufacturer and model
information, serial number, and partition (if applicable).

The udevd daemon operates as follows:

  1. The kernel sends udevd a notification event, called a uevent, through an internal network link.
  2. udevd loads all of the attributes in the uevent.
  3. udevd parses its rules, and it takes actions or sets more attributes based on those rules.

The rules files are in the `/lib/udev/rules.d` and `/etc/udev/rules.d` directories.
The rules in `/lib` are the defaults, and the rules in `/etc` are overrides.

The udevadm program is an administration tool for udevd.
The most powerful features of udevadm are the ability to search for and
explore system devices and the ability to monitor uevents as udevd receives them
from the kernel.

to look at all of the udev attributes for a device such as `/dev/sda`:

```
udevadm info --query=all –-name=/dev/sda
```

The output:

```
P: /devices/pci0000:00/0000:00:11.0/ata1/host0/target0:0:0/0:0:0:0/block/sda
N: sda
S: disk/by-id/ata-ST1000LM024_HN-M101MBB_S2SMJ9EDA15511
S: disk/by-id/wwn-0x50004cf20b84bf31
E: DEVLINKS=/dev/disk/by-id/ata-ST1000LM024_HN-M101MBB_S2SMJ9EDA15511 /dev/disk/by-id/wwn-0x50004cf20b84bf31
E: DEVNAME=/dev/sda
E: DEVPATH=/devices/pci0000:00/0000:00:11.0/ata1/host0/target0:0:0/0:0:0:0/block/sda
E: DEVTYPE=disk
E: ID_ATA=1
E: ID_ATA_DOWNLOAD_MICROCODE=1
E: ID_ATA_FEATURE_SET_APM=1

--etc--
```

the P: at the top is the sysfs device path

the N: is the device node (that is, the name given to the /dev file)

S: indicates a symbolic link to the device node that udevd placed in /dev according to its rules
and E: is additional device information extracted in the udevd rules.

To monitor uevents with udevadm, use the monitor command:
```
udevadm monitor
```

Output (for example, when you insert a flash media device) looks like this:

```
KERNEL[32838.533923] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1 (usb)
KERNEL[32838.534658] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1/5-1:1.0 (usb)
KERNEL[32838.534964] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1/5-1:1.0/host5 (scsi)
KERNEL[32838.535065] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1/5-1:1.0/host5/scsi_host/host5 (scsi_host)
UDEV  [32838.563838] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1 (usb)
UDEV  [32838.567627] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1/5-1:1.0 (usb)
UDEV  [32838.569553] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1/5-1:1.0/host5 (scsi)
UDEV  [32838.571410] add      /devices/pci0000:00/0000:00:10.0/usb5/5-1/5-1:1.0/host5/scsi_host/host5 (scsi_host)
```

There are two copies of each message in this output because the default behavior
is to print both the incoming
message from the kernel (marked with KERNEL) and the message that udevd sends out
to other programs when it’s finished processing and filtering the event.

To see only kernel events, add the --kernel option,
and to see only outgoing events, use --udev.

There’s an lsusb command that is similar to lsscsi.

When a user-space process communicates with the SCSI subsystem, it normally does
so through the block device layer and/or another other kernel service that sits on
top of an SCSI device class driver (like sd or sr).

In other words, most user processes never need to know anything about SCSI devices or their commands.

However, user processes can bypass device class drivers
and give SCSI protocol commands directly to devices through their generic devices.

to show the generic devices:
```
lsscsi -g

[0:0:0:0]    disk    ATA      ST1000LM024 HN-M 2AR2  /dev/sda   /dev/sg0
[1:0:0:0]    cd/dvd  PLDS     DVD-RW DU8A5SH   NL61  /dev/sr0   /dev/sg1
[2:0:0:0]    disk    Generic- xD/SD/M.S.       1.00  /dev/sdb   /dev/sg2
```

In addition to the usual block device file, each entry lists an SCSI generic
device file in the last column.

For example, the generic device for the optical drive at `/dev/sr0` is `/dev/sg1`.

Therefore, to write to an optical disc in Linux, you run a program that talks to a
generic SCSI device, such as /dev/sg1. Due to the relative simplicity of reading
compared to writing, however, you still read from the device using the specialized sr optical device driver in the kernel.
