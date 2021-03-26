/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/19/2021
*Project :  Lab 9 part 3
*File :  lab 9 part 3. c
*Description : 7 segment LED button control sequencer
**********************************************************************/
#include "msp.h"
#include <stdio.h>

void  SetupPort2Interrupts(void);
int i = 0;
void SysTick_Init (void);
void delay_ms (unsigned ms);

/****| main | *****************************************
* Brief: Initailizes variables and prints a number to
* the 7 segment LED. The number displayed is changed via
* interrupts.
* Inputs: int speed
* Outputs: none
*/
void main(void){
    __disable_irq ();
    const unsigned char numberPattern [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //stop watchdog timer
    P4->SEL0 &= ~0xFF;              //Set P4 as I/O
    P4->SEL1 &= ~0xFF;
    P4->DIR  |=  0xFF;              //P4 output

    P5->SEL0 &= ~BIT0;              //clears P5.0 in P2SEL0
    P5->SEL1 &= ~BIT0;              //clears P5.0 in P2SEL1
    P5->DIR  |=  BIT0;              //5.0 output

    P6->SEL0  |= 0x80;              //clears P6.7 in P2SEL0
    P6->SEL1 &= ~0x80;              //clears P6.7 in P2SEL1
    P6->DIR  |=  0x80;              //P6.7 output

    SetupPort2Interrupts();
    SysTick_Init ();
    NVIC_EnableIRQ (PORT2_IRQn);
    NVIC_SetPriority(TA2_0_IRQn, 3);
    NVIC_EnableIRQ(TA2_0_IRQn);

    __enable_irq ( );


while(1) {
P5->OUT |= BIT0;
P4->OUT = numberPattern[i];
}
}// end of main

/****| SetupPort2Interrupts | *****************************************
* Brief: initialization for the button interrupt ports
* Inputs: none
* Outputs: none
*/
void  SetupPort2Interrupts( ){
    P2->SEL0 &=~ 0x30; //P2.4, P2.5 Input, Pull Up Resistor
    P2->SEL1 &=~ 0x30;
    P2->DIR &=~ 0x30;
    P2->REN |= 0x30;
    P2->OUT |= 0x30; //Input, Pull Up Resistor
    P2->IES |= 0x30; //Set pin interrupt to trigger for P2.4-P2.5
    P2->IFG = 0;
    P2->IE = 0x30;      //enable interrupt 2.4 - 2.5
}
/****| PORT2_IRQHandler | *****************************************
* Brief: Detects which button has been pressed and sets i to the
* appropriate value.
* Inputs: none
* Outputs: int i
*/
void PORT2_IRQHandler(void){
    if ((P2->IFG & 0x10) && (i <= 8)){
        delay_ms(150);                          //extremely long switchbounce delay to compensate for a lot of switchbounce
            if ((P2->IFG & 0x10) && (i < 9)){
                i = i+1;            //increases i by 1
            }
            P2->IFG &= ~BIT4;   //clear flag
}
if ((P2->IFG & 0x20) && (i > 0)){ //If P2.5 had an interrupt (comparing the status with the BIT)
        delay_ms(150);

        if ((P2->IFG & 0x20) && (i > 0)){ //If P2.5 had an interrupt (comparing the status with the BIT)
            i = i-1;                //decreases i by 1
        }
        P2->IFG &= ~BIT5;
}
}
/****| delay_ms | *****************************************
* Brief: uses the systick timer to create 1 ms delays
* Inputs: unsigned ms
* Outputs: none
*/
void delay_ms (unsigned ms){
    SysTick -> LOAD = (ms*3000 - 1); // 1 us delay
    SysTick -> VAL = 0; //clears counter
    SysTick -> CTRL = 0x5; //enable timer
    while((SysTick -> CTRL & 0x00010000) == 0);
    SysTick -> CTRL = 0; //stop timer
}
/****| SysTick_Init | *****************************************
* Brief: initilization sequence for the systick timer
* Inputs: none
* Outputs: none
*/
void SysTick_Init (void){
    SysTick -> CTRL = 0; //disable SysTick During step
    SysTick -> LOAD = 0x00FFFFFF; //max reload value
    SysTick -> VAL = 0; //any write to current clears it
    SysTick -> CTRL = 0x00000005; //enable systic, 3MHz, No interupts
}
