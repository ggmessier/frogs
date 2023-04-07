
#include <stdio.h>
#include <string.h>
#include "lib-avr.h"



uint8_t spiNumFiles;
SpiFlashFileInfo spiFileArray[FLASH_NUM_FILES];
char str[30];


uint8_t SpiFlashReadStatus(uint8_t registerNum)
{
    SELECT_FLASH_CHIP;

    // 0x05
    if (registerNum == 1){
      SPI_TX_BYTE(0x05);
    }
    else{
      SPI_TX_BYTE(0x35);
    }
    
    uint8_t regVal;
    SPI_RX_BYTE(regVal);
    
    DESELECT_FLASH_CHIP;
    
    return regVal;
}


void SpiFlashWriteEnable(){
  
    SELECT_FLASH_CHIP;
    
    SPI_TX_BYTE(0x06);
    
    WAIT_FOR_WRITE_ENABLE;
    
    DESELECT_FLASH_CHIP;

    return;
}






#ifdef STILL_NEED_TO_PORT

void SpiFlashDeleteFilesystem()
{
  SpiFlashWriteEnable();
  
  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  SPI_FLASH_TX_BYTE(0x60);

  DESELECT_FLASH_CHIP;

  WAIT_FOR_IDLE_CHIP;

  return;
}


void SpiFlashLoadFileTable()
{
  sprintf(str,"== Load File Table ==\n\r");
  USART_PRINT_STR(str);

  
  // Do a block read of the max possible file table size
  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  // Send opcode
  SPI_FLASH_TX_BYTE(0x03);

  // Send address
  SPI_FLASH_TX_BYTE( 0x000000 );
  SPI_FLASH_TX_BYTE( 0x000000 );
  SPI_FLASH_TX_BYTE( 0x000000 );

  // Get data bytes
  uint8_t indData = 0;
  uint8_t *buffer = (uint8_t *)(spiFileArray);
  while(SPI_FLASH_TABLE_SIZE - indData){
    SPI_FLASH_RX_BYTE(buffer[indData++]);
  }

  DESELECT_FLASH_CHIP;
 
  // Determine number of valid file entries
  spiNumFiles = 0;
  while( (spiFileArray[spiNumFiles].startSym == SPI_FLASH_FILE_START_SYM) 
          && (spiNumFiles < SPI_FLASH_MAX_NUM_FILES) ){
    
    sprintf(str,"File %d\n\r",spiNumFiles);
    UART_PRINT_STR(str);
    sprintf(str," Name: %s\n\r",spiFileArray[spiNumFiles].name);
    UART_PRINT_STR(str);
    sprintf(str," Start Addr: 0x%x\n\r",spiFileArray[spiNumFiles].startAddr);
    UART_PRINT_STR(str);
    sprintf(str," Max Addr: 0x%x\n\r",spiFileArray[spiNumFiles].maxAddr);
    UART_PRINT_STR(str);
    sprintf(str," End Addr: 0x%x\n\r",spiFileArray[spiNumFiles].endAddr);
    UART_PRINT_STR(str);

    spiNumFiles++;

  }

  sprintf(str,"Num valid files: %d",spiNumFiles);
  UART_PRINT_STR(str);

  
  return;
}



void SpiPrintFileTable()
{
  char str[30];
  
  uint8_t ind = 0;
  while( (spiFileArray[ind].startSym == SPI_FLASH_FILE_START_SYM) 
          && (ind < SPI_FLASH_MAX_NUM_FILES) ){
    
    sprintf(str,"File %d\n\r",ind);
    UART_PRINT_STR(str);
    sprintf(str," Name: %s\n\r",spiFileArray[ind].name);
    UART_PRINT_STR(str);
    sprintf(str," Start Addr: 0x%lx\n\r",spiFileArray[ind].startAddr);
    UART_PRINT_STR(str);
    sprintf(str," Max Addr: 0x%lx\n\r",spiFileArray[ind].maxAddr);
    UART_PRINT_STR(str);
    sprintf(str," End Addr: 0x%lx\n\r",spiFileArray[ind].endAddr);
    UART_PRINT_STR(str);

    ind++;

  }
  
  
  
}

void SpiFlashSaveFileTable()
{
  // -- Erase file table block --
  SpiFlashWriteEnable();
  
  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  // Send opcode
  SPI_FLASH_TX_BYTE(0x20);

  // Send address
  SPI_FLASH_TX_BYTE( 0x000000 );
  SPI_FLASH_TX_BYTE( 0x000000 );
  SPI_FLASH_TX_BYTE( 0x000000 );

  DESELECT_FLASH_CHIP;

  WAIT_FOR_IDLE_CHIP;
  
  // -- Write file table --
  SpiFlashWriteEnable();

  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  // Send opcode
  SPI_FLASH_TX_BYTE(0x02);

  // Send address
  SPI_FLASH_TX_BYTE( 0x000000 );
  SPI_FLASH_TX_BYTE( 0x000000 );
  SPI_FLASH_TX_BYTE( 0x000000 );

  // Set data bytes
  uint8_t indData = 0;
  uint8_t *buffer = (uint8_t *)(spiFileArray);
  while((SPI_FLASH_TABLE_ENTRY_SIZE * spiNumFiles) - indData){
    SPI_FLASH_TX_BYTE(buffer[indData++]);
  }

  DESELECT_FLASH_CHIP;
  
  WAIT_FOR_IDLE_CHIP;
}




