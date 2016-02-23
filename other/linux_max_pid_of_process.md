#What is the maximum value of the PID of a process?

On Linux, you can find the maximum PID value for your system with 
this:

```
$ cat /proc/sys/kernel/pid_max
```

This value can also be written using the same file, however the value 
can only be extended up to a theoretical maximum of 32768 for 32 bit 
systems or 4194304 for 64 bit:

```
$ echo 32768 > /proc/sys/kernel/pid_max
```

It seems to be normative practice on most 64 bit systems to set this 
value to the maximum 32 bit value, but this is by convention rather 
than a requirement.

From `man 5 proc`:

```
/proc/sys/kernel/pid_max (since Linux 2.5.34)
This file specifies the value at which  PIDs  wrap  around
(i.e., the value in this file is one greater than the maximum PID).  
PIDs greater than this  value  are  not  allocated; thus, 
the value in this file also acts as a system-wide limit on the 
total number of processes  and  threads.
The  default  value  for  this file, 32768, results in the
same range of PIDs as on earlier kernels.  On 32-bit platforms,  
32768 is the maximum value for pid_max.  On 64-bit
systems, pid_max can be  set  to  any  value  up  to  2^22
(PID_MAX_LIMIT, approximately 4 million).
```

And no, you cannot change the PID of a running process. It gets 
assigned as a sequential number by the kernel at the time the process 
starts and that is it's identifier from that time on. The only thing 
you could do to get a new one is have your code fork a new process and 
terminate the old one.
