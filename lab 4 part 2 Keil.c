/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  02/12/2021
*Project :  Lab 4 part 2
*File :  lab 4 part 2 Keil. c
*Description : LED blink speed controller
**********************************************************************/
#include "msp.h"
#include <stdio.h>
#define LED_RED     (1U << 0)
#define LED_GREEN   (1U << 1)
#define LED_BLUE    (1U << 2)

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    P1SEL0 &= ~0x12;     // Clear SEL0 bit for P1.1 and P1.4
    P1SEL1 &= ~0x12;
    P1OUT = 0x00;
    P2OUT = 0x00;
    P2DIR |= LED_RED;       // P2.0 output
    P2DIR |= LED_GREEN;  // P2.1 output
    P2DIR |= LED_BLUE;     // P2.2 output
    P1DIR |=  0xFF;         // set all P1 pin output
    P1DIR &= ~0x12;       // P1.1 & P1.4 input
    P1REN |=  0x12;        // P1.1 & P1.4 pull-up/down resistor enable
    P1OUT |= 0x12;       // P1.1 & P1.4 pull-down resistor
    int i = 1;
    int K = 0;


    while(1) {

        if ((!(P1IN & 0x02))){           //true if button is pressed
            __delay_cycles(50000);       //five millisecond delay
            if (!(P1IN & 0x02)){         //double checks to see if switch is still pressed
            K = 1;                       //prevents LED from showing white
        }
        }

while (K==1){     //second while loop

    if ((i==1) && (K==1)){
P2OUT &= ~LED_BLUE; //turns off blue LED
P2OUT |= LED_RED;   //turns on red LED
__delay_cycles(1000000);    //one second delay
i=2;                        //assigns next color to move to
if ((P1IN & 0x02)){ //true if button is released
__delay_cycles(30000);
if ((P1IN & 0x02)){
        K = 0;      //prevents code from moving on to the next color after the button has been released
}
}
}

    if ((i==2) && (K==1)){
P2OUT &= ~LED_RED;  //turns off red LED
P2OUT |= LED_GREEN; //turns on green LED
__delay_cycles(1000000);
i=3;
if ((P1IN & 0x02)){
__delay_cycles(30000);
if ((P1IN & 0x02)){
        K = 0;
}
}
}
    if ((i==3) && (K==1)){
P2OUT &= ~LED_GREEN; //turns off green LED
P2OUT |= LED_BLUE;   //turns on blue LED
__delay_cycles(1000000);
i=1;
if ((P1IN & 0x02)){
__delay_cycles(30000);
if ((P1IN & 0x02)){
        K = 0;
}
}
}

}//secondary while
}//while loop
}// end of main
