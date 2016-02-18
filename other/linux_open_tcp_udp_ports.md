#HowTo: UNIX / Linux Open TCP / UDP Ports

A **port** is an application-specific or process-specific software 
construct serving as a communications endpoint and it is identified by 
its number such as TCP port number 80. A **port number** is a 16-bit 
unsigned integer, thus ranging from 0 to 65535.

The well-known ports are defined by the Internet Assigned Numbers 
Authority (IANA). Type the following command to see list well-known of 
TCP and UDP port numbers:

```
$ less /etc/services
```

##Privileged Ports

Typically port number less than 1024 are used by well know network 
servers such as Apache. Under UNIX and Linux like oses root (super 
user) privileges are required to open privileged ports. Almost all 
clients uses a high port numbers for short term use. This is also known
as an ephemeral port. For example Apache use TCP port 80

The port numbers are divided into three ranges:

  1. Well Known Ports: those from 0 through 1023.
  2. Registered Ports: those from 1024 through 49151
  3. Dynamic and/or Private Ports: those from 49152 through 65535

You can increase **local port range** by typing the following command 
(Linux specific example):

```
# echo 1024 65535 > /proc/sys/net/ipv4/ip_local_port_range
```

You can also increase or decrease **socket timeout** (Linux specific 
example):

```
# echo 2000 > /proc/sys/net/ipv4/tcp_keepalive_time
```

##How Do I See Open Ports and Socket Information Under UNIX or Linux?

You can use the netstat command:

```
# netstat -tulpn
```

To list open IPv4 connections use the lsof command:

```
# lsof -Pnl +M -i4
```

Each TCP or UDP port is opened using a UNIX service or daemon such as 
Apache web server. You can also write a program using C, C++, Perl, 
Shell or Bash to open any port. You can also use utilities such as 
`nc` command.

##nc Command Example

The nc (or netcat utility) is used for just about anything under the 
sun involving TCP or UDP. It can open TCP connections, send UDP 
packets, listen on arbitrary TCP and UDP ports, do port scanning, and 
deal with both IPv4 and IPv6. In this example, open port 5000 using 
nc command:

```
$ nc -l 5000
```

On a second console or from a second UNIX / Linux machine, connect to 
the machine and port being listened on:

```
$ nc localhost 5000
```

OR

```
$ nc your_ip_here 5000
```

In this example, send data from one computer to another:

```
$ nc -l 5555 > output.txt
```

Using a second machine, connect to the listening nc process 
(@ port 5555), feeding it the file which is to be transferred:

```
$ nc your_ip_here 5555 < input.txt
```

You can run `netstat` command to view open ports:

```
$ netstat -a
$ netstat -nat | grep LISTEN
```



