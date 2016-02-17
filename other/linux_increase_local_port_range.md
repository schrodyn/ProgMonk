#Linux Increase Local Port Range

You can use the `sysctl` command to to modify kernel parameters at 
runtime. The parameters available are those listed under `/proc/sys/`. 
Please note that this hack is only useful for high bandwidth, busy 
Linux servers or large scale grid servers.

##How to find current port range type

Type the following command

```
$ sysctl net.ipv4.ip_local_port_range
```

Sample outputs:

```
net.ipv4.ip_local_port_range = 32768    61000
```

##Set new local port range

You can set the range with any one of the following command. You must 
be root user:

```
# echo 1024 65535 > /proc/sys/net/ipv4/ip_local_port_range
```

OR

```
$ sudo sysctl -w net.ipv4.ip_local_port_range="1024 64000"
```

Finally, edit `/etc/sysctl.conf` file, to make changes to `/proc` 
filesystem permanently i.e. append the following line to your 
`/etc/sysctl.conf` file:

```
# increase system IP port limits
net.ipv4.ip_local_port_range = 1024 65535
```