int SpiFlashFileOpen(uint8_t *name, uint32_t fileSize)
{
  uint8_t newId;

  sprintf(str,"== Open New File ==\n\r");
  UART_PRINT_STR(str);

  if(spiNumFiles >= SPI_FLASH_MAX_NUM_FILES)
    return SPI_FLASH_MAX_FILES_REACHED;
  
  // File must be at least 2^12 = 4 kB
  if(fileSize < 4096)
    return SPI_FLASH_FILE_TOO_SMALL;
  
  newId = spiNumFiles++;

  sprintf(str,"fileId: %d, num files: %d\n\r",newId,spiNumFiles);
  UART_PRINT_STR(str);
  
  spiFileArray[newId].startSym = SPI_FLASH_FILE_START_SYM;
  
  memcpy(spiFileArray[newId].name,name,MAX_FNAME_SIZE);
  
  if(newId > 0){
    spiFileArray[newId].startAddr = spiFileArray[newId-1].maxAddr+1;
  }
  else{
    spiFileArray[newId].startAddr = SPI_FLASH_FILE_START_ADDR;    
  }
  
  spiFileArray[newId].readAddr = spiFileArray[newId].startAddr;
  spiFileArray[newId].endAddr = spiFileArray[newId].startAddr;
  spiFileArray[newId].maxAddr = spiFileArray[newId].startAddr+fileSize-1;


  
  return newId;
  
}


int SpiFlashRewindRead(uint8_t fileId)
{
  if( (fileId < 0) || (fileId > SPI_FLASH_MAX_NUM_FILES) )
    return SPI_FLASH_BAD_FILE_ID;
  
  spiFileArray[fileId].readAddr = spiFileArray[fileId].startAddr;
  
  return fileId;
}

int SpiFlashWrite(uint8_t fileId, uint8_t numBytes, uint8_t *buffer)
{
  if( (fileId < 0) || (fileId > SPI_FLASH_MAX_NUM_FILES) )
    return SPI_FLASH_BAD_FILE_ID;

  if( spiFileArray[fileId].endAddr + numBytes > spiFileArray[fileId].maxAddr)
    return SPI_FLASH_FILE_OVERFLOW;
  
  uint8_t indData = 0;
  
  while(numBytes-indData){
    SpiFlashWriteEnable();
  
    SELECT_FLASH_CHIP;
    SSP1CON1bits.WCOL = 0;
  
    // Send opcode
    SPI_FLASH_TX_BYTE(0x02);

    // Send address
    SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].endAddr >> 16 ));
    SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].endAddr >> 8 ));
    SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].endAddr));

    spiFileArray[fileId].endAddr += 1;

    SPI_FLASH_TX_BYTE(buffer[indData++]);
  
    DESELECT_FLASH_CHIP;

    WAIT_FOR_IDLE_CHIP;
  }
  
  return fileId;
}

int SpiFlashRead(uint8_t fileId, uint8_t numBytes, uint8_t *buffer)
{
  if( (fileId < 0) || (fileId > SPI_FLASH_MAX_NUM_FILES) )
    return SPI_FLASH_BAD_FILE_ID;

  if( spiFileArray[fileId].readAddr + numBytes > spiFileArray[fileId].endAddr)
    return SPI_FLASH_END_OF_FILE;
  
  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  // Send opcode
  SPI_FLASH_TX_BYTE(0x03);

  // Send address
  SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].readAddr >> 16 ));
  SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].readAddr >> 8 ));
  SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].readAddr));

  spiFileArray[fileId].readAddr += numBytes;

  // Send data bytes
  uint8_t indData = 0;
  while(numBytes - indData){
    SPI_FLASH_RX_BYTE(buffer[indData++]);
  }

  DESELECT_FLASH_CHIP;

  return fileId;
  
}






void QuickFilesystemDelete()
{
  SpiFlashWriteEnable();
  
  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  SPI_FLASH_TX_BYTE(0xd8);
  
  SPI_FLASH_TX_BYTE( 0 );
  SPI_FLASH_TX_BYTE( 0 );
  SPI_FLASH_TX_BYTE( 0 );

  DESELECT_FLASH_CHIP;

  WAIT_FOR_IDLE_CHIP;

  return;
}



#endif
