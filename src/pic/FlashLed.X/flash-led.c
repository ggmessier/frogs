/* 
 * File:   flash-led.c
 * Author: gmessier
 *
 * Created on October 17, 2020, 2:50 PM
 */

// In order to use device register names like PORTB you must include <xc.h> 
#include <xc.h>

// The config registers are set using the #pragma config <FIELD> = <VALUE>
// - The keywords are from the datasheet (Section 4.0).
// - <FIELD> equals the register field names (ie. FCMEN, FOSC, etc.)
// - <VALUE> can be a number or the value names given in the datasheet
//   (ie. ON, OFF, NSLEEP, etc.)

// This instruction sets the watchdog timer to user-controlled, 
// so the device will not automatically reset itself.
#pragma config WDTE = SWDTEN 

// This is necessary for the builtin function __delay_ms() to work.
// It tells the compiler what frequency the device will be running at.
// Note: We're not actually setting the frequency here.
#define _XTAL_FREQ 500000



int main(int argc, char** argv) {

    // Register bits can be accessed using the <REG>bits.<FIELD> 
    // datastructure defined in the device specific include file
    // located in the compiler pic/include/proc directory.  
    // - <REG> matches the name of the register in the datasheet.
    // - <FIELD> matches the field names in the register datasheet table.

    // Set the internal oscillator to 500kHz.
    OSCCONbits.IRCF = 0x07;

    // Registers can also be accessed by writing an 8 bit number to the 
    // whole register.
    
    // Set all PortB pins to output.
    TRISB = 0x00;
    
    // Use an open drain output to sink current on the pin driving the LED.
    // - In theory, the pin can source sufficient current to drive the LED
    //   but it seems to be affected by whether other pins are also driving
    //   something.
    ODCONBbits.ODB5 = 1;
    
    
    while(1){
        
        // Turn LED on.  
        PORTBbits.RB5 = 0;
        
        // Wait 1 sec.
        __delay_ms(1000);
        
        // Turn LED off.  
        PORTBbits.RB5 = 1;
        
        // Wait 1 sec.
        __delay_ms(1000);

    }
    
    return (EXIT_SUCCESS);
}

