#Sort Files Like A Master With The Linux Sort Command

Sorting is a fundamental task when it comes to programming, if you have
a decent knowledge of various sorting algorithms, their advantages and 
disadvantages, you will be a better software developer for it. However,
often enough you just don't need to draw on this deeper knowledge.

The extent of most people's knowledge ends with:

```
sort some_file.txt
```

Which is fair enough, you rarely need to dig deeper, the default 
behaviour will usually do what you need and when it doesn't – we have 
Python, Ruby or Perl, we can hack something together. 

##Sorting Basics

To start with we have the following file:

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat letters.txt
b
D
c
A
C
B
d
a
```

We'll do the most basic sort first:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort letters.txt
a
A
b
B
c
C
d
D
```

Looks good, how about doing it in reverse:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -r letters.txt
D
d
C
c
B
b
A
a
```

Also easy, but what if we want to be case insensitive? Hang on a sec, 
according to the output it's already case insensitive. But the man 
page has an option for this:

```
-f, --ignore-case
              fold lower case to upper case characters
```

If sort is case insensitive by default, what is this option for.
But the man page also contains the following:

```
*** WARNING *** The locale specified by the  environment  affects  sort
       order.  Set LC_ALL=C to get the traditional sort order that uses native
	          byte values.
```

What this means that we need to set the `LC_ALL` environment variable 
to get the behaviour that we would expect from sort (i.e. capital 
letters before non-capitals). Let's try that:

```
alan@alan-ubuntu-vm:~/tmp/sort$ export LC_ALL=C
alan@alan-ubuntu-vm:~/tmp/sort$ sort letters.txt
A
B
C
D
a
b
c
d
```

That's better, and now our `-f` option is actually useful:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -f letters.txt
A
a
B
b
C
c
D
d
```

That looks ok, but something still seems a little funny, all the 
capitals appear before all the non-capitals every time. That's because
the sort is not **stable**, but we can make it stable:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -f -s letters.txt
A
a
b
B
c
C
D
d
```

Now that's exactly what we wanted, it's case insensitive and stable, 
i.e. **if the small letter appeared before the capital when unsorted 
(and the letters are the same), this order will be the same in the 
sorted list.**

Ok, but what if we have numbers:

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat numbers.txt
5
4
12
1
3
56
```

A normal sort is not what we want:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort numbers.txt
1
12
3
4
5
56
```

But we can fix that:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -n numbers.txt
1
3
4
5
12
56
```

And, if our lines happen to have some leading blanks:

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat blank_letters.txt
b
D
   c
   A
   C
       B
	   d
	   a
```

We can easily ignore those and still sort correctly 
(using the `-b` flag):

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -f -s -b blank_letters.txt
A
a
b
    B
	   c
	   C
	   D
	   d
```

Of course none of this actually writes the sorted output back to the 
file, we only get it on standard output. If we want to write it back 
to the file, we have to redirect the output to a new file and then 
replace the old file with the new file:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -f -s -b blank_letters2.txt > blank_letters2.sorted
alan@alan-ubuntu-vm:~/tmp/sort$ cat blank_letters2.sorted
A
a
b
    B
	   c
	   C
	   D
	   d
alan@alan-ubuntu-vm:~/tmp/sort$ mv blank_letters2.sorted blank_letters2.txt
alan@alan-ubuntu-vm:~/tmp/sort$ cat blank_letters2.txt
A
a
b
	B
		c
		C
		D
		d
```

As an alternative to redirection, sort also has the `-o` option:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -f -s -b blank_letters2.txt -o blank_letters2.sorted
alan@alan-ubuntu-vm:~/tmp/sort$ cat blank_letters2.sorted
A
a
b
    B
	   c
	   C
	   D
	   d
```

Alright, this is all pretty standard stuff, let's see how we can do something a little bit more fancy.

#Advanced Sort Usage

One of the most common use cases when it comes to sort is to pipe 
its output to `uniq`, in order to remove any duplicate lines, but this
is often not necessary as sort has a uniq-type option built right in 
(`-u`):

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat blank_letters.txt
b
D
c
A
C
B
d
a
alan@alan-ubuntu-vm:~/tmp/sort$ sort -f -s -u blank_letters.txt
A
b
c
D
```

It even took into account the fact that we wanted to be case 
insensitive. Compare that to a case-sensitive sort with a **uniq** 
option set:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -s -u blank_letters.txt
A
B
C
D
a
b
c
d
```

Nothing is removed as there are no duplicate lines.

But what if my lines are a little bit more complicated than just a 
single letter, what if there are multiple columns and I want to sort 
by one of those columns (*not the first one*). This is also possible:

