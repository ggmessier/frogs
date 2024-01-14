/*
 * File:   char-lcd-test.c
 * Author: gmessier
 *
 * Created on December 23, 2023, 3:36 PM
 */


#include <avr/io.h>
#include "../lib-Avr128DbX.X/lib-avr.h"


int main(void) {
  
  
  // Use the internal 32.768kHz oscillator.
  CCP = 0xd8;
  CLKCTRL.MCLKCTRLA = 0x01;
  while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
    ;
  }
  
  
LCD_PINS_PD5_TO_7_PA4_TO_7;
LCD_STARTUP_DELAY_32K_OSC;
LCD_INIT_4PIN_2LINE;
    
while(1){
  LCD_PRINT_TOP("I love..");
  LCD_PRINT_BOTTOM("DOGS!");

  // Use this delay just as a convenient way to pause the screen.
  LCD_STARTUP_DELAY_32K_OSC;
  LCD_STARTUP_DELAY_32K_OSC;

  LCD_PRINT_TOP("Of");
  LCD_PRINT_BOTTOM("course:)");

  // Use this delay just as a convenient way to pause the screen.
  LCD_STARTUP_DELAY_32K_OSC;
  LCD_STARTUP_DELAY_32K_OSC;
  
}
  

  
}
