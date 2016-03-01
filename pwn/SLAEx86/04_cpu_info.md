#Exercise 1.2 CPU Info

Now I requested you to find CPU details on the Ubuntu System.
The first thing was to figure out if your processor is 32/64-bit.
And at the same time to find other additional capabilities such
as the presence of FPU, MMX, SSE, SSE2 etc.

You can do this using two commands. One of this is `lscpu`.

```
osboxes@osboxes:~$ lscpu
Architecture:          i686
CPU op-mode(s):        32-bit
Byte Order:            Little Endian
CPU(s):                1
On-line CPU(s) list:   0
Thread(s) per core:    1
Core(s) per socket:    1
Socket(s):             1
Vendor ID:             AuthenticAMD
CPU family:            21
Model:                 19
Stepping:              1
CPU MHz:               2495.338
BogoMIPS:              4990.67
Hypervisor vendor:     KVM
Virtualisation type:   full
L1d cache:             16K
L1i cache:             64K
L2 cache:              2048K
```

and the other is `cat /proc/cpuinfo`

```
osboxes@osboxes:~$ cat /proc/cpuinfo 
processor	: 0
vendor_id	: AuthenticAMD
cpu family	: 21
model		: 19
model name	: AMD A10-5750M APU with Radeon(tm) HD Graphics  
stepping	: 1
microcode	: 0x6000626
cpu MHz		: 2495.338
cache size	: 2048 KB
physical id	: 0
siblings	: 1
core id		: 0
cpu cores	: 1
apicid		: 0
initial apicid	: 0
fdiv_bug	: no
f00f_bug	: no
coma_bug	: no
fpu		: yes
fpu_exception	: yes
cpuid level	: 13
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 syscall nx mmxext fxsr_opt rdtscp extd_apicid pni pclmulqdq monitor ssse3 cx16 sse4_1 sse4_2 popcnt aes xsave avx hypervisor lahf_lm cr8_legacy abm sse4a misalignsse 3dnowprefetch arat
bogomips	: 4990.67
clflush size	: 64
cache_alignment	: 64
address sizes	: 48 bits physical, 48 bits virtual
power management:
```

important field is **flags**. This is important because it tells us
a lot of capabilities of current CPU. So it has `fpu` unit and it
does support `mmx` instructions, `sse`,`sse2`

So this tool commands allow you to quickly figure out your CPU
information to give you an idea about your working environment.
So that's all for this video.
