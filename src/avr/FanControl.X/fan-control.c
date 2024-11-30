/*
 * File:   fan-control.c
 * Author: gmessier
 *
 * Version 1.0:
 * - Initial fully functional controller software.
 */

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/cpufunc.h>
#include "../lib-Avr128DbX.X/lib-avr.h"


#define NUM_FANS 2  
#define MAX_NUM_FANS 3

// TCBn Pointers
TCB_t* tCB[MAX_NUM_FANS] = { (TCB_t*)0x0b00, (TCB_t*)0x0b10, (TCB_t*)0x0b20 };  

/*
 * --- Timers ---
 */

#define MAX_FAN_STEPS 32

typedef struct {
  
  uint8_t nSteps;  // Number of piecewise steps in fan speed profile.
  uint8_t speedStep[MAX_FAN_STEPS]; 
  uint8_t timeStep[MAX_FAN_STEPS];
  
  uint8_t time;
  uint8_t step;
} fan_t;

fan_t fan;

#define RTC_TICK 2 // seconds


char str[50];

void InitFans()
{
  fan.time = 0;
  fan.step = 0;
  for(uint8_t n=0; n < NUM_FANS; n++){
    tCB[n]->CCMPH = fan.speedStep[0];
  }  
  return;
}

void UpdateFans()
{
  fan.time += RTC_TICK;
  
  sprintf(str,"%d\n\r",fan.time);
  USART_PRINT_STR(str);
  
  if( fan.timeStep[fan.step] <= fan.time ){
    fan.step++;
    if(fan.step == fan.nSteps){
      InitFans();
    }
    else{
      for(uint8_t n=0; n < NUM_FANS; n++){
        tCB[n]->CCMPH = fan.speedStep[fan.step];
      }  
    }
  }
  return;
}



void __interrupt(RTC_PIT_vect_num) RTCIsr(void)
{
  UpdateFans();  
//  USART_PRINT_STR("tick\n\r");
  RTC.PITINTFLAGS = 0x01; // Clear interrupt.
}


/*
 * --- Fan Control Menu Functions
 */


void SetFanSpeed()
{
  USART_PRINT_STR("\n\r\n\r>> Set fan speed. <<\n\r");
  
  USART_PRINT_STR(" Enter fan number (3 for all): ");
  USART_READ_STR(str,50);
  uint8_t fanNum = atoi(str);
  
  USART_PRINT_STR(" Enter speed (1-10): ");
  USART_READ_STR(str,50);
  uint8_t newSpeed = atoi(str);
 
  if( fanNum < 3 ){
    tCB[fanNum]->CCMPH = newSpeed;
  }
  else{
    for(uint8_t n=0; n < NUM_FANS; n++){
      tCB[n]->CCMPH = newSpeed;
    }  
  }
}


int main(void) {
  
  // Use the internal 32.768kHz oscillator.
  CCP = 0xd8;
  CLKCTRL.MCLKCTRLA = 0x01;
  while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
      ;
  }
  
  SREG = 0b10000000;
  
  // ---------- Real Time Clock (RTC) Timer ----------
  
  RTC.CLKSEL = 0x01;  // 1.024kHz internal timer clock.
  RTC.PITINTCTRL = 0x01; // Enable periodic interrupt.
  RTC.PITCTRLA = 0b01010001;  // Interrupt every 2048 cycles (2sec).
  
  
  
  // ------------ Timer B ----------------
  
  // Configure timer waveform output (WO) pins as outputs.
    
  for(uint8_t i=0; i < NUM_FANS; i++){
    // Configure TCB timers.
    tCB[i]->CTRLA = 0b00000001;  // CLK_PER clock source, enable.
    tCB[i]->CTRLB = 0b00010111;  // Enable WO, PWM8 mode.
  
    // Configure timer PWM output pins.
    if(i==0){
        PORTA.DIRSET = 0b00000100;  // TCB0 - PA2 (Fan1)
    }
    else if(i==1){
        PORTA.DIRSET = 0b00001000;  // TCB0 - PA3 (Fan2)
    }
    else if(i==1){
        PORTC.DIRSET = 0b00000001;  // TCB2 - PC0 (Fan3)
    }
    
    tCB[i]->CCMPL = 10;  // 3.2768 kHz PWM frequency.
    tCB[i]->CCMPH = 1;   // Default to 10% duty cycle.
  }
    
  // ------------ UART -------------
  // 2400 8N1
  USART0.BAUD = 0x006d;
  USART0.CTRLC = 0b00000011;
  PORTA.DIRSET = 0b00000001;
  USART0.CTRLB = 0b11000010;

  uint8_t uartMenuSize = 1;
  uint8_t help0[] = "Set fan speed.";
  
  UartCommand uartMenu[] = {
    { 0, help0, &SetFanSpeed }
  };

  fan.nSteps = 2;
  fan.speedStep[0] = 8;
  fan.timeStep[0] = 10;
  fan.speedStep[1] = 4;
  fan.timeStep[1] = 50;

  InitFans();
  
  while (1) {
      USART_MENU_PROMPT;
      USART_READ_MENU_COMMAND;        
  }
  
}