```
alan@alan-ubuntu-vm:~/tmp/sort$ ls -al | sort -k5
total 36
-rw-r--r-- 1 alan alan   14 May  9 00:12 numbers.txt
-rw-r--r-- 1 alan alan   16 May  9 00:00 letters.txt
-rw-r--r-- 1 alan alan   16 May  9 00:40 blank_letters.txt
-rw-r--r-- 1 alan alan   20 May  9 00:33 blank_letters2.sorted
-rw-r--r-- 1 alan alan   20 May  9 00:33 blank_letters2.txt
-rw-r--r-- 1 alan alan   20 May  9 00:37 blank_letters3.txt
-rw-r--r-- 1 alan alan   84 May  8 23:15 file1.txt
drwxr-xr-x 3 alan alan 4096 May  8 23:13 ..
drwxr-xr-x 2 alan alan 4096 May  9 00:40 .
```

As you can see we sorted the output of `ls` by the size 
(*the 5th column*). This is what the `-k` option is for. Basically 
`-k` tells sort to start sorting at a particular column given a 
particular column separator. **The column separator is, by default, 
any blank character**. So in the above example, we told sort to sort 
by the 5th column given the fact that the column separator is blanks.

But we don't have to be restricted by the default separator, we can 
specify our own using the `-t` option. Let's sort the first 10 lines 
of my `/etc/passwd` file by the 4th column – the group id.
As you know the `/etc/passwd` file uses the `:` (colon) character as 
the separator.

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat /etc/passwd | head                 
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/bin/sh
man:x:6:12:man:/var/cache/man:/bin/sh
lp:x:7:7:lp:/var/spool/lpd:/bin/sh
mail:x:8:8:mail:/var/mail:/bin/sh
news:x:9:9:news:/var/spool/news:/bin/sh
```

There is a group id of 65534 in there which should appear last. 
Let's sort it:

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat /etc/passwd | head | sort -t: -k4 -n
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
lp:x:7:7:lp:/var/spool/lpd:/bin/sh
mail:x:8:8:mail:/var/mail:/bin/sh
news:x:9:9:news:/var/spool/news:/bin/sh
man:x:6:12:man:/var/cache/man:/bin/sh
games:x:5:60:games:/usr/games:/bin/sh
sync:x:4:65534:sync:/bin:/bin/sync
```

We had to do a numeric sort since we're dealing with numbers, and we 
specified `:` (colon) as the column separator. The output is sorted 
correctly with 65534 being on the last line. Pretty cool! But the fun 
doesn't stop there, we can sort by multiple columns, one after the 
other. Consider this list of IP addresses:

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat ips.txt
192.168.0.25
127.0.0.12
192.168.0.1
127.0.0.3
127.0.0.6
192.168.0.5
```

Let's sort it by the first column, so that all the addresses starting 
with 127 go together, and then sort it by the 4th column, to make sure
that the IPs are sorted by the last column within each range.

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat ips.txt | sort -t. -k 2,2n -k 4,4n
127.0.0.3
127.0.0.6
127.0.0.12
192.168.0.1
192.168.0.5
192.168.0.25
```

We specified the dot as the separator. The `-k 2,2n` syntax has the 
following meaning. Do a sort by column (`-k`), start at the beginning 
of column 2 and go to the end of column 2 (`2,2`). The `n` on the end 
is to indicate that we want to do a numeric sort since we are dealing 
with numbers. That is some powerful stuff, wouldn't you agree?

#Cool/Useful Stuff

There is still more we can do with the sort command. Have you ever 
wanted to randomize the lines in a file? It is not a common use case, 
but does come in handy once in a while (*if only for testing purposes,
sometimes*). Well, the sort command has you covered here also with the`-R` option (that's capital R):

```
alan@alan-ubuntu-vm:~/tmp/sort$ cat numbers.txt
5
4
12
1
3
56
alan@alan-ubuntu-vm:~/tmp/sort$ cat numbers.txt | sort -R
5
4
1
3
12
56
alan@alan-ubuntu-vm:~/tmp/sort$ cat numbers.txt | sort -R
3
4
1
56
5
12
```

We get a different order every time, which is what we would expect 
from randomizing the lines.

If you give sort multiple files on the command line, it will combine 
the contents of all the files and sort it as a whole:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -n numbers.txt numbers2.txt
1
1
3
4
4
5
7
8
10
12
22
23
26
56
56
68
```

This is really handy, but sometimes, the files you have are already 
sorted, you just want to **merge** them. Sort provides the `-m` 
options just for this purpose. The output of using sort on two files 
will be exactly the same whether you use `-m` or not, but merging 
should be faster:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -n -m numbers1.sorted numbers2.sorted
1
1
3
4
4
5
7
8
10
12
22
23
26
56
56
68
```

Lastly, if you just want to check if a file is sorted or not, without 
actually performing the sort, you have the `-c` option:

```
alan@alan-ubuntu-vm:~/tmp/sort$ sort -n -c numbers.txt
sort: numbers.txt:2: disorder: 4
alan@alan-ubuntu-vm:~/tmp/sort$ sort -n -c numbers1.sorted
alan@alan-ubuntu-vm:~/tmp/sort$
```


