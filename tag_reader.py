#!/usr/bin/python
import sys

pipe = open('/dev/bus/usb/001/024', 'r')
while True:
	for character in pipe.read(1):
		sys.stdout.write(repr(character))
		sys.stdout.flush()
