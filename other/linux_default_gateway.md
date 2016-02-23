#How do i get the default gateway in LINUX?

To select the default route (from possibly many)

```
$ ip -4 route list 0/0   # use -6 instead of -4 for ipv6 selection.
default via 192.168.1.1 dev wlp2s0
```


