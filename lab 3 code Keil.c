#include "msp.h"
/*********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  02/5/2021
*Project :  Lab 3 code
*File :  lab 3 code Keil. c
*Description : LED blink speed controller
**********************************************************************/

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	    P1->DIR = BIT0;
	    int i;
	    int j;
	    while(1){
	        P1->OUT ^= BIT0;
	        for(i=j; i>0; i--);  //j can be changed from the variables tab while running the code and the blink rate will change

	    }

}
