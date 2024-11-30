/* 
 * File:   rtd-sensor.h
 * Author: gmessier
 * Routines for resistive temperature device sensors.
 */

#ifndef RTD_SENSOR_H
#define	RTD_SENSOR_H

#include <avr/io.h>

// Non-inverting amplifier for RTD voltage. 
// - Voltage gain Av=16
// - OP0INP Input: PD1, OP0OUT Output: PD2
// - PD2 must be unused since it acts as ADC input.
#define RTD_SENSOR_OPAMP0_CONFIG { \
  OPAMP.CTRLA = 0b00000001; /* Enable opamps. */ \
  OPAMP.OP0CTRLA = 0b00000101; /* Normal output, always on. */ \
  OPAMP.OP0RESMUX = 0b11110101; /* R1=1R,R2=15R, MUXBOT:GND, MUXTOP:OUT */ \
  OPAMP.OP0INMUX = 0b00010000; /* MUXNEG:WIP, MUXPOS:INP */ \
}

// ADC Configuration
#define RTD_SENSOR_ADC_CONFIG { \
  SREG = 0b10000000; /* Enable global interrupts. */ \
  VREF.ADC0REF = 0b10000101; /* Set the ADC reference level to VDD. */ \
  ADC0.INTCTRL = 0b00000001; /* Enable the ADC interrupt. */ \
  ADC0.MUXPOS = 0x02; /* PD2 (AIN2) is input. */ \
  ADC0.CTRLC = 0x00; /* Minimum clock divide. */ \
  ADC0.CTRLA = 0b00000111; /* 10 bit, single ended, free running. */ \
  ADC0.CTRLB = 0b00000101; /* 64 sample accumulation. */ \
  ADC0.COMMAND = 0x01; /* Start conversion. */ \
}

  
  
// Call before reading ADC0.RES.
//   while( (OPAMP.OP0STATUS & 0b00000001) == 0 ) ; /* Wait for opamp to settle. */ 
#define RTD_SENSOR_READ_ENABLE { \
  ADC0.CTRLA |= 0b00000001;  /* Enable ADC. */ \
  ADC0.INTFLAGS |= 0b00000001; /* Clear ADC interrupt. */ \
  while( (ADC0.INTFLAGS & 0b00000001) == 0 ) ;  /* Wait for reading. */ \
  ADC0.INTFLAGS |= 0b00000001; /* Clear ADC interrupt. */ \
}

// Call after reading ADC0.RES.
#define RTD_SENSOR_READ_DISABLE { \
  ADC0.CTRLA &= 0b11111110;  /* Disable ADC. */ \
}
//  OPAMP.CTRLA &= 0b11111110; /* Disable opamps. */ 

#endif
