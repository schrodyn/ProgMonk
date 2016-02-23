#Linux IP forwarding

You can enable or disable routing/forwarding of IP packets between 
interfaces with following command:

```
# cat /proc/sys/net/ipv4/ip_forward
# echo "1" > /proc/sys/net/ipv4/ip_forward
# echo "0" > /proc/sys/net/ipv4/ip_forward
```

