/*
 * File:   spi-ext-flash.c
 * Author: gmessier
 *
 * Created on December 28, 2022, 1:59 PM
 */


#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include "../lib-Avr128DbX.X/lib-avr.h"


void TestSpi(){
    
//    SpiFlashWriteEnable();
    
//    USART_PRINT_STR("done!\n\r");
    
    
    SELECT_FLASH_CHIP;
    SPI_TX_BYTE(0x06);
    DESELECT_FLASH_CHIP;

    char str[30];
    sprintf(str,"\n\r0x%x\n\r",SpiFlashReadStatus(1));
    USART_PRINT_STR(str);

    sprintf(str,"0x%x\n\r",SpiFlashReadStatus(1));
    USART_PRINT_STR(str);

    sprintf(str,"0x%x\n\r",SpiFlashReadStatus(1));
    USART_PRINT_STR(str);

    DESELECT_FLASH_CHIP;
            
}


int main(void) {
    
    // Use the internal 32.768kHz oscillator.
    CCP = 0xd8;
    CLKCTRL.MCLKCTRLA = 0x01;
    while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
        ;
    }
    
    // Configure PA3 as the FLASH SPI select output.
    PORTA.DIRSET = 0b00001000;   
    DESELECT_FLASH_CHIP;
    
    
    // Enable SPI, master mode, double clock mode.
    SPI0.CTRLA = 0b00110001;
    
    // Non-buffered (normal) mode,
    // disable slave select line, SPI Mode 0.
    SPI0.CTRLB = 0b01000100;
    
    // Enable normal mode interrupts.
    SPI0.INTCTRL = 0b00000001;
 


    // -- SPI Configuration --
    // SCLK: PA6
    // MISO (Master In, Slave Out): PA5
    // MOSI (Master Out, Slave In): PA4
    PORTA.DIRSET = 0b01010000;
    
    SELECT_FLASH_CHIP;
    SPI0.DATA = 0x66; 
    
    while( (SPI0.INTFLAGS & 0x80) == 0){ ; }

    DESELECT_FLASH_CHIP;

    
    // -- UART Configuration --
    // 2400 baud
    USART0.BAUD = 0x006d;
    
    // Enable Data Register Empty Interrupt Flag (DREIF).
    USART0.CTRLA = 0b00100000;
        
    // Asynchronous, 8N1 (8 data, no parity, 1 stop)
    USART0.CTRLC = 0b00000011;
    
    // Configure PA0 as an output for TxD.
    PORTA.DIRSET = 0b00000001;

    // Enable transmitter, receiver and double speed mode.
    USART0.CTRLB = 0b11000010;
        
    uint8_t uartMenuSize = 1;
    uint8_t help0[] = "Test SPI.";
    
    UartCommand uartMenu[] = {
        { 0, help0, &TestSpi }
    };
    
    while (1) {
        USART_MENU_PROMPT;
        USART_READ_MENU_COMMAND;        
    }
    
    
}
