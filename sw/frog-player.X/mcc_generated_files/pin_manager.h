/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F1778
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.15	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RA0 aliases
#define IO_RA0_TRIS                 TRISAbits.TRISA0
#define IO_RA0_LAT                  LATAbits.LATA0
#define IO_RA0_PORT                 PORTAbits.RA0
#define IO_RA0_WPU                  WPUAbits.WPUA0
#define IO_RA0_OD                   ODCONAbits.ODA0
#define IO_RA0_ANS                  ANSELAbits.ANSA0
#define IO_RA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define IO_RA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define IO_RA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define IO_RA0_GetValue()           PORTAbits.RA0
#define IO_RA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define IO_RA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define IO_RA0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define IO_RA0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define IO_RA0_SetPushPull()        do { ODCONAbits.ODA0 = 0; } while(0)
#define IO_RA0_SetOpenDrain()       do { ODCONAbits.ODA0 = 1; } while(0)
#define IO_RA0_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define IO_RA0_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set RA1 procedures
#define RA1_SetHigh()               do { LATAbits.LATA1 = 1; } while(0)
#define RA1_SetLow()                do { LATAbits.LATA1 = 0; } while(0)
#define RA1_Toggle()                do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define RA1_GetValue()              PORTAbits.RA1
#define RA1_SetDigitalInput()       do { TRISAbits.TRISA1 = 1; } while(0)
#define RA1_SetDigitalOutput()      do { TRISAbits.TRISA1 = 0; } while(0)
#define RA1_SetPullup()             do { WPUAbits.WPUA1 = 1; } while(0)
#define RA1_ResetPullup()           do { WPUAbits.WPUA1 = 0; } while(0)
#define RA1_SetAnalogMode()         do { ANSELAbits.ANSA1 = 1; } while(0)
#define RA1_SetDigitalMode()        do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set RA2 procedures
#define RA2_SetHigh()               do { LATAbits.LATA2 = 1; } while(0)
#define RA2_SetLow()                do { LATAbits.LATA2 = 0; } while(0)
#define RA2_Toggle()                do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RA2_GetValue()              PORTAbits.RA2
#define RA2_SetDigitalInput()       do { TRISAbits.TRISA2 = 1; } while(0)
#define RA2_SetDigitalOutput()      do { TRISAbits.TRISA2 = 0; } while(0)
#define RA2_SetPullup()             do { WPUAbits.WPUA2 = 1; } while(0)
#define RA2_ResetPullup()           do { WPUAbits.WPUA2 = 0; } while(0)
#define RA2_SetAnalogMode()         do { ANSELAbits.ANSA2 = 1; } while(0)
#define RA2_SetDigitalMode()        do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RA5 procedures
#define RA5_SetHigh()               do { LATAbits.LATA5 = 1; } while(0)
#define RA5_SetLow()                do { LATAbits.LATA5 = 0; } while(0)
#define RA5_Toggle()                do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define RA5_GetValue()              PORTAbits.RA5
#define RA5_SetDigitalInput()       do { TRISAbits.TRISA5 = 1; } while(0)
#define RA5_SetDigitalOutput()      do { TRISAbits.TRISA5 = 0; } while(0)
#define RA5_SetPullup()             do { WPUAbits.WPUA5 = 1; } while(0)
#define RA5_ResetPullup()           do { WPUAbits.WPUA5 = 0; } while(0)
#define RA5_SetAnalogMode()         do { ANSELAbits.ANSA5 = 1; } while(0)
#define RA5_SetDigitalMode()        do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()               do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()                do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()                do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()       do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()      do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()               do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()                do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()                do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()       do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()      do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)

// get/set SCK aliases
#define SCK_TRIS                 TRISCbits.TRISC2
#define SCK_LAT                  LATCbits.LATC2
#define SCK_PORT                 PORTCbits.RC2
#define SCK_WPU                  WPUCbits.WPUC2
#define SCK_OD                   ODCONCbits.ODC2
#define SCK_ANS                  ANSELCbits.ANSC2
#define SCK_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define SCK_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define SCK_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define SCK_GetValue()           PORTCbits.RC2
#define SCK_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define SCK_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define SCK_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define SCK_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define SCK_SetPushPull()        do { ODCONCbits.ODC2 = 0; } while(0)
#define SCK_SetOpenDrain()       do { ODCONCbits.ODC2 = 1; } while(0)
#define SCK_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define SCK_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set SDO aliases
#define SDO_TRIS                 TRISCbits.TRISC3
#define SDO_LAT                  LATCbits.LATC3
#define SDO_PORT                 PORTCbits.RC3
#define SDO_WPU                  WPUCbits.WPUC3
#define SDO_OD                   ODCONCbits.ODC3
#define SDO_ANS                  ANSELCbits.ANSC3
#define SDO_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SDO_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SDO_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SDO_GetValue()           PORTCbits.RC3
#define SDO_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SDO_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define SDO_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define SDO_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define SDO_SetPushPull()        do { ODCONCbits.ODC3 = 0; } while(0)
#define SDO_SetOpenDrain()       do { ODCONCbits.ODC3 = 1; } while(0)
#define SDO_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define SDO_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set SDI aliases
#define SDI_TRIS                 TRISCbits.TRISC4
#define SDI_LAT                  LATCbits.LATC4
#define SDI_PORT                 PORTCbits.RC4
#define SDI_WPU                  WPUCbits.WPUC4
#define SDI_OD                   ODCONCbits.ODC4
#define SDI_ANS                  ANSELCbits.ANSC4
#define SDI_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SDI_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SDI_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SDI_GetValue()           PORTCbits.RC4
#define SDI_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SDI_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define SDI_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define SDI_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define SDI_SetPushPull()        do { ODCONCbits.ODC4 = 0; } while(0)
#define SDI_SetOpenDrain()       do { ODCONCbits.ODC4 = 1; } while(0)
#define SDI_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define SDI_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set IO_RC7 aliases
#define IO_RC7_TRIS                 TRISCbits.TRISC7
#define IO_RC7_LAT                  LATCbits.LATC7
#define IO_RC7_PORT                 PORTCbits.RC7
#define IO_RC7_WPU                  WPUCbits.WPUC7
#define IO_RC7_OD                   ODCONCbits.ODC7
#define IO_RC7_ANS                  ANSELCbits.ANSC7
#define IO_RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define IO_RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define IO_RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define IO_RC7_GetValue()           PORTCbits.RC7
#define IO_RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define IO_RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define IO_RC7_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define IO_RC7_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define IO_RC7_SetPushPull()        do { ODCONCbits.ODC7 = 0; } while(0)
#define IO_RC7_SetOpenDrain()       do { ODCONCbits.ODC7 = 1; } while(0)
#define IO_RC7_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define IO_RC7_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/