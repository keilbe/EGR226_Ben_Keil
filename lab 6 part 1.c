/********************************************************************
*Author :  Ben Keil
*Course : EGR 226-905
*Date :  03/06/2021
*Project :  Lab 6 part 1
*File :  lab 6 part 1. c
*Description : Keypad entry printer
**********************************************************************/
#include "msp.h"
#include <stdio.h>
void keypad_init (void); // prototype for GPIO initialization
void Systick_delay (void); //prototype for systick delay
void Systick_init (void); //prototype for systick initlization
uint8_t Read_Keypad (void); // prototype for keypad scan subroutine
void Print_Keys (void); // Print Key pressed
uint8_t num, pressed; // GLOBAL!



/****| main | *****************************************
* Brief: prompts user to press a key on the keypad then
* calls the nessicary functions.
* Inputs: pressed
* Outputs: none
*/
void main (void){
WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
printf("Press a key on the keypad.\n");
keypad_init ( ); // Call initialization functions for functionality
while ( 1 )
{
pressed = Read_Keypad ( ); // Call Function to read Keypad
if ( pressed )
Print_Keys ( );
__delay_cycles(30000); // 10ms delay through the loop before reading keypad again
}
}
/****| keypad_init | *****************************************
* Brief: initilizes keypad
* Inputs: none
* Outputs: none
*/
void keypad_init (void)     //initalize keypad???
{
    P4SEL0 &= ~0xFF;
    P4SEL1 &= ~0xFF;
    //set keypad Row 0 to P4.0 Row 1 to P4.1 and Row2 to P4.2 and Row3 to P4.3
    P4->DIR &= ~(BIT0| BIT1 | BIT2| BIT3);
    //enable pull resistor on all rows P4.0, P4.1, P4.2, P4.3
    P4REN |= (BIT0 | BIT1 | BIT2| BIT3);
    //configure rows with pull-up rows P4.0, P4.1, P4.2, P4.3
    P4OUT |= (BIT0 | BIT1 | BIT2| BIT3);
    //Set keypad columns to inputs P4.6 Col_1 P4.5 and Keypad Col_0 P4.4
    P4DIR &= ~(BIT4 | BIT5| BIT6);
}

/****| SysTick_init | *****************************************
* Brief: initilization for systick
* Inputs: none
* Outputs: none
*/
void SysTick_init(void){   //systick initilization function
    SysTick->CTRL = 0; // disable systick during step
    SysTick->LOAD = 0x00FFFFFF; // maximum reload value
    SysTick->VAL = 0; // free to be written over
    SysTick->CTRL = 0x00000005; // enable systick, 3Mhz, no interrupts
}
/****| SysTick_delay | *****************************************
* Brief: delay function for systick
* Inputs: none
* Outputs: none
*/
void SysTick_delay (uint16_t delay){ // Systick delay function
    SysTick->LOAD = ((delay * 3000) - 1); //delay values are equal to 1ms
    SysTick->VAL = 0; // any write to CVR clears it
    while ( (SysTick->CTRL & 0x00010000) == 1); // wait for flag to be SET
}
/****| Read_Keypad | *****************************************
* Brief: reads user input from the keypad into the board and
* stores them in a global variable called num
* Inputs: col, row
* Outputs: num
*/
uint8_t Read_Keypad(void)
{
uint8_t col, row;
for ( col = 0; col < 3; col++ )
{
P4->DIR = 0x00; // Set Columns to inputs
P4->DIR |= BIT ( 4+col ); // Set column 3 to output
P4->OUT &=~ BIT ( 4+col ); // Set column 3 to LOW
SysTick_delay (10); // Delay the while loop
row = P4->IN & 0x0F; // read all rows
while ( !(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3) );
if (row != 0x0F){
    SysTick_delay (5);    //prevents switch bounce
    if (row != 0x0F){
    break; // if one of the input is low, some key is pressed.
    }
}
}
P2->DIR = 0x00; // Set Columns to inputs
if ( col == 3)
return 0;
if (row == 0x0E) num = col + 1; // key in row 0
if (row == 0x0D) num = 3 + col +1; // key in row 1
if (row == 0x0B) num = 6 + col +1; // key in row 2
if (row == 0x07) num = 9 + col+1; // key in row 3
return 1;
}
/****| Print_Keys | *****************************************
* Brief: prints out what key has been pressed on the keypad.
* If the # key has been pressed it also calls either Read()
* or Check_Keys().
* Inputs: K, num
* Outputs: i, array[i]
*/
void Print_Keys ( void)
{
    if((num == 1)){
        printf("1");
        fflush (stdout);
    }
    if((num == 2)){
        printf("2");
        fflush (stdout);
    }
    if((num == 3)){
        printf("3");
        fflush (stdout);
    }
    if((num == 4)){
        printf("4");
        fflush (stdout);
    }
    if((num == 5)){
        printf("5");
        fflush (stdout);
    }
    if((num == 6)){
        printf("6");
        fflush (stdout);
    }
    if((num == 7)){
        printf("7");
        fflush (stdout);
    }
    if((num == 8)){
        printf("8");
        fflush (stdout);
    }
    if((num == 9)){
        printf("9");
        fflush (stdout);
    }
    if((num == 10)){
        printf("*");
        fflush (stdout);
    }
    if((num == 11)){
        printf("0");
        fflush (stdout);
    }
    if((num == 12)){
        printf("#");
        fflush (stdout);
    }
}
