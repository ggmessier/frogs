/*
 * File:   tone.c
 * Author: gmessier
 *
 * Created on October 18, 2020, 11:37 AM
 */


#include <xc.h>
#include <stdlib.h>

// This instruction sets the watchdog timer to user-controlled, 
// so the device will not automatically reset itself.
#pragma config WDTE = SWDTEN 


// Declare an array of 500*(1+sin(2*pi*x/100)) for x = 0:99
const short sineArray[] = 
        {500, 531, 563, 594, 624, 655, 684, 713, 741, 768,
         794, 819, 842, 864, 885, 905, 922, 938, 952, 965, 
         976, 984, 991, 996, 999,1000, 999, 996, 991, 984,
         976, 965, 952, 938, 922, 905, 885, 864, 842, 819,
         794, 768, 741, 713, 684, 655, 624, 594, 563, 531,
         500, 469, 437, 406, 376, 345, 316, 287, 259, 232,
         206, 181, 158, 136, 115,  95,  78,  62,  48,  35,
          24,  16,   9,   4,   1,   0,   1,   4,   9,  16, 
          24,  35,  48,  62,  78,  95, 115, 136, 158, 181, 
         206, 232, 259, 287, 316, 345, 376, 406, 437, 469};


void main(void)
{
    // ------ General Configuration -------
    // Set the system clock speed to 16MHz.
    OSCCONbits.SPLLEN = 1;
    OSCCONbits.IRCF = 0x0f;
    
    //Wait for the clock switch until the "HF Oscillator Ready" flag is true.
    while(OSCSTATbits.HFIOFR == 0);
    
    // Configure pin RC4 as a digital input, to read the button.
    TRISCbits.TRISC4 = 1;
    ANSELCbits.ANSC4 = 0;
        

    // -------- DAC Configuration ---------
    // Enable DAC1.  Overrides digital I/O settings for output pin.
    DAC1CON0bits.EN = 1;  

    // Right justify the 10 bit value.
    DAC1CON0bits.FM = 0;
    
    // Enable RA2 as the DAC output.
    DAC1CON0bits.OE1 = 1;
    DAC1CON0bits.OE2 = 0;
    
    
    // Use VDD and VSS as our voltage range.
    DAC1CON0bits.PSS = 0x00;
    DAC1CON0bits.NSS = 0x00;

    
    // -------- Timer Configuration ---------
    // Ftimer = Fosc/4 * 1/(CKPS*OUTPS*TPR)
    // Ftimer = Fnote/100 since we have 100 samples per sine
    // Fnote = [ 440/A4, 493.88/B4, 523.25/C5 ]
    
    T2CLKCONbits.CS = 0x01;  // Timer clock is Fosc/4 = 4 MHz
    T2CONbits.ON = 1;  // Turn Timer 2 on.
    
    // No need for pre or post scaling.
    T2CONbits.CKPS = 0x00;
    T2CONbits.OUTPS = 0x00;

    unsigned char noteT2PR[3];  
    noteT2PR[0] = 91;
    noteT2PR[1] = 81;
    noteT2PR[2] = 76;
    
    T2PR = noteT2PR[0];
    
    PIR1bits.TMR2IF = 0;

    unsigned char noteInd = 0;
    unsigned char edgeDetect = 0;
    unsigned char arrayInd = 0;
    
    while(1){

        short x = sineArray[arrayInd];
        arrayInd++;
        if(arrayInd == 100)
            arrayInd = 0;
        DAC1REFL = (unsigned char)(0xff & x);
        DAC1REFH = (unsigned char)(0xff & (x >> 8));
        DACLDbits.DAC1LD = 1;

        while(!PIR1bits.TMR2IF);  
        PIR1bits.TMR2IF = 0;  
                
        if( PORTCbits.RC4 == 0 && !edgeDetect){
            noteInd++;
            if( noteInd == 3)
                noteInd = 0;
            T2PR = noteT2PR[noteInd];
            edgeDetect = 1;
        }
        else if ( PORTCbits.RC4 == 1 )
            edgeDetect = 0;
       
    }
    
    
        
    return;
}
