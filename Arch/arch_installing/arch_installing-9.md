##Step \#10 Let’s get the time right

It’s time to configure the time zone of your system. If you don’t know the exact name
of your time zone, run the following command to find it. In my case America is the zone,
and New York is the sub-zone.

```
# ls /usr/share/zoneinfo/
```

Once you have the time zine, create a symlink:

```
# ln -s /usr/share/zoneinfo/your-time-zone > /etc/localtime
```

In my case it was:

```
# ln -s /usr/share/zoneinfo/America/New_York > /etc/localtime
```

Let’s now configure the hardware clock. It is recommended to use UTC instead of localtime.

```
# hwclock --systohc --utc
```

##Get a hostname

You may very much want to have a custom name for your system. Run the following command:

```
# echo swapnil > /etc/hostname
```

Exchange ‘swapnil’ with the desired name for the system.
