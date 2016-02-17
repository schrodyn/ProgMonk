#Finding the IP address of an ssh connection made in the past

If you're lucky, the IP address of the visited hosts can be obtained 
from `~/.ssh/known_hosts`

Also 

```
last -n 20 -a user
```

`-n` for the 20 most recent logins

`-a` to have IP at end of line (useful if ipv6)

*user* to filter the username you want ...
