#include "mcc_generated_files/mcc.h"
#include "audio.h"
#include "spi-flash.h"

extern SpiFlashFileInfo spiFileArray[SPI_FLASH_MAX_NUM_FILES];

#define DAC1_DoubleBufferLatch() \
    (DACLDbits.DAC1LD = 1)

void AudioPlayFlashFile(uint8_t fileId, uint32_t numBytes)
{
  
  uint32_t byteCount=0;
  
  SELECT_FLASH_CHIP;
  SSP1CON1bits.WCOL = 0;
  
  SPI_FLASH_TX_BYTE(0x03);
  SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].startAddr >> 16 ));
  SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].startAddr >> 8 ));
  SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].startAddr));

  uint8_t val = 0;
  DAC1CON0bits.DAC1FM = 0;
  while(1){
    
    if(PIR1bits.TMR2IF){
      
      PIR1bits.TMR2IF = 0;  
  
      SPI_FLASH_RX_BYTE(val);
      byteCount++;
      DAC1REFL  = val;  
      DAC1_DoubleBufferLatch();

      if(byteCount == numBytes){
        
        DESELECT_FLASH_CHIP;
        
        SELECT_FLASH_CHIP;
        SSP1CON1bits.WCOL = 0;
  
        SPI_FLASH_TX_BYTE(0x03);
        SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].startAddr >> 16 ));
        SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].startAddr >> 8 ));
        SPI_FLASH_TX_BYTE( 0xff & (spiFileArray[fileId].startAddr));
        
        byteCount = 0;
      }
      
      
      
      
    }
    
  }
    
}

