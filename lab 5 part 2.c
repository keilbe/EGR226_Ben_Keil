/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  02/19/2021
*Project :  Lab 5 part 2
*File :  lab 5 part 2. c
*Description : LED blink speed controller with timer
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
    P1SEL0 &= ~BUTTON_ONE;     // Clear SEL0 bit for P1.6
    P1SEL1 &= ~BUTTON_ONE;
    P1OUT = 0x00;
    P2OUT = 0x00;
    P2DIR |= LED_GREEN;       // P2.5 output
    P2DIR |= LED_YELLOW;      // P2.6 output
    P2DIR |= LED_RED;         // P2.7 output
    P1DIR &= ~BUTTON_ONE;       // P1.6 input
    P1DIR |=  BUTTON_ONE;       // set all P1 pin output
    P1REN |=  BUTTON_ONE;       // P1.6 pull-up/down resistor enable
    P1OUT |=  BUTTON_ONE;       // P1.6 pull-down resistor

    int i = 1;
    int K = 0;
while(1) {

    if ((!(P1IN & BUTTON_ONE))){           //true if button is pressed
        SysTick_delay (5);                //ten millisecond delay
        if (!(P1IN & BUTTON_ONE)){         //double checks to see if switch is still pressed
        K = 1;                             //prevents LED from showing white
    }
    }

while (K==1){     //second while loop

if ((i==1) && (K==1)){
P2OUT &= ~LED_RED; //turns off red LED
P2OUT |= LED_GREEN;   //turns on green LED
__delay_cycles(1000000);    //one second delay
i=2;                            //assigns next color to move to, in this case yellow
if ((P1IN & BUTTON_ONE)){ //true if button is released
    SysTick_delay (5);
if ((P1IN & BUTTON_ONE)){
    K = 0;      //prevents code from moving on to the next color after the button has been released
}
}
}

if ((i==2) && (K==1)){
P2OUT &= ~LED_GREEN;  //turns off green LED
P2OUT |= LED_YELLOW; //turns on yellow LED
__delay_cycles(1000000);
i=3;                        //will turn on the red LED next
if ((P1IN & BUTTON_ONE)){
    SysTick_delay (5);
if ((P1IN & BUTTON_ONE)){
    K = 0;
}
}
}
if ((i==3) && (K==1)){
P2OUT &= ~LED_YELLOW; //turns off yellow LED
P2OUT |= LED_RED;   //turns on red LED
__delay_cycles(1000000);
i=1;                        //will turn on the green LED next
if ((P1IN & BUTTON_ONE)){
    SysTick_delay (5);
if ((P1IN & BUTTON_ONE)){
    K = 0;
}
}
}

}//secondary while
}//while loop
}// end of main
