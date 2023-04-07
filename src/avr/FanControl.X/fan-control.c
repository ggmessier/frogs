/*
 * File:   fan-control.c
 * Author: gmessier
 *
 * Created on March 4, 2023, 2:36 PM
 */

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/cpufunc.h>
#include "../lib-Avr128DbX.X/lib-avr.h"



/*
 * Fan Experiment Observations:
 * - Duty cycle can range down to 87 if moving in increments of 1.
 * - The fan must start at full speed.
 * - Sudden decreases in speed will cause the fan to go unstable and for
 *   its speed to oscillate.
 */

#define NUM_FANS 3

/*
 * --- Timers ---
 */

#define TCB_PERIOD 100 // Number of TCB timer period ticks.
#define F_TCA 32  // fTCA

// TCBn Pointers
TCB_t* tCB[NUM_FANS] = { (TCB_t*)0x0b00, (TCB_t*)0x0b10, (TCB_t*)0x0b20 };  

void SetSpeed(uint8_t nTimer, uint8_t speed)
{
  /* TCB is used to generate a variable fan control voltage using
   * a variable duty cycle waveform that is RC low pass filtered.
   */
  
  // Speed translates directly to duty cycle since TCB_PERIOD=100.
  tCB[nTimer]->CCMPL = TCB_PERIOD-1;
  tCB[nTimer]->CCMPH = speed;
}

/*
 * --- Fan Control Settings ---
 */

// Speed values correspond to TCB duty cycle settings that are
// RC filtered to produce a variable voltage level.
uint8_t defaultSpeedHi[NUM_FANS] = { 87, 87, 87 };
uint8_t defaultSpeedLo[NUM_FANS] = { 84, 84, 85 };

#define DEFAULT_SPEED_START 99 // Speed to get fans going after reset.
#define DEFAULT_SPEED_OFF 1 // Slow enough to turn the fans off.
#define DEFAULT_SPEED_STEP 1 // Fan speed increment step size.

// A single fan "cycle" sweeps the fan from high to low 
// speed and back again.  The number of ticks in a cycle
// period equals the number of control voltage steps.
#define DEFAULT_CYCLE_PERIOD_SEC 60

// The cycle of fan N is offset by a fraction of 
// 1/DEFAULT_OFFSET_DENOM fan cycles.  Use multiples of 2.
#define DEFAULT_OFFSET_DENOM 4 

// The fraction of the fan cycle to stay at full power in order
// to spin the fan up is 1/DEFAULT_START_DENOM. Use multiples of 2.
#define DEFAULT_START_DENOM 16

// A single fan update tick is FAN_TICK/F_TCA seconds.
#define FAN_TICK 16 // One fan tick every 0.5sec.


typedef struct {  
  uint8_t start[NUM_FANS]; // number of ticks at max speed to get fan going.
  uint8_t delay[NUM_FANS]; // Number of delay ticks until the cycle starts.
  uint8_t speed[NUM_FANS]; // Current fan duty cycle setting.
  int8_t inc[NUM_FANS]; // Fan increment.
  
  uint8_t speedHi[NUM_FANS];
  uint8_t speedLo[NUM_FANS];  
  uint8_t updateTicks[NUM_FANS]; // Number of fan ticks between speed updates.
  
  uint8_t cyclePeriodTicks;  // Number of fan ticks per cycle.
  uint8_t tickCounter;  // Counts fan ticks.
  
  uint8_t startDenom; 
  uint8_t offsetDenom;
  
  uint8_t on[NUM_FANS];
} fan_t;

fan_t fan;

/*
 * --- Fan Adjustment Interrupt Routines ---
 */

void FanCycleTick()
{
  
  for(uint8_t n=0; n < NUM_FANS; n++){
    
    if( fan.on[n] ){
      if( fan.start[n] == 0){
        if( fan.delay[n] == 0){
          if( (fan.tickCounter % fan.updateTicks[n]) == 0 ){
            fan.speed[n] += fan.inc[n];
            SetSpeed(n,fan.speed[n]);

            if(fan.speed[n] == fan.speedLo[n]){
              fan.inc[n] = DEFAULT_SPEED_STEP;
            }
            if(fan.speed[n] == fan.speedHi[n]){
              fan.inc[n] = -DEFAULT_SPEED_STEP;
            }
          }
        }
        else{
          fan.delay[n]--;
        }
      }
      else{
        fan.start[n]--;
      }
    }
  }  
  
  fan.tickCounter++;
}

void __interrupt(TCA0_CMP0_vect_num) TCA0Isr(void)
{
  FanCycleTick();  
  
  TCA0.SINGLE.INTFLAGS = 0b00010000;  // Clear interrupt.
  
  return;
}


