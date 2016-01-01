turn on adapter

```
ifconfig wlan0 up
```
replace "myrouter" with the SSID of your router, "mypassphrase" with your WPA passphrase

```
wpa_passphrase myrouter > /etc/wpa.conf
mypassphrase
```

or

```
wpa_passphrase myrouter mypassphrase > /etc/wpa.conf
```

You should end up with a file looking like this:

```
network={
    ssid="myrouter"
    #psk="mypassphrase"
    psk=8ada1f8dbea59704ac379538b4d9191f6a72390581b4cd7a72864cea685b1a7f
}
```

##getting connected

set up access point SSID

```
iwconfig wlan0 essid myrouter
```

disable network manager for avoid conflicts

```
sudo service stop network-manager
```

set up connection

```
wpa_supplicant -D[driver] -i[device] -c[/path/to/config]
```

in my case:
```
wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa.conf
```

release interface

```
dhclient -r wlan0
```

getting ip from dhcp server

```
dhclient wlan0
```

That's all :)
