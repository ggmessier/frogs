#include "mcc_generated_files/eusart.h"
#include "pic-kermit.h"


/*
 * Global variables
 */

// No file size so risk of overwriting or running out of memory.
uint8_t *fileWritePtr;
uint16_t bufferCapacity;
uint8_t bufferCheckOverride=0;

ReqInitData remoteInitData;  // Mandatory datafields for remote terminal
ReqInitData localInitData = {
  .maxl = 94 + 32,
  .time = 5 + 32,
  .npad = 0 + 32,
  .padc = 0 ^ 64,
  .eol = '\r' + 32,
  .qctl = '#' + 32
};

void PicKermitRxFile()
{
  KermitRxStates state = RX_SEND_INIT_WAIT;
  uint8_t returnVal=0, packetType, txSeq = 0, rxSeq, rxSeqOld;
  
  // Modify this infinite loop to have a timeout limit based on the kermit
  // init parameters.
  for(uint8_t byteCount=0; byteCount < 200; byteCount++)
  {
    switch(state)
    {
      case RX_SEND_INIT_WAIT:
        
        // Init packet described in Section 5.
        returnVal = PicKermitRxPacket(&packetType,&rxSeq,sizeof(remoteInitData),(uint8_t *)(&remoteInitData));
        
        if( returnVal && packetType == 'S'){
          remoteInitData.maxl -= 32;
          remoteInitData.time -= 32;
          remoteInitData.npad -= 32;
          remoteInitData.padc ^= 64;
          remoteInitData.eol -= 32;
          remoteInitData.qctl -= 32; 
          
          rxSeqOld = rxSeq;
        
          PicKermitTxPacket('Y',txSeq,sizeof(localInitData),(uint8_t *)(&localInitData));
          
          state = RX_FILE_HEADER_WAIT;
        }
        break;  

      case RX_FILE_HEADER_WAIT:

        // The file name is in the data field but we don't do anything with it.
        returnVal = PicKermitRxPacket(&packetType,&rxSeq,0,0);
      
        if( returnVal ){
          
          rxSeqOld = rxSeq;
          
          if(packetType == 'F'){
            state = RX_DATA_PACKET_WAIT;
            txSeq++;
            PicKermitTxPacket('Y',txSeq,0,0);
          }
          else if(packetType == 'S'){
            PicKermitTxPacket('Y',txSeq,sizeof(localInitData),(uint8_t *)(&localInitData));          
          }
          else if(packetType == 'Z'){
            PicKermitTxPacket('Y',txSeq,0,0);
          }
          else if(packetType == 'B'){
            PicKermitTxPacket('Y',txSeq,0,0);
            state = RX_SEND_COMPLETE;
          }

        }
        break;
        
      case RX_DATA_PACKET_WAIT:

        // Abort if there's not enough room in the buffer for another packet
        if( !bufferCheckOverride && (bufferCapacity < remoteInitData.maxl-3) ){
          PicKermitTxPacket('E',txSeq,0,0);
          state = RX_ABORT;
          break;
        }
        
        
        // Max data field is max length minus 3 (sequence num, type and checksum)
        returnVal = PicKermitRxPacket(&packetType,&rxSeq,remoteInitData.maxl-3,fileWritePtr);

        if( returnVal ){
          if( packetType == 'D' && rxSeq != rxSeqOld){
            fileWritePtr += returnVal-5;
            bufferCapacity -= returnVal-5;
            txSeq++;
            PicKermitTxPacket('Y',txSeq,0,0);
            rxSeqOld = rxSeq;
          }
          else if( packetType == 'Z'){
            txSeq++;
            PicKermitTxPacket('Y',txSeq,0,0);
            state = RX_FILE_HEADER_WAIT;
            rxSeqOld = rxSeq;
          }
        }

        break;
       
      case RX_ABORT:
      case RX_SEND_COMPLETE:
        return;
        break;
    }
      
  }
    
}

void PicKermitTxPacket(uint8_t txPacketType, uint8_t txSeq, uint8_t nDataBytes, uint8_t *dataPtr)
{
  
  uint8_t check=0;
  
  // Send mark character.
  EUSART_Write(0x01);
  
  // Send packet length.
  EUSART_Write(3+nDataBytes + 32);
  check += 3 + nDataBytes + 32;
  
  // Send sequence number.
  EUSART_Write(txSeq + 32);
  check += txSeq + 32;
  
  // Send packet type.
  EUSART_Write(txPacketType);
  check += txPacketType;
  
  // Send data.
  while(nDataBytes){
    EUSART_Write(*dataPtr);
    check += *dataPtr;
    dataPtr++;
    nDataBytes--;
  }
  
  // Send checksum
  check = (( check + ((check & 0xc0)>>6) ) & 0x3f) + 32;
  EUSART_Write(check);
  
  // Send end of packet char.
  EUSART_Write('\r');
  
  return;
  
}


uint8_t PicKermitRxPacket(uint8_t *packetType, uint8_t *rxSeq, uint8_t nDataBytes, uint8_t *dataPtr)
{
  uint8_t rxByte,bytesLeft,packetLength,seqNum,check=0,ctrlByte=0;
  
  // Check for mark character. 
  rxByte = EUSART_Read();
  if(rxByte != 0x01) 
    return 0;
  
  rxByte = EUSART_Read();
  check += rxByte;

  bytesLeft = rxByte - 32;
  packetLength = bytesLeft + 2;
  
  rxByte = EUSART_Read();
  check += rxByte;

  seqNum = rxByte - 32;
  bytesLeft--;
  
  *rxSeq = seqNum;
  
  rxByte = EUSART_Read();
  check += rxByte;

  *packetType = rxByte;
  bytesLeft--;
  
  while(bytesLeft){
    
    rxByte = EUSART_Read();
    bytesLeft--;

    if(bytesLeft && *packetType == 'D' && rxByte == 35){
      ctrlByte = 1;   
      nDataBytes--;
    }
    else if(bytesLeft && nDataBytes && ctrlByte){
      *(dataPtr++) = rxByte ^ 64;
      nDataBytes--;
      ctrlByte = 0;
    }
    else if(bytesLeft && nDataBytes && !ctrlByte){
      *(dataPtr++) = rxByte;
      nDataBytes--;
    }
    
    
    if( !bytesLeft ){
      
      check = (( check + ((check & 0xc0)>>6) ) & 0x3f) + 32;
      
      if(rxByte != check)
        return 0;      
    }
    else{
      check += rxByte;    
    }
    
  }
  
  // Clear end of packet char.
  EUSART_Read();
      
  return packetLength;
  
}


