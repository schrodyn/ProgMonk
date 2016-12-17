# JDB Tips

Start listening server:

```
java -agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=localhost:1044 Serialize_write
```

**jdwp** -- Java Debug Wire Protocol

**transport=dt_socket** -- tcp/ip socket transport

there are two: Socket Transport and Shared Memory Transport (only on 
Windows)

**server=y** -- server mode

**suspend=y** -- start execution on connection

Connect with jdb:

```
jdb -attach localhost:1044
```

