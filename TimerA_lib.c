/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/16/2021
*Project :  TimerA library
*File :  TimerA_lib.c
*Description : TimerA library for final project
**********************************************************************/
#include "TimerA_lib.h"
#include <stdio.h>
/****| TA_init | ******************************************
* Brief: Initilizations to setup timerA for the various pins
* Inputs: none
* Outputs: none
*/
void TA_init(void){
        //motor P2.7
        TIMER_A0->CCR[0] = 50000; //sets period 40Hz --dc
        P2SEL0  |= 0x80;              //Clear P2.7 in P2SEL0
        P2SEL1 &= ~0x80;              //Clear P2.7 in P2SEL1
        P2DIR  |=  0x80;              //P2.7 output
        TIMER_A0->CCTL[4] = 0xE0; //CCR4 set/reset mode motor
        //brightness
        P2SEL0  |= 0x40;              //Clear P2.6 in P2SEL0
        P2SEL1 &= ~0x40;              //Clear P2.6 in P2SEL1
        P2DIR  |=  0x40;              //P2.6 output
        TIMER_A0->CCTL[3] = 0xE0; //CCR3 set/reset mode brightness
        TIMER_A0->CTL = 0x0214; //count up

        //RGB PWM P7.5-P7.7
        TIMER_A1->CCR[0] = 60000; //sets period 50Hz
        P7SEL0  |= 0xE0;              //Clear P7.5-P7.7 in P2SEL0
        P7SEL1 &= ~0xE0;              //Clear 7.5-P7.7 in P2SEL1
        P7DIR  |=  0xE0;              //7.5-P7.7 output
        TIMER_A1->CCTL[1] = 0xE0; //CCR1 set/reset mode red
        TIMER_A1->CCTL[2] = 0xE0; //CCR2 set/reset mode blue
        TIMER_A1->CCTL[3] = 0xE0; //CCR3 set/reset mode green
        TIMER_A1->CTL = 0x0214;   //count up

        //speaker
        TIMER_A2->CCR[0] = 8400;   //sets period 7Hz
        P5SEL0  |= 0x80;              //Clear P5.7 in P2SEL0
        P5SEL1 &= ~0x80;              //Clear P5.7 in P2SEL1
        P5DIR  |=  0x80;              //P5.7 output
        TIMER_A2->CCTL[2] = 0xE0; //CCR2 set/reset mode
        TIMER_A2->CTL = 0x0214;   //count up
        //servo
        TIMER_A3->CCR[0] = 60000; //sets period 50Hz
        P10SEL0  |= 0x20;              //Clear P10.5 in P2SEL0
        P10SEL1 &= ~0x20;              //Clear P10.5 in P2SEL1
        P10DIR  |=  0x20;              //P10.5 output
        TIMER_A3->CCTL[1] = 0xE0; //CCR1 set/reset mode
        TIMER_A3->CTL = 0x0214;   //count up
}