/*
 * --- Fan Control Menu Functions
 */

char str[50];

void PrintFanSettings()
{
  USART_PRINT_STR(" Fan settings: \n\r")

  uint8_t cyclePeriodSec = fan.cyclePeriodTicks*FAN_TICK/F_TCA;
  
  sprintf(str,"  Cycle Period (sec): %d\n\r",cyclePeriodSec);
  USART_PRINT_STR(str);
  sprintf(str,"  Delay Offset (sec): %d\n\r",cyclePeriodSec/fan.offsetDenom);
  USART_PRINT_STR(str);
  
  for(uint8_t n=0; n < NUM_FANS; n++){
    sprintf(str,"  Fan %d:\n\r",n+1);
    USART_PRINT_STR(str);
    sprintf(str,"    Hi Speed: %d\n\r",fan.speedHi[n]);
    USART_PRINT_STR(str);
    sprintf(str,"    Lo Speed: %d\n\r",fan.speedLo[n]);
    USART_PRINT_STR(str);
    sprintf(str,"    Update Ticks: %d\n\r",fan.updateTicks[n]);
    USART_PRINT_STR(str);
  }
}

void RestartFans()
{  
  uint16_t offset = fan.cyclePeriodTicks / fan.offsetDenom;
  
  for(uint8_t n=0; n < NUM_FANS; n++){
    fan.start[n] = fan.cyclePeriodTicks / fan.startDenom;
    fan.delay[n] = n*offset;
    fan.speed[n] = DEFAULT_SPEED_START;
    fan.inc[n] = -DEFAULT_SPEED_STEP;
    fan.updateTicks[n] = 
            fan.cyclePeriodTicks /
            ( 2*(fan.speedHi[n]-fan.speedLo[n])/DEFAULT_SPEED_STEP );
    
    SetSpeed(n,fan.speed[n]);
  }  
  PrintFanSettings();
}

void SetFansToDefault()
{
  USART_PRINT_STR("\n\r\n\r>> Fans set to default. <<\n\r");
  
  fan.startDenom = DEFAULT_START_DENOM;
  fan.offsetDenom = DEFAULT_OFFSET_DENOM;
  fan.cyclePeriodTicks = DEFAULT_CYCLE_PERIOD_SEC*F_TCA/FAN_TICK;
  fan.tickCounter = 0;
  
  for(uint8_t n=0; n < NUM_FANS; n++){
    fan.speedHi[n] = defaultSpeedHi[n];
    fan.speedLo[n] = defaultSpeedLo[n];
    fan.on[n] = 1;
  }
  
  RestartFans();
}

void ShutOffFan()
{
  USART_PRINT_STR("\n\r\n\r>> Turn Fan(s) off. <<\n\r");
  USART_PRINT_STR(" Enter fan number (0 for all): ");
  USART_READ_STR(str,50);
  uint8_t fanNum = atoi(str);

  if( fanNum > 0 ){
    fan.on[fanNum-1] = 0;
    SetSpeed(fanNum-1,DEFAULT_SPEED_OFF);
  }
  else{
    for(uint8_t n=0; n < NUM_FANS; n++){
      fan.on[n] = 0;
      SetSpeed(n,DEFAULT_SPEED_OFF);
    }  
  }  
}

void TurnOnFans()
{
  USART_PRINT_STR("\n\r\n\r>> Turn Fans on. <<\n\r");
  for(uint8_t n=0; n < NUM_FANS; n++){
    fan.on[n] = 1;
  }  
  RestartFans();  
}


void SetFanMaxSpeed()
{
  USART_PRINT_STR("\n\r\n\r>> Set new max speed. <<\n\r");
  
  USART_PRINT_STR(" Enter fan number (0 for all): ");
  USART_READ_STR(str,50);
  uint8_t fanNum = atoi(str);
  
  USART_PRINT_STR(" Enter speed: ");
  USART_READ_STR(str,50);
  uint8_t newSpeed = atoi(str);
 
  if( fanNum > 0 ){
    fan.speedHi[fanNum-1] = newSpeed;
  }
  else{
    for(uint8_t n=0; n < NUM_FANS; n++){
      fan.speedHi[n] = newSpeed;
    }  
  }
  RestartFans();
}

