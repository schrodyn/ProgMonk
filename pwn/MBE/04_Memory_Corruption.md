#Memory Corruption

It's modifying a **binary’s** memory in a way that was not intended

The vast majority of system-level exploits (real-world and 
competition) involve memory corruption

##Print ABCD

```sh
$ echo -e '\x41\x42\x43\x44'
$ printf '\x41\x42\x43\x44'
$ python -c 'print "\x41\x42\x43\x44"'
$ perl -e 'print "\x41\x42\x43\x44";'
```

##Print 100 As

```sh
$ echo/printf (hold down alt; type 100) A
$ python -c 'print "A"*100'
$ perl -e 'print "A" x 100;'
```

##BASH refresher

Use command output as an argument

```sh
$ ./vulnerable `your_command_here`
$ ./vulnerable $(your_command_here)
```

Use command as input

```sh
$ your_command_here | ./vulnerable
```

Write command output to file

```sh
$ your_command_here > filename
```

Use file as input

```sh
$ ./vulnerable < filename
```

##gdb io

Use command output as an argument

```
(gdb) r $(your_command_here)
```

Use command as input

```
(gdb) r < <(your_command_here)
```

Write command output to file

```
(gdb) r > filename
```

Use file as input

```
(gdb) r < filename
```



