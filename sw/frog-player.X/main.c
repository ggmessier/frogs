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
#include <libfrog-serial.h>


/*
                         Main application
 */


extern uint8_t bufferCheckOverride;
extern ReqInitData remoteInitData;
extern uint8_t *fileWritePtr;
extern uint16_t bufferCapacity;
uint8_t dataBuffer[40];

void PrintBufferContentsText(){
  for(uint8_t i=0; i<40;i++)
    EUSART_Write(dataBuffer[i]);
  
  EUSART_Write('\n');
  EUSART_Write('\r');
}

void PrintBufferContentsBinary(){
  for(uint8_t i=0; i<40;i++)
    EUSART_Write(dataBuffer[i]+32);
  
  EUSART_Write('\n');
  EUSART_Write('\r');
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


void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    
    //EUSART_Initialize();
        
    
    bufferCapacity = 40;
    bufferCheckOverride = 1;
    fileWritePtr = dataBuffer;
    for(uint8_t i=0; i<40; i++)
      dataBuffer[i] = 0;
    
    uint8_t uartMenuSize = 4;
    uint8_t help0[] = "Kermit rx.";
    uint8_t help1[] = "Print kermit init results.";
    uint8_t help2[] = "Print file buffer contents (text file).";
    uint8_t help3[] = "Print file buffer contents (binary file).";

    
    UartCommand uartMenu[] = {
        { 0, help0, &PicKermitRxFile },
        { 1, help1, &PrintKermitInitResults },
        { 2, help2, &PrintBufferContentsText },
        { 3, help3, &PrintBufferContentsBinary }
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