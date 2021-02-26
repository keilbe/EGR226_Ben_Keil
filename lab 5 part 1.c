/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  02/12/2021
*Project :  Lab 5 part 1
*File :  lab 5 part 1. c
*Description : LED blink speed controller
**********************************************************************/
#include "msp.h"
#include <stdio.h>
#define LED_GREEN     (1U << 5)
#define LED_YELLOW   (1U << 6)
#define LED_RED    (1U << 7)
#define BUTTON_ONE (1U << 6)

void SysTick_delay(unit16_tdelay){
    SysTick->CTRL = 0; // disable systick during step
    SysTick->LOAD = 0x00FFFFFF; // maximum reload value
    SysTick->VAL = 0; // free to be written over
    SysTick->CTRL = 0x00000005; // enable systick, 3Mhz, no interrupts
}
void main(void){

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    P1SEL0 &= ~BUTTON_ONE;     // Clear SEL0 bit for P1.1 and P1.4
    P1SEL1 &= ~BUTTON_ONE;
    P1OUT = 0x00;
    P2OUT = 0x00;
    P2DIR |= LED_GREEN;       // P2.5 output
    P2DIR |= LED_YELLOW;      // P2.6 output
    P2DIR |= LED_RED;         // P2.7 output
    P1DIR &= ~BUTTON_ONE;       // P1.6 input
    P1DIR |=  BUTTON_ONE;         // set all P1 pin output
    P1REN |=  BUTTON_ONE;        // P1.6 pull-up/down resistor enable
    P1OUT |=  BUTTON_ONE;       // P1.6 pull-down resistor

    int i = 0;
    int K = 0;


    while(1) {

        if (!(P1IN & BUTTON_ONE)){
           SysTick_delay (10);          //delays while loop to accmodate for switch bounce
            if (!(P1IN & BUTTON_ONE)){
            K = 1;                     //condition for while loop to start
            i = i+1;                   //changes the color
        }
        }

while (K==1){   //condition met after button is pressed

if (i==4){      //allows code to loop back to green(i=1) after red(i=3)
i = 1;
}

    if (i==1){
P2OUT &= ~LED_RED;          //turns off red LED
P2OUT |= LED_GREEN;    //turns on green LED
}
    if (i==2){
P2OUT &= ~LED_GREEN;      //turns off green LED
P2OUT |= LED_YELLOW;       //turns on yellow LED
}
    if (i==3){
P2OUT &= ~LED_YELLOW;    //turns off yellow LED
P2OUT |= LED_RED;      //turns on red LED
}


if ((P1IN & BUTTON_ONE)){     //checks if button has been released
    SysTick_delay (10);
if ((P1IN & BUTTON_ONE)){
        K = 0;  //K is reset to zero so the while loop does not continue to run infinitely
}
}
}//secondary while
}//while loop
}// end of main

