/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/02/2021
*Project :  Lab 10 part 1
*File :  lab 10 part 1. c
*Description : ADC potentiometer reader
**********************************************************************/
#include "msp432.h"
#include "stdio.h"

void SysTick_Init (void);
void delay_micro (unsigned microsec);
void delay_ms(unsigned ms);
void adcsetup(void);
float result;

/****| main | *****************************************
* Brief: calls the functions to initialize SysTick and
* setup the ADC (analog digital converter). It then
* converts a value and that value is stored in the
* variable "result". This value is then printed out.
* Inputs: none
* Outputs: none
*/
int main(void) {
    SysTick_Init ();
    adcsetup();
    while(1) {
        delay_ms(500);
    ADC14->CTL0 |=1; //start a conversion
    while(!ADC14->IFGR0); // wait until conversion complete
    result = ADC14->MEM[5]; // immediately store value in a variable
    result = (result*3.3)/16738; //converts result into volts
    printf ("Voltage is:\n\t%lf V\n", result); //prints out result

        delay_ms(500);
    }
}
/****| adcsetup | *****************************************
* Brief: Setup/initilization function to setup the ADC (analog digital converter).
* Inputs: none
* Outputs: none
*/
void adcsetup(void){
    ADC14->CTL0 = 0x00000010; //power on and disabled during configuration
    ADC14->CTL0 |= 0x04D80300; // S/H pulse mode, MCLCK, 32 sample clocks, sw trigger, /4 clock divide
    ADC14->CTL1 = 0x00000030; // 14-bit resolution
    ADC14->MCTL[5] = 0; // A0 input, single ended, vref=avcc
    P5->SEL1 |= 0x20; // configure pin 5.5 for AO
    P5->SEL0 |= 0x20;
    ADC14->CTL1 |= 0x00050000; //start converting at memory register 5
    ADC14->CTL0 |= 2; //enable ADC after configuration
}

/****| delay_ms | *****************************************
* Brief: uses the systick timer to create 1 ms delays
* Inputs: unsigned ms
* Outputs: none
*/
void delay_ms(unsigned ms) {
    SysTick->LOAD = (ms*3000 - 1); //1ms delay
    SysTick->VAL = 0; //any write to CVR clears it
    SysTick -> CTRL = 0x5; // enable timer
    while ( (SysTick->CTRL & 0x00010000) == 0); //wait for flag to be SET
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
