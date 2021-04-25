/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  04/16/2021
*Project :  final project keypad library
*File :  keypad_lib. c
*Description : kepad library for final project
**********************************************************************/
#include "keypad_lib.h"
#include <stdio.h>

/****| keypad_init | *****************************************
* Brief: initilizes keypad
* Inputs: none
* Outputs: none
*/
void keypad_init (void)     //initalize keypad
{
    P4SEL0 &= ~0xFF;
    P4SEL1 &= ~0xFF;
    P4->DIR &= ~(BIT0| BIT1 | BIT2| BIT3);//set keypad Row 0 to P4.0 Row 1 to P4.1 and Row2 to P4.2 and Row3 to P4.3
    P4REN |= (BIT0 | BIT1 | BIT2| BIT3);//enable pull resistor on all rows
    P4OUT |= (BIT0 | BIT1 | BIT2| BIT3);//set up rows with pull-up
    P4DIR &= ~(BIT4 | BIT5| BIT6);//Set keypad columns to inputs Col_2  = P4.6, Col_1 = P4.5, and Col_0 = P4.4
}

/****| Read_Keypad | *****************************************
* Brief: reads user input from the keypad into the board and
* stores them in a global variable called num
* Inputs: col, row
* Outputs: num
*/
uint8_t Read_Keypad(void){       //reads keypad entry into the global variable num
    uint8_t col, row;
    for ( col = 0; col < 3; col++ ){
        P4->DIR = 0x00;             // Set Columns to inputs
        P4->DIR |= BIT ( 4+col );   // Set column 3 to output
        P4->OUT &=~ BIT ( 4+col );  // Set column 3 to LOW
        delay_ms (10);     // Delay the while loop
        row = P4->IN & 0x0F;    // read all rows
    while ( !(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3) );
    if (row != 0x0F){
        delay_ms (5);    //prevents switch bounce
        if (row != 0x0F){
        break; // if one of the input is low, some key is pressed.
        }
    }
    }
    P4->DIR = 0x00; // Set Columns to inputs
    if ( col == 3)
    return 0;
    if (row == 0x0E) num = col + 1; // key in row 0
    if (row == 0x0D) num = 3 + col +1; // key in row 1
    if (row == 0x0B) num = 6 + col +1; // key in row 2
    if (row == 0x07) num = 9 + col+1; // key in row 3
return 1;
}