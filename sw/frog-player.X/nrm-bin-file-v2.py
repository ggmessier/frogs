#!/opt/local/bin/python3


import sys, os

def main(argv):
	inFileStr = sys.argv[1]
	outFileStr = inFileStr[0:-4] + ".bin"
	nBytes = os.path.getsize(inFileStr)

        print("Analyzing %s ..." % (inFileStr));

        maxDes = 255;
        minDes = 0;

        maxByte = -1000;
        minByte = 1000;

        inF = open(inFileStr,"rb");
        for i in range(0,nBytes):
        
                byte = inF.read(1);
                val = int.from_bytes(byte,byteorder='big',signed=True);

                if(val > maxByte):
                        maxByte = val;
                        if(val < minByte):
                                minByte = val;

        inF.close();
        
        print("Max byte: %d, Min byte: %d" % (maxByte,minByte));

        rng = maxByte - minByte;
        rngDes = maxDes - minDes;

        scale = rngDes/rng;

        print("Normalizing file...");

        outF = open(outFileStr,"wb");
        inF = open(inFileStr,"rb");
        for i in range(0,nBytes):
        
                byte = inF.read(1);
                val = int.from_bytes(byte,byteorder='big',signed=True);
                outF.write(bytearray([ int( (val-minByte)*scale ) ] ));
        
        outF.close();
        inF.close();

        print("Verifying file...");

        maxByte = -1000;
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



if __name__ == "__main__":
   main(sys.argv[1:])
