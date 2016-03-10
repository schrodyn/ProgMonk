#!/usr/bin/env python2

from random import randint
import string
import sys

if len(sys.argv) < 2:
	print '''
usage: {} NUM

NUM -- number of characters
'''.format(sys.argv[0])
	sys.exit(1)


dictionary=string.ascii_lowercase
dictionary+=string.ascii_uppercase
dictionary+=string.digits
dictionary+="_$@!&^+-"

num=int(sys.argv[1])
passw=[dictionary[randint(0,len(dictionary))] 
								for i in range(num)]
print "".join(passw)
