#Linux Soft Hard Limits

These are: a 'soft' and a 'hard' limit for number of files a process 
may have opened at a time. Both limit the same resource (no relation 
to hard links or anything). The difference is: the soft limit may be 
changed later, up to the hard limit value, by the process running with 
these limits and hard limit can **only be lowered** – the process 
cannot assign itself more resources by increasing the hard limit 
(except processes running with superuser privileges (as root)).

Similar limits can be set for other system resources: system memory, 
CPU time, etc. See the `setrlimit(2)` manual page or the description of
your shell's `ulimit` build-in command (e.g. in the `bash(1)` manual 
page.
