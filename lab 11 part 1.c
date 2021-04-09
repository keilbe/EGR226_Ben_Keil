/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/02/2021
*Project :  Lab 11 part 1
*File :  lab 11 part 1. c
*Description : IR emitter code
**********************************************************************/
#include "msp432.h"
#include "stdio.h"
void TimerA2Init(void);

int main(void) {
TimerA2Init();
while(1){

}
}
void TimerA2Init(void){
    P5->SEL0 |= BIT6;
    P5->SEL1 &=~ BIT6;
    P5->DIR |= BIT6;
    TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK
    TIMER_A_CTL_MC__UP | // Up mode
    TIMER_A_CTL_CLR | //Clear timer
    TIMER_A_CTL_ID__8; //Divide clock by 8
    TIMER_A2->CCR[0] = 37505; // 10 Hz with clock divisor of 8
    TIMER_A2->CCR[1] = 37505 / 2; // 50% duty cycle
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
}
