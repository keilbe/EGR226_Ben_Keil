/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/19/2021
*Project :  Lab 9 part 1
*File :  lab 9 part 1. c
*Description : button motor speed controller
**********************************************************************/
#include "msp.h"
#include <stdio.h>

void  SetupPort2Interrupts(void);

int speed=0;

/****| main | *****************************************
* Brief: Initailizes variables and sets the duty cycle
* in a while loop. The duty cycle is changed via via
* interrupts.
* Inputs: int speed
* Outputs: none
*/
void main(void){
    __disable_irq ();    //disable interrupts for initilization
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    P6SEL0  |= 0x80;              //clears P6.7 in P2SEL0
    P6SEL1 &= ~0x80;              //clears P6.7 in P2SEL1
    P6DIR  |=  0x80;              //P6.7 output

    TIMER_A2->CTL = 0x02D1;
    TIMER_A2->CCR[0] = 60000; //sets period
    TIMER_A2->CCTL[4] = 0xE0; //CCR4 set/reset mode

    SetupPort2Interrupts();

    NVIC_EnableIRQ (PORT2_IRQn);
    NVIC_SetPriority(TA2_0_IRQn, 3);
    NVIC_EnableIRQ(TA2_0_IRQn);

    __enable_irq ( );   //enable interrupts

while(1) {

TIMER_A2->CCR[4] = speed; //sets duty cycle
}
}

/****| SetupPort2Interrupts | *****************************************
* Brief: initialization for the button interrupt ports
* Inputs: none
* Outputs: none
*/
void  SetupPort2Interrupts( ){

    P2->SEL0 &=~ 0x70; //P2.4- P2.6 Input, Pull Up Resistor
    P2->SEL1 &=~ 0x70;
    P2->DIR &=~ 0x70;
    P2->REN |= 0x70;
    P2->OUT |= 0x70; //Input, Pull Up Resistor
    P2->IES |= 0x70; //Set pin interrupt to trigger for P2.4-P2.6
    P2->IFG = 0;
    P2->IE = 0x70;      //enable interrupt P2.4 - P2.6
}
/****| PORT2_IRQHandler | *****************************************
* Brief: Detects which button has been pressed and sets i to the
* appropriate value.
* Inputs: none
* Outputs: int speed
*/
void PORT2_IRQHandler(void){
    if ((P2->IFG & 0x10) && (speed < 60000)){   //If P2.6 had an interrupt and the speed is not already at maximum
        __delay_cycles(300000);         //switch bounce
            if (P2->IFG & 0x10){
    speed = speed + 6000; //increases duty cycle
    printf("press1\n");
    P2->IFG &= ~BIT4;
            }
        P2->IFG &= ~BIT4;   //Clear flag
}
if ((P2->IFG & 0x20) && (speed > 0)){ //If P2.5 had an interrupt and the speed is not already at minimum
    __delay_cycles(300000);
        if ((P2->IFG & 0x20)){
    speed -= 6000; //decreases duty cycle
    printf("press2\n");
    P2->IFG &= ~BIT5;
        }
        P2->IFG &= ~BIT5;
}
if (P2->IFG & 0x40){ //If P2.6 had an interrupt
    __delay_cycles(300000);
        if (P2->IFG & 0x40){
    speed = 0; //sets duty cycle to 0, stopping the motor completely
    printf("press3\n");
    P2->IFG &= ~BIT6;
}
    P2->IFG &= ~BIT6;
}
}

