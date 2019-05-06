#!/usr/bin/python
import math as m

fs = 16e3;
f = 2000.0;
a = 255.0;

nSample = 16384*16;
#nSample = 4096;
#nSample = 7

fileId = open('sn.bin','wb');

for n in range(0,nSample):
	val = int( a*(0.5+0.5*m.sin(2*m.pi*f/fs*n)) );
	print("%d - %d 0x%x" % (n, val, val));
	fileId.write(bytearray([ int( a*(0.5+0.5*m.sin(2*m.pi*f/fs*n)) ) ] ));

fileId.close();
