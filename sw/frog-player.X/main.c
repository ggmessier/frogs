/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F1778
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <stdio.h>
#include <conio.h>
#include "mcc_generated_files/mcc.h"
#include "uart-menu.h"
#include "pic-kermit.h" 
#include "spi-flash.h"
#include "audio.h"


/*
                         Main application
 */

//#define MUSIC_FILE_SIZE 262144
#define MUSIC_FILE_SIZE 1048576



#define DATA_BUFF_SIZE 100
extern ReqInitData remoteInitData;
extern uint8_t *fileWritePtr;
extern uint8_t remBufferCapacity;
uint8_t dataBuffer[DATA_BUFF_SIZE];






void BlankFlashFs()
{
  //QuickFilesystemDelete();
  SpiFlashDeleteFilesystem();
}


void PrintKermitInitResults()
{
  char str[20];
  sprintf(str,"maxl: %d\n\r",remoteInitData.maxl);
  UART_PRINT_STR(str);
  sprintf(str,"time: %d\n\r",remoteInitData.time);
  UART_PRINT_STR(str);
  sprintf(str,"npad: %d\n\r",remoteInitData.npad);
  UART_PRINT_STR(str);
  sprintf(str,"padc: %d\n\r",remoteInitData.padc);
  UART_PRINT_STR(str);
  sprintf(str,"eol: %d\n\r",remoteInitData.eol);
  UART_PRINT_STR(str);
  sprintf(str,"qctl: %d\n\r",remoteInitData.qctl);
  UART_PRINT_STR(str);
  
  
  UART_PRINT_STR("\n\r");
  
  
}

void TestDac()
{
  AudioPlayFlashFile(0,MUSIC_FILE_SIZE);  
}

void CreateFile(){
  SpiFlashFileOpen("bs.bin",MUSIC_FILE_SIZE);
  SpiFlashSaveFileTable();
}

void PrintFileTable(){
  SpiPrintFileTable();
}


uint16_t dbgNumBytesWritten = 0;
void PrintDebug()
{
  char str[30];
  sprintf(str," Written: %d\n\r",dbgNumBytesWritten);
  UART_PRINT_STR(str);
  
  for(uint8_t i=0; i< dbgNumBytesWritten; i++){
    
    sprintf(str," %d - 0x%x\n\r",i,dataBuffer[i]);
    UART_PRINT_STR(str);
    
    
  }
}


void KermitFileDownload(){
  
  //char str[30];
  while( PicKermitRxFile() == PIC_KERMIT_BUFFER_FULL ){
    
    //sprintf(str," Bfull: %d\n\r",bufferCapacity-remBufferCapacity);
    //UART_PRINT_STR(str);

    dbgNumBytesWritten += DATA_BUFF_SIZE-remBufferCapacity;
    
    SpiFlashWrite(0,DATA_BUFF_SIZE-remBufferCapacity,dataBuffer);
    remBufferCapacity = DATA_BUFF_SIZE;
    fileWritePtr = dataBuffer;
    
    
   // return;
  }
  
  //sprintf(str," Done: %d\n\r",bufferCapacity-remBufferCapacity);
  //UART_PRINT_STR(str);

  SpiFlashWrite(0,DATA_BUFF_SIZE-remBufferCapacity,dataBuffer);
  
  SpiFlashSaveFileTable();

}

void ReadDownloadedFile(){
  char str[30];
  uint8_t val;
  uint16_t addr=0;
    
  SpiFlashRewindRead(0);
  
  while( SpiFlashRead(0,1,&val) == 0  && addr < 200){
    sprintf(str,"0x%x- 0x%x\n\r",addr++,val);
    UART_PRINT_STR(str);
  }
}



void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    //TMR1_SetInterruptHandler(AudioTimerCallback);

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
   // INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    DAC1_Initialize();
    EUSART_Initialize();
    SPI_Initialize();
    SpiFlashInit();
    TMR2_Initialize();
        
    
    //AudioPlayFlashFile(0,MUSIC_FILE_SIZE);  

    
    
    remBufferCapacity = DATA_BUFF_SIZE;
    fileWritePtr = dataBuffer;
    for(uint8_t i=0; i<remBufferCapacity; i++)
      dataBuffer[i] = 0;
    
    uint8_t uartMenuSize = 8;
    uint8_t help0[] = "Print kermit init results.";
    uint8_t help1[] = "Kermit file download.";
    uint8_t help2[] = "Print downloaded file bytes.";
    uint8_t help3[] = "Blank FLASH filesystem.";
    uint8_t help4[] = "Test DAC.";
    uint8_t help5[] = "Create file.";
    uint8_t help6[] = "Print file table.";
    uint8_t help7[] = "Debug.";

    
    UartCommand uartMenu[] = {
        { 0, help0, &PrintKermitInitResults },
        { 1, help1, &KermitFileDownload },
        { 2, help2, &ReadDownloadedFile },
        { 3, help3, &BlankFlashFs },
        { 4, help4, &TestDac },
        { 5, help5, &CreateFile },
        { 6, help6, &PrintFileTable },
        { 7, help7, &PrintDebug }
    };
    
    
    UART_PRINT_STR("   \r\n");
    UART_PRINT_STR("   \r\n");
    UART_PRINT_STR("   \r\n");
    
    while (1)
    {
        UART_MENU_PROMPT;
        UART_READ_MENU_COMMAND;        
    }
}
/**
 End of File
*/