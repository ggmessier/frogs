#!/usr/bin/python3


inFileStr = "bb.raw";
outFileStr = "bb.bin";
nBytes = 1048576;

print("Analyzing %s ..." % (inFileStr));

maxDes = 255;
minDes = 79;

maxByte = 0;
minByte = 1000;

inF = open(inFileStr,"rb");
for i in range(0,nBytes):
        
        byte = inF.read(1);
        val = int.from_bytes(byte,byteorder='big',signed=False);

        if(val > maxByte):
                maxByte = val;
        if(val < minByte):
                minByte = val;

inF.close();

print("Max byte: %d, Min byte: %d" % (maxByte,minByte));

rng = maxByte - minByte;
rngDes = maxDes - minDes;

scale = rngDes/rng;
offs = minDes;

print("Normalizing file...");

outF = open(outFileStr,"wb");
inF = open(inFileStr,"rb");
for i in range(0,nBytes):

        byte = inF.read(1);
        val = int.from_bytes(byte,byteorder='big',signed=False);
        outF.write(bytearray([ int( (val-minByte)*scale+offs ) ] ));
        
outF.close();
inF.close();

print("Verifying file...");

maxByte = 0;
minByte = 1000;

outF = open(outFileStr,"rb");
for i in range(0,nBytes):
        
        byte = outF.read(1);
        val = int.from_bytes(byte,byteorder='big',signed=False);

        if(val > maxByte):
                maxByte = val;
        if(val < minByte):
                minByte = val;

outF.close();

if maxByte == maxDes and minByte == minDes:
        print("Ok :)");
else:
        print("Verification failed.. :(");



