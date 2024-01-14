/* 
 * File:   lcd-display.h
 * Author: gmessier
 * Macros character based LCD displays.
 * Created on January 6, 2024
 */

#ifndef LCD_DISPLAY_H
#define	LCD_DISPLAY_H

#include <avr/io.h>

// Control Routines for a 2 Line LCD Display (ST7066U Driver Chip)

/* Notes:
 * - Tested only for the 32.786kHz internal oscillator.  May need to add
 *   control line and/or E line delays for faster clocks.
 * - LCD tends to be put in a weird state when loading new code into the AVR.
 *   A clean screen init usually requires a reset/power cycle.
 * - Careful of having peripherals connected (ie. usb to uart) that partially
 *   power the chip and prevent a clean reset.
 */


/*
 * LCD Pin Mapping:
 *  PD7: E
 *  PD6: RW
 *  PD5: RS
 *  PA4-PA7: DB4-DB7
 */
#define LCD_PINS_PD5_TO_7_PA4_TO_7 { \
  PORTD.DIRSET = 0b11100000; \
  PORTA.DIRSET = 0b11110000; \
}

/*
 * Approximate 1 sec startup delay assuming
 * 32.768k internal oscillator and 5 instructions
 * per loop iteration.
 */
#define LCD_STARTUP_DELAY_32K_OSC { \
  for(uint16_t cc=0; cc < 6553; cc++ ) ; \
}

// Control Pin Set/Clear Routines
#define LCD_SET_E { PORTD.OUT |= 0b10000000; }
#define LCD_CLR_E { PORTD.OUT &= 0b01111111; }
#define LCD_SET_RW { PORTD.OUT |= 0b01000000; }
#define LCD_CLR_RW { PORTD.OUT &= 0b10111111; }
#define LCD_SET_RS { PORTD.OUT |= 0b00100000; }
#define LCD_CLR_RS { PORTD.OUT &= 0b11011111; }


/*
 * Block while LCD is busy.
 * 1. Set DB7 pin to input.
 * 2. Set control lines.
 * 3. Loop while busy.  DB7 read while E is high.  Need a second E cycle
 *    to clear the AC information from the input pin due to being in 4 bit mode.
 * 4. Set DB7 back to an output line.
 */
#define LCD_BLK { \
    PORTA.DIRCLR = 0b10000000; \
    LCD_CLR_RS; \
    LCD_SET_RW; \
    uint8_t rd; \
    do{ \
      LCD_SET_E; \
      rd = PORTA.IN & 0b10000000; \
      LCD_CLR_E; \
      LCD_SET_E; \
      LCD_CLR_E; \
    }while(rd); \
    PORTA.DIRSET = 0b10000000; \
}

/*
 * Write data to the LCD in 4 bit transfer mode.  Assumes control bits
 * already set to enable either a character or control command.
 */  
#define LCD_WRITE(d) { \
    LCD_SET_E; \
    PORTA.OUT &= 0x0f; \
    PORTA.OUT |= 0xf0 & (d); \
    LCD_CLR_E; \
    LCD_SET_E; \
    PORTA.OUT &= 0x0f; \
    PORTA.OUT |= 0xf0 & ( d << 4 ); \
    LCD_CLR_E; \
}

/*
 * Write control command to LCD in 4 bit transfer mode.
 */
#define LCD_CMD(d) { \
    LCD_BLK; \
    LCD_CLR_RW; \
    LCD_CLR_RS; \
    LCD_WRITE(d); \
}
/*
 * Write character to LCD in 4 bit transfer mode.
 */
#define LCD_CHAR(d) { \
    LCD_BLK; \
    LCD_CLR_RW; \
    LCD_SET_RS; \
    LCD_WRITE(d); \
}

/*
 * Initialize LCD screen.
 * 1. Set to 8 bit mode (0x30).  Do this twice in case the LCD initializes
 *    to 4 bit mode and expects two transfers.
 * 2. Set to 4 bit mode (0x20).  Single transfer since we're in 8 bit mode.
 * 3. Set to 4 bit, 2 line mode (0x28).
 * 4. Turn on display with no cursor (0x0c).
 * 5. Clear display (0x01).
 */
#define LCD_INIT_4PIN_2LINE { \
  LCD_CLR_RW; \
  LCD_CLR_RS; \
  LCD_SET_E; \
  PORTA.OUT &= 0b00001111; \
  PORTA.OUT |= 0b00110000; \
  LCD_CLR_E; \
  LCD_SET_E; \
  LCD_CLR_E; \
  LCD_SET_E; \
  PORTA.OUT &= 0b00001111; \
  PORTA.OUT |= 0b00100000; \
  LCD_CLR_E; \
  LCD_CMD(0x28); \
  LCD_CMD(0x0c); \
  LCD_CMD(0x01); \
}


/*
 * Print a string to the top line.
 */
#define LCD_PRINT_TOP(x) { \
  LCD_CMD(0x80); \
  uint8_t lcd_cc = 0; \
  while( (lcd_cc<8) && ((x)[lcd_cc]!=0) ){ \
    LCD_CHAR((x)[lcd_cc]); \
    lcd_cc++; \
  } \
  while( lcd_cc<8 ){ \
    LCD_CHAR(' '); \
    lcd_cc++; \
  } \
}

/*
 * Print a string to the bottom line.
 */
#define LCD_PRINT_BOTTOM(x) { \
  LCD_CMD(0xc0); \
  uint8_t lcd_cc = 0; \
  while( (lcd_cc<8) && ((x)[lcd_cc]!=0) ){ \
    LCD_CHAR((x)[lcd_cc]); \
    lcd_cc++; \
  } \
  while( lcd_cc<8 ){ \
    LCD_CHAR(' '); \
    lcd_cc++; \
  } \
}


#endif
