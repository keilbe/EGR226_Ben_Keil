/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/19/2021
*Project :  Lab 9 part 2
*File :  lab 9 part 2. c
*Description : 7 segment LED timed sequencer
**********************************************************************/
#include "msp.h"
#include <stdio.h>

void SysTick_Init (void);
void delay_Sec (unsigned second);

/****| main | *****************************************
* Brief: initailizes variables and controls sequencing of the
* 7 segment LED in a while loop
* Inputs: unsigned second
* Outputs: none
*/
void main(void){
    const unsigned char numberPattern [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};    //adresses for the 7 segment LED
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     //stop watchdog timer
    SysTick_Init (); // initialize SysTick
    int i = 0;  //i starts at zero

    P4->SEL0 &= ~0xFF;              //Set P4 as I/O
    P4->SEL1 &= ~0xFF;
    P4->DIR  |=  0xFF;              //P4 output

    P5->SEL0 &= ~BIT0;              //Set P5.0 as I/O
    P5->SEL1 &= ~BIT0;
    P5->DIR  |=  BIT0;              //5.0 output

while(1) {
P5->OUT |= BIT0;                //select output pin
P4->OUT = numberPattern[i];     //sends desired number to 7 segment LED
delay_Sec(1);           //1 second delay
P5->OUT &= ~BIT0;
i++;                    //indexes to next number
    if (i == 10){           //resets count
    i = 0;
    }
}
}

/****| delay_Sec | *****************************************
* Brief: uses the systick timer to create 1 us delays
* Inputs: unsigned microsec
* Outputs: none
*/
void delay_Sec (unsigned second){
    SysTick -> LOAD = (second*3000000 - 1); // 1 s delay
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
