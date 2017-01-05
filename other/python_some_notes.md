# Python Some Notes

Created in 1989 by Guido Van Rossum

**Variable** is just a Reference to **Object**

**Variable** doesn't have any type associated with it,
**Object** has type associated with it.

Types are associated only with objects.

get the address (memory location) of the object:

```py
>>> name = "flynston"
>>> id(name)
140036763573824
>>> hex(id(name))
'0x7f5cd98c5e40'
```

another way:

```py
>>> name.__repr__
<method-wrapper '__repr__' of str object at 0x7f5cd98c5e40>
```

raw string (don't interpret escape sequences):

```py
>>> name = r"hello\nworld"
>>> print name
hello\nworld
```

unicode string:

```py
name = u'Elena'
```

unicode to regular string conversion:

```py
str(name)
```

regular to unicode string conversion:

```py
unicode(name)
```

unicode is also known as "wide characters"

strings are immutable objects in Python

##String Formatting

"Pwn this IP: %s" % ip

"Pwn %s with IP %s" % (domain, ip)

"Pwn %(domain)s with IP %(ip)s" % {"domain":domain, "ip":ip}

## List operations

Append -- list.append()

Extend -- list.extend([])

Reverse -- list.reverse()

Pop -- list.pop()

Insert -- list.insert(index, item)

Delete -- del list[index]

## Tuples

Tuples are similar to lists but **immutable**

Can convert from list to tuple and vice versa

  * tuple(list)
  * list(tuple)

## Sets

Unordered collection of unique objects

List to set: b = set(a)

Set to list: a = list(b)

Set Operations:

  * Union `a|b`
  * Intersection `a&b`
  * Difference `a-b`

## Dictionaries

Unordered key-value pairs

  * keys are unique and immutable objects
  * value can change
  * `dict = {}`, `dict['name'] = 'r00t'`
  * `dict(name='r00t', age='77')`
  * `dict = {'name':'r00t', 'age':77}`

Check if a given key is present:

  * `dict.has_key(key)`
  * `key in dict` 

Dictionary operations:

  * Get list of items (tuples): `dict.items()`
  * Get list of keys: `dict.keys()`
  * Get list of values: `dict.values()`
  * Get a particular item: `dict.get(key)`

Item deletion:

  * All items: `dict.clear()`
  * One item: `del dict[key]`

Getting help on methods:

  * `dir()` -- lists all attributes
  * `help(string.replace)` -- list method help


prompt user for input:

```
name = raw_input("What is your name? ")
```

to turn simple directory into python package we should 
create **__init__.py** file in this directory

Exceptions:

```py
try :
	a = 10/20
except:
	print "Exception"
else:
	print "No Exception"
finally:
	print "Cleanup code"
```

specify exception:

```py
try:
	a = 0/0
except ZeroDivisionError:
	print "Divide By Zero"
except:
	print "Unknown error"
```

with exception details:

```py
try:
	a = 0/0
except Exception as im:
	print im
```

sqlite3 cheatsheet:

  * **.tables** -- view all tables available
  * **.schema tableName** -- view table schema
  * **import sqlite3** -- sqlite client

File I/O in Python:

  * open(file_name, access_mode, buffering)
  * read(byte_count)
  * write(data)
  * close()
  * os.rename()
  * os.delete()

Directory Navigation:

  * `os.getcwd()` -- absolute path for current working directory
  * `os.mkdir("NewDirectory")` -- create new directory
  * `os.listdir(".")` -- list specified directory

```py
for item in os.listdir("."):
	if os.path.isfile(item):
		print item + " is a file"
	elif os.path.isdir(item):
		print item + " is a directory"
	else:
		print "unknown!"
```

more advanced file searching:

```py
import glob

for item in glob.glob(os.path.join(".", "*.py")) :
	print item
```

todo: go through **glob** documentation

Forking:

```py
#!/usr/bin/env python2

import os
import sys

def child_process():
    print "I am child process and my PID is %d" % os.getpid()

def parent_process():
    print "I am the parent process with PID: %d" % os.getpid()

    childid = os.fork()

    if childid == 0:
        # we are inside the child
        child_process()
    elif childid > 0:
        # we are inside the parent process
        pid, status = os.waitpid(childid, 0)
        print "We are inside the parent process"
        print "Our child has the PID: %d" % childid
    else:
        print "Fork Error!"
        sys.exit(-1)

parent_process()
```

Thread module:

```py
#!/usr/bin/env python2

import thread
import time

def worker_thread(id) :
    
    print "Thread ID %d now alive!" % id

    count = 1
    while True:
        print "Thread with ID %d has counter value %d" % (id, count)
        time.sleep(2)
        count += 1


for i in range(5) :
    thread.start_new_thread(worker_thread, (i,))

print "Main thread going for infinite wait loop"
while True:
    pass
```

Threading and Queues:

  * Create task queues
  * Threads receive tasks
  * Threads complete tasks and inform the queue
  * All threads exit once queue is empty

```py
#!/usr/bin/env python2

import threading    
import Queue
import time

class WorkerThread(threading.Thread) :

    def __init__(self, queue) :
        threading.Thread.__init__(self)
        self.queue = queue

    def run(self) :
        print "In WorkingThread"
        while True:
            counter = self.queue.get()
            print "Ordered to sleep for %d seconds!" % counter
            time.sleep(counter)
            print "Finished sleeping for %d seconds" % counter
            self.queue.task_done()


queue = Queue.Queue()

for i in range(10):
    print "Creating WorkerThread : %d" %i
    worker = WorkerThread(queue)
    worker.setDaemon(True)
    worker.start()
    print "WorkerThread %d Created!" %i

for i in range(10) :
    queue.put(i)

queue.join()

print "All tasks over!"
```

Signals

signal list:

```
man 7 signal
```

subprocess:

calls program and simply write to stdout

```py
subprocess.call(['ps', 'aux'])
```

returns output as string:

```py
lines = subprocess.check_output(['ls', '-la'])
line_list = lines.split('\n')
```

to interact with **stdin**, **stdout** and **stder** use
`subprocess.Popen`:

```py
p = subprocess.Popen(['ls', '-la'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr = subprocess.PIPE)
p.stdout.read()
```

## Network Programming

Socket Programming:

  * TCP and UDP Sockets

  Regular Servers and Clients

  * Raw Sockets

  Sniffing and Injection

Server Side:


```py
#!/usr/bin/env python2

import socket

tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

tcp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

tcp_socket.bind(("0.0.0.0", 8000))

tcp_socket.listen(5)

print "waiting for a client..."
client, (ip, port) = tcp_socket.accept()
```

Client side:

```py
#!/usr/bin/env python2

import socket
import sys

tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

tcp_socket.connect((sys.argv[1]), 8000)

while 1:
    user_input = raw_input("Please enter a string: ")
    tcp_socket.send(user_input)
    print tcp_socket.recv(4096)
```

## SocketServer

  * Framework in Python to create TCP and UDP servers
  * Comes in handy if you want to create a server to
  lure a client and analyze its behavior

```py
#!/usr/bin/env python2

import SocketServer

class EchoHandler(SocketServer.BaseRequestHandler) :

    def handle(self) :

        print "Got connection from : %s" % str(self.client_address)
        data = 'dummy'

        while len(data):
            data = self.request.recv(1024)
            print "Client sent: %s" % str(data)
            self.request.send(data)

        print "Client left"

server_addr = ("0.0.0.0", 9000)

server = SocketServer.TCPServer(server_addr, EchoHandler)

server.serve_forever()
#server.handle_request()
```

## Creating a Web Server

Simple Web Application Server

  * SimpleHTTPServer class
  * Implement `do_GET()`

```py
#!/usr/bin/env python2

import SocketServer
import SimpleHTTPServer

class HttpRequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler) :

    def do_GET(self) :

        if self.path == '/admin' :
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write('This page is only for admin!')

        else :
            SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self)

httpServer = SocketServer.TCPServer(("0.0.0.0", 9000), HttpRequestHandler)

httpServer.serve_forever()
```

## Packet Sniffing with Raw Sockets

Raw Socket Basics

  * Raw sockets provide a way to bypass the whole network stack
  traversal of a packet and deliver it directly to an application

  * Multiple ways to create raw sockets. We will use PF\_PACKET
  interface

What is PF\_PACKET interface ?

  * It is a software interface to send/receive packets at layer 2
  of the OSI i.e. device driver

  * All packets received will be complete with all headers and data

  * All packets sent will be transmitted without modification by
  the kernel to the medium

  * Supports filtering using Berkley Packet Filters

Creating Raw Sockets

  * use the socket module

  * read packets

  * interpret and analyze them

  * can send out responses as well

Byte order:

```
>>> import struct
>>> struct.pack("B", 1) <--- byte (unsigned char)
'\x01'
>>> struct.pack("H", 1) <--- unsigned short
'\x01\x00'
>>> struct.pack(">H", 1)
'\x00\x01'
>>> struct.pack("!H", 1)
'\x00\x01'
```

unsigned long:

```py
>>> struct.pack(">L", 1)
'\x00\x00\x00\x01'
```

unpack value:

```py
>>> struct.unpack(">L", '\x00\x00\x00\x01')
(1,)
```

in order to create raw socket we typically use familiar socket call
with one key difference: we use **PF_PACKET** to actually tell the 
kernell this is going to use packet interface and **SOCK_RAW** 
to indicate that it is the raw socket

```py
raw_socket = socket.socket(socket.PF_PACKET, socket.SOCK_RAW,
											socket.htons(0x0800))
```


`socket.htons(0x0800)` basically tells the kernel which protocol
we are interesting in. the value `0x0800` indicates that we are
interested in the IP protocol.

how can we figure out this values ?

Basically we can figure out it in `/usr/include/linux/if_ether.h`
header file which contains all of these values

Now as soon as you have raw socket you can call `recvfrom` method
on it to read a packet

```py
pkt = raw_socket.recvfrom(2048)
```

this contains a lot of info in the form of a tuple, however, what we 
are interested in is just the first element which is the whole raw
packet 

```py
>>> pkt = raw_socket.recvfrom(2048)
>>> pkt
('@\xf0/+!\xe8\x90\xe6\xba\xf0\x06\xc9\x08\x00E\x00\x00`\x00\x00@\x00/\x11\x12\xa6UB\x1d\xde\xc0\xa8\x05\x1f#\x81\xc8\xd5\x00L\xce"d1:rd2:id20:\xde\xa7\x96\x1f|sr\xdfj?x)X\ng\x86v\xac\x1b:2:ip4:m\xa7\xfdne1:t4:pn\x00\x001:v4:LT\x00\x0f1:y1:re', ('wlp2s0', 2048, 0, 1, '\x90\xe6\xba\xf0\x06\xc9'))
```

Now the next thing we need to do is parse the packet

The first header is **Ethernet** header. The size of the Ethernet
header is always constant 14 bytes. The first 6 bytes is the 
destination MAC address, the next 6 bytes is the source MAC address
and finally the last two bytes is **EthType** which is IP in our case

```
------------------------------------------------------------
| EthDHost(6 bytes) | EthSHost(6 bytes) | EthType(2 bytes) |
------------------------------------------------------------
```

now let's fetch ethernet header

```py
>>> ethernet_header = pkt[0][0:14]
```

and parse this header using `unpack`. Here is what we need to do:
the first six bytes is the destination MAC address, the next six
bytes are basically source MAC address, and the last 2 bytes is
the Ethernet type:

```py
>>> eth_hdr = struct.unpack(">6s6s2s", ethernet_header)
```

If we look at `eth_hdr` it is a simply tuple consisting of
three elements

```py
>>> eth_hdr
('@\xf0/+!\xe8', '\x90\xe6\xba\xf0\x06\xc9', '\x08\x00')
```

Let's print it in hex form

```py
>>> binascii.hexlify(eth_hdr[0])
'40f02f2b21e8'
>>> binascii.hexlify(eth_hdr[1])
'90e6baf006c9'
>>> binascii.hexlify(eth_hdr[2])
'0800'
```

The next header is IP header. IP packet without Options field
is basically 20 bytes long.

So we are going to take next 20 bytes and try to decode that as
IP header

```py
ip_header = pkt[0][14:34]
```

and unplack it

```py
>>> ip_hdr = struct.unpack(">12s4s4s", ip_header)
>>> ip_hdr
('E\x00\x00`\x00\x00@\x00/\x11\x12\xa6', 'UB\x1d\xde', '\xc0\xa8\x05\x1f')
```

So what I am going to do is print source IP address and destination
IP address directly

```py
>>> print "Source IP address : " + socket.inet_ntoa(ip_hdr[1])
Source IP address : 85.66.29.222
>>> print "Destination IP address: " + socket.inet_ntoa(ip_hdr[2])
Destination IP address: 192.168.5.31
```

now let's parse tcp header

```py
>>> tcp_header = pkt[0][34:54]
>>> tcp_hdr = struct.unpack(">HH16s", tcp_header)
>>> tcp_hdr
(9089, 51413, '\x00L\xce"d1:rd2:id20:')
```

First two numbers are port numbers 

## Packet Injection

Ability to inject raw packets into the network without having to
go through regular network stack.

The whole idea is that we will create arbitrary packets in userspace
and send them out on the wire.

First we create raw socket

```py
raw_socket = socket.socket(socket.PF_PACKET, socket.SOCK_RAW, socket.htons(0x0800))
```

In this case what we are going to do is first decide on which
interface to do the packet injection, and this is done by
binding to the specific interface:

```py
raw_socket.bind(("wlp2s0", socket.htons(0x0800)))
```

Even on packet injection we can bind to only one interface

Now we create a very simple internet packet, it's going to have
destination address, source address and ether type. And after
that we are going to inject random string inside.

In order to do this we use `struct.pack`:

```py
packet = struct.pack(">6s6s2s", '\xaa'*6, '\xbb'*6, '\x08\x00')
```

destination address -- all `\xaa`, source address -- all `\xbb`

and then finally ether type to the IP.

After that we just send this packet

```py
raw_socket.send(packet + "Hello there!")
```







