#!/usr/bin/python

fileBytePattern = [ 0x00, 0xff ];
fileSize = 8;

fileId = open('testfile.bin','wb');

for i in range(0,int(fileSize/2)):
	fileId.write(bytearray(fileBytePattern));

fileId.close();
