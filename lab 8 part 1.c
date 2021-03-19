/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/12/2021
*Project :  Lab 8 part 1
*File :  lab 8 part 1. c
*Description : motor pwm code
**********************************************************************/
#include "msp432.h"
volatile uint32_t intervalCnt;
void SysTick_Init_interrupt (void);

/****| Main | *****************************************
* Brief: reads user input from the keypad into the board and
* stores them in a global variable named "num".
* Inputs: col, row
* Outputs: num
*/
int main(void){
WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Halting the Watchdog
P2->SEL0 &=~ (BIT7); // Set pin 2.7 as GPIO
P2->SEL1 &=~ (BIT7); // Set pin 2.7 as GPIO
P2->DIR |= (BIT7); // Set pin 2.7 as OUTPUT
SysTick_Init_interrupt (); // initialize SysTick with interrupt
__enable_irq ( ); // enable global interrupts
float DS;   //duty cycle
int on, off;
while (1){              //set the value of DS between 0-100 in the variables tab
    off = 200* (DS/100);
    on = 200*(1-(DS/100));
    if((intervalCnt % on) == 0){ //Toggle P2.7 on for the duration of the on cycle
        P2->OUT ^= BIT7;    //Motor turned on
    }
    if((intervalCnt % off) == 0){ //Turn off P2.7 for the duration of the off cycle
        P2->OUT &=~ BIT7;   //Motor is turned off
    }
}
}
/****| SysTick_Init_interrupt | *****************************************
* Brief: initialization for systick, interrupts enabled, .0005s delays.
* Inputs: none
* Outputs: none
*/
void SysTick_Init_interrupt (void) { //initialization of systic timer
SysTick -> CTRL = 0; // disable SysTick During step
SysTick -> LOAD = 150; // reload value for 0.0005s interrupts
SysTick -> VAL = 0; // any write to current clears it
SysTick -> CTRL = 0x00000007; // enable SysTick, 3MHz, Interrupts enabled
}
/****| SysTick_Handler | *****************************************
* Brief: indexes the the interval counter for systick. This is
* used to time when the delays occur.
* Inputs: none
* Outputs: IntervalCnt
*/
void SysTick_Handler (void) {
intervalCnt++ ; // increment interval counter
}
