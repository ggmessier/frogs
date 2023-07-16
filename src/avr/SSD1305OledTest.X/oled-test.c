/*
 * File:   oled-test.c
 * Author: gmessier
 *
 * Created on April 8, 2023, 8:43 AM
 */


#include <avr/io.h>

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
  
  
  
  /* Replace with your application code */
  while (1) {
  }
}
