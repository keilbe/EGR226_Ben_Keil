/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/19/2021
*Project :  Lab 8 part 2
*File :  lab 8 part 2. c
*Description : motor PWM code with timerA
**********************************************************************/
#include "msp432.h"
void main()
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; //stop watchdog timer
  int speed=100;        //declared non universally so it can be edited from the variables tab

  P2SEL0  |= 0x80;              //clears P2.7 in P2SEL0
  P2SEL1 &= ~0x80;              //clears P2.7 in P2SEL1
  P2DIR  |=  0x80;              //P2.7 output
  TIMER_A0->CCR[0] = 60000-1; //sets period
  TIMER_A0->CCTL[4] = 0xE0; //CCR4 set/reset mode
  TIMER_A0->CTL = 0x0214; //count up
  while(1){
      TIMER_A0->CCR[4] = speed; //sets duty cycle
    }
}