void SetFanMinSpeed()
{
  USART_PRINT_STR("\n\r\n\r>> Set new min speed. <<\n\r");
  
  USART_PRINT_STR(" Enter fan number (0 for all): ");
  USART_READ_STR(str,50);
  uint8_t fanNum = atoi(str);
  
  USART_PRINT_STR(" Enter speed: ");
  USART_READ_STR(str,50);
  uint8_t newSpeed = atoi(str);
 
  if( fanNum > 0 ){
    fan.speedLo[fanNum-1] = newSpeed;
  }
  else{
    for(uint8_t n=0; n < NUM_FANS; n++){
      fan.speedLo[n] = newSpeed;
    }  
  }
  RestartFans();
}


void SetFanOffset()
{
  USART_PRINT_STR("\n\r\n\r>> Set new offset denominator. <<\n\r");
  
  sprintf(str," Enter denominator: ");
  USART_READ_STR(str,50);
 
  fan.offsetDenom = atoi(str); 

  RestartFans();
}

void SetFanCycleTime()
{
  USART_PRINT_STR("\n\r\n\r>> Set fan cycle time. <<\n\r");
  
  USART_PRINT_STR(" Enter time (sec): ")
  USART_READ_STR(str,50);
  
  uint16_t fanCycleTimeSec = atoi(str);

  fan.cyclePeriodTicks = fanCycleTimeSec * F_TCA / FAN_TICK;
  
  RestartFans();
}




int main(void) {
  
  // Use the internal 32.768kHz oscillator.
  CCP = 0xd8;
  CLKCTRL.MCLKCTRLA = 0x01;
  while( CLKCTRL.MCLKSTATUS & 0b00000001 ){
      ;
  }
  
  // --------------- Timer A ------------------
  
  PORTC.DIRSET = 0b00000010;
  PORTC.OUT = 0xff;
  
  // Enable CMP0 interrupt.
  TCA0.SINGLE.INTCTRL = 0b00010000;
  
  // Configure TCA0 clock source.
  // - fTCA = 32,768 Hz / 1024 = 32 Hz (32 ticks/sec)
  TCA0.SINGLE.CTRLA = 0b00001111; // fClk/1024, enable.
  
  // Set when interrupts occur.
  TCA0.SINGLE.PER = FAN_TICK;
  TCA0.SINGLE.CMP0 = FAN_TICK-1;
  TCA0.SINGLE.CNT = 0x0000;

  
  // Enable interrupts in order to use TCA0
  SREG = 0b10000000;
  
  
  // ------------ Timer B ----------------
  
  // Configure timer waveform output (WO) pins as outputs.
  PORTA.DIRSET = 0b00000100;  // TCB0 - PA2 (Fan1)
  PORTA.DIRSET = 0b00001000;  // TCB0 - PA3 (Fan2)
  PORTC.DIRSET = 0b00000001;  // TCB2 - PC0 (Fan3)
    
  for(uint8_t i=0; i < NUM_FANS; i++){
    // Configure TCB timers.
    tCB[i]->CTRLA = 0b00000001;  // CLK_PER clock source, enable.
    tCB[i]->CTRLB = 0b00010111;  // Enable WO, PWM8 mode.
  
    // Use PWM mode and an RC circuit to create a variable fan FET
    // control voltage.
    // - CCMPL controls period (use 100 for easy duty cycle calcs).
    // - CCMPH controls pulse width.
    // - RC circuit connected to the output has a time constant
    //   of (10kOhm)(1uF) = 10ms.
    tCB[i]->CCMPL = 100;  // 3.052ms period
    tCB[i]->CCMPH = 1;   // 1% duty cycle (fan will be off)
  }
    
  // ------------ UART -------------
  // 2400 8N1
  USART0.BAUD = 0x006d;
  USART0.CTRLC = 0b00000011;
  PORTA.DIRSET = 0b00000001;
  USART0.CTRLB = 0b11000010;

  uint8_t uartMenuSize = 7;
  uint8_t help0[] = "Turn fan(s) off.";
  uint8_t help1[] = "Turn all fans on.";
  uint8_t help2[] = "Reset to default settings.";
  uint8_t help3[] = "Set max speed.";
  uint8_t help4[] = "Set min speed.";
  uint8_t help5[] = "Set cycle offset.";
  uint8_t help6[] = "Set cycle time.";
  
  UartCommand uartMenu[] = {
    { 0, help0, &ShutOffFan },
    { 1, help1, &TurnOnFans },
    { 2, help2, &SetFansToDefault },
    { 3, help3, &SetFanMaxSpeed },
    { 4, help4, &SetFanMinSpeed },
    { 5, help5, &SetFanOffset },
    { 6, help6, &SetFanCycleTime },
  };

  SetFansToDefault();
  
  
  while (1) {
      USART_MENU_PROMPT;
      USART_READ_MENU_COMMAND;        
  }
  
}
