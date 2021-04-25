/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/16/2021
*Project :  systick library
*File :  SysTick_lib.c
*Description : systick library for final project
**********************************************************************/
#include "SysTick_lib.h"
#include <stdio.h>

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
/****| delay_micro | *****************************************
* Brief: uses the systick timer to create 1 us delays
* Inputs: unsigned microsec
* Outputs: none
*/
void delay_micro (unsigned microsec){
    SysTick -> LOAD = (microsec*3 - 1); // 1 us delay
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