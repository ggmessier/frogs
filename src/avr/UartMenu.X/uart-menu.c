/*
 * File:   uart-menu.c
 * Author: gmessier
 *
 * Created on December 22, 2022, 11:29 AM
 */


#include <avr/io.h>
#include "../lib-Avr128DbX.X/lib-avr.h"


void PrintFrog(){
    USART_PRINT_STR("\n\rfrog!\n\r");
}

void PrintDog(){
    USART_PRINT_STR("\n\rdog!\n\r");
}

int main(void) {
    
    // Use the internal 32.768kHz oscillator.
    CCP = 0xd8;
    CLKCTRL.MCLKCTRLA = 0x01;
    while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
        ;
    }
    
    // PA0 is TxD, PA1 is RxD
    
    // Set BAUD rate.
    // - Peripheral clock, f_CLK_PER = 32.768 kHz (CLK_MAIN prescaler disabled)
    // - Desired baud rate, f_BAUD = 2400
    // - Samples per Bit, S = 8 (async, double speed mode)
    // - BAUD = 64*f_CLK_PER/(S*f_BAUD) = 109.23 -> 109 = 0x6d
    // - f_BAUD_Act = 64*f_CLK_PER/(S*BAUD) = 2405
    // - RelErr = 2405/2400 = 100.2% (well within specs on pg. 382)
    USART0.BAUD = 0x006d;
    
    // Enable Data Register Empty Interrupt Flag (DREIF).
    USART0.CTRLA = 0b00100000;
        
    // Asynchronous, 8N1 (8 data, no parity, 1 stop)
    USART0.CTRLC = 0b00000011;
    
    // Configure PA0 as an output for TxD.
    PORTA.DIRSET = 0b00000001;

    // Enable transmitter, receiver and double speed mode.
    USART0.CTRLB = 0b11000010;
    
    uint8_t uartMenuSize = 2;
    uint8_t help0[] = "Print frog?";
    uint8_t help1[] = "Print dog?";
    
    UartCommand uartMenu[] = {
        { 0, help0, &PrintFrog },
        { 1, help1, &PrintDog }
    };
    
    while (1) {
        USART_MENU_PROMPT;
        USART_READ_MENU_COMMAND;        
    }
    
}
