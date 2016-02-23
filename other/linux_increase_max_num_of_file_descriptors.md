#Linux Increase The Maximum Number Of Open Files / File Descriptors (FD)

How do I open more file descriptors under Linux?

The `ulimit` command provides control over the resources available to 
the shell and/or to processes started by it, on systems that allow such
control. The maximum number of open file descriptors displayed with 
that command. 

#Command To List Number Of Open File Descriptors

Use the following command to display maximum number of open file 
descriptors:

```
cat /proc/sys/fs/file-max
```

Output:

```
75000
```

75000 files normal user can have open in single login session. To see 
the hard and soft values, issue the command as follows:

```
# ulimit -Hn
# ulimit -Sn
```

To see the hard and soft values for httpd or oracle user, issue the 
command as follows:

```
# su username
```

In this example, su to oracle user, enter:

```
# su oracle
$ ulimit -Hn
$ ulimit -Sn
```

#System-wide File Descriptors (FD) Limits

The number of concurrently open file descriptors throughout the system 
can be changed via `/etc/sysctl.conf` file under Linux operating 
systems.

**The Number Of Maximum Files Was Reached, How Do I Fix This Problem?**

Many application such as Oracle database or Apache web server needs 
this range quite higher. So you can increase the maximum number of open
files by setting a new value in kernel variable `/proc/sys/fs/file-max`
as follows (login as the root):

```
# sysctl -w fs.file-max=100000
```

Above command forces the limit to 100000 files. You need to edit 
`/etc/sysctl.conf` file and put following line so that after reboot the
setting will remain as it is:

```
# vi /etc/sysctl.conf
```

Append a config directive as follows:

```
fs.file-max = 100000
```

Save and close the file. Users need to log out and log back in again to
changes take effect or just type the following command:

```
# sysctl -p
```

Verify your settings with command:

```
# cat /proc/sys/fs/file-max
```

OR

```
# sysctl fs.file-max
```

#User Level FD Limits

The above procedure sets **system-wide** file descriptors (FD) limits.
However, you can limit httpd (or any other users) user to specific 
limits by editing `/etc/security/limits.conf` file, enter:

```
# vi /etc/security/limits.conf
```

Set `httpd` user soft and hard limits as follows:

```
httpd soft nofile 4096
httpd hard nofile 10240
```

Save and close the file. To see limits, enter:

```
# su httpd
$ ulimit -Hn
$ ulimit -Sn
```










