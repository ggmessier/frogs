/*
 * File:   newavr-main.c
 * Author: gmessier
 *
 * Created on February 13, 2021, 3:28 PM
 */


#include <avr/io.h>

int main(void) {
    
    
    CLKCTRL.OSCHFCTRLA |= 0x09;
    PORTA.DIRSET |= 0x20; // PA6 is an output.
            
    /* Replace with your application code */
    while (1) {
        PORTA.OUT |= 0x20;
        PORTA.OUT &= 0xdf;       
    }
}